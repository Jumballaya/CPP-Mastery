#pragma once

#include <cstddef>
#include <cstdint>

template <typename V, size_t N>
class SlotMap {
 public:
  SlotMap() noexcept : _size(0) {}
  ~SlotMap() {
    for (size_t i = 0; i < _size; ++i) {
      if (_occupied[i]) {
        _values[i].~V();
      }
    }
  }

  SlotMap(const SlotMap& other) noexcept : _size(other._size) {
    for (size_t i = 0; i < N; ++i) {
      if (other._occupied[i]) {
        new (&_values[i]) V(other._values[i]);
      }
      _slots[i] = other._slots[i];
      _backrefs[i] = other._backrefs[i];
      _occupied[i] = other._occupied[i];
    }
  }

  SlotMap(SlotMap&& other) noexcept : _size(other._size) {
    for (size_t i = 0; i < N; ++i) {
      if (other._occupied[i]) {
        new (&_values[i]) V(std::move(other._values[i]));
      }
      _slots[i] = other._slots[i];
      _backrefs[i] = other._backrefs[i];
      _occupied[i] = other._occupied[i];
    }
    other._size = 0;
  }

  SlotMap& operator=(const SlotMap& other) noexcept {
    if (this == &other) return *this;
    this->~SlotMap();
    new (this) SlotMap(other);
    return *this;
  }

  SlotMap& operator=(SlotMap&& other) noexcept {
    if (this == &other) return *this;
    this->~SlotMap();
    new (this) SlotMap(std::move(other));
    return *this;
  }

  V& operator[](uint32_t key) noexcept {
    return _values[_slots[key]];
  }

  const V& operator[](uint32_t key) const noexcept {
    return _values[_slots[key]];
  }

  uint32_t insert(const V& value) {
    if (_size >= N) return UINT32_MAX;

    uint32_t key = _size;

    size_t index = 0;

    while (index < N && _occupied[index]) ++index;
    if (index >= N) return UINT32_MAX;  // Defensive fallback

    new (&_values[index]) V(value);
    _slots[key] = index;
    _backrefs[index] = key;
    _occupied[index] = true;

    ++_size;
    return key;
  }

  V* get(uint32_t key) {
    if (key >= N) return nullptr;
    size_t index = _slots[key];
    if (!_occupied[index]) return nullptr;
    return &_values[index];
  }

  const V* get(uint32_t key) const {
    if (key >= N) return nullptr;
    size_t index = _slots[key];
    if (!_occupied[index]) return nullptr;
    return &_values[index];
  }

  bool erase(uint32_t key) {
    if (key >= N) return false;

    size_t index = _slots[key];
    if (!_occupied[index]) return false;

    // Destroy the object
    _values[index].~V();
    _occupied[index] = false;
    --_size;

    // Swap with last element to keep things compact
    if (index != _size) {
      _values[index] = std::move(_values[_size]);
      _occupied[index] = _occupied[_size];

      uint32_t movedKey = _backrefs[_size];
      _slots[movedKey] = index;
      _backrefs[index] = movedKey;
    }

    _occupied[_size] = false;

    return true;
  }

  V* begin() noexcept { return &_values[0]; }
  V* begin() const noexcept { return &_values[0]; }

  V* end() noexcept { return &_values[_size]; }
  V* end() const noexcept { return &_values[_size]; }

  size_t size() const { return _size; }
  bool empty() const { return _size == 0; }

 private:
  V _values[N];
  size_t _size = 0;

  uint32_t _slots[N];      // logical -> physical
  size_t _backrefs[N];     // physical -> logical
  bool _occupied[N] = {};  // tracks active slots
};