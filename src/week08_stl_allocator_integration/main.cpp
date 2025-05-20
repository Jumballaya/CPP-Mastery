#include <iostream>
#include <memory_resource>
#include <string>
#include <vector>

#include "FrameAllocator.hpp"
#include "FrameResource.hpp"

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

int main() {
  demo_2_nested_vectors();
}