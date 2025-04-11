#pragma once
#include <functional>

template <typename F>
class ScopedGuard {
 public:
  ScopedGuard(F&& deferred) : _deferred(std::forward<F>(deferred)) {}

  ~ScopedGuard() {
    if (!_dismissed) {
      _deferred();
    }
  }

  // Can't copy/assign
  ScopedGuard(const ScopedGuard&) = delete;
  ScopedGuard& operator=(const ScopedGuard&) = delete;

  void dismiss() {
    _dismissed = true;
  }

 private:
  bool _dismissed = false;

  F _deferred;
};