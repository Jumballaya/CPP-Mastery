# Case Study – Month 1

## AI Based Learning

This month, I setup ChatGPT up to act as a 'C++ Mentor' and had it build me a 24 week course to help increase my C++ skills and experience in an effort to be employable to work on a game engine or game dev tooling. I spent an or so with it to set up the ground rules, reasons and come up with learning path.

I was surprised by how well it stayed on track and stuck to the course it had built so far. It did veer off course a little bit each week, especially between days, but it was easy enough to remind it of what we really should be on. Sometimes in the middle of the Monday lesson it would try to wrap up the week and I would have to remind it that the day was still Monday. Besides that it gave me great feedback and provided a lot of material for me to work on and interact with.

I wouldn't recommend learning a new skill with an AI agent like ChatGPT, but it is great for going deeper into skills. I have already had instances where it has produced basically nonsense code (some examples: using methods that havent been talked about or implemented, telling me a method needs to return a const and then giving me test code that runs that method and tries to mutate the returned object, etc.). This has been fixable by pointing out its mistakes, asking it why it gave that code, things like that. It has usually corrected its mistakes and produced the correct code.

Another quick blurb about the AI: It is set up to never give me code that I can copy/paste, and it is working pretty well. It is giving me pseudocode, text explainations and implementations examples, but never the actual code that I need to write. I have it in this cycle where it gives me a class to write, the requirements and a demo function to run in that week's `main.cpp` that has some `std::cout`s. I paste the console output after running and it critiques my code without seeing it. Once I do that, I give it my actual class and it will critique it and give me ideas of where to make it better or where to take the class in the future.

**Here is the month's case study:**

## Memory, Ownership, and Lifetimes: Foundational Systems for Safe Engine Code

The last few weeks have been focused on RAII, object lifetimes and resource ownership. I created the following classes:

- `ResourceManager<T>` — RAII + smart pointer ownership
- `TrackedObject`, `TrackedVector`, `TrackedBuffer` — Rule-of-5 lifecycle tracing
- `ScopedGuard`, `ScopedTransaction`, `TransactionGroup` — rollback + atomicity
- `UndoableComponent`, `UndoableRegistry`, `UndoableEntity` — local edit/undo/commit
- `SliceView`, `ConstexprView`, `SparseSlice` — safe, non-owning memory views
- `FieldOffsetTable<T>` — pointer-to-member offset introspection

## Problem Space

Every game engine is, at its core, an abstract over memory. Having a mastery over that memory gives the engine predictable behavior for critical system (assets, components, simulations, etc.) and understanding how to properly access the data via move or copy can drastically impact your engine's performance positively. Game engines need to be fast and safe when accessing data and a lot of containers don't need ownership and possibly shouldn't even have ownership of underlying resources. Game engines need to be able to track changes, commit changes atomically, and have automatic rollback in case of failure or non-comittal.

I used this month to gain a solid understand of how data is store and utilized in C++ and how to write my classes to effictively manage that data. I wrote 29 classes for month 1 and most of the were repetitive, but they solve the problems presented above.

**Let's talk about some key classes**

### `TrackedObject<T>` [Week 2] Rule of 5

I implemented a base class that logs every constructor, assignment, and destructor call — showing me exactly how my types behave during vector push_back, copying, moving, and destruction. This was critical for debugging:

```cpp
TrackedObject a("A");
std::vector<TrackedObject> vec;
vec.push_back(a);             // Copy
vec.push_back(std::move(a));  // Move
```

This class went on to be the base for many other classes, and provided a great way to see move/copy effects in later weeks.

### `ScopedTransaction<T>` [Week 3] Scoped Lifetimes

Gives me rollback semantics using RAII — commit if needed, otherwise rollback on scope exit.

```cpp
ScopedTransaction<Buffer> tx(myBuffer);
tx.get().write(...);
if (ok) tx.commit();  // otherwise automatic rollback
```

This pattern ensures safety in AI, physics, or asset systems where multiple actions must succeed atomically.

### `UndoableComponent<T>` [Week 3] Memory Management

Three copies: `committed`, `working`, `rollback`. This gave me full undo/redo support in `UndoableRegistry` and `UndoableEntity`.

```cpp
entity.edit("health").resize(100);
entity.commit("health");
entity.undo("health");
```

This mimics systems like Unity's inspector + undo stack or editor scripting tools.

### `SliceView<T>`, `ConstexprView<T, N>`, `SparseSlice<T>` [Week 4] Ownerless Views

Let me build readonly, type-safe access to:

- Full slices of data (e.g., particles, transforms)
- Sparse slices (e.g., selected units in RTS)
- Compile-time array introspection

```cpp
SliceView<int> view(arr.data(), arr.size());
ConstexprView<int, 4> view = ConstexprView::from_array(std::array<int, 4>{...});
SparseSlice<Component> selection({ &a, &b, &c });
```

I also learned to avoid misuse of `memcpy` on anything with ownership — I now rely strictly on copy/move constructors.

## Real-World Engine Connections

| System                 | Real-World Use                                    |
| ---------------------- | ------------------------------------------------- |
| `ScopedTransaction<T>` | Unreal’s `FScopedTransaction`                     |
| `UndoableComponent`    | Unity’s `Undo.RecordObject()`                     |
| `SliceView`            | ECS archetype iteration, GPU memory views         |
| `FieldOffsetTable`     | Field-level data binding, hot reloading, debug UI |

## What I’d Improve

- **TrackedVector**: Move away from `memcpy`, use smart pointer containers or iterators.
- **ConstexprView**: Revisit design — very limited benefit right now, but a good exploration of `constexpr`.
- **UndoableEntity**: Support nested fields or composed undo trees — possibly add serialization hooks later.
- **General**: Move away from raw pointers as much as possible. This month was about ownership, including pointers, but in the future smart pointers seem like the way to go.

## Closing Thought

I used to write C++ like C with classes. Now I’m starting to think in ownership, rollback, and architectural boundaries. This shift is key to writing lead-level engine code.
