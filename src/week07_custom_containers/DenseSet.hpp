#pragma once

#include <cstddef>

template <typename T, size_t N>
class DenseSet {
 public:
  DenseSet() noexcept : _size(0) {}
  ~DenseSet() = default;

  DenseSet(const DenseSet& other) noexcept : _size(other._size) {
    for (size_t i = 0; i < N; ++i) {
      _dense[i] = other._dense[i];
      _sparse[i] = other._sparse[i];
      _occupied[i] = other._occupied[i];
    }
  }
  DenseSet(DenseSet&& other) noexcept : _size(other._size) {
    for (size_t i = 0; i < N; ++i) {
      _dense[i] = std::move(other._dense[i]);
      _sparse[i] = other._sparse[i];
      _occupied[i] = other._occupied[i];
    }
    other._size = 0;
  }

  DenseSet& operator=(const DenseSet& other) noexcept {
    if (this == &other) return *this;
    _size = other._size;
    for (size_t i = 0; i < N; ++i) {
      _dense[i] = other._dense[i];
      _sparse[i] = other._sparse[i];
      _occupied[i] = other._occupied[i];
    }
    return *this;
  }

  DenseSet& operator=(DenseSet&& other) noexcept {
    if (this == &other) return *this;
    _size = other._size;
    for (size_t i = 0; i < N; ++i) {
      _dense[i] = std::move(other._dense[i]);
      _sparse[i] = other._sparse[i];
      _occupied[i] = other._occupied[i];
    }
    other._size = 0;
    return *this;
  }

  bool insert(T value) {
    if (value >= N) return false;
    if (_occupied[value]) return false;

    size_t index = _size++;
    _dense[index] = value;
    _sparse[value] = index;
    _occupied[value] = true;
    return true;
  }

  bool erase(const T& value) {
    if (value >= N || !_occupied[value]) return false;
    _occupied[value] = false;

    size_t index = _sparse[value];
    T last = _dense[_size - 1];
    _dense[index] = last;
    _sparse[last] = index;

    --_size;
    return true;
  }

  bool contains(const T& value) {
    if (value >= N) return false;
    return _occupied[value];
  }

  size_t size() const { return _size; }
  bool empty() const { return _size == 0; }

  T* begin() noexcept {
    return &_dense[0];
  }

  T* end() noexcept {
    return &_dense[_size];
  }

  const T* begin() const noexcept {
    return &_dense[0];
  }
  const T* end() const noexcept {
    return &_dense[_size];
  }

 private:
  T _dense[N];
  size_t _size;
  size_t _sparse[N];
  bool _occupied[N] = {};
};