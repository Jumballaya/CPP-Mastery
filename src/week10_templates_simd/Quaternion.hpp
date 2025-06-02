#pragma once

#include <pmmintrin.h>
#include <xmmintrin.h>

#include <cmath>
#include <iostream>

#include "SimdExpr.hpp"
#include "Vec3f.hpp"

struct alignas(16) Quaternion : public SimdExpr<Quaternion> {
  union {
    struct {
      float x, y, z, w;
    };
    __m128 simd;
  };

  Quaternion() : x(0), y(0), z(0), w(1) {}
  Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
  Quaternion(__m128 simd) : simd(simd) {}

  template <typename Expr>
  Quaternion(const SimdExpr<Expr>& expr) {
    simd = expr.evaluate();
  }

  inline Quaternion operator*(const Quaternion& rhs) const {
    const float x1 = x, y1 = y, z1 = z, w1 = w;
    const float x2 = rhs.x, y2 = rhs.y, z2 = rhs.z, w2 = rhs.w;

    return Quaternion(
        w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2,
        w1 * y2 - x1 * z2 + y1 * w2 + z1 * x2,
        w1 * z2 + x1 * y2 - y1 * x2 + z1 * w2,
        w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2);
  }

  // Load/store helpers
  inline __m128 load() const { return simd; }
  inline void store(__m128 value) { simd = value; }

  // Identity quaternion
  static Quaternion identity() { return Quaternion(0, 0, 0, 1); }

  void print(const char* label = "") const {
    std::cout << label << ": <" << x << ", " << y << ", " << z << ", " << w << ">\n";
  }

  inline float length() const {
    __m128 mul = _mm_mul_ps(simd, simd);     // x², y², z², w²
    __m128 shuf = _mm_movehdup_ps(mul);      // duplicate y and w: [y², y², w², w²]
    __m128 sum = _mm_add_ps(mul, shuf);      // partial: x²+y², y²+y², z²+w², w²+w²
    shuf = _mm_movehl_ps(sum, sum);          // move z²+w² into low lanes
    sum = _mm_add_ss(sum, shuf);             // final sum in lane 0
    return _mm_cvtss_f32(_mm_sqrt_ss(sum));  // sqrt and extract
  }

  inline Quaternion normalize() const {
    float invLen = 1.0f / length();
    __m128 scaled = _mm_mul_ps(simd, _mm_set1_ps(invLen));
    return Quaternion(scaled);
  }

  //
  //  The Quaternion conjugate negates the imaginary vector (x,y,z) and
  //  leave the scalar part (w) untouched.
  //  This is commonly used in quaternion inverse and vector rotation.
  //
  //    SIMD plan:
  //      Create an __m128 mask of negative zeros (except the w) and then XOR
  //      the quaternion __m128 value against it to flip the sign bit on
  //      the x/y/z values.
  //
  inline Quaternion conjugate() const {
    const __m128 conjMask = _mm_set_ps(+0.0f, -0.0f, -0.0f, -0.0f);
    __m128 conj = _mm_xor_ps(simd, conjMask);
    return Quaternion(conj);
  }

  inline Vec3f rotate(const Vec3f& v) const {
    Quaternion vq(v.x, v.y, v.z, 0.0f);
    Quaternion rotated = (*this) * vq * conjugate();
    return Vec3f(rotated.x, rotated.y, rotated.z);
  }
};