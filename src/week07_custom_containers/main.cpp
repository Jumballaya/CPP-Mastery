#include <iostream>
#include <string>

#include "DenseSet.hpp"
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

void demo_3_dense_set() {
  DenseSet<uint32_t, 8> set;

  std::cout << "Initial insertions:\n";
  set.insert(3);
  set.insert(1);
  set.insert(6);

  for (auto v : set) {
    std::cout << "- " << v << '\n';
  }

  std::cout << "\nContains 1? " << (set.contains(1) ? "yes" : "no") << '\n';
  std::cout << "Contains 5? " << (set.contains(5) ? "yes" : "no") << '\n';

  std::cout << "\nErase 1\n";
  set.erase(1);

  std::cout << "\nAfter erase:\n";
  for (auto v : set) {
    std::cout << "- " << v << '\n';
  }

  std::cout << "\nInsert 2\n";
  set.insert(2);

  std::cout << "\nFinal state:\n";
  for (auto v : set) {
    std::cout << "- " << v << '\n';
  }
}

int main() {
  demo_3_dense_set();
}