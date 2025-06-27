## Theme of the week

Fibers & Task Graphs

## What did I learn?

- I learned about many different ways of building a job system. I learned about fibers and threads and how game engines use them for light-weight task switching to back their job systems.
- I added work stealing by using a queue per thread and allowing available threads to steal work from another thread.
- I designed a move-only inline job storage that can store a type-erased function and all of its storage inline with move-only semantics using a variable amount of space using a templated struct.
- I created a task graph that the job system can use to execute a graph of jobs.

## What was the most difficult thing to learn?

- Making sure to use the correct memory order when working with atomics.
- Architecting the Job System so that I can do long running and frame-bound tasks. Right now it only really supports frame-bound tasks.
- Architecting the jobs themselves so that I can wrap functions and inject cleanup & on-complete callback calls into the actual job that gets stored took me a while to get down.

## How does this help me get to lead C++ developer?

- Systems like this can make or break bigger engines. Having this knowledge means that I can start to build better system using multithreading and atomics. It also helps me evaluate other people's code that does this better.
- This system is foundational for ECS, asset loading, AI scheduling, and rendering pipelines in a modern game engine. If I ever get a job working on systems like those, I will need a solid foundation in async task execution.
