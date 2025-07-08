## Theme of the week

ECS Parallel Execution

## What did I learn?

- I learned that I can use my `LockFreeQueue<T>` with other systems, like the audio system, to build command queues. This is a powerful pattern because it allows me to utilize a system that has to run on a single thread, like the audio playback, from many other threads by queueing commands up in a thread safe way and running them all at once, once per frame.
- I learned how to build a dependency graph where not all of the node types are known from the start. In this case, Systems can be added from feature modules that are unknown to the core modules (including modules I haven't made yet!). I am using type traits to build type-based `DependsOn` and `Provides` at compile and during run-time the ECS builds the `TaskGraph` and sets dependencies based on the traits.
- I also used those traits on a per-System basis to show the components it `Reads` and `Writes` to for determining memory access order. All of this is done using `std::type_index` to link the dependencies without any hardcoding or even and `#include`s.

## What was the most difficult thing to learn?

- The hardest part was designing and implementing the distributed dependencies. Making sure no hardcoding or previous knowledge, or updates, were required. The key idea was modularity and to make sure that modules are never tightly coupled.

## How does this help me get to lead C++ developer?

- I got practice using traits to build out System dependencies, allowing for the graph to be laid out using compile-time type traits.
- I fully integrated the multithreaded ECS into the Journeyman Engine, a toy game engine I am building around the capstone of this project. This helped reinforce everything I built, and validated the modular build system that I had been thinking of using cmake with: modular libraries for `ecs`, `tasks`, and `async`.
