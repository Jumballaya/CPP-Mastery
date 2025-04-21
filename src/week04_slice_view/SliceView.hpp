#pragma once

template <typename T>
class SliceView {
 public:
  SliceView() = default;
  ~SliceView() = default;

  SliceView(const T* data, size_t size) : _data(data), _size(size) {}

  size_t size() const { return _size; }
  bool empty() const { return _size == 0; }

  const T& operator[](size_t index) const {
    return _data[index];
  }

  const T* data() const { return _data; }

  const T* begin() const { return _data; }
  const T* end() const { return _data + _size; }

  const T& front() const { return _data[0]; }
  const T& back() const { return _data[_size - 1]; }

  SliceView<T> subview(size_t offset, size_t length) const {
    if (offset >= _size) {
      return SliceView<T>(nullptr, 0);
    }
    size_t newLength = std::min(length, _size - offset);
    return SliceView<T>(&_data[offset], newLength);
  };

 private:
  const T* _data = nullptr;
  size_t _size = 0;
};