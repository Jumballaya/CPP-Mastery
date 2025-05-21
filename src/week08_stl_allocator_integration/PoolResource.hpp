#pragma once

#include <cstddef>
#include <memory_resource>

#include "PoolAllocator.hpp"

class PoolResource : public std::pmr::memory_resource {
 public:
  PoolResource(PoolAllocator* allocator) : _allocator(allocator) { assert(allocator != nullptr); }

  ~PoolResource() = default;
  PoolResource(const PoolResource&) = delete;
  PoolResource& operator=(const PoolResource&) = delete;

  PoolResource(PoolResource&& other) : _allocator(other._allocator) { other._allocator = nullptr; }

  PoolResource& operator=(PoolResource&& other) {
    if (this == &other) return *this;
    _allocator = other._allocator;
    other._allocator = nullptr;
    return *this;
  }

  void* do_allocate(size_t size, size_t alignment) override {
    auto ret = _allocator->allocate();
    if (ret == nullptr) {
      throw std::bad_alloc();
    }
    return ret;
  }

  void do_deallocate(void* ptr, size_t size, size_t alignment) override {
    _allocator->deallocate(ptr);
  }

  bool do_is_equal(const memory_resource& other) const noexcept override { return this == &other; }

 private:
  PoolAllocator* _allocator;
};