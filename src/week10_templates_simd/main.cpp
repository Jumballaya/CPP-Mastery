#include <iostream>

#include "Mat4f.hpp"
#include "Mat4fOps.hpp"
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

void demo_8_quaternion_conjugate() {
  Quaternion q(1, 2, 3, 4);
  Quaternion qc = q.conjugate();

  q.print("q");
  qc.print("q_conjugate");
}

void demo_9_quaternion_multiplication() {
  Quaternion a(1, 2, 3, 4);
  Quaternion b(5, 6, 7, 8);
  Quaternion c = a * b;
  c.print("c = a * b");
}

void demo_10_quaternion_rotate_vec3f() {
  Vec3f v(1, 0, 0);

  // 90° rotation around Z axis
  float angleRad = 3.14159265f / 2.0f;
  float c = std::cos(angleRad / 2.0f);
  float s = std::sin(angleRad / 2.0f);
  Quaternion q(0, 0, s, c);  // axis: z, angle: 90 deg
  Quaternion qn = q.normalize();

  Vec3f result = qn.rotate(v);

  v.print("v");
  qn.print("q (normalized)");
  result.print("rotated v");
}

void demo_11_mat4f_mul() {
  using namespace std;

  // Define two simple transform-style matrices (scaling and translation)
  Mat4f A(
      _mm_set_ps(0, 0, 0, 1),   // Row 0
      _mm_set_ps(0, 0, 2, 0),   // Row 1
      _mm_set_ps(0, 3, 0, 0),   // Row 2
      _mm_set_ps(4, 0, 0, 1));  // Row 3

  Mat4f B(
      _mm_set_ps(0, 0, 0, 1),   // Row 0
      _mm_set_ps(0, 0, 1, 0),   // Row 1
      _mm_set_ps(0, 1, 0, 0),   // Row 2
      _mm_set_ps(2, 0, 0, 1));  // Row 3

  // Multiply using expression templates (lazy)
  Mat4f C = A * B;

  // Print result
  cout << "Matrix A:\n";
  A.print();

  cout << "\nMatrix B:\n";
  B.print();

  cout << "\nMatrix C = A * B:\n";
  C.print();
}

int main() {
  demo_11_mat4f_mul();
}