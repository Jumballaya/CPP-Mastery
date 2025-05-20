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

int main() {
  demo_1_frame_allocator();
}