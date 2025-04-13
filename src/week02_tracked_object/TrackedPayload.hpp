#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "TrackedObject.hpp"

class TrackedPayload : public TrackedObject {
 public:
  TrackedPayload(std::string name, int size) : TrackedObject(name), _data(size, 42) {
    std::cout << "[ " << name << " ] initialized with <" << size << "> elements" << std::endl;
  }

 private:
  std::vector<int> _data;
};