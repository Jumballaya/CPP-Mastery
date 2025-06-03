#pragma once

#include <xmmintrin.h>

#include <iostream>

#include "SimdExpr.hpp"

struct alignas(16) Mat4f : public SimdExpr<Mat4f> {
  __m128 rows[4];

  Mat4f(__m128 r0, __m128 r1, __m128 r2, __m128 r3) {
    rows[0] = r0;
    rows[1] = r1;
    rows[2] = r2;
    rows[3] = r3;
  }

  template <typename Expr>
  Mat4f(const SimdExpr<Expr>& expr) {
    const auto& e = static_cast<const Expr&>(expr);
    Mat4f tmp = e.evaluate();
    for (int i = 0; i < 4; ++i) {
      rows[i] = tmp.row(i);
    }
  }

  template <typename Expr>
  Mat4f& operator=(const SimdExpr<Expr>& expr) {
    Mat4f result = expr.evaluate();
    for (int i = 0; i < 4; ++i) {
      rows[i] = result.row(i);
    }
    return *this;
  }

  static Mat4f identity() {
    return Mat4f{
        _mm_set_ps(0, 0, 0, 1),
        _mm_set_ps(0, 0, 1, 0),
        _mm_set_ps(0, 1, 0, 0),
        _mm_set_ps(1, 0, 0, 0),
    };
  }

  // Row access
  __m128& row(int i) { return rows[i]; }
  const __m128& row(int i) const { return rows[i]; }

  const Mat4f& evaluate() const {
    return *this;
  }

  void print(const char* label = "") const {
    std::cout << label << std::endl;
    alignas(16) float r[4];
    for (int i = 0; i < 4; ++i) {
      _mm_store_ps(r, rows[i]);
      std::cout << "[ " << r[0] << ", " << r[1] << ", " << r[2] << ", " << r[3] << " ]\n";
    }
  }
};