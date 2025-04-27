#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>

template <size_t N>
class SmallAllocator {
 public:
  SmallAllocator() : _offset(0) {}
  ~SmallAllocator() = default;

  void* allocate(size_t size, size_t alignment) {
    size_t padding = (alignment - (_offset % alignment)) % alignment;
    size_t aligned_offset = _offset + padding;
    if (aligned_offset + size > N) {
      return nullptr;  // too large, out of space
    }

    void* ptr = &_buffer[aligned_offset];
    _offset = aligned_offset + size;
    return ptr;
  }

  void reset() { _offset = 0; }

 private:
  alignas(std::max_align_t) unsigned char _buffer[N];
  size_t _offset;
};