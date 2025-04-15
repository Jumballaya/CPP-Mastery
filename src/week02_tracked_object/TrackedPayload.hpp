#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "TrackedObject.hpp"

class TrackedPayload : public TrackedObject {
 public:
  TrackedPayload() : TrackedObject("default"), _data{} {
    std::cout << "[ " << _name << " ] initialized with <" << _data.size() << "> elements" << std::endl;
  }
  TrackedPayload(std::string_view name, int size) : TrackedObject(name), _data(size, 42) {
    std::cout << "[ " << _name << " ] initialized with <" << _data.size() << "> elements" << std::endl;
  }

  size_t size() const {
    return _data.size();
  }

  void resize(size_t s) {
    _data.resize(s);
  }

 private:
  std::vector<int> _data;
};