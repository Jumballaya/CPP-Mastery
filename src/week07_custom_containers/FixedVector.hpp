#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>

template <typename T, size_t N>
class FixedVector {
 public:
  FixedVector() noexcept : _size(0) {}
  ~FixedVector() {
    for (size_t i = 0; i < _size; ++i) {
      ptr(i)->~T();
    }
  }

  FixedVector(const FixedVector& other) noexcept : _size(other._size) {
    for (size_t i = 0; i < _size; ++i) {
      new (ptr(i)) T(*other.ptr(i));
    }
  }

  FixedVector(FixedVector&& other) noexcept : _size(other._size) {
    for (size_t i = 0; i < _size; ++i) {
      new (ptr(i)) T(std::move(*other.ptr(i)));
    }
    other._size = 0;
  }

  FixedVector& operator=(const FixedVector& other) noexcept {
    if (this == &other) return *this;
    this->~FixedVector();
    _size = other._size;
    for (size_t i = 0; i < _size; ++i) {
      new (ptr(i)) T(*other.ptr(i));
    }
    return *this;
  }

  FixedVector& operator=(FixedVector&& other) noexcept {
    if (this == &other) return *this;
    this->~FixedVector();
    _size = other._size;
    for (size_t i = 0; i < _size; ++i) {
      new (ptr(i)) T(std::move(*other.ptr(i)));
    }
    other._size = 0;
    return *this;
  }

  T& operator[](size_t index) noexcept {
    return *ptr(index);
  }

  const T& operator[](size_t index) const noexcept {
    return *ptr(index);
  }

  T& at(size_t index) {
    if (index >= _size) {
      throw std::out_of_range("FixedVector index out of range");
    }
    return *ptr(index);
  }

  const T& at(size_t index) const {
    if (index >= _size) {
      throw std::out_of_range("FixedVector index out of range");
    }
    return *ptr(index);
  }

  void push_back(const T& item) {
    if (_size + 1 > N) return;
    new (ptr(_size)) T(item);
    ++_size;
  }

  void push_back(T&& item) {
    if (_size + 1 > N) return;
    new (ptr(_size)) T(std::forward<T>(item));
    ++_size;
  }

  void pop_back() {
    if (_size > 0) {
      --_size;
      ptr(_size)->~T();
    }
  }

  T* begin() noexcept { return ptr(0); }
  T* end() noexcept { return ptr(_size); }
  const T* begin() const noexcept { return ptr(0); }
  const T* end() const noexcept { return ptr(_size); }

  size_t size() const noexcept { return _size; }
  size_t capacity() const noexcept { return N; };
  bool empty() const noexcept { return _size == 0; };

 private:
  alignas(T) unsigned char _storage[sizeof(T) * N];
  size_t _size = 0;

  T* ptr(size_t i) noexcept { return reinterpret_cast<T*>(&_storage[sizeof(T) * i]); }
  const T* ptr(size_t i) const noexcept { return reinterpret_cast<const T*>(&_storage[sizeof(T) * i]); }
};