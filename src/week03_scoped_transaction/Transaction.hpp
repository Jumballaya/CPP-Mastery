#pragma once

template <typename T>
class Transaction {
 public:
  Transaction(T& ref) : _ref(&ref), _original(ref), _working(ref) {}
  ~Transaction() {
    if (_committed) return;
    rollback();
  };

  Transaction(const Transaction& other) noexcept = delete;
  Transaction& operator=(const Transaction& other) noexcept = delete;

  Transaction(Transaction&& other) noexcept
      : _ref(other._ref),
        _original(other._original),
        _working(other._working),
        _committed(other._committed),
        _dirty(other._dirty) {
    other._ref = nullptr;
  }

  Transaction& operator=(Transaction&& other) noexcept {
    if (this == &other) return *this;

    _ref = other._ref;
    _original = std::move(other._original);
    _committed = std::move(other._committed);
    _dirty = other._dirty;
    other._ref = nullptr;

    return *this;
  }

  [[nodiscard]]
  T& edit() {
    _dirty = true;
    return _working;
  }

  void commit() {
    if (_committed || !_ref) return;
    *_ref = std::move(_working);
    _committed = true;
  }

  void rollback() {
    if (_ref) {
      *_ref = std::move(_original);
    }
    _committed = false;
  }

  bool is_dirty() const { return _dirty; }

  bool committed() const { return _committed; }

  bool has_value() const { return _ref != nullptr; }

 private:
  T* _ref = nullptr;
  T _original;
  T _working;
  bool _committed = false;
  bool _dirty = false;
};