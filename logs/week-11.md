## Theme of the week

ECS Core Architecture

## What did I learn?

- ECS Architecture and a little more about data-oriented-design. I learned more about data locality and how it can be properly organized in C++.
- I learned how I can abstract iteration of a sets variable amount of objects, this allowed me to expose an iterator that skips invalid entities without making the end user do anything themselves.
- I had to lookup and learn a whole lot about `std::tuple` which helped me build the `View<Ts...>` class.

## What was the most difficult thing to learn?

- Getting `View<Ts...>` to compile and behave correctly was surprisingly difficult. The template error messages were brutal, especially around `std::get` and tuple unpacking. I had to do a lot of reading on variadic templates, tuple indexing and iterator design.
- Designing the system to do what I wanted required constant decisions around ownership, iteration, ergonomics, and constraints.

## How does this help me get to lead C++ developer?

- Keeping data organized by how the program will access it instead of where the compiler puts it will help me write better and faster programs.
- This week proves I can build foundational engine systems from scratch. I now have:
  - A complete working ECS core that is starting to mirror industry designs (Unity, EnTT, Bevy)
  - Clear separation of concerns and ownership semantics
  - A mental model for how game engines manage entity state, system updates, and memory efficiently
