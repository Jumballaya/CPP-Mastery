#include <iostream>

#include "memory/FrameArena.hpp"
#include "memory/LinearAllocator.hpp"
#include "memory/PoolAllocator.hpp"

//
// @TODO: TaggedAllocator, FallbackAllocator
//

void demo_1_simple_frame_arena() {
  struct Vec3 {
    float x;
    float y;
    float z;
  };
  FrameArena arena(1024);

  for (int j = 0; j < 5; j++) {
    arena.reset();

    Vec3* positions = arena.allocate<Vec3>(10);

    for (int i = 0; i < 10; ++i) {
      positions[i].x = j * i * 10.0f;
      positions[i].y = j * i * 15.0f;
      positions[i].z = j * i * 20.0f;
    }

    for (int i = 0; i < 10; ++i) {
      std::cout << "Vec3: < " << positions[i].x << ", " << positions[i].y << ", " << positions[i].x << " >\n";
    }
  }
}

void demo_2_linear_allocator() {
  struct Position {
    float x;
    float y;
  };

  LinearAllocator allocator(sizeof(Position) * 100);

  for (int i = 0; i < 100; ++i) {
    Position* pos = allocator.allocate<Position>(1);
    pos->x = (float)i;
    pos->y = (float)i * 2.5;
    std::cout << "Position: " << i << "\nx: " << pos->x << "\ny: " << pos->y << "\n";
  }
}

void demo_3_pool_allocator() {
  struct MyObject {
    int x;
    int y;
  };

  PoolAllocator<MyObject> pool(4);
  std::cout << "Capacity: " << pool.capacity() << "\n";

  MyObject* a = pool.allocate();
  MyObject* b = pool.allocate();
  MyObject* c = pool.allocate();
  MyObject* d = pool.allocate();

  if (!a || !b || !c || !d) {
    std::cerr << "Allocation failed unexpectedly.\n";
    return;
  }

  a->x = 1;
  a->y = 10;
  b->x = 2;
  b->y = 20;
  c->x = 3;
  c->y = 30;
  d->x = 4;
  d->y = 40;

  std::cout << "Allocated Objects:\n";
  std::cout << "A: " << a->x << ", " << a->y << "\n";
  std::cout << "B: " << b->x << ", " << b->y << "\n";
  std::cout << "C: " << c->x << ", " << c->y << "\n";
  std::cout << "D: " << d->x << ", " << d->y << "\n";

  std::cout << "Used slots: " << pool.size() << "\n";

  pool.deallocate(a);
  pool.deallocate(b);
  pool.deallocate(c);
  pool.deallocate(d);

  std::cout << "Used slots after deallocate: " << pool.size() << "\n";

  MyObject* e = pool.allocate();
  if (e) {
    e->x = 99;
    e->y = 999;
    std::cout << "Reused slot: " << e->x << ", " << e->y << "\n";
    pool.deallocate(e);
  }

  pool.reset();
  std::cout << "Used slots after reset: " << pool.size() << "\n";
}

int main() {
  demo_3_pool_allocator();
}
