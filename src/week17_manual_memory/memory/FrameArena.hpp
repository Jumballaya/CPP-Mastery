#pragma once

#include <cassert>
#include <cstddef>
#include <span>

class FrameArena {
 public:
  explicit FrameArena(size_t size);
  explicit FrameArena(void* buffer, size_t size);
  explicit FrameArena(std::span<std::byte> backing);
  ~FrameArena() = default;

  FrameArena(const FrameArena&) = delete;
  FrameArena& operator=(const FrameArena&) = delete;

  void reset();

  template <typename T>
  T* allocate(size_t count = 1);

  void* allocateRaw(size_t bytes, size_t aligment = alignof(std::max_align_t));

  size_t used() const;
  size_t capacity() const;
  size_t remaining() const;

 private:
  std::byte* _start = nullptr;
  std::byte* _ptr = nullptr;
  size_t _size = 0;
};

template <typename T>
T* FrameArena::allocate(size_t count) {
  static_assert(!std::is_abstract_v<T>, "FrameArena only supports POD-like types.");
  static_assert(std::is_trivially_destructible_v<T>, "FrameArena only supports POD-like types.");
  assert(count > 0);
  size_t totalSize = sizeof(T) * count;
  size_t alignment = alignof(T);
  void* raw = allocateRaw(totalSize, alignment);

  if (!raw) {
    return nullptr;
  }
  return reinterpret_cast<T*>(raw);
}