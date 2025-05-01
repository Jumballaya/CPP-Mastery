#include <iostream>
#include <string>

#include "FixedVector.hpp"
#include "SlotMap.hpp"

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

void demo_2_slotmap() {
  SlotMap<std::string, 8> map;

  uint32_t a = map.insert("alpha");
  uint32_t b = map.insert("bravo");
  uint32_t c = map.insert("charlie");

  std::cout << "Initial insertions:" << std::endl;
  for (auto& val : map) {
    std::cout << "- " << val << '\n';
  }

  std::cout << "\nAccess by key:" << std::endl;
  std::cout << "Key b = " << b << " -> " << map[b] << '\n';

  map.erase(b);

  std::cout << "\nAfter erase:" << std::endl;
  for (auto& val : map) {
    std::cout << "- " << val << '\n';
  }

  uint32_t d = map.insert("delta");
  std::cout << "\nInserted delta at key " << d << '\n';

  std::cout << "\nFinal state:" << std::endl;
  for (auto& val : map) {
    std::cout << "- " << val << '\n';
  }
}

int main() {
  demo_2_slotmap();
}