#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <vector>

template <size_t N>
class Allocator {
 public:
  static constexpr size_t kMaxSize = N;

  Allocator() : _offset(0) {}
  ~Allocator() = default;

  void* allocate(size_t size, size_t alignment) {
    // Block reuse
    for (auto it = _freeBlocks.begin(); it != _freeBlocks.end(); ++it) {
      if (it->size >= size) {
        void* ptr = &_buffer[it->offset];
        size_t leftover = it->size - size;
        if (leftover > 0) {
          _freeBlocks.push_back(Block{it->offset + size, leftover});
        }
        _activeAllocs[ptr] = size;
        _freeBlocks.erase(it);
        return ptr;
      }
    }

    // allocate from ptr
    size_t padding = (alignment - (_offset % alignment)) % alignment;
    size_t aligned_offset = _offset + padding;
    if (aligned_offset + size > N) {
      return nullptr;  // No space
    }

    void* ptr = &_buffer[aligned_offset];
    _activeAllocs[ptr] = size;
    _offset = aligned_offset + size;
    return ptr;
  }

  void deallocate(void* ptr) {
    if (!ptr || _activeAllocs.find(ptr) == _activeAllocs.end()) return;
    size_t offset = static_cast<unsigned char*>(ptr) - &_buffer[0];
    size_t size = _activeAllocs[ptr];
    _freeBlocks.push_back(Block{offset, size});
    _activeAllocs.erase(ptr);
  }

  void reset() {
    _offset = 0;
    _freeBlocks.clear();
    _activeAllocs.clear();
  }

 private:
  alignas(std::max_align_t) unsigned char _buffer[N];
  size_t _offset;

  struct Block {
    size_t offset;
    size_t size;
  };

  std::vector<Block> _freeBlocks;
  std::unordered_map<void*, size_t> _activeAllocs;
};