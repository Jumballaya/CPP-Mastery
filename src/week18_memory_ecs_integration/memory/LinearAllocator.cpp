#include "LinearAllocator.hpp"

LinearAllocator::LinearAllocator(size_t size)
    : _start(new std::byte[size]), _ptr(_start), _size(size), _ownsMemory(true) {
  assert(size > 0 && "LinearAllocator size must be greater than 0");
}

LinearAllocator::LinearAllocator(std::span<std::byte> span)
    : _start(span.data()), _ptr(_start), _size(span.size()), _ownsMemory(false) {
  assert(span.size() > 0 && "LinearAllocator size must be greater than 0");
}

LinearAllocator::~LinearAllocator() {
  if (_ownsMemory) {
    delete[] _start;
  }
}

void* LinearAllocator::allocateRaw(size_t bytes, size_t alignment) {
  assert(bytes > 0);
  assert((alignment & (alignment - 1)) == 0);  // power of two alignment check

  uintptr_t current = reinterpret_cast<uintptr_t>(_ptr);
  uintptr_t aligned = (current + alignment - 1) & ~(alignment - 1);
  uintptr_t newPtr = aligned + bytes;

  if (newPtr > reinterpret_cast<uintptr_t>(_start) + _size) {
    return nullptr;
  }

  _ptr = reinterpret_cast<std::byte*>(newPtr);
  return reinterpret_cast<void*>(aligned);
}

void LinearAllocator::reset() {
  _ptr = _start;
}

size_t LinearAllocator::used() const { return static_cast<size_t>(_ptr - _start); }
size_t LinearAllocator::capacity() const { return _size; }
size_t LinearAllocator::remaining() const { return _size - used(); }
bool LinearAllocator::ownsMemory() const { return _ownsMemory; }
