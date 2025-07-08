#pragma once

#include <cstddef>
#include <span>

class FrameArena {
 public:
  FrameArena(void* buffer, size_t size);
  FrameArena(std::span<std::byte> backing);
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
T* FrameArena::allocate(size_t count = 1) {
}