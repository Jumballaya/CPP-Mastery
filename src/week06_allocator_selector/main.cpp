#include <iostream>

#include "./events/Entity.hpp"
#include "./events/SmallEventDispatcher.hpp"
#include "SmallAllocator.hpp"
#include "SmallFunctionRef.hpp"
#include "SmallVariantCallback.hpp"
#include "StoragePolicy.hpp"

void demo_1_heap_allocator() {
  using Alloc = AllocatorSelector<HeapStorage>::type;

  auto* p = Alloc::allocate<int>(42);
  std::cout << "Allocated int: " << *p << '\n';
  Alloc::deallocate(p);
}

struct Foo {
  int a, b, c;
  Foo(int a, int b, int c) : a(a), b(b), c(c) {}
};

void demo_2_inline_allocator() {
  using Alloc = AllocatorSelector<InlineStorage<sizeof(Foo)>>::type;
  auto* p = Alloc::allocate<Foo>(1, 2, 3);
  std::cout << p->a << ", " << p->b << ", " << p->c << '\n';
  Alloc::deallocate(p);
  auto* ptr = Alloc::allocate<Foo>(3, 2, 1);
  std::cout << ptr->a << ", " << ptr->b << ", " << ptr->c << '\n';
}

void demo_3_small_func_ref() {
  using Fn = SmallFunctionRef<64, void()>;

  Fn a([] { std::cout << "Hello from A\n"; });
  Fn b([] { std::cout << "Hello from B\n"; });

  a();  // should print Hello from A
  b();  // should print Hello from B

  b = std::move(a);  // move a into b

  b();  // should now print Hello from A
  try {
    a();  // should throw!
  } catch (const std::bad_function_call&) {
    std::cout << "Caught bad_function_call on moved-from A\n";
  }
}

void demo_4_small_variant_callback() {
  SmallVariantCallback<64, void(int), void(float)> cb;

  // Assign an int handler
  cb = [](int x) {
    std::cout << "Received int: " << x << "\n";
  };

  cb(42);  // Should print: Received int: 42

  // Move cb into a new one
  SmallVariantCallback<64, void(int), void(float)> cb2 = std::move(cb);

  try {
    cb(99);  // This should throw std::bad_function_call
  } catch (const std::bad_function_call& e) {
    std::cout << "Caught bad_function_call when calling moved-from cb\n";
  }

  cb2(7);  // Should print: Received int: 7

  // Now overwrite with a float handler
  cb2 = [](float f) {
    std::cout << "Received float: " << f << "\n";
  };

  cb2(3.14f);  // Should print: Received float: 3.14
}

struct Bar {
  int x, y;
};

void demo_5_small_allocator() {
  SmallAllocator<128> alloc;

  Bar* bar1 = static_cast<Bar*>(alloc.allocate(sizeof(Bar), alignof(Bar)));
  assert(bar1 != nullptr);
  bar1->x = 10;
  bar1->y = 20;

  Bar* bar2 = static_cast<Bar*>(alloc.allocate(sizeof(Bar), alignof(Bar)));
  assert(bar2 != nullptr);
  bar2->x = 30;
  bar2->y = 40;

  std::cout << "Bar1: (" << bar1->x << ", " << bar1->y << ")\n";
  std::cout << "Bar2: (" << bar2->x << ", " << bar2->y << ")\n";

  alloc.reset();

  Bar* bar3 = static_cast<Bar*>(alloc.allocate(sizeof(Foo), alignof(Bar)));
  assert(bar3 != nullptr);
  bar3->x = 50;
  bar3->y = 60;

  std::cout << "Bar3 after reset: (" << bar3->x << ", " << bar3->y << ")\n";
}

void demo_6_small_allocator_overflow() {
  SmallAllocator<64> alloc;

  void* a = alloc.allocate(32, alignof(std::max_align_t));
  assert(a != nullptr);

  void* b = alloc.allocate(40, alignof(std::max_align_t));  // too big!
  assert(b == nullptr);

  std::cout << "Overflow test passed.\n";
}

enum class TestEvent : uint32_t {
  Log,
  Damage,
  Click
};

void demo_8_small_event_dispatcher() {
  using Dispatcher = SmallEventDispatcher<TestEvent, 64, void(int), void(float)>;
  Dispatcher dispatcher;

  auto handle = dispatcher.addListener(TestEvent::Click, [](int n) {
    std::cout << "♦ Received Click: " << n << '\n';
  });

  dispatcher.emit(TestEvent::Click, 42);
  dispatcher.removeListener(handle);
  dispatcher.emit(TestEvent::Click, 99);
}

enum class Msg { HealthChanged,
                 DamageTaken };

void demo_9_entity_bus() {
  using Dispatcher = SmallEventDispatcher<Msg, 64, void(int), void(float)>;
  Dispatcher bus;

  Entity<Msg, 64, void(int), void(float)> player(bus);
  Entity<Msg, 64, void(int), void(float)> enemy(bus);

  player.on(Msg::HealthChanged, [](int hp) {
    std::cout << "Player HP changed to: " << hp << '\n';
  });

  enemy.on(Msg::DamageTaken, [](float dmg) {
    std::cout << "Enemy took damage: " << dmg << '\n';
  });

  player.emit(Msg::HealthChanged, 95);
  enemy.emit(Msg::DamageTaken, 12.5f);
}

int main() {
  demo_9_entity_bus();
}