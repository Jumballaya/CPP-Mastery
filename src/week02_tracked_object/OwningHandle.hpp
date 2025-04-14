#pragma once
#include <iostream>

class OwningHandle {
 public:
  OwningHandle() : _handle(OwningHandle::next_id++) {};
  ~OwningHandle() {
    std::cout << "Handle [" << _handle << "] Destroyed" << std::endl;
  };

  // Copy Constructor overload
  // if we copy, it needs NEW handle
  OwningHandle(const OwningHandle& other) noexcept : _handle(OwningHandle::next_id++) {
    std::cout << "Copy constructed [" << _handle << "] <- [" << other._handle << "]" << std::endl;
  }
  // Copy Assignment overload
  // if we copy, it needs NEW handle
  OwningHandle& operator=(const OwningHandle& other) noexcept {
    if (this == &other) return *this;
    std::cout << "Copy assigned [" << _handle << "] <- [" << other._handle << "]" << std::endl;
    _handle = OwningHandle::next_id++;
    return *this;
  }

  OwningHandle(OwningHandle&& other) noexcept : _handle(other._handle) {
    other._handle = -1;
    std::cout << "Moved Handle [" << _handle << "] from [" << other._handle << "]" << std::endl;
  }

  OwningHandle& operator=(OwningHandle&& other) noexcept {
    if (this == &other) return *this;
    std::cout << "Move Assigned [" << _handle << "] <- [" << other._handle << "]" << std::endl;
    _handle = other._handle;
    other._handle = -1;
    return *this;
  }

 private:
  int _handle = -1;
  static int next_id;
};

int OwningHandle::next_id = 0;