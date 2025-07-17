#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <span>
#include <type_traits>

class LinearAllocator {
 public:
  LinearAllocator(size_t size);
  LinearAllocator(std::span<std::byte> span);
  ~LinearAllocator();

  void* allocateRaw(size_t bytes, size_t alignment = alignof(std::max_align_t));

  template <typename T>
  T* allocate(size_t count = 1);

  void reset();

  size_t used() const;
  size_t capacity() const;
  size_t remaining() const;
  bool ownsMemory() const;

 private:
  std::byte* _start = nullptr;
  std::byte* _ptr = nullptr;
  size_t _size = 0;
  bool _ownsMemory = false;
};

template <typename T>
T* LinearAllocator::allocate(size_t count) {
  static_assert(std::is_trivially_destructible_v<T>, "LinearAllocator only supports POD-like types.");
  assert(count > 0);

  size_t totalSize = sizeof(T) * count;
  size_t alignment = alignof(T);

  void* raw = allocateRaw(totalSize, alignment);
  return reinterpret_cast<T*>(raw);
}