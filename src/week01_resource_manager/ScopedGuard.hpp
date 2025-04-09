#pragma once
#include <functional>

template <typename F>
class ScopeGuard {
 public:
  ScopeGuard(F&& deferred) : _deferred(std::forward<F>(deferred)) {}

  ~ScopeGuard() {
    if (!_dismissed) {
      _deferred();
    }
  }

  // Can't copy/assign
  ScopeGuard(const ScopeGuard&) = delete;
  ScopeGuard& operator=(const ScopeGuard&) = delete;

  void dismiss() {
    _dismissed = true;
  }

 private:
  bool _dismissed = false;

  F _deferred;
};