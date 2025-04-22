## Theme of the week

Memory Views & Indexed Safety

## What did I learn?

- I learned a lot about handling readonly data and compile-time data. I worked a lot with const references and pointers and explored when to make use of read-only data structures.
- I can use constexpr and understand a lot more about how powerful it is. It feels like a builtin macro that is literally apart of the language, but somehow even more powerful.
- `SparseSlice<T>` can be used to hold a list of nullable items and showed me how to deal with pointers-to-pointers.
- `FieldOffsetTable<T>` gives introspection of offsets inside structs using pointer-to-member tricks.
- There are 4 main casts: `static_cast`, `const_cast`, `reinterpret_cast` and `dynamic_cast` and `C`-style casts actually try each of the 4 casts behind the scenes.

## What was the most difficult thing to learn?

- Managing `const`, `constexpr` and non-const data all together. I spent quite a long time reading compiler errors while trying to figure out the correct typings.
- `reinterpret_cast` for pointer-to-member offsets — tricky to reason about at first.
- Building `ConstexprView` with real compile-time guarantees — required avoiding runtime allocation, and I ended up compromising on some of the design as to not copy data.
- Keeping const-correctness and iterator types aligned across all the views.

## How does this help me get to lead C++ developer?

- These patterns map directly to ECS iteration, memory-mapped I/O, serialization, and GPU buffer management.
- Building `FieldOffsetTable<T>` gave me a little bit of insight into how reflection-like systems work in statically typed languages like C++.
- This deepened my understanding of lifetimes, layout, and non-owning views — the kind of system-aware code I’d need to write in engine cores.
- Understanding how to maximize compile-time calculations can lead to a lot of runtime optimizations and pursuing that niche could help me find a job fine-tuning a game engine.
