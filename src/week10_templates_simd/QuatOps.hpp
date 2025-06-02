#pragma once

#include <xmmintrin.h>

#include <type_traits>

#include "SimdExpr.hpp"

template <typename L, typename R>
struct QuatMulExpr : public SimdExpr<QuatMulExpr<L, R>> {
  const L& lhs;
  const R& rhs;

  QuatMulExpr(const L& l, const R& r) : lhs(l), rhs(r) {}

  inline __m128 evaluate() const {
    const __m128 a = lhs.evaluate();
    const __m128 b = rhs.evaluate();

    const float x1 = ((float*)&a)[0];
    const float y1 = ((float*)&a)[1];
    const float z1 = ((float*)&a)[2];
    const float w1 = ((float*)&a)[3];

    const float x2 = ((float*)&b)[0];
    const float y2 = ((float*)&b)[1];
    const float z2 = ((float*)&b)[2];
    const float w2 = ((float*)&b)[3];

    Quaternion q(
        w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2,
        w1 * y2 - x1 * z2 + y1 * w2 + z1 * x2,
        w1 * z2 + x1 * y2 - y1 * x2 + z1 * w2,
        w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2);

    return q.simd;
  }
};

template <typename L, typename R>
inline QuatMulExpr<L, R> operator*(const SimdExpr<L>& lhs, const SimdExpr<R>& rhs) {
  return QuatMulExpr<L, R>(lhs.derived(), rhs.derived());
}