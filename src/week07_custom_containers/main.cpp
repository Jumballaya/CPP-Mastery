#include <iostream>
#include <string>

#include "FixedVector.hpp"

void demo_1_fixed_vector() {
  FixedVector<std::string, 4> words;

  words.push_back("one");
  words.push_back("two");
  words.push_back("three");
  words.push_back("four");

  for (auto& w : words) {
    std::cout << w << std::endl;
  }
}

int main() {
  demo_1_fixed_vector();
}