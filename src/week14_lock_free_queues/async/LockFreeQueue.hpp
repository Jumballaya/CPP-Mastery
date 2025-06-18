#pragma once

#include <atomic>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <new>
#include <type_traits>

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
  // ~LockFreeQueue();

  LockFreeQueue(const LockFreeQueue&) = delete;
  LockFreeQueue& operator=(const LockFreeQueue&) = delete;

  bool try_enqueue(const T& item) {
    size_t tail = _tail.fetch_add(1, std::memory_order_relaxed);
    size_t index = tail % _capacity;
    Slot& slot = _buffer[index];

    size_t seq = slot.sequence.load(std::memory_order_acquire);

    // Slot is not ready yet
    if (seq != tail) {
      return false;
    }

    new (slot.data_ptr()) T(item);
    slot.sequence.store(tail + 1, std::memory_order_release);
    return true;
  }

  bool try_dequeue(T& out) {
    size_t head = _head.fetch_add(1, std::memory_order_relaxed);
    size_t index = head % _capacity;
    Slot& slot = _buffer[index];

    size_t seq = slot.sequence.load(std::memory_order_acquire);

    // Slot is not ready (needs to be read)
    if (seq != head + 1) {
      return false;
    }

    out = std::move(*slot.data_ptr());
    slot.data_ptr()->~T();
    slot.sequence.store(head + _capacity, std::memory_order_release);
    return true;
  }

  // size_t capacity() const noexcept;
  // size_t size_approx() const noexcept;

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
};