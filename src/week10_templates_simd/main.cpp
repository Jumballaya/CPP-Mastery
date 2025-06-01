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

void demo_2_vec3f_add() {
  Vec3f v1(1.0f, 2.0f, 3.0f);
  Vec3f v2(4.0f, 5.0f, 6.0f);
  Vec3f v3(7.0f, 8.0f, 9.0f);
  Vec3f v4 = v1 + v2 + v3;

  std::cout << "V1: < " << v1.x << ", " << v1.y << ", " << v1.z << " >" << std::endl;
  std::cout << "V2: < " << v2.x << ", " << v2.y << ", " << v2.z << " >" << std::endl;
  std::cout << "V3: < " << v3.x << ", " << v3.y << ", " << v3.z << " >" << std::endl;
  std::cout << "V4: < " << v4.x << ", " << v4.y << ", " << v4.z << " >" << std::endl;
}

void demo_3_vec3f_mult() {
  Vec3f v1(1.0f, 2.0f, 3.0f);
  Vec3f v2(4.0f, 5.0f, 6.0f);
  Vec3f v3(7.0f, 8.0f, 9.0f);
  Vec3f v4 = v1 * v2 * v3;

  std::cout << "V1: < " << v1.x << ", " << v1.y << ", " << v1.z << " >" << std::endl;
  std::cout << "V2: < " << v2.x << ", " << v2.y << ", " << v2.z << " >" << std::endl;
  std::cout << "V3: < " << v3.x << ", " << v3.y << ", " << v3.z << " >" << std::endl;
  std::cout << "V4: < " << v4.x << ", " << v4.y << ", " << v4.z << " >" << std::endl;
}

void demo_4_vec3f_scalar_mult() {
  Vec3f v1(1.0f, 2.0f, 3.0f);
  Vec3f v2 = v1 * 10.0f;

  std::cout << "V1: < " << v1.x << ", " << v1.y << ", " << v1.z << " >" << std::endl;
  std::cout << "V2: < " << v2.x << ", " << v2.y << ", " << v2.z << " >" << std::endl;
}

void demo_5_vec3f_dot_product() {
  Vec3f a(1, 2, 3);
  Vec3f b(4, 5, 6);

  float d = a.dot(b);
  std::cout << "dot: " << d << "\n";
}

void demo_6_vec3f_normalized() {
  Vec3f v(3, 0, 4);
  std::cout << "length: " << v.length() << "\n";
  Vec3f vn = v.normalize();
  std::cout << "Normalized: < " << vn.x << ", " << vn.y << ", " << vn.z << " >" << std::endl;
}

int main() {
  demo_6_vec3f_normalized();
}