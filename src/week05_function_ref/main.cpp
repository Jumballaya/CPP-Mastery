#include <iostream>

#include "EventBus.hpp"
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

void demo_3_event_bus() {
  EventBus<void(int), void(float), void(char)> b42;

  auto onClick = [](int n) { std::cout << "Click " << n << std::endl; };
  auto onClickID = b42.subscribe(EventTag::Click, onClick);
  b42.publish<int>(EventTag::Click, 42);
  b42.unsubscribe(EventTag::Click, onClickID);
  b42.publish<int>(EventTag::Click, 42);

  auto onDamage = [](float n) { std::cout << "Damage " << n << std::endl; };
  auto onDamageID = b42.subscribe(EventTag::Damage, onDamage);
  b42.publish<float>(EventTag::Damage, 42.0f);
  b42.unsubscribe(EventTag::Damage, onDamageID);
  b42.publish<float>(EventTag::Damage, 42.0f);

  auto onLog = [](char n) { std::cout << "Log " << n << std::endl; };
  auto onLogID = b42.subscribe(EventTag::Log, onLog);
  b42.publish<float>(EventTag::Log, 'a');
  b42.unsubscribe(EventTag::Log, onLogID);
  b42.publish<float>(EventTag::Log, 'a');
}

int main() {
  demo_3_event_bus();
}