#pragma once

#include <xmmintrin.h>

#include "Mat4f.hpp"
#include "SimdExpr.hpp"

template <typename L, typename R>
struct MatMulExpr : public SimdExpr<MatMulExpr<L, R>> {
  const L& lhs;
  const R& rhs;

  MatMulExpr(const L& l, const R& r) : lhs(l), rhs(r) {}

  inline Mat4f evaluate() const {
    const Mat4f& A = lhs.evaluate();
    const Mat4f& B = rhs.evaluate();

    alignas(16) float b[16];
    for (int i = 0; i < 4; ++i) {
      _mm_store_ps(&b[i * 4], B.row(i));
    }

    __m128 bCols[4] = {
        _mm_set_ps(b[12], b[8], b[4], b[0]),
        _mm_set_ps(b[13], b[9], b[5], b[1]),
        _mm_set_ps(b[14], b[10], b[6], b[2]),
        _mm_set_ps(b[15], b[11], b[7], b[3]),
    };

    Mat4f result{
        _mm_add_ps(
            _mm_add_ps(
                _mm_mul_ps(_mm_shuffle_ps(A.row(0), A.row(0), _MM_SHUFFLE(0, 0, 0, 0)), bCols[0]),
                _mm_mul_ps(_mm_shuffle_ps(A.row(0), A.row(0), _MM_SHUFFLE(1, 1, 1, 1)), bCols[1])),
            _mm_add_ps(
                _mm_mul_ps(_mm_shuffle_ps(A.row(0), A.row(0), _MM_SHUFFLE(2, 2, 2, 2)), bCols[2]),
                _mm_mul_ps(_mm_shuffle_ps(A.row(0), A.row(0), _MM_SHUFFLE(3, 3, 3, 3)), bCols[3]))),

        _mm_add_ps(
            _mm_add_ps(
                _mm_mul_ps(_mm_shuffle_ps(A.row(1), A.row(1), _MM_SHUFFLE(0, 0, 0, 0)), bCols[0]),
                _mm_mul_ps(_mm_shuffle_ps(A.row(1), A.row(1), _MM_SHUFFLE(1, 1, 1, 1)), bCols[1])),
            _mm_add_ps(
                _mm_mul_ps(_mm_shuffle_ps(A.row(1), A.row(1), _MM_SHUFFLE(2, 2, 2, 2)), bCols[2]),
                _mm_mul_ps(_mm_shuffle_ps(A.row(1), A.row(1), _MM_SHUFFLE(3, 3, 3, 3)), bCols[3]))),

        _mm_add_ps(
            _mm_add_ps(
                _mm_mul_ps(_mm_shuffle_ps(A.row(2), A.row(2), _MM_SHUFFLE(0, 0, 0, 0)), bCols[0]),
                _mm_mul_ps(_mm_shuffle_ps(A.row(2), A.row(2), _MM_SHUFFLE(1, 1, 1, 1)), bCols[1])),
            _mm_add_ps(
                _mm_mul_ps(_mm_shuffle_ps(A.row(2), A.row(2), _MM_SHUFFLE(2, 2, 2, 2)), bCols[2]),
                _mm_mul_ps(_mm_shuffle_ps(A.row(2), A.row(2), _MM_SHUFFLE(3, 3, 3, 3)), bCols[3]))),

        _mm_add_ps(
            _mm_add_ps(
                _mm_mul_ps(_mm_shuffle_ps(A.row(3), A.row(3), _MM_SHUFFLE(0, 0, 0, 0)), bCols[0]),
                _mm_mul_ps(_mm_shuffle_ps(A.row(3), A.row(3), _MM_SHUFFLE(1, 1, 1, 1)), bCols[1])),
            _mm_add_ps(
                _mm_mul_ps(_mm_shuffle_ps(A.row(3), A.row(3), _MM_SHUFFLE(2, 2, 2, 2)), bCols[2]),
                _mm_mul_ps(_mm_shuffle_ps(A.row(3), A.row(3), _MM_SHUFFLE(3, 3, 3, 3)), bCols[3])))};

    return result;
  }
};

template <typename L, typename R,
          typename = std::enable_if_t<
              std::is_same_v<L, Mat4f> &&
              std::is_same_v<R, Mat4f>>>
inline MatMulExpr<L, R> operator*(const SimdExpr<L>& lhs, const SimdExpr<R>& rhs) {
  return MatMulExpr<L, R>(static_cast<const L&>(lhs), static_cast<const R&>(rhs));
}