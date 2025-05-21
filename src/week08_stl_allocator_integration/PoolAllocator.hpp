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
  struct Slot {
    Slot* next;
    alignas(std::max_align_t) std::byte userData[1];  // marker for where user memory begins
  };

 public:
  PoolAllocator(std::byte* buffer, size_t slotSize, size_t capacity)
      : _buffer(buffer), _slotSize(slotSize), _capacity(capacity), _usedCount(0) {
    // This is all to make sure that the buffer and the slotSize are properly
    // aligned

    assert(buffer != nullptr);
    assert(reinterpret_cast<uintptr_t>(_buffer) % alignof(std::max_align_t) == 0);
    assert(_slotSize >= _slotHeaderSize);
    assert(_slotSize % alignof(std::max_align_t) == 0);

    for (size_t i = 0; i < capacity - 1; ++i) {
      auto cur = reinterpret_cast<Slot*>(buffer + i * slotSize);
      auto next = reinterpret_cast<Slot*>(buffer + (i + 1) * slotSize);
      cur->next = next;
    }
    auto* last = reinterpret_cast<Slot*>(buffer + (capacity - 1) * slotSize);
    last->next = nullptr;
    _freeListHead = reinterpret_cast<Slot*>(buffer);
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

  template <typename T>
  static constexpr size_t slotSizeFor() {
    return align_up(offsetof(Slot, userData) + sizeof(T), alignof(std::max_align_t));
  }

  void* allocate() {
    assert(_usedCount < _capacity);
    if (_freeListHead == nullptr) {
      return nullptr;
    }

    Slot* slot = _freeListHead;
    _freeListHead = slot->next;
    ++_usedCount;
    void* ptr = userFromSlot(slot);
    std::cout << "Allocated: " << ptr << " from slot: " << static_cast<void*>(slot) << ", next: " << static_cast<void*>(slot->next) << "\n";

    return ptr;
  }

  void deallocate(void* ptr) {
    Slot* slot = slotFromUser(ptr);
    assert(owns(ptr));

    // Put the deallocated block at the front of the free list
    // and have it point to the previous head of the free list
    slot->next = _freeListHead;
    _freeListHead = slot;
    --_usedCount;
  }

  bool owns(void* ptr) const {
    // check if the ptr's slot is in range and that it is properly aligned
    auto* slotPtr = reinterpret_cast<std::byte*>(ptr) - _slotHeaderSize;
    uintptr_t base = reinterpret_cast<uintptr_t>(_buffer);
    uintptr_t p = reinterpret_cast<uintptr_t>(slotPtr);
    uintptr_t offset = p - base;

    return (slotPtr >= _buffer) &&
           (slotPtr < _buffer + (_slotSize * _capacity)) &&
           (offset % _slotSize == 0);
  }

  size_t slotSize() const { return _slotSize; }
  size_t capacity() const { return _capacity; }
  size_t used() const { return _usedCount; }

  size_t indexOf(void* ptr) const {
    auto* slotStart = reinterpret_cast<std::byte*>(ptr) - _slotHeaderSize;
    size_t offset = static_cast<size_t>(slotStart - _buffer);
    return offset / _slotSize;
  }

 private:
  std::byte* _buffer;
  size_t _slotSize;
  size_t _capacity;
  size_t _usedCount;
  Slot* _freeListHead;

  static constexpr size_t _slotHeaderSize = offsetof(Slot, userData);

  std::byte* userFromSlot(Slot* s) const {
    return reinterpret_cast<std::byte*>(s) + _slotHeaderSize;
  }

  Slot* slotFromUser(void* user) const {
    return reinterpret_cast<Slot*>(reinterpret_cast<std::byte*>(user) - _slotHeaderSize);
  }

  static constexpr size_t align_up(size_t size, size_t alignment) {
    return (size + alignment - 1) & ~(alignment - 1);
  }
};