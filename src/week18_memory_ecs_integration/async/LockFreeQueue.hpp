#pragma once

#include <atomic>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <new>
#include <thread>
#include <type_traits>

//
//  References:
//
//      https://www.1024cores.net/home
//      https://en.wikipedia.org/wiki/Non-blocking_algorithm
//      https://en.wikipedia.org/wiki/Read-copy-update
//      https://en.wikipedia.org/wiki/Seqlock
//

template <typename T>
class LockFreeQueue {
 public:
  explicit LockFreeQueue(size_t capacity) : _capacity(capacity), _head(0), _tail(0) {
    assert(capacity >= 1 && "Capacity must be at least 1");
    _buffer = new Slot[capacity];

    for (size_t i = 0; i < capacity; ++i) {
      _buffer[i].sequence.store(i, std::memory_order_relaxed);
    }
  }

  ~LockFreeQueue() {
    for (size_t i = 0; i < _capacity; ++i) {
      Slot& slot = _buffer[i];
      size_t seq = slot.sequence.load(std::memory_order_acquire);
      if (seq == i + 1) {
        slot.data_ptr()->~T();
      }
    }
    delete[] _buffer;
  }

  LockFreeQueue(const LockFreeQueue&) = delete;
  LockFreeQueue& operator=(const LockFreeQueue&) = delete;

  void shutdown() {
    _valid.store(false, std::memory_order_release);
  }

  bool try_enqueue(T&& item) {
    if (!_valid.load(std::memory_order_acquire)) return false;

    size_t tail;
    Slot* slot;
    size_t index;
    size_t seq;

    while (true) {
      tail = _tail.load(std::memory_order_relaxed);
      index = tail % _capacity;
      slot = &_buffer[index];

      seq = slot->sequence.load(std::memory_order_acquire);
      intptr_t diff = static_cast<intptr_t>(seq) - static_cast<intptr_t>(tail);

      if (diff == 0) {
        // Try to claim this slot
        if (_tail.compare_exchange_weak(tail, tail + 1, std::memory_order_relaxed)) {
          break;  // we own the slot
        }
      } else if (diff < 0) {
        return false;  // full
      } else {
        std::this_thread::yield();  // spin
      }
    }

    new (slot->data_ptr()) T(std::move(item));
    slot->sequence.store(tail + 1, std::memory_order_release);
    return true;
  }

  bool try_dequeue(T& out) {
    if (!_valid.load(std::memory_order_acquire)) return false;

    size_t head;
    Slot* slot;
    size_t index;
    size_t seq;

    while (true) {
      head = _head.load(std::memory_order_relaxed);
      index = head % _capacity;
      slot = &_buffer[index];

      seq = slot->sequence.load(std::memory_order_acquire);
      intptr_t diff = static_cast<intptr_t>(seq) - static_cast<intptr_t>(head + 1);

      if (diff == 0) {
        // Try to claim this slot
        if (_head.compare_exchange_weak(head, head + 1, std::memory_order_relaxed)) {
          break;  // we own the slot
        }
      } else if (diff < 0) {
        return false;  // not ready yet
      } else {
        std::this_thread::yield();  // spin
      }
    }

    out = std::move(*slot->data_ptr());
    slot->data_ptr()->~T();
    slot->sequence.store(head + _capacity, std::memory_order_release);
    return true;
  }

  size_t capacity() const noexcept { return _capacity; }

  size_t size_approx() const noexcept {
    return _tail.load(std::memory_order_relaxed) - _head.load(std::memory_order_relaxed);
  }

  bool is_valid() const noexcept {
    return _valid.load(std::memory_order_relaxed);
  }

 private:
  struct Slot {
    std::atomic<size_t> sequence;
    alignas(alignof(T)) unsigned char storage[sizeof(T)];

    T* data_ptr() noexcept {
      return std::launder(reinterpret_cast<T*>(&storage));
    }
  };

  size_t _capacity;
  Slot* _buffer;
  alignas(64) std::atomic<size_t> _head;
  alignas(64) std::atomic<size_t> _tail;
  std::atomic<bool> _valid{true};
};