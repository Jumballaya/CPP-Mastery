## Theme of the week

Expression Templates & SIMD

## What did I learn?

- Expression templates can be powerful, but hard to reason about when trying to expand on them. It was easy to get them working with Vec3f and Quaternion, but harder to add Vec3f x Scalar and even harder to add Mat4f support.
- Lazy evaluation with Expression Templates is a great tool to have. This gave some speed-up to chained operations in the Vec3f compared to a naive implementation.
- SIMD instrinsics (SSE). I used `_mm_add_ps`, `_mm_mul_ps`, and more to create a SIMD backed Vec3f.
- I wrote a simple 'benchmark' to test the naive vec3f with the SIMD version and saw almost a 1.5x speed up.

## What was the most difficult thing to learn?

- Expression templates using traits to narrow down types using SFINAE can become a pain to reason about, and eventually, to read. The `Vec3fOps.hpp` specifically is a bit hard to read and understand. I just tinkered until it worked, and I think for this pattern to be really useful, you need a clear plan before implementing.
- Understanding how to use SIMD is a switch in thinking. I got a taste of some of the SSE2 and SSE3 operations, I am eager to continue learning.

## How does this help me get to lead C++ developer?

- SIMD is an extremely powerful tool that I need to learn to master low-level performance optimizations. I can now start to think about where I can use SIMD and then go off and build a few projects to learn how to properly use it.
- Expression templates are also pretty awesome and could be well utilized in something like a scipting language or a render tree, or something like a DOM. I am not sure if it is widely used in the game engine world, but if so then that is another great reason to keep practicing it.
