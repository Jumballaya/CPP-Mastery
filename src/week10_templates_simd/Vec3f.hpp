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

  template <typename Expr>
  Vec3f(const VecExpr<Expr>& expr) {
    store(expr.evaluate());
  }

  template <typename Expr>
  Vec3f& operator=(const VecExpr<Expr>& expr) {
    store(expr.evaluate());
    return *this;
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