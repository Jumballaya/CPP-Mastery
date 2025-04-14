#pragma once

#include <cstring>
#include <iomanip>
#include <iostream>

#include "TrackedObject.hpp"

constexpr size_t MAX_CAPACITY = 0xFFFFFFFFFFFF;

class TrackedBuffer : public TrackedObject {
 public:
  TrackedBuffer(std::string_view label, size_t capacity)
      : TrackedObject(label),
        _capacity(capacity),
        _data(new char[capacity]) {
    std::fill(_data, _data + _capacity, 0);
  }

  ~TrackedBuffer() {
    delete[] _data;
  }

  TrackedBuffer(const TrackedBuffer& other) noexcept
      : TrackedObject(other._name),
        _capacity(other._capacity),
        _size(other._size),
        _data(new char[_capacity]) {
    std::memcpy(_data, other._data, _capacity);
  }

  TrackedBuffer& operator=(const TrackedBuffer& other) noexcept {
    TrackedObject::operator=(other);
    if (this == &other) return *this;
    _capacity = other._capacity;
    _size = other._size;
    delete[] _data;
    _data = new char[_capacity];
    std::memcpy(_data, other._data, _capacity);
    return *this;
  }

  TrackedBuffer(TrackedBuffer&& other) noexcept
      : TrackedObject(std::move(other._name)),
        _capacity(other._capacity),
        _size(other._size),
        _data(other._data) {
    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
  }

  TrackedBuffer& operator=(TrackedBuffer&& other) noexcept {
    TrackedObject::operator=(other);
    if (this == &other) return *this;
    _capacity = other._capacity;
    _size = other._size;
    delete[] _data;
    _data = other._data;
    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
    return *this;
  }

  constexpr size_t size() { return _size; }
  constexpr size_t capacity() { return _capacity; }

  friend std::ostream& operator<<(std::ostream& os, const TrackedBuffer& buf) {
    os << "TrackedBuffer(size=" << buf._size << ", capacity=" << buf._capacity << "): [";
    for (size_t i = 0; i < buf._size; i++) {
      if (i != 0) os << ' ';
      os << "0x" << std::hex << std::uppercase
         << std::setw(2) << std::setfill('0')
         << static_cast<int>(buf._data[i]);
    }
    os << std::dec << std::nouppercase << "]";
    return os;
  }

  template <typename T>
  void write(size_t index, const T& value) {
    if (_capacity == 0) return;
    if (index + sizeof(T) > _capacity) return;
    std::memcpy(_data + index, &value, sizeof(T));
    if (index + sizeof(T) > _size) {
      _size = index + sizeof(T);
    }
  };

  template <typename T>
  [[nodiscard]]
  T* read(size_t index) const {
    if (_capacity == 0) return nullptr;
    if (index >= _capacity) return nullptr;
    return (T*)(_data + index);
  };

  void clear() {
    std::fill(_data, _data + _capacity, 0);
    _size = 0;
  }

  bool resize(size_t newCapacity) {
    if (newCapacity <= _capacity) return false;
    char* newData = new char[newCapacity];
    if (newData == nullptr) return false;
    std::fill(newData, newData + newCapacity, 0);
    std::memcpy(newData, _data, _capacity);
    _capacity = newCapacity;
    delete[] _data;
    _data = newData;
    return true;
  }

  bool append(const char* data, size_t len) {
    if (len == 0 || !data || _size + len > MAX_CAPACITY) return false;
    if (_size + len >= _capacity) {
      resize(std::max(_size * 2, _size + len));
    }
    std::memcpy(_data + _size, data, len);
    _size += len;
    return true;
  }

 private:
  size_t _size = 0;  // current pointer
  size_t _capacity;
  char* _data;
};