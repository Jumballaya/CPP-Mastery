#include <iostream>

#include "Quaternion.hpp"
#include "Vec3f.hpp"

void demo_1_vec3f() {
  Vec3f v1;
  Vec3f v2(1.0f, 2.0f, 3.0f);
  Vec3f v3{5.0f, 6.0f, 7.0f};
  v1.print("V1");
  v2.print("V2");
  v3.print("V3");
}

void demo_2_vec3f_add() {
  Vec3f v1(1.0f, 2.0f, 3.0f);
  Vec3f v2(4.0f, 5.0f, 6.0f);
  Vec3f v3(7.0f, 8.0f, 9.0f);
  Vec3f v4 = v1 + v2 + v3;
  v1.print("V1");
  v2.print("V2");
  v3.print("V3");
  v4.print("V4");
}

void demo_3_vec3f_mult() {
  Vec3f v1(1.0f, 2.0f, 3.0f);
  Vec3f v2(4.0f, 5.0f, 6.0f);
  Vec3f v3(7.0f, 8.0f, 9.0f);
  Vec3f v4 = v1 * v2 * v3;
  v1.print("V1");
  v2.print("V2");
  v3.print("V3");
  v4.print("V4");
}

void demo_4_vec3f_scalar_mult() {
  Vec3f v1(1.0f, 2.0f, 3.0f);
  Vec3f v2 = v1 * 10.0f;
  v1.print("V1");
  v2.print("V2");
}

void demo_5_vec3f_dot_product() {
  Vec3f v1(1, 2, 3);
  Vec3f v2(4, 5, 6);

  float d = v1.dot(v2);
  std::cout << "dot: " << d << "\n";
}

void demo_6_vec3f_normalized() {
  Vec3f v(3, 0, 4);
  std::cout << "length: " << v.length() << "\n";
  Vec3f vn = v.normalize();
  vn.print("Normalized");
}

void demo_7_quaternion_length_normalize() {
  Quaternion q(1, 2, 3, 4);
  float len = q.length();
  Quaternion norm = q.normalize();

  q.print("q");
  std::cout << "length: " << len << "\n";
  norm.print("normalized q");
  std::cout << "normalized length: " << norm.length() << "\n";
}

int main() {
  demo_7_quaternion_length_normalize();
}