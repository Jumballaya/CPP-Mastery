#include "LinearAllocator.hpp"

LinearAllocator::LinearAllocator(size_t size) {}

LinearAllocator::LinearAllocator(std::span<std::byte> span) {}

LinearAllocator::~LinearAllocator() {}

void* LinearAllocator::allocateRaw(size_t bytes, size_t alignment = alignof(std::max_align_t)) {}

void LinearAllocator::reset() {}

size_t LinearAllocator::used() const { return 0; }
size_t LinearAllocator::capacity() const { return _size; }
size_t LinearAllocator::remaining() const { return 0; }
bool LinearAllocator::ownsMemory() const { return _ownsMemory; }
