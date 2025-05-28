## Theme of the week

CRTP & Static Polymorphism

## What did I learn?

- `CRTP`: Curiously Recurring Template Pattern - This is when you have a class `T` that derives from a templated class using `T` as the template argument. Example: `class T : public Base<T> {};`. This allows for compile time polymorphism without the use of `virtual`. This is an extremely powerful tool because it can reduce the usage of `virtual` methods, lookup tables and indirection.
- Building the Component to using an allocator now, for later, was a lot simpler than I thought it would be.

## What was the most difficult thing to learn?

- Not really anything this week. I have used ECS libraries and built my own before in different languages. CRTP is really cool and I think I got it right away.

## How does this help me get to lead C++ developer?

- Working towards completely removing `virtual`, at least in critical code, is something that I think a lot of game engine devs do so this pattern will go a long ways.
- I am definitely on the hunt for remove costly abstractions!
