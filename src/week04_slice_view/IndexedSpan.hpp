#pragma once

template <typename T>
class IndexedSpan {
 public:
  IndexedSpan(T* data, const size_t* indices, size_t count)
      : _data(data), _indices(indices), _count(count) {}

  T& operator[](size_t i) {
    return _data[_indices[i]];
  }
  const T& operator[](size_t i) const { return _data[i]; }

  size_t size() const { return _count; }
  bool empty() const { return _count == 0; }

  T* data() { return _data; }
  const size_t* indices() { return _indices; }

  T& front() { return &_data[0]; }
  T& back() { return &_data[_count - 1]; }

 private:
  T* _data = nullptr;
  const size_t* _indices = nullptr;
  size_t _count = 0;
};