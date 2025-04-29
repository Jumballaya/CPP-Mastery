#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>

template <typename T, size_t N>
class FixedVector {
 public:
  FixedVector() noexcept;
  ~FixedVector();

  FixedVector(const FixedVector&) noexcept;
  FixedVector(FixedVector&&) noexcept;

  FixedVector& operator=(const FixedVector&) noexcept;
  FixedVector& operator=(FixedVector&&) noexcept;

  T& operator[](size_t index) noexcept;
  const T& operator[](size_t index) const noexcept;

  T& at(size_t index);
  const T& at(size_t index) const;

  void push_back(const T&);
  void push_back(T&&);
  void pop_back();

  T* begin() noexcept;
  T* end() noexcept;
  const T* begin() const noexcept;
  const T* end() const noexcept;

  size_t size() const noexcept { return _size; }
  size_t capacity() const noexcept { return N; };
  bool empty() const noexcept { return _size == 0; };

 private:
  alignas(T) unsigned char _storage[sizeof(T) * N];
  size_t _size = 0;

  T* ptr(size_t i) noexcept { return reinterpret_cast<T*>(&_storage[sizeof(T) * i]); }
  const T* ptr(size_t i) const noexcept { return reinterpret_cast<const T*>(&_storage[sizeof(T) * i]); }
};

template <typename T, size_t N>
FixedVector<T, N>::FixedVector() noexcept : _size(0) {}

template <typename T, size_t N>
FixedVector<T, N>::~FixedVector() {
  for (size_t i = 0; i < _size; ++i) {
    ptr(i)->~T();
  }
}

template <typename T, size_t N>
FixedVector<T, N>::FixedVector(const FixedVector<T, N>& other) noexcept : _size(other._size) {
  for (size_t i = 0; i < _size; ++i) {
    new (ptr(i)) T(*other.ptr(i));
  }
}

template <typename T, size_t N>
FixedVector<T, N>::FixedVector(FixedVector<T, N>&& other) noexcept : _size(other._size) {
  for (size_t i = 0; i < _size; ++i) {
    new (ptr(i)) T(std::move(*other.ptr(i)));
  }
  other._size = 0;
}

template <typename T, size_t N>
FixedVector<T, N>& FixedVector<T, N>::operator=(const FixedVector<T, N>& other) noexcept {
  if (this == &other) return *this;
  this->~FixedVector();
  _size = other._size;
  for (size_t i = 0; i < _size; ++i) {
    new (ptr(i)) T(*other.ptr(i));
  }
  return *this;
}

template <typename T, size_t N>
FixedVector<T, N>& FixedVector<T, N>::operator=(FixedVector<T, N>&& other) noexcept {
  if (this == &other) return *this;
  this->~FixedVector();
  _size = other._size;
  for (size_t i = 0; i < _size; ++i) {
    new (ptr(i)) T(std::move(*other.ptr(i)));
  }
  other._size = 0;
  return *this;
}

template <typename T, size_t N>
T& FixedVector<T, N>::operator[](size_t index) noexcept {
  return *ptr(index);
}

template <typename T, size_t N>
const T& FixedVector<T, N>::operator[](size_t index) const noexcept {
  return *ptr(index);
}

template <typename T, size_t N>
T& FixedVector<T, N>::at(size_t index) {
  if (index >= _size) {
    throw std::out_of_range("FixedVector index out of range");
  }
  return *ptr(index);
}

template <typename T, size_t N>
const T& FixedVector<T, N>::at(size_t index) const {
  if (index >= _size) {
    throw std::out_of_range("FixedVector index out of range");
  }
  return *ptr(index);
}

template <typename T, size_t N>
void FixedVector<T, N>::push_back(const T& item) {
  if (_size + 1 > N) return;
  new (ptr(_size)) T(item);
  ++_size;
}

template <typename T, size_t N>
void FixedVector<T, N>::push_back(T&& item) {
  if (_size + 1 > N) return;
  new (ptr(_size)) T(std::forward<T>(item));
  ++_size;
}

template <typename T, size_t N>
void FixedVector<T, N>::pop_back() {
  if (_size > 0) {
    --_size;
    ptr(_size)->~T();
  }
}

template <typename T, size_t N>
T* FixedVector<T, N>::begin() noexcept {
  return ptr(0);
}

template <typename T, size_t N>
T* FixedVector<T, N>::end() noexcept {
  return ptr(_size);
}

template <typename T, size_t N>
const T* FixedVector<T, N>::begin() const noexcept {
  return ptr(0);
}

template <typename T, size_t N>
const T* FixedVector<T, N>::end() const noexcept {
  return ptr(_size);
}