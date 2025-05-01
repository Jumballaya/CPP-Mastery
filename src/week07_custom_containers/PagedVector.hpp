#pragma once

#include <cstddef>
#include <new>
#include <stdexcept>

template <typename T, size_t PageSize, size_t NumPages>
class PagedVector {
 public:
  PagedVector() : _size(0) {
    for (size_t i = 0; i < NumPages; ++i) {
      _pages[i] = nullptr;
    }
  }

  ~PagedVector() {
    for (size_t p = 0; p < NumPages; ++p) {
      T* page = _pages[p];
      if (page) {
        // Only destroy elements that were constructed
        size_t count = PageSize;
        if ((p + 1) * PageSize > _size) {
          count = _size % PageSize;
        }

        for (size_t i = 0; i < count; ++i) {
          page[i].~T();
        }

        delete[] reinterpret_cast<unsigned char*>(page);
        _pages[p] = nullptr;
      }
    }
  }

  PagedVector(const PagedVector&) = delete;
  PagedVector& operator=(const PagedVector&) = delete;

  PagedVector(PagedVector&& other) noexcept
      : _size(other._size) {
    for (size_t i = 0; i < NumPages; ++i) {
      _pages[i] = other._pages[i];
      other._pages[i] = nullptr;
    }
    other._size = 0;
  }

  PagedVector& operator=(PagedVector&& other) noexcept {
    if (this == &other) return *this;
    this->~PagedVector();

    // Move from other
    _size = other._size;
    for (size_t i = 0; i < NumPages; ++i) {
      _pages[i] = other._pages[i];
      other._pages[i] = nullptr;
    }
    other._size = 0;

    return *this;
  };

  void push_back(const T& value) {
    if (_size >= PageSize * NumPages) return;

    size_t page = _size / PageSize;
    size_t offset = _size % PageSize;

    if (_pages[page] == nullptr) {
      // ::operator new(...) is the global new operator
      // this is a placement new that allocates a whole new page
      _pages[page] = reinterpret_cast<T*>(::operator new(sizeof(T) * PageSize));
    }

    new (&_pages[page][offset]) T(value);
    ++_size;
  }

  void push_back(T&& value) {
    if (_size >= PageSize * NumPages) return;

    size_t page = _size / PageSize;
    size_t offset = _size % PageSize;

    if (_pages[page] == nullptr) {
      _pages[page] = reinterpret_cast<T*>(::operator new(sizeof(T) * PageSize));
    }

    new (&_pages[page][offset]) T(std::move(value));
    ++_size;
  }

  void pop_back() {
    if (_size == 0) return;

    size_t lastIndex = _size - 1;
    size_t page = lastIndex / PageSize;
    size_t offset = lastIndex % PageSize;

    _pages[page][offset].~T();
    --_size;

    if (_size % PageSize == 0 && _pages[page]) {
      // ::operator delete(...) is the global delete operator
      // this is here because we manually allocated with the
      // global new operator
      ::operator delete(reinterpret_cast<void*>(_pages[page]));
      _pages[page] = nullptr;
    }
  }

  T& operator[](size_t index) noexcept {
    return _pages[index / PageSize][index % PageSize];
  }

  const T& operator[](size_t index) const noexcept {
    return _pages[index / PageSize][index % PageSize];
  }

  T& at(size_t index) {
    if (index >= _size) throw std::out_of_range("PagedVector out of range");
    return _pages[index / PageSize][index % PageSize];
  }

  const T& at(size_t index) const {
    if (index >= _size) throw std::out_of_range("PagedVector out of range");
    return _pages[index / PageSize][index % PageSize];
  }

  T& front() { return _pages[0][0]; }
  T& back() { return _pages[(_size - 1) / PageSize][(_size - 1) % PageSize]; }

  void clear() {
    for (size_t i = 0; i < _size; ++i) {
      size_t page = i / PageSize;
      size_t offset = i % PageSize;
      _pages[page][offset].~T();
    }
    _size = 0;
  };

  size_t size() const noexcept { return _size; }
  size_t capacity() const noexcept { return PageSize * NumPages; }
  bool empty() const noexcept { return _size == 0; }

 private:
  T* _pages[NumPages] = {};
  size_t _size = 0;
  size_t _pageCount = 0;
};