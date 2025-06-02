#pragma once

#include <xmmintrin.h>

#include <type_traits>

#include "SimdExpr.hpp"

template <typename L, typename R>
struct VecAddExpr : public SimdExpr<VecAddExpr<L, R>> {
  const L& lhs;
  const R& rhs;

  VecAddExpr(const L& l, const R& r) : lhs(l), rhs(r) {}

  inline __m128 evaluate() const {
    return _mm_add_ps(lhs.evaluate(), rhs.evaluate());
  }
};

template <typename L, typename R>
inline VecAddExpr<L, R> operator+(const SimdExpr<L>& lhs, const SimdExpr<R>& rhs) {
  return VecAddExpr<L, R>(static_cast<const L&>(lhs), static_cast<const R&>(rhs));
}

template <typename L, typename R>
struct VecMulExpr : public SimdExpr<VecMulExpr<L, R>> {
  const L& lhs;
  const R& rhs;

  VecMulExpr(const L& l, const R& r) : lhs(l), rhs(r) {}

  inline __m128 evaluate() const {
    return _mm_mul_ps(lhs.evaluate(), rhs.evaluate());
  }
};

template <typename L, typename R>
inline VecMulExpr<L, R> operator*(const SimdExpr<L>& lhs, const SimdExpr<R>& rhs) {
  return VecMulExpr<L, R>(static_cast<const L&>(lhs), static_cast<const R&>(rhs));
}

template <typename T, typename S>
struct VecMulScalarExpr : public SimdExpr<VecMulScalarExpr<T, S>> {
  const T& expr;
  S scalar;

  VecMulScalarExpr(const T& e, S s) : expr(e), scalar(s) {}

  inline __m128 evaluate() const {
    return _mm_mul_ps(expr.evaluate(), _mm_set1_ps(scalar));
  }
};

// vector * scalar
// This use of type_traits is to make sure that the compiler can
// correctly infer the correct types, and not try to use the
// VecMulExpr from above
template <typename T,
          typename S,
          typename = std::enable_if_t<
              std::is_base_of_v<SimdExpr<T>, T> && std::is_arithmetic_v<S>>>
inline VecMulScalarExpr<T, S> operator*(const T& expr, S scalar) {
  return VecMulScalarExpr<T, S>(expr, scalar);
}

// scalar * vector
template <typename S,
          typename T,
          typename = std::enable_if_t<
              std::is_base_of_v<SimdExpr<T>, T> && std::is_arithmetic_v<S>>>
inline VecMulScalarExpr<T, S> operator*(S scalar, const T& expr) {
  return VecMulScalarExpr<T, S>(expr, scalar);
}