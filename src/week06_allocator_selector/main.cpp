#include <iostream>

#include "SmallFunctionRef.hpp"
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

int main() {
  demo_3_small_func_ref();
}