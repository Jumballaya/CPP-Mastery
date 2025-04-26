## Theme of the week

Templates & Lightweight Dispatch

## What did I learn?

- The C++ templating system is massive, complex and best of all expressive. I learned about type inference, specifically how C++ uses class overloading to split types like `float(int)` into the args and return types.
- I learned about traits and how to build utility classes that can do things like break up a tuple and apply it to another classes type signature. I learned how to add type constraints to allow for better compile-time type checking with traits as well.
- I learned how to properly erase function types and stored lambdas that wrapped a function execution + forwarded args. This allowed me to use a functional programming style inside C++.
- I got a lot more experience with lvalues and rvalues and how/when to use them and how to use them in VariantCallback together.
- I learned how to create a Document Object Model (DOM) style event system with a DOM tree and DOMNodes that all have `.addEventListener` just like JavaScript.

## What was the most difficult thing to learn?

- Template programming is tough and I ran into a lot of errors and spent a lot of time debugging and reading C++ docs. The errors were particularly dreadful and unhelpful a lot of the time.
- Trying to mentally keep track of which type was which in the template types where I am using class overloads, helper trait classes, helper constraints, etc. Some of the `template <>` declarations are 3-4 lines long.
- Having to do `x.template y<T>()` confused me. I get that it is because it causes confusion while parsing, but it feels weird.
- I keep having to learn the lesson that if you return something from a function, but you don't store the return value when you call that function: that return value still gets created and then goes out of scope, calling its destructor. I returned a `ListenerHandle` from `.addEventListener` and in my demo function, I never saved the handle. Well, the handle's destructor unsubs the listener function, so I spent 30 minutes hunting down the bug, just to realize all of this.

## How does this help me get to lead C++ developer?

- This is the real crazy C++ knowledge that I need to get good at. Know this kind of stuff and more advanced patterns will help me achieve a deep T shaped knowledge (broad knowledge in general, deep into 1 - 2 specific areas). I could be good at understanding how to build expressive, performant, and reliable C++ code using these techniques.
- Passing functions around is a core technique in most languages and patterns and unlocks a wide array of application architecture patterns, especially distributed/event-based/reactive/async patterns.
- Practice, Practice, Practice is always an answer for this section. Just putting in the hours writing code, and more hours reflecting and writing about, and even more hours reading code. There is no alternative to just putting in the time and getting the experience.
