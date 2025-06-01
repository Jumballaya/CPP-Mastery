#pragma once

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
};