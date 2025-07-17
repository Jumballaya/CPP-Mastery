#include "AllocatorTagRegistry.hpp"

#include <cassert>
#include <iostream>

std::unordered_map<const void*, AllocatorTagRegistry::TagInfo> AllocatorTagRegistry::_allocators;
std::unordered_map<std::string, size_t> AllocatorTagRegistry::_tagTotals;

void AllocatorTagRegistry::track(const void* allocatorPtr, const std::string& tag, size_t capacity) {
  assert(allocatorPtr != nullptr);
  assert(capacity > 0 && "Tracked capacity must be greater than 0");

  if (_allocators.contains(allocatorPtr)) {
    std::cerr << "[AllocatorTagRegistry] Warning: allocatorPtr already tracked.\n";
    return;
  }

  _allocators[allocatorPtr] = TagInfo{tag, capacity};
  _tagTotals[tag] += capacity;
}

void AllocatorTagRegistry::untrack(const void* allocatorPtr) {
  auto it = _allocators.find(allocatorPtr);
  if (it == _allocators.end()) {
    std::cerr << "[AllocatorTagRegistry] Warning: allocatorPtr not found in untrack.\n";
    return;
  }

  const auto& info = it->second;
  _tagTotals[info.tag] -= info.capacity;
  if (_tagTotals[info.tag] == 0) {
    _tagTotals.erase(info.tag);
  }

  _allocators.erase(it);
}

size_t AllocatorTagRegistry::getUsageForTag(const std::string& tag) {
  auto it = _tagTotals.find(tag);
  return it != _tagTotals.end() ? it->second : 0;
}

void AllocatorTagRegistry::printStats() {
  std::cout << "=== AllocatorTagRegistry Stats ===\n";
  for (const auto& [tag, total] : _tagTotals) {
    std::cout << "Tag: " << tag << " → " << total << " bytes\n";
  }
  std::cout << "Total Allocators Tracked: " << _allocators.size() << "\n";
}