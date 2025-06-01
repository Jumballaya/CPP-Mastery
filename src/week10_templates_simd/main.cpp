#include <iostream>

#include "Vec3f.hpp"

void demo_1_vec3f() {
  Vec3f v1;
  Vec3f v2(1.0f, 2.0f, 3.0f);
  Vec3f v3{5.0f, 6.0f, 7.0f};

  std::cout << "V1: < " << v1.x << ", " << v1.y << ", " << v1.z << " >" << std::endl;
  std::cout << "V2: < " << v2.x << ", " << v2.y << ", " << v2.z << " >" << std::endl;
  std::cout << "V3: < " << v3.x << ", " << v3.y << ", " << v3.z << " >" << std::endl;
}

int main() {
  demo_1_vec3f();
}