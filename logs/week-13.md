## Theme of the week

Threads & Async

## What did I learn?

- `std::thread` and `std::mutex`. I learned how to create an abstraction that feels similar to simple goroutines.
- I explored thread-safe task submission in the form of `std::function<void()>` tasks. This could be expanded into a struct to wrap the type-erased function with state + functionality similar to a JS promise.
- I learned about network programming in C++. It is pretty straight forward, and is a lot like creating server in other languages: bind to IP and port and then listen in a loop. It is just more involved and more OS oriented (I am using Linux).
- I built a simple HTTP server (serves up a small bit of static HTML). I also built a TCP client/server for a simple text-based TCP protocol that was based off of URL query parameters, e.g. ?x=4&y=hello, with types so it looked like: ?int:x=4&str:y=hello. Both used the `ThreadPool` class to dispatch the connection handlers.

## What was the most difficult thing to learn?

- Timing the ThreadPool tasks, my early experiments made a lot of assumptions about how multithreading works based off of how it works in other languages. The main thing was making sure that my main thread ran long enough.
- Parsing data from the client and server on the TCP commands was tough. I had to make extensive use of the debugger to check values as the program ran.
- Handling dangling file descriptors was tough in some parts. Create proper RAII infrastructure solved most of the issues and the other solutions were mostly about timing of function execution.

## How does this help me get to lead C++ developer?

- Building strong, secure and performant multithreaded code is going to be key in proving that I can work as a senior game engine developer. These skills are just starting to grow, but after hours of practice it will directly help me land a job.
- I am getting better at designing APIs in and architecting C++ code better than ever. 11 more weeks left and I might actually come out of this a strong C++ developer.
