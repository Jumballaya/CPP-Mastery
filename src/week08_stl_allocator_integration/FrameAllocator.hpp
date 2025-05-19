#pragma once

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <stdexcept>

class FrameAllocator {
 public:
  FrameAllocator(std::byte* buffer, size_t capacity)
      : _buffer(buffer), _capacity(capacity), _offset(0) {
    assert(capacity != 0);
    assert(buffer != nullptr);
  }

  ~FrameAllocator() = default;
  FrameAllocator(FrameAllocator& const other) = delete;
  FrameAllocator& operator=(FrameAllocator& const other) = delete;
  FrameAllocator(FrameAllocator&& const other) = delete;
  FrameAllocator& operator=(FrameAllocator&& const other) = delete;

  void* allocate(size_t size, size_t alignment) {
    assert(alignment >= alignof(std::max_align_t));
    assert((alignment & (alignment - 1)) == 0);  // alignment is a power of 2

    size_t aligned_offset = (_offset + alignment - 1) & ~(alignment - 1);
    if (aligned_offset + size <= _capacity) {
      _offset = aligned_offset + size;
      return _buffer + aligned_offset;
    }
    return nullptr;
  }

  void reset() { _offset = 0; }

  size_t capacity() const { return _capacity; }
  size_t used() const { return _offset; }
  size_t remaining() const { return _capacity - _offset; }

  void deallocate() {}  // no-op required by std::pmr

 private:
  std::byte* _buffer;
  std::size_t _capacity;
  std::size_t _offset;
};