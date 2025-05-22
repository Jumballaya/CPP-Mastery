## Theme of the week

Polymorphic Memory Resources & Frame Allocators

## What did I learn?

- I learned about `std::pmr`, and the `std::pmr::memory_resource` abstract class. The biggest take away was that I can create memory arenas that work directly with STL containers. I don't think I would use this everywhere, but it can be great for when I would reach for those containers.
- Working with the `std::pmr::vector` class challanged my practical knowledge of copy/move semantics and when they are used when using a vector.
- I learned about the `std::pmr::monotonic_buffer_resource` and `std::pmr::polymorphic_allocator` to get me going right out of the box.
- Im build the `PoolAllocator` learned how to use an intrusive free-list to maintain a O(1) allocation of available slots in the pool. It uses a linked-list of the next available slots.

## What was the most difficult thing to learn?

- Working with `std::pmr::vector` and
- Building `PoolAllocator` and trying to get it to working with heap allocated types like `std::string` was a pain and I abandoned it for just a pool of fixed-sized, data-only objects. Basically just for stuff like components.
- Building the intrusive free-list was difficult and bacame impossible to use with complicated containers. I had to do a lot of pointer math that took a lot of time to debug, but it gave me more pointer math experience.

## How does this help me get to lead C++ developer?

- Memory arenas and allocators are foundational to high-performing and well-written C++ software. Learning how to write this stuff and then practicing it means I can get into great habits early. That translates directly into hireability.
