#include <array>
#include <cstring>
#include <iostream>
#include <memory_resource>
#include <string>
#include <vector>

#include "FrameAllocator.hpp"
#include "FrameResource.hpp"
#include "PoolAllocator.hpp"
#include "PoolResource.hpp"

void demo_1_frame_allocator() {
  constexpr size_t bufferSize = 1024;
  std::byte buffer[bufferSize];

  FrameAllocator allocator(buffer, bufferSize);
  FrameResource memory(&allocator);

  std::pmr::vector<std::string> names(&memory);

  names.emplace_back("Zero");
  names.emplace_back("One");
  names.emplace_back("Two");

  std::cout << "Vector contents:\n";
  for (const auto& name : names) {
    std::cout << "- " << name << "\n";
  }

  std::cout << "Used bytes before reset: " << allocator.used() << "\n";

  allocator.reset();

  std::cout << "Used bytes after reset: " << allocator.used() << "\n";
}

void demo_2_nested_vectors() {
  constexpr size_t bufferSize = 2048;
  std::byte buffer[bufferSize];

  FrameAllocator allocator(buffer, bufferSize);
  FrameResource memory(&allocator);

  using InnerVec = std::pmr::vector<std::string>;
  std::pmr::vector<InnerVec> table(&memory);

  for (int row = 0; row < 3; ++row) {
    InnerVec inner(&memory);  // each row uses the same FrameResource
    inner.emplace_back("Row " + std::to_string(row) + " Col 0");
    inner.emplace_back("Row " + std::to_string(row) + " Col 1");
    inner.emplace_back("Row " + std::to_string(row) + " Col 2");

    table.push_back(std::move(inner));
  }

  std::cout << "Nested Vector Contents:\n";
  for (const auto& row : table) {
    for (const auto& cell : row) {
      std::cout << "- " << cell << "\n";
    }
  }

  std::cout << "Used bytes before reset: " << allocator.used() << "\n";
  allocator.reset();
  std::cout << "Used bytes after reset: " << allocator.used() << "\n";
}

void demo_3_pool_allocator() {
  using T = std::array<uint8_t, 32>;
  constexpr size_t capacity = 8;
  constexpr size_t slotSize = PoolAllocator::slotSizeFor<T>();

  alignas(std::max_align_t) std::byte buffer[slotSize * capacity];
  PoolAllocator pool(buffer, slotSize, capacity);

  void* ptrs[capacity];

  std::cout << "Manual allocation test (no STL):\n";

  for (size_t i = 0; i < capacity; ++i) {
    ptrs[i] = pool.allocate();
    std::memset(ptrs[i], static_cast<int>(i), sizeof(T));
    std::cout << "  Allocated[" << i << "]: " << ptrs[i]
              << " (used = " << pool.used() << "/" << pool.capacity() << ")\n";
  }

  std::cout << "\nNow deallocating:\n";

  for (size_t i = 0; i < capacity; ++i) {
    pool.deallocate(ptrs[i]);
    std::cout << "  Freed[" << i << "]: " << ptrs[i]
              << " (used = " << pool.used() << "/" << pool.capacity() << ")\n";
  }

  std::cout << "\nDone.\n";
}

int main() {
  demo_3_pool_allocator();
}