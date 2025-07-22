#include "FrameArena.hpp"

FrameArena::FrameArena(size_t size)
    : _start(new std::byte[size]), _ptr(_start), _size(size) {
  assert((size & (size - 1)) == 0);  // size is a power of 2
}

FrameArena::FrameArena(void* buffer, size_t size)
    : _start(reinterpret_cast<std::byte*>(buffer)),
      _ptr(_start),
      _size(size) {}

FrameArena::FrameArena(std::span<std::byte> backing)
    : _start(backing.data()),
      _ptr(_start),
      _size(backing.size()) {}

void* FrameArena::allocateRaw(size_t bytes, size_t alignment) {
  assert(bytes > 0);
  assert((alignment & (alignment - 1)) == 0);  // alignment is a power of 2
  size_t offset = reinterpret_cast<size_t>(_ptr);
  size_t alignmentPadding = ((offset + alignment - 1) & ~(alignment - 1)) - offset;
  size_t alignedSize = alignmentPadding + bytes;

  if (_ptr + alignedSize <= _start + _size) {
    void* result = _ptr + alignmentPadding;
    _ptr += alignedSize;
    return result;
  }
  return nullptr;
}

void FrameArena::reset() { _ptr = _start; }
size_t FrameArena::used() const { return static_cast<size_t>(_ptr - _start); }
size_t FrameArena::capacity() const { return _size; }
size_t FrameArena::remaining() const { return _size - used(); }
