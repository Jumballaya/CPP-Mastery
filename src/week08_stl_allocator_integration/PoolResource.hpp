#pragma once

#include <cstddef>
#include <memory_resource>

#include "PoolAllocator.hpp"

class PoolResource : public std::pmr::memory_resource {
 public:
  PoolResource(PoolAllocator* allocator) : _allocator(allocator) {}

  T* do_allocate(size_t size, size_t alignment) override;
  void do_deallocate(void* ptr, size_t size, size_t alignment) override;

  bool do_is_equal(const memory_resource& other) const noexcept override {
    return this == &other;
  }

 private:
  PoolAllocator* _allocator;
};