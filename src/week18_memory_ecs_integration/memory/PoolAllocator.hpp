#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <span>
#include <type_traits>

template <typename T>
class PoolAllocator {
  static_assert(std::is_trivially_destructible_v<T>, "PoolAllocator only supports trivially destructible types.");
  static_assert(!std::is_abstract_v<T>, "PoolAllocator cannot allocate abstract types.");

 public:
  explicit PoolAllocator(size_t capacity)
      : _buffer(new std::byte[capacity * sizeof(T)]), _capacity(capacity), _ownsMemory(true) {
    assert(capacity > 0 && "Capacity must be larger than 0");
    initializeFreeList();
  }

  explicit PoolAllocator(std::span<std::byte> backing)
      : _buffer(backing.data()), _capacity(backing.size() / sizeof(T)), _ownsMemory(false) {
    assert(backing.size() % sizeof(T) == 0 && "Backing span must be evenly divisible by sizeof(T)");
    initializeFreeList();
  }

  ~PoolAllocator() {
    if (_ownsMemory) {
      delete[] _buffer;
    }
  }

  T* allocate() {
    if (!_freeList) return nullptr;
    FreeNode* node = _freeList;
    _freeList = node->next;
    ++_count;
    return reinterpret_cast<T*>(node);
  }

  void deallocate(T* ptr) {
    if (!ptr) return;

    auto* node = reinterpret_cast<FreeNode*>(ptr);
    node->next = _freeList;
    _freeList = node;
    --_count;
  }

  void reset() {
    _count = 0;
    initializeFreeList();
  }

  size_t capacity() const { return _capacity; }
  size_t size() const { return _count; }
  bool ownsMemory() const { return _ownsMemory; }

 private:
  struct FreeNode {
    FreeNode* next = nullptr;
  };

  std::byte* _buffer = nullptr;
  FreeNode* _freeList = nullptr;
  size_t _capacity = 0;
  size_t _count = 0;
  bool _ownsMemory = false;

  void initializeFreeList() {
    _freeList = reinterpret_cast<FreeNode*>(_buffer);

    for (size_t i = 0; i < _capacity - 1; ++i) {
      auto* current = reinterpret_cast<FreeNode*>(_buffer + i * sizeof(T));
      auto* next = reinterpret_cast<FreeNode*>(_buffer + (i + 1) * sizeof(T));
      current->next = next;
    }

    auto* last = reinterpret_cast<FreeNode*>(_buffer + (_capacity - 1) * sizeof(T));
    last->next = nullptr;
  }
};