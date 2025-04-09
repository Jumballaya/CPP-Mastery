#pragma once

#include <iostream>
#include <string_view>

class ScopedLogger {
 public:
  ScopedLogger(std::string_view label, bool enabled = true) : _label(label), _enabled(enabled) {
    if (_enabled) {
      std::cout << "⏱️ Entering: " << _label << std::endl;
    }
  }

  // Make sure the object can't be copied or assigned!
  ScopedLogger(const ScopedLogger&) = delete;
  ScopedLogger& operator=(const ScopedLogger&) = delete;

  ~ScopedLogger() {
    if (logging) {
      end();
    }
  }

  void end() {
    if (_enabled) {
      std::cout << "✅ Exiting: " << _label << std::endl;
    }
    logging = false;
  };

 private:
  const std::string_view _label;
  bool logging = true;
  bool _enabled = true;
};