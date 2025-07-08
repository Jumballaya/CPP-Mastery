#include "FrameArena.hpp"

FrameArena::FrameArena(void* buffer, size_t size) {}

FrameArena::FrameArena(std::span<std::byte> backing) {}

void FrameArena::reset() {}

void* FrameArena::allocateRaw(size_t bytes, size_t aligment = alignof(std::max_align_t)) {}

size_t FrameArena::used() const {}

size_t FrameArena::capacity() const {}

size_t FrameArena::remaining() const {}
