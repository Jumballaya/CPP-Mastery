#pragma once
#include <cstddef>
#include <cstring>
#include <iomanip>
#include <iostream>

constexpr size_t MAX_CAPACITY = 0xFFFFFFFFFFFF;

struct MoveOnlyBuffer {
  MoveOnlyBuffer(size_t capacity) : _capacity(capacity), _data(new char[capacity]) {
    std::fill(_data, _data + capacity, 0);
  }

  ~MoveOnlyBuffer() {
    delete[] _data;
  }

  // Copy Constructor/Assignment
  MoveOnlyBuffer(const MoveOnlyBuffer&) = delete;
  MoveOnlyBuffer& operator=(const MoveOnlyBuffer&) = delete;

  // Move Constructor/Assignment
  MoveOnlyBuffer(MoveOnlyBuffer&& other) noexcept
      : _data(other._data), _capacity(other._capacity), _size(other._size) {
    other._data = nullptr;
    other._capacity = 0;
    other._size = 0;
  };

  MoveOnlyBuffer& operator=(MoveOnlyBuffer&& other) noexcept {
    if (this == &other) return *this;
    delete[] _data;
    _data = other._data;
    _capacity = other._capacity;
    _size = other._size;
    other._data = nullptr;
    other._capacity = 0;
    other._size = 0;
    return *this;
  };

  friend std::ostream& operator<<(std::ostream& os, const MoveOnlyBuffer& buf) {
    os << "MoveOnlyBuffer(size=" << buf._size
       << ", capacity=" << buf._capacity << "): [";

    for (size_t i = 0; i < buf._size; i++) {
      if (i != 0) os << ' ';
      os << "0x" << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
         << static_cast<int>(static_cast<unsigned char>(buf._data[i]));
    }

    os << std::dec << std::nouppercase << "]";
    return os;
  }

  size_t size() {
    return _size;
  }

  template <typename T>
  void write(size_t index, const T& value) {
    if (_capacity == 0) return;
    if (index + sizeof(T) >= _capacity) return;
    std::memcpy(_data + index, &value, sizeof(T));
    if (index + sizeof(T) >= _size) {
      _size = index + sizeof(T);
    }
  };

  template <typename T>
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
  char* _data = nullptr;
  size_t _size = 0;
  size_t _capacity = 0;
};