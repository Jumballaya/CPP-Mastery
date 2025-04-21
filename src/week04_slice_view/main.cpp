#include <iostream>
#include <vector>

#include "SliceView.hpp"

void demo_1_sliceview() {
  std::vector<int> numbers = {10, 20, 30, 40, 50, 60};

  // Create a view over the full vector
  SliceView<int> view_full(numbers.data(), numbers.size());
  std::cout << "Full view: ";
  for (const int& n : view_full) {
    std::cout << n << " ";
  }
  std::cout << "\n";

  // Create a subview (offset = 2, length = 3) => 30 40 50
  auto sub = view_full.subview(2, 3);
  std::cout << "Subview [2, 3]: ";
  for (const int& n : sub) {
    std::cout << n << " ";
  }
  std::cout << "\n";

  // Out-of-bounds subview (should be empty)
  auto empty = view_full.subview(10, 5);
  std::cout << "Empty subview size: " << empty.size() << "\n";

  // Access operator[] test
  std::cout << "Index 1 in full view: " << view_full[1] << "\n";  // 20

  // Manual data pointer test
  const int* raw = view_full.data();
  std::cout << "First element via data(): " << *raw << "\n";  // 10
}

void demo_2_sliceview_for_loop() {
  std::vector<int> numbers = {1, 2, 3, 4, 5};
  SliceView<int> view(numbers.data(), numbers.size());

  std::cout << "Front: " << view.front() << "\n";
  std::cout << "Back: " << view.back() << "\n";

  for (auto x : view) {
    std::cout << x << " ";
  }
}

int main() {
  demo_2_sliceview_for_loop();
}
