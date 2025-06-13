# Case Study – Month 3

## AI Based Learning

ChatGPT is drifting more and more, but the good news is that I rely on it less and less. I use it to explore ideas and explain the relevant weekly ideas, as well as give me quizzess and critique my code. I can't rely on it for much more and anything in depth, I just have to spend the time learning it. I feel a lot more hopeful than last month even though I can see the ChatGPT's cracks widening.

It tends to spin around in circles where it will try to help me resolve an issue (setting up a cmake file to use a certain library for instance) and it will suggest a fix, then I run into an issue and the next fix is the code it just had me fix. I have to try and break out of the loop and hope I am not in a much, much larger loop.

It will try to do exactly what you ask, even if what you are asking is not what you want.

**Here is the month's case study:**

## Static Polymorphism and ECS Architecture

This month has been insanely insightful. I learned and practiced the following patterns:

- CRTP - Curiously Recurring Template Pattern
- Expression Templates
- SIMD

I got to re-use th following knowledge:

- RAII and Object Lifetime management
- Template traits and compile-time policies
- Custom allocators

The firsty big project was building the `Component<T>` class using CRTP. This is a pattern where you derive a class from a templated base-class with the _derived_ class as the template argument.

CRTP:

```cpp

template <typename T>
class X {};

class Y : public X<Y> {}
```

This allows for compile-time variants instead of using `virtual` indirection. I found this pattern great for things like `Component<T>` and any type of `Handle<T>` class for typed handles. One major drawback to this is the increased compile time.

Later on I learned and applied the Expression Template pattern. This pattern basically hi-jacks the normal operation flow to allow a lazy evaluation flow. I used this in a `Vec3f` struct that could lazily calculate expressions. It worked by creating a `Vec3Expr` class that wraps evaluation in a function called `.evaluate()`, so any `Vec3f + Vec3f` would produce a `Vec3Expr` from the lhs and rhs (which both have a `.evaluate()` themselves).

I learned about SIMD! This was crazy. The API feels a little too low-level and I hope they build something into the STL to support it instead of using the low level instrinsic functions. I was able to conver the `Vec3f` to SIMD and did a few simple benchmarks and the SIMD version did about 1.5x - 2x better, but this was a VERY simple test and not thorough or on any other machine with any other setup.

The `SimdExpr` looks like this:

```cpp
template <typename Derived>
struct SimdExpr {
  inline const Derived& derived() const {
    return static_cast<const Derived&>(*this);
  }
  inline __m128 evaluate() const {
    return derived().evaluate();
  }
};
```

This is an example of the `Vec3f` dot product function:

```cpp
  //    1. Multiply v1 * v2 --> (a.x * b.x), (a.y * b.y), (a.z * b.z)
  //    2. Move a.z*b.z to lane 0, then add it to a.x*b.x for a partial sum
  //    3. Broadcast a.y*b.y and add it to the partial sum for the full dot product
  //    4. Extract that sum value from lane 0
  template <typename Expr>
  float dot(const SimdExpr<Expr>& other) const {
    __m128 va = load();
    __m128 vb = other.evaluate();
    __m128 prod = _mm_mul_ps(va, vb);                            // [x*x', y*y', z*z', _]
    __m128 shuf = _mm_movehl_ps(prod, prod);                     // [z*z', ?, ?, ?]
    __m128 sum = _mm_add_ps(prod, shuf);                         // [x*x' + z*z', ?, ?, ?]
    shuf = _mm_shuffle_ps(prod, prod, _MM_SHUFFLE(1, 1, 1, 1));  // [y*y', y*y', y*y', y*y']
    sum = _mm_add_ss(sum, shuf);                                 // [dot, _, _, _]
    return _mm_cvtss_f32(sum);                                   // extract dot
  }
```

It was a lot of low level bit shifting (this time at 128 bytes!) and reading through docs and examples, etc. etc. to come to this and it is most likely FAR from performant, but I wanted to do it all in SIMD operations.

After that was building the ECS. I had created some before and once in C++ so I was able to lean on that knowledge + everything I've learned so far. I took a few days and built a small project to test out the ECS and it held up to everything I threw at it!

This was a very successful month and I look forward to the next.
