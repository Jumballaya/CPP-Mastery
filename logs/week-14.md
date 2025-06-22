## Theme of the week

Lock-Free Queues

## What did I learn?

- More Multithreading terms: Lock-free and Wait-free. Lock-free means no locks are used and wait-free means that no thread is stuck spinning waiting for other threads to read/write.
- The Michael-Scott non-blocking algorithm. I used the algorithm as the basis for the `LockFreeQueue` class. This was done through the following steps:
  - The Queue is a ring buffer of memory chunks + meta data (a slot)
  - Sequence numbers for each slot in the queue. If `sequence == tail` then the queue is ready for writing to. If `sequence == head + 1` then it is read for reading. Otherwise something else is using it.
  - Using memory order (acquire & release)
  - Using Compare and Swap to claim a slot via `compare_exchange_weak` on the head and tail
- Compare and Swap `CAS`: It is an atomic operation that compares the atomic's previous value with the current and swaps in the new value if they are.

## What was the most difficult thing to learn?

- Race conditions are hard to fix when you are still learning the STL capabilities. Also, race conditions happened in a test function because the test was not properly setup. This caused me to chase a bug in my ThreadPool that was just in my demo.
- Progress tracking in the `AssetPipeline` class was a little difficult and I am not entirely pleased with my solution.

## How does this help me get to lead C++ developer?

- Multithreading is used at the core of most game engines, even hobby one. It is such a core skill.
- This will also head me understand how to build game engines that can do a lot more at once while keeping a smooth feel. Now I can run audio on a different thread than rendering and another thread for physics, etc. etc.
