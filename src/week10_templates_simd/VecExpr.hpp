#pragma once

#include <xmmintrin.h>

template <typename Derived>
struct VecExpr {
  // Forward to the derived .evaulate()
  inline __m128 evaluate() const {
    return static_cast<const Derived&>(*this).evaluate();
  }
};