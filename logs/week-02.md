## Theme of the week

Lifetimes and move/copy semantics.

## What did I learn?

- I learned how powerful rule of 5 is, making sure my underlying `T` was correctly implementing rule of 5 meant that when I got to the `TrackedVector`, I ended up writing very little code, and the code I did write was extremely easy to ready and reason about. I originally used memcpy like I did with the trackedbuffer, but quickly learned not to use it on actual classes with real resource management. Learning this gives me a lot of ideas, `TrackedBuffer` + `TrackedVector` could be combined to build vectors of buffers that can be addresses like memory, you could take the ideas from `TrackedVector` and make a `TrackedGraph` that uses the `TrackedBuffer`s to build out a graph memory structure, and an arena is similar, but with a giant chunk of memory underneath it
- Deep vs shallow copying with `std::memcpy` vs. the copy constructor or a copy assignment. With raw memory, like a buffer or a struct like a POD you can shallow copy the memory with memcpy. If it is a fully managed class, like a container, you need to rely on the copy semantics built into the language.
- I should understand move/copy semantics because it will help me offload work. I should trust that the classes I use have the correct move semantics, because it is easier to just call the assignment operator to initiate a copy than it is to manually call memcpy and try to figure out the underlying memory structure myself.
- Container as a policy not just a data structure. The type system of C++ is expressive in a way that allows fine-grain control of when resources are moved vs. copied, from a constructor or assignment, etc. The entire lifecycle of a container can be completely described in its types.
- This has sparked design ideas:
  - A `TrackedGraph` built from `TrackedBuffer` and `TrackedVector`
  - Memory arenas powered by raw chunk buffers with type-safe access
  - Graph-based memory layouts that are debuggable and inspectable

## What was the most difficult thing to learn?

- memcpy vs. RAII: I had to unlearn old habits. Using memcpy on resource-owning objects caused bugs, but this drove home a core C++ lesson: never copy an object manually if you don’t fully understand its internals.
- There are so many constructor overloads, it is hard to keep track of them. I also ran into the issue of needing a default constructor for containers to copy/move data into.
- Making sure not to double free when setting up move constructor/assignment operator. A few times I either freed something and didn't set it to nullptr or another ptr and the destructor tried to free it again causing an error.

## How does this help me get to lead C++ developer?

- I now own the object lifecycle, not just in theory but in practice. The Rule of 5 is embedded in my workflow. I can reason clearly about when resources are owned, moved, destroyed, or reused.
- These patterns are the backbone of low-level engine architecture: ECS, allocators, system managers, scene graphs, asset loaders, and more.
- I am getting more confident in my ability to write safe and predictable software in C++ and I can see myself starting to think more like an engine developer.
  - Encapsulate ownership
  - Trust RAII
  - Let types describe behavior
