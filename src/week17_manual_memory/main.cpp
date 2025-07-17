#include <iostream>

#include "memory/FrameArena.hpp"
#include "memory/LinearAllocator.hpp"

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

int main() {
  demo_2_linear_allocator();
}
