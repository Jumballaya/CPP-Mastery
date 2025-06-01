#pragma once
#include <xmmintrin.h>

#include <cassert>
#include <cstddef>
#include <type_traits>

#include "Vec3fOps.hpp"
#include "VecExpr.hpp"

struct alignas(16) Vec3f : public VecExpr<Vec3f> {
  float x;
  float y;
  float z;
  float _pad = 0.0f;  // pad to 16 bytes

  Vec3f() : x(0.0f), y(0.0f), z(0.0f) {}
  Vec3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
  ~Vec3f() = default;

  void print(const char* label = "") const {
    std::cout << label << ": <" << x << ", " << y << ", " << z << ">\n";
  }

  template <typename Expr>
  Vec3f(const VecExpr<Expr>& expr) {
    store(expr.evaluate());
  }

  template <typename Expr>
  Vec3f& operator=(const VecExpr<Expr>& expr) {
    store(expr.evaluate());
    return *this;
  }

  inline Vec3f normalize() const {
    float len = length();
    return *this * (1.0f / len);
  }

  //
  //  SIMD Magnitude (length)
  //
  //  Similar to dot product, but with the end being _mm_sqrt_ss(sum)
  //  which square roots the sum of the elements
  //
  inline float length() const {
    __m128 len2 = _mm_mul_ps(load(), load());
    __m128 shuf = _mm_movehl_ps(len2, len2);
    __m128 sum = _mm_add_ps(len2, shuf);
    shuf = _mm_shuffle_ps(len2, len2, _MM_SHUFFLE(1, 1, 1, 1));
    sum = _mm_add_ss(sum, shuf);
    return _mm_cvtss_f32(_mm_sqrt_ss(sum));
  }

  //
  //  SIMD dot product:
  //
  //    1. Multiply v1 * v2 --> (a.x * b.x), (a.y * b.y), (a.z * b.z)
  //    2. Move a.z*b.z to lane 0, then add it to a.x*b.x for a partial sum
  //    3. Broadcast a.y*b.y and add it to the partial sum for the full dot product
  //    4. Extract that sum value from lane 0
  //
  template <typename Expr>
  float dot(const VecExpr<Expr>& other) const {
    __m128 va = load();
    __m128 vb = other.evaluate();
    __m128 prod = _mm_mul_ps(va, vb);                            // [x*x', y*y', z*z', _]
    __m128 shuf = _mm_movehl_ps(prod, prod);                     // [z*z', ?, ?, ?]
    __m128 sum = _mm_add_ps(prod, shuf);                         // [x*x' + z*z', ?, ?, ?]
    shuf = _mm_shuffle_ps(prod, prod, _MM_SHUFFLE(1, 1, 1, 1));  // [y*y', y*y', y*y', y*y']
    sum = _mm_add_ss(sum, shuf);                                 // [dot, _, _, _]
    return _mm_cvtss_f32(sum);                                   // extract dot
  }

  // SIMD Stuff

  static Vec3f fromSIMD(__m128 v) {
    Vec3f result;
    result.store(v);
    return result;
  }

  // SIMD: load [x,y,z,pad] into __m128
  inline __m128 load() const {
    return _mm_load_ps(&x);
  }

  // SIMD: stores the __m128 back into this vector
  inline void store(__m128 value) {
    _mm_store_ps(&x, value);
  }

  // Expression Template entry
  inline __m128 evaluate() const {
    return load();
  }
};

// Assert that Vec3f is aligned with SIMD needs
static_assert(std::is_trivially_copyable_v<Vec3f>, "Vec3f must be trivially copyable");
static_assert(sizeof(Vec3f) == 16, "Vec3f must be 16 bytes");