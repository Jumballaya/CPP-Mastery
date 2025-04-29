## Theme of the week

Templates & Compile-time Policies

## What did I learn?

- I gained a lot more experience with compile-time policies using templates. I got to re-iterate on a lot of what I did last week, helping cement the knowledge.
- I learned more about the power of Allocators. I created a few different types of allocators and how to use them. I spent a lot of time getting a custom alloc/dealloc working and used it in a small demo.
- I learned how to build an allocator using static classes and select between them using the AlocatorSelector. This is an extremely powerful pattern

## What was the most difficult thing to learn?

- Building out the storage policies was tough at first, I had to do a lot of reading and external research to figure out how to do it. I struggled quite a bit, but in the long run it helped me out a whole lot.
- Creating the architecture for the mini-project (week06/events) was a pain. I tried a few ideas that were different from the previous week, but in the ended just copied the same architecture. A big issue was circlular dependencies between the Handle and Dispatcher.

## How does this help me get to lead C++ developer?

- Understanding low-level memory allocation and management will help me design and build performant and cpu/memory efficient software.
- Being able to create an API that manages pools of objects from raw memory in a developer-friendly way will allow my future code to be more accessible to developers. This is critical on-the-job where a wide variety of devs might have to use my APIs.
