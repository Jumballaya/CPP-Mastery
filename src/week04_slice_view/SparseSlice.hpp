#pragma once

#include <vector>

template <typename T>
class SparseSlice {
 public:
  SparseSlice() = default;
  SparseSlice(std::vector<T*>&& data) : _data(std::move(data)) {};

  T& operator[](size_t index) {
    return *_data[index];
  };

  T** data() { return _data.data(); };

  size_t size() const { return _data.size(); };

  bool empty() const { return _data.empty(); };

  T** begin() { return _data.data(); };

  T** end() { return _data.data() + _data.size(); }

  std::vector<T*> slice() const {
    return _data;
  };

  template <typename F>
  const T* find(F&& predicate) const {
    for (const T* item : _data) {
      if (predicate(item)) {
        return item;
      }
    }
    return nullptr;
  }

 private:
  std::vector<T*> _data;
};