#pragma once

#include <cstddef>
#include <memory_resource>

#include "./FrameAllocator.hpp"

class FrameResource : public std::pmr::memory_resource {
 public:
  explicit FrameResource(FrameAllocator* allocator) : _allocator(allocator) {}

  ~FrameResource() = default;
  FrameResource(const FrameResource&) = delete;
  FrameResource(FrameResource&&) = delete;
  FrameResource& operator=(const FrameResource&) = delete;
  FrameResource& operator=(FrameResource&&) = delete;

  void* do_allocate(size_t bytes, size_t alignment) override {
    auto ret = _allocator->allocate(bytes, alignment);
    if (ret == nullptr) {
      throw std::bad_alloc();
    }
    return ret;
  }

  [[maybe_unused]]
  void do_deallocate(void* p, size_t bytes, size_t alignment) override {}  // no-op for std::pmr, just like in FrameAllocator

  bool do_is_equal(const memory_resource& other) const noexcept override {
    return this == &other;
  }

 private:
  FrameAllocator* _allocator;
};