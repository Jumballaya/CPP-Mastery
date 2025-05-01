#pragma once

#include <cassert>
#include <cstddef>
#include <iostream>

template <typename T, size_t N>
class RingBuffer {
 public:
  RingBuffer() noexcept : _count(0), _head(0), _tail(0) {}
  ~RingBuffer() noexcept {
    for (size_t i = 0; i < _count; ++i) {
      size_t idx = (_tail + i) % N;
      _buffer[idx].~T();
    }
  };

  RingBuffer(const RingBuffer&) = delete;
  RingBuffer& operator=(const RingBuffer&) = delete;

  RingBuffer(RingBuffer&& other) noexcept
      : _count(other._count), _head(other._head), _tail(other._tail) {
    for (size_t i = 0; i < _count; ++i) {
      size_t idx = (_tail + i) % N;
      _buffer[idx] = std::move(other._buffer[idx]);
    }
    other._count = 0;
    other._head = 0;
    other._tail = 0;
  }

  RingBuffer& operator=(RingBuffer&& other) noexcept {
    if (this == &other) return *this;
    _count = other._count;
    _head = other._head;
    _tail = other._tail;
    for (size_t i = 0; i < _count; ++i) {
      size_t idx = (_tail + i) % N;
      _buffer[idx] = std::move(other._buffer[idx]);
    }
    other._count = 0;
    other._head = 0;
    other._tail = 0;
    return *this;
  };

  void push(T&& value) {
    _head = _head % N;
    size_t idx = _head;
    _buffer[_head++] = std::move(value);
    _count = _count + 1 > N ? N : _count + 1;
    if (idx == _tail) {
      _tail = (_tail + 1) % N;
    }
  }

  T pop() {
    assert(_count > 0);

    size_t idx = _tail;
    _tail = (_tail + 1) % N;
    if (_tail == _head + 1) {
      _tail = _head;
    }
    if (_count > 0) _count--;
    return _buffer[idx];
  }

  T& front() {
    return _buffer[_tail];
  }

  T& back() {
    size_t backIndex = (_head + N - 1) % N;
    return _buffer[backIndex];
  }

  const T& front() const {
    return _buffer[_tail];
  }
  const T& back() const {
    size_t backIndex = (_head + N - 1) % N;
    return _buffer[backIndex];
  }

  T& operator[](size_t index) {
    size_t idx = (_tail + index) % N;
    return _buffer[idx];
  }

  const T& operator[](size_t index) const {
    size_t idx = (_tail + index) % N;
    return _buffer[idx];
  }

  bool empty() const { return _count == 0; }
  bool full() const { return _count == N; }

  size_t size() const { return _count; }
  size_t capacity() const { return N; }

 private:
  size_t _head;
  size_t _tail;
  size_t _count;
  T _buffer[N];
};