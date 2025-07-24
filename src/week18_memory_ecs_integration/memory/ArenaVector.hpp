#pragma once

#include <cstddef>
#include <cstring>  // for memcpy
#include <span>
#include <utility>

#include "FrameArena.hpp"

template <typename T>
class ArenaVector {
 public:
  ArenaVector() = default;
  ~ArenaVector() = default;

  explicit ArenaVector(FrameArena* arena, size_t initialCapacity = 1)
      : _arena(arena), _capacity(initialCapacity), _data(_arena->allocate<T>(initialCapacity)) {
    assert(_data && "Arena out of memory");
  }

  ArenaVector(const ArenaVector&) noexcept = delete;
  ArenaVector& operator=(const ArenaVector&) noexcept = delete;

  ArenaVector(ArenaVector&& other) noexcept
      : _arena(other.arena), _data(other.data), _size(other.size), _capacity(other.capacity) {
    other._arena = nullptr;
    other._data = nullptr;
    other.size = 0;
    other.capacity = 0;
  }
  ArenaVector& operator=(ArenaVector&& other) noexcept {
    if (&other == this) {
      return *this;
    }

    _arena = other._arena;
    _data = other._data;
    _size = other._size;
    _capacity = other._capacity;

    other._arena = nullptr;
    other._data = nullptr;
    other.size = 0;
    other.capacity = 0;

    return *this;
  }

  T& operator[](size_t i) {
    assert(i < _size);
    return _data[i];
  }

  const T& operator[](size_t i) const {
    assert(i < _size);
    return _data[i];
  }

  void push_back(const T& value) {
    if (_size >= _capacity) {
      grow();
    }
    _data[_size++] = value;
  }

  template <typename... Args>
  T& emplace_back(Args&&... args) {
    if (_size >= _capacity) {
      grow();
    }
    T* target = &_data[_size++];
    new (target) T(std::forward<Args>(args)...);
    return *target;
  }

  void clear() {
    _size = 0;
  }

  T* begin() { return _data; }
  T* end() { return _data + _size; }

  const T* begin() const { return _data; }
  const T* end() const { return _data + _size; }

  std::span<T> span() { return {_data, _size}; }
  const T* data() const { return _data; }
  size_t size() const { return _size; }
  size_t capacity() const { return _capacity; }
  bool empty() const { return _size > 0; }

 private:
  void grow() {
    size_t newCapacity = _capacity * 2;
    T* newData = _arena->allocate<T>(newCapacity);
    assert(newData && "Arena out of memory");
    std::memcpy(newData, _data, sizeof(T) * _size);
    _data = newData;
    _capacity = newCapacity;
  }

  FrameArena* _arena = nullptr;
  T* _data = nullptr;
  size_t _size = 0;
  size_t _capacity = 0;
};