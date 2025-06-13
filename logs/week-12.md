## Theme of the week

Concepts & Constraints: compile-time type safety

## What did I learn?

- I applied a lot of compile-time strategies, but this week was more about cleaning up the ECS and adding guard rails.
- I did learn about the `concept` keyword and use it on the guard rails. This allows compile-time constraints, like asserts, but built into the templating system.

## What was the most difficult thing to learn?

- Architecting a solution that removes and circular dependencies while maintaining a clear boundary between APIs. I either had way to much coupling or no coupling but circular dependencies. In the end it was about simplifying the API and creating different layers based on domain: Is it a handle? Is it internal the ECS? Is the intent for it to be exposed? questions like that.

## How does this help me get to lead C++ developer?

- Learning good and bad patterns as well as really understanding my own C++ habits will help me better my skills by giving me metrics to watch.
