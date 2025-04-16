#pragma once

template <typename T>
class ScopedTransaction {
 public:
  ScopedTransaction(T& ref) : _ref(ref), _copy(ref) {}

  ~ScopedTransaction() {}

  ScopedTransaction(const ScopedTransaction& other) noexcept = delete;
  ScopedTransaction(ScopedTransaction&& other) noexcept = delete;
  ScopedTransaction& operator=(const ScopedTransaction& other) noexcept = delete;
  ScopedTransaction& operator=(ScopedTransaction&& other) noexcept = delete;

  void commit() {
    if (_committed) return;
    _ref = std::move(_copy);
    _committed = true;
  }

  T& get() {
    return _copy;
  }

 private:
  T& _ref;
  T _copy;
  bool _committed = false;
};