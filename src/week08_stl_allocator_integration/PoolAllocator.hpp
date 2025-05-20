#pragma once

#include <cstddef>
#include <cstdlib>

class PoolAllocator {
 public:
  PoolAllocator(std::byte* buffer, size_t slotSize, size_t capacity)
      : _buffer(buffer), _slotSize(slotSize), _capacity(capacity), _usedCount(0) {
    auto tail = _freeListHead;
    for (size_t i = 0; i < capacity; ++i) {
    }
  }

  ~PoolAllocator() = default;
  PoolAllocator(const PoolAllocator&) = delete;
  PoolAllocator& operator=(const PoolAllocator&) = delete;

  PoolAllocator(PoolAllocator&& other);
  PoolAllocator& operator=(PoolAllocator&& other);

  void* allocate();
  void deallocate(void* ptr);
  bool owns(void* ptr) const;

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