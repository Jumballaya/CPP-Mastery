#include <iostream>
#include <string>

#include "DenseSet.hpp"
#include "FixedVector.hpp"
#include "PagedVector.hpp"
#include "RingBuffer.hpp"
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

void demo_4_ring_buffer() {
  RingBuffer<std::string, 4> buffer;

  buffer.push("one");
  buffer.push("two");
  buffer.push("three");
  buffer.push("four");

  std::cout << "Beginning size:" << std::endl;
  for (size_t i = 0; i < buffer.size(); ++i) {
    std::cout << "- " << buffer[i] << "\n";
  }

  std::cout << "Front: " << buffer.front() << "\n";
  std::cout << "Back: " << buffer.back() << "\n";

  buffer.push("five");  // should overwrite "one"
  std::cout << "After push(five):" << std::endl;
  for (size_t i = 0; i < buffer.size(); ++i) {
    std::cout << "- " << buffer[i] << "\n";
  }

  auto popped = buffer.pop();  // pops "two"
  std::cout << "Popped: " << popped << "\n";

  buffer.push("six");
  std::cout << "Final state after push(six):" << std::endl;
  for (size_t i = 0; i < buffer.size(); ++i) {
    std::cout << "- " << buffer[i] << "\n";
  }
}

void demo_5_paged_vector() {
  std::cout << "PagedVector demo:\n";

  // Define a paged vector with 3 items per page and 2 pages max (capacity = 6)
  PagedVector<std::string, 3, 2> names;

  names.push_back("Alpha");
  names.push_back("Bravo");
  names.push_back("Charlie");

  std::cout << "After filling first page:\n";
  for (size_t i = 0; i < names.size(); ++i) {
    std::cout << "- " << names[i] << "\n";
  }

  names.push_back("Delta");
  names.push_back("Echo");
  names.push_back("Foxtrot");

  std::cout << "\nAfter filling second page:\n";
  for (size_t i = 0; i < names.size(); ++i) {
    std::cout << "- " << names[i] << "\n";
  }

  std::cout << "\nFront: " << names.front() << "\n";
  std::cout << "Back: " << names.back() << "\n";

  std::cout << "\nCalling pop_back() twice...\n";
  names.pop_back();
  names.pop_back();

  std::cout << "\nAfter popping 2 values:\n";
  for (size_t i = 0; i < names.size(); ++i) {
    std::cout << "- " << names[i] << "\n";
  }
}

int main() {
  demo_5_paged_vector();
}