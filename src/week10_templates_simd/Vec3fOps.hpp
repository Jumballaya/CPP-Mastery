#pragma once

#include <xmmintrin.h>

#include "VecExpr.hpp"

template <typename L, typename R>
struct VecAddExpr : public VecExpr<VecAddExpr<L, R>> {
  const L& lhs;
  const R& rhs;

  VecAddExpr(const L& l, const R& r) : lhs(l), rhs(r) {}

  inline __m128 evaluate() const {
    return _mm_add_ps(lhs.evaluate(), rhs.evaluate());
  }
};

template <typename L, typename R>
inline VecAddExpr<L, R> operator+(const VecExpr<L>& lhs, const VecExpr<R>& rhs) {
  return VecAddExpr<L, R>(static_cast<const L&>(lhs), static_cast<const R&>(rhs));
}

template <typename L, typename R>
struct VecMulExpr : public VecExpr<VecMulExpr<L, R>> {
  const L& lhs;
  const R& rhs;

  VecMulExpr(const L& l, const R& r) : lhs(l), rhs(r) {}

  inline __m128 evaluate() const {
    return _mm_mul_ps(lhs.evaluate(), rhs.evaluate());
  }
};

template <typename L, typename R>
inline VecMulExpr<L, R> operator*(const VecExpr<L>& lhs, const VecExpr<R>& rhs) {
  return VecMulExpr<L, R>(static_cast<const L&>(lhs), static_cast<const R&>(rhs));
}

template <typename T, typename S>
struct VecMulScalarExpr : public VecExpr<VecMulScalarExpr<T, S>> {
  const T& expr;
  const S& scalar;

  VecMulScalarExpr(const T& e, S s) : expr(e), scalar(s) {}

  inline __m128 evaluate() const {
    __m128 v = expr.evaluate();
    __m128 s = _mm_set1_ps(scalar);  // set the scalar on all channels
    return _mm_mul_ps(v, s);
  }
};

template <typename T, typename S>
inline VecMulScalarExpr<T, S> operator*(const VecExpr<T>& expr, S scalar) {
  return VecMulScalarExpr<T, S>(static_cast<const T&>(expr), scalar);
}

template <typename T, typename S>
inline VecMulScalarExpr<T, S> operator*(S scalar, const VecExpr<T>& rhs) {
  return VecMulScalarExpr<T, S>(static_cast<const T&>(rhs), scalar);
}