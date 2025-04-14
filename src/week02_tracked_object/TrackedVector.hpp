#pragma once
#include <cstddef>
#include <cstring>
#include <iostream>

template <typename T>
class TrackedVector {
 public:
  TrackedVector() : _capacity(0), _size(0), _data(new T[0]) {
    std::cout << "[Vector] Constructor Called" << std::endl;
  }

  ~TrackedVector() {
    std::cout << "[Vector] Destructor Called" << std::endl;
    delete[] _data;
  }

  TrackedVector(const TrackedVector& other) noexcept : _capacity(other._capacity), _size(other._size), _data(new T[other._capacity]) {
    if (this == &other) return;
    std::cout << "[Vector] Copy Constructor Called" << std::endl;
    std::memcpy(_data, other._data, _capacity);
  }

  TrackedVector(TrackedVector&& other) noexcept : _capacity(other._capacity), _size(other._size), _data(other._data) {
    if (this == &other) return;
    std::cout << "[Vector] Move Constructor Called" << std::endl;
    other._capacity = 0;
    other._size = 0;
    other._data = nullptr;
  }

  TrackedVector& operator=(const TrackedVector& other) noexcept {
    std::cout << "[Vector] Copy Assignment Called" << std::endl;
    if (this == &other) return *this;
    delete[] _data;
    _capacity = other._capacity;
    _size = other._size;
    _data = new T[_capacity];
    std::memcpy(_data, other._data, _capacity);
    return *this;
  }

  TrackedVector& operator=(TrackedVector&& other) noexcept {
    std::cout << "[Vector] Move Assignment Called" << std::endl;
    if (this == &other) return *this;
    delete[] _data;

    _capacity = other._capacity;
    _size = other._size;
    _data = other._data;

    other._capacity = 0;
    other._size = 0;
    other._data = nullptr;
    return *this;
  }

  void push_back(const T& item) {
    if (_size >= _capacity) {
      resize((_capacity + 1) * 2);
    }
    _data[_size] = item;
    _size++;
  }

  void resize(size_t newCapacity) {
    if (newCapacity <= _capacity) return;
    T* newData = new T[newCapacity];
    if (newData == nullptr) return;
    std::memcpy(newData, _data, _size * sizeof(T));
    delete[] _data;
    _data = newData;
    _capacity = newCapacity;
  }

  void clear() {
    _size = 0;
  }

  const T* at(size_t index) const {
    if (index > _size - 1) return nullptr;
    return &_data[index];
  }

 private:
  size_t _size;
  size_t _capacity;
  T* _data = nullptr;
};