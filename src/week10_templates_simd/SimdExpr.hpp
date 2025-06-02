#pragma once

#include <xmmintrin.h>

template <typename Derived>
struct SimdExpr {
  inline const Derived& derived() const {
    return static_cast<const Derived&>(*this);
  }
  inline __m128 evaluate() const {
    return derived().evaluate();
  }
};