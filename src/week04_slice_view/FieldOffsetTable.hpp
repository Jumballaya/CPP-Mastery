#pragma once

#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

template <typename T>
class FieldOffsetTable {
 public:
  FieldOffsetTable() = default;
  ~FieldOffsetTable() = default;

  void registerField(std::string_view name, auto T::* fieldPtr) {
    size_t offset = reinterpret_cast<size_t>(&(reinterpret_cast<T*>(0)->*fieldPtr));
    _offsets[std::string(name)] = offset;
  }

  size_t offsetOf(std::string_view name) {
    return _offsets[std::string(name)];
  };

  std::vector<std::string> keys() const {
    std::vector<std::string> out;
    out.reserve(_offsets.size());

    for (auto& pair : _offsets) {
      out.push_back(pair.first);
    }
    return out;
  }

  void debugPrint() const {
    for (auto& [name, offset] : _offsets) {
      std::cout << name << ": " << offset << ";" << std::endl;
    }
  }

 private:
  std::unordered_map<std::string, size_t> _offsets;
};