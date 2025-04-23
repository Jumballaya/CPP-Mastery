#include <iostream>

#include "FunctionRef.hpp"
#include "VariantCallback.hpp"

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

void demo_2_variant_callback() {
  // 1) keep a lambda alive (l-value)
  auto printer = [](int v) { std::cout << "print: " << v << '\n'; };
  auto doubler = [](int v) { std::cout << v * 2 << '\n'; };

  // 2) pick ONE function signature list
  VariantCallback<void(int)> cb{printer};  // ok, l-value

  cb(3);  // prints 3

  cb = VariantCallback<void(int)>{doubler};  // same INST-TYPE on both sides
  cb(4);                                     // prints 8

  cb.reset();
  if (!cb) std::cout << "empty\n";
}

int main() {
  demo_2_variant_callback();
}