#pragma once
#include <cstddef>
#include <iostream>

//
//    FOR MONDAY:
//
//    1. add _capacity and _size
//        a. capacity: total bytes alloc | size: how many bytes in use
//    2. write() should update size if written to a place past current size
//        a. e.g. if size is 3 and .write(6, 'f') is called, the size is 7 (indices 0->6)
//        b. if clear() is called, reset size to 0, but keep the alloc'd memory
//    3. resize(size_t newCapacity) added
//        a. if newCapacity > capacity then alloc new memory and memcpy old contents
//        b. zero out new memory if expanding
//        c. update capacity
//    4. append(const char* data, size_t len)
//        a. append len bytes to after _size
//        b. expands via resize() if needed
//        c. returns true if append succeeded
//        d. returns false if it would exceed a MAX_CAPACITY set outside the class
//    5. Type safe read/write
//
//            template <typename T>
//            void write(size_t offset, const T& value);
//
//            template <typename T>
//            T read(size_t offset) const;
//
//        a. write uses std::memcpy
//        b. Read returns a copy of the value at that offset
//        c. Include bounds checking (offset + sizeof(T) must fit)
//        d. Add static_assert on trivially copyable types
//        e. Bonus: add a read_into<T>(size_t offset, T& out) to avoid copies
//    6. Clear method
//        a. void clear()
//        b. Resets _size = 0
//        c. Leaves _capacity untouched (no dealloc)
//        d. Optionally zero memory if a flag is set
//    7. Debug Output
//        a. friend std::ostream& operator<<(std::ostream& os, const MoveOnlyBuffer& buf);
//        b. Example output:
//            i. MoveOnlyBuffer(size=6, capacity=32): [48 45 4C 4C 4F 0A]
//        c. Use hex or ASCII view
//        d. Print size and capacity
//
//
//

struct MoveOnlyBuffer {
  MoveOnlyBuffer(size_t size) : _size(size) {
    data = new char[size];
    std::fill(data, data + size, 0);
  }

  ~MoveOnlyBuffer() {
    delete[] data;
  }

  // Copy Constructor/Assignment
  MoveOnlyBuffer(const MoveOnlyBuffer&) = delete;
  MoveOnlyBuffer& operator=(const MoveOnlyBuffer&) = delete;

  // Move Constructor/Assignment
  MoveOnlyBuffer(MoveOnlyBuffer&& other) noexcept
      : data(other.data), _size(other._size) {
    other.data = nullptr;
    other._size = 0;
  };

  MoveOnlyBuffer& operator=(MoveOnlyBuffer&& other) noexcept {
    if (this == &other) return *this;
    delete[] data;
    data = other.data;
    _size = other._size;
    other.data = nullptr;
    other._size = 0;
    return *this;
  };

  void write(size_t index, char value) {
    if (_size == 0) return;
    if (index >= _size) return;
    data[index] = value;
  };

  char read(size_t index) const {
    if (_size == 0) return '\0';
    if (index >= _size) return '\0';
    return data[index];
  };

 private:
  char* data = nullptr;
  size_t _size = 0;
};