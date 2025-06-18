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
  ~LockFreeQueue();

  LockFreeQueue(const LockFreeQueue&) = delete;
  LockFreeQueue& operator=(const LockFreeQueue&) = delete;

  bool try_enqueue(const T& item);
  bool try_dequeue(T& out);

  size_t capacity() const noexcept;
  size_t size_approx() const noexcept;

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