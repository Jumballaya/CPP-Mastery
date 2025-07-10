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
  explicit PoolAllocator(size_t capacity);
  explicit PoolAllocator(std::span<std::byte> backing);
  ~PoolAllocator();

  T* allocate();
  void deallocate(T* ptr);
  void reset();

  size_t capacity() const;
  size_t size() const;
  bool ownsMemory() const;

 private:
  struct FreeNode {
    FreeNode* next = nullptr;
  };

  std::byte* _buffer = nullptr;
  FreeNode* _freeList = nullptr;
  size_t _capacity = 0;
  size_t _count = 0;
  bool _ownsMemory = false;

  void initializeFreeList();
};