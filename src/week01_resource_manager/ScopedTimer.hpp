#pragma once
#include <chrono>
#include <iostream>
#include <string_view>

class ScopedTimer {
 public:
  ScopedTimer(std::string_view label, bool enabled = true) : _label(label), _enabled(enabled), _start(std::chrono::steady_clock::now()) {}
  ~ScopedTimer() {
    if (_timing) {
      stop();
    }
  }

  // Prevent copy
  ScopedTimer(const ScopedTimer&) = delete;
  ScopedTimer& operator=(const ScopedTimer&) = delete;

  void stop() {
    if (_enabled) {
      auto now = std::chrono::steady_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - _start);
      std::cout << _label << ": " << elapsed.count() << "ms" << std::endl;
    }
    _timing = false;
  }

 private:
  const std::string_view _label;
  std::chrono::steady_clock::time_point _start;
  bool _timing = true;
  bool _enabled = true;
};