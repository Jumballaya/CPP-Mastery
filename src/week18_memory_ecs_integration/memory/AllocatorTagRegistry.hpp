#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>

class AllocatorTagRegistry {
 public:
  static void track(const void* allocatePtr, const std::string& tag, size_t capacity);
  static void untrack(const void* allocatePtr);
  static size_t getUsageForTag(const std::string& tag);
  static void printStats();

 private:
  struct TagInfo {
    std::string tag;
    size_t capacity = 0;
  };

  static std::unordered_map<const void*, TagInfo> _allocators;
  static std::unordered_map<std::string, size_t> _tagTotals;
};