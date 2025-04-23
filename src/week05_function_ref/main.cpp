#include <iostream>

#include "FunctionRef.hpp"

void demo_1_function_ref() {
  std::cout << "--- Demo: FunctionRef ---\n";

  // Lambda example
  auto say_hello = []() {
    std::cout << "Hello from lambda!\n";
  };

  FunctionRef<void()> ref1(say_hello);
  ref1();  // Expected: "Hello from lambda!"

  // Callable with capture
  int factor = 3;
  auto multiplier = [factor](int x) {
    return x * factor;
  };

  FunctionRef<int(int)> ref2(multiplier);
  std::cout << "3 * 5 = " << ref2(5) << std::endl;  // Expected: 15

  // Function pointer
  auto add = [](int a, int b) { return a + b; };

  FunctionRef<int(int, int)> ref3(add);
  std::cout << "4 + 7 = " << ref3(4, 7) << std::endl;  // Expected: 11
}

int main() {
  demo_1_function_ref();
}