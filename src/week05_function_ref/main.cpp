#include <iostream>

#include "EventBus.hpp"
#include "EventDispatcher.hpp"
#include "EventQueue.hpp"
#include "FunctionRef.hpp"
#include "ListenerHandle.hpp"
#include "MultiCallback.hpp"
#include "VariantCallback.hpp"
// #include "dom/DOM.hpp"

enum class TestEvent : uint32_t {
  Log,
  Damage,
  Click
};
using TestFns = std::tuple<void(int), void(float, float)>;

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
  EventBus<TestEvent, void(int), void(float), void(char)> b42;

  auto onClick = [](int n) { std::cout << "Click " << n << std::endl; };
  auto onClickID = b42.subscribe(TestEvent::Click, onClick);
  b42.publish<int>(TestEvent::Click, 42);
  b42.unsubscribe(TestEvent::Click, onClickID);
  b42.publish<int>(TestEvent::Click, 42);

  auto onDamage = [](float n) { std::cout << "Damage " << n << std::endl; };
  auto onDamageID = b42.subscribe(TestEvent::Damage, onDamage);
  b42.publish<float>(TestEvent::Damage, 42.0f);
  b42.unsubscribe(TestEvent::Damage, onDamageID);
  b42.publish<float>(TestEvent::Damage, 42.0f);

  auto onLog = [](char n) { std::cout << "Log " << n << std::endl; };
  auto onLogID = b42.subscribe(TestEvent::Log, onLog);
  b42.publish<float>(TestEvent::Log, 'a');
  b42.unsubscribe(TestEvent::Log, onLogID);
  b42.publish<float>(TestEvent::Log, 'a');
}

void demo_4_multi_callback() {
  MultiCallback<void(int)> clickCallbacks;

  auto f1 = [](int n) { std::cout << "Listener A: " << n << "\n"; };
  auto f2 = [](int n) { std::cout << "Listener B: " << n << "\n"; };

  auto id1 = clickCallbacks.add(f1);
  auto id2 = clickCallbacks.add(f2);

  clickCallbacks(42);  // Should call both

  auto res1 = clickCallbacks.remove(id1);

  auto res2 = clickCallbacks.remove(id1);
  std::cout << res1 << " " << res2 << std::endl;

  clickCallbacks(99);  // Should call only Listener B
}

void demo_5_event_queue() {
  std::cout << "EventQueue Demo 5" << std::endl;
  EventBus<TestEvent, void(int)> bus;
  auto rec = [](int n) { std::cout << "Event received: " << n << std::endl; };
  bus.subscribe(TestEvent::Click, rec);

  EventQueue<TestEvent, void(int)> queue(bus);
  queue.enqueue(TestEvent::Click, 42);
  queue.enqueue(TestEvent::Click, 99);
  std::cout << "Created EventQueue and queued up 42 & 99" << std::endl;

  queue.dispatch_now();
}

void demo_6_listener_handle() {
  EventBus<TestEvent, void(int)> bus;

  auto handler = [](int n) { std::cout << "Event received: " << n << std::endl; };

  ListenerHandle<TestEvent, void(int)> handle(&bus, TestEvent::Click, bus.subscribe(TestEvent::Click, handler));

  std::cout << "Handle Active: " << handle.active() << std::endl;
  std::cout << "Handle ID: " << handle.id() << std::endl;
  bus.publish(TestEvent::Click, 42);

  handle.unsubscribe();
  std::cout << "Handle Active: " << handle.active() << std::endl;
  std::cout << "Handle ID: " << handle.id() << std::endl;
  bus.publish(TestEvent::Click, 99);
}

void demo_7_event_dispatcher() {
  using Dispatcher = ApplyTuple<TestFns>::apply<EventDispatcher, TestEvent>;
  Dispatcher dispatcher;

  // Add listeners
  auto handle1 = dispatcher.addEventListener(TestEvent::Click, [](int n) {
    std::cout << "[Immediate] Received number: " << n << std::endl;
  });

  auto handle2 = dispatcher.addEventListener(TestEvent::Damage, [](float x, float y) {
    std::cout << "[Immediate] Received point: (" << x << ", " << y << ")" << std::endl;
  });

  // Immediate emission
  dispatcher.emit(TestEvent::Click, 42);
  dispatcher.emit(TestEvent::Damage, 1.5f, 2.5f);

  // Enqueue events
  dispatcher.enqueue(TestEvent::Click, 99);
  dispatcher.enqueue(TestEvent::Damage, 3.0f, 4.0f);

  std::cout << "--- Dispatching queued events ---" << std::endl;
  dispatcher.dispatch_now();

  // Manual removal (optional)
  dispatcher.removeEventListener(handle1);
  dispatcher.removeEventListener(handle2);

  // Emit after removal (should not print anything)
  dispatcher.emit(TestEvent::Click, 777);
  dispatcher.emit(TestEvent::Damage, 7.7f, 8.8f);
}

void demo_8_dom() {
  // DOM dom;
  // auto body = dom.createNode();
  // auto item = dom.createNode();
  // item.addEventListener(DOMEvents::Change, [](float r, float g, float b) {
  //   std::cout << "Color changed: ( " << r << ", " << g << ", " << b << " )" << std::endl;
  // });
  // body->appendChild(item);
  // dom.root()->appendChild(body);

  // item->setColor(0.3, 0.1, 0.8);
}

int main() {
  demo_7_event_dispatcher();
}