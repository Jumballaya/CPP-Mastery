## Theme of the week

Custom Container Implementations

## What did I learn?

- I learned more about manual memory management beyond pointers, but how to get a fine-grained control over that memory.
  - Use `::operator new(...)` to get raw memory,
  - Use `new (ptr) T(...)` to construct in-place,
  - Use `ptr->~T()` to destroy manually,
  - Use `::operator delete(...)` to release memory (optional if it’s arena-managed)
- I learned more about how to create stack-allocated data structures

## What was the most difficult thing to learn?

- Comprehending the different memory allocation and construction patterns and when to use them.
- Tracking the `_head` and `_tail` on the `RingBuffer`
- I am still getting used to setting up Rule-of-5 properly, but it is almost there.

## How does this help me get to lead C++ developer?

- Learning the low-level tools for memory management like in the `RingBuffer` and the `PagedVector` will help a lot.
- Continuing to get experience creating container classes will help me write containers faster and safer.
