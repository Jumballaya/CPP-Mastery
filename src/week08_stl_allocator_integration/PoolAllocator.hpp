#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>

// PoolAllocator is non-owning. It is safe to move, but the caller must ensure
// the backing buffer remains valid for the lifetime of all moved-from and
// moved-to instances.
class PoolAllocator {
 public:
  PoolAllocator(std::byte* buffer, size_t slotSize, size_t capacity)
      : _buffer(buffer), _slotSize(slotSize), _capacity(capacity), _usedCount(0) {
    // This is all to make sure that the buffer and the slotSize are properly
    // aligned
    assert(buffer != nullptr);
    assert(reinterpret_cast<uintptr_t>(_buffer) % alignof(std::max_align_t) == 0);
    assert(_slotSize >= sizeof(void*));
    assert(_slotSize % alignof(std::max_align_t) == 0);

    for (size_t i = 0; i < capacity - 1; ++i) {
      std::byte* cur = buffer + i * slotSize;
      std::byte* next = buffer + (i + 1) * slotSize;
      *reinterpret_cast<void**>(cur) = next;
    }
    void* last = _buffer + (_capacity - 1) * _slotSize;
    *reinterpret_cast<void**>(last) = nullptr;
    _freeListHead = buffer;
  }

  ~PoolAllocator() = default;
  PoolAllocator(const PoolAllocator&) = delete;
  PoolAllocator& operator=(const PoolAllocator&) = delete;

  PoolAllocator(PoolAllocator&& other)
      : _buffer(other._buffer),
        _slotSize(other._slotSize),
        _capacity(other._capacity),
        _usedCount(other._usedCount),
        _freeListHead(other._freeListHead) {
    other._slotSize = 0;
    other._capacity = 0;
    other._usedCount = 0;
    other._freeListHead = nullptr;
  }
  PoolAllocator& operator=(PoolAllocator&& other) {
    if (this == &other) return *this;

    _buffer = other._buffer;
    _slotSize = other._slotSize;
    _capacity = other._capacity;
    _usedCount = other._usedCount;
    _freeListHead = other._freeListHead;

    other._buffer = nullptr;
    other._slotSize = 0;
    other._capacity = 0;
    other._usedCount = 0;
    other._freeListHead = nullptr;

    return *this;
  }

  void* allocate() {
    assert(_usedCount < _capacity);
    if (_freeListHead == nullptr) {
      return nullptr;
    }
    void* result = _freeListHead;
    _freeListHead = *reinterpret_cast<void**>(_freeListHead);  // move to the next
    _usedCount++;
    return result;
  }

  void deallocate(void* ptr) {
    assert(owns(ptr));

    // Put the deallocated block at the front of the free list
    // and have it point to the previous head of the free list
    *reinterpret_cast<void**>(ptr) = _freeListHead;
    _freeListHead = ptr;
    _usedCount--;
  }

  bool owns(void* ptr) const {
    // check if the ptr is in range and that it is properly aligned
    uintptr_t base = reinterpret_cast<uintptr_t>(_buffer);
    uintptr_t p = reinterpret_cast<uintptr_t>(ptr);
    uintptr_t offset = p - base;

    return (ptr >= _buffer) && (ptr < _buffer + (_slotSize * _capacity)) &&
           (offset % _slotSize == 0);
  }

  size_t slotSize() const { return _slotSize; }
  size_t capacity() const { return _capacity; }
  size_t used() const { return _usedCount; }

 private:
  std::byte* _buffer;
  size_t _slotSize;
  size_t _capacity;
  size_t _usedCount;
  void* _freeListHead;
};