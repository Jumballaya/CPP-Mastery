#pragma once

#include <pmmintrin.h>
#include <xmmintrin.h>

#include <cmath>
#include <iostream>

struct alignas(16) Quaternion {
  union {
    struct {
      float x, y, z, w;
    };
    __m128 simd;
  };

  Quaternion() : x(0), y(0), z(0), w(1) {}
  Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
  Quaternion(__m128 simd) : simd(simd) {}

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
};