# Case Study – Month 2

## AI Based Learning

ChatGPT is not getting any better. I have to rely on external sources and documentation a lot more. This isn't so bad as it is giving me a lot more practice time than last month.

I recommend doing this even less after these last 4 weeks. I am not sure what the next 4 are going to be like. In week 5 I added a small project to see if ChatGPT could walk me through the whole thing from start to finish without going off course. It laid out a plan that seemed like it was a good idea, but I soon ran into implementation issues that it was unable to solve. I spent some time and re-architected the solution and used ChatGPT to help me clean it up and improve it a bit. The project is a simple DOM tree with Elements that all have an `.addEventListener()` method on them just like in the browser.

In week 6 I tried to work on a small project again, with even more problems from the AI. I ended up solving the issues on my own and had ChatGPT grade/critique the code like I did the week before. This time I created a pub-sub event listener/dispatcher on an Entity that might be used in an ECS. This process made me stop trying to create a project each week, ChatGPT just isn't ready for that, at least not in the way I am using it.

Week 8 was filled with issues. I spent a lot of time learning about `std::pmr`, monotonic resources, polymorphic containers, etc. There were a lot of pitfalls with the way ChatGPT initially presented the topic that didn't really set me up to be able to create the weekly classes. It had issues building the PoolAllocator to work with certain types of classes like `std::string` and `std::vector`. This caused a lot of frustration and a lot of reading and practice.

I hope the next month is a little more forgiving.

**Here is the month's case study:**

## Advanced Templates, Type Safety, STL Mastery

This month has been a tour of how to use templates to create expressive types, dynamically and statically as well as being able to create and manage chunks of memory that hold those types.

I have always loved the expressive type system of TypeScript, but C++ takes it to a whole new level for me. Utilizing types as values (meaning that they are actually manipulatable as part of the language and not just stripped away like in TS) and being able to use the Trait classes means that I can write code that fundementally changes the other bits of code I use it on. Meta-templates/trait-classes/etc. are extremely powerful. For example, I can create a class that can be used to break up a type `T` into a return type and the function arguments tuple if `T` is a function:

```cpp

template <typename Fn>
struct SignatureTraits;

template <typename R, typename... Args>
struct SignatureTraits<R(Args...)> {
  using Return = R;
  using ArgsTuple = std::tuple<Args...>;

  template <typename Callable>
  using IsCompatible = std::is_invocable_r<R, Callable, Args...>;
};

```

This can be applied like `SignatureTraits<int(string)>` and it would return `int` for `SignatureTraits<int(int)>::R` and `std::tuple<string>` for `SignatureTraits<int(int)>::ArgsTuple`. It also provides a way to see if an object can be used as the callable object by: `SignatureTraits<int(int)>::IsCompatible<Callable>`.

Building on this, I created another struct that can be used to select a compatible `Fn` signature from a list of `Fn`s

```cpp

template <typename Callable, typename... Fns>
struct SelectMatchingFn {
  using type = void;
};

template <typename Callable, typename Fn, typename... Rest>
struct SelectMatchingFn<Callable, Fn, Rest...> {
  using type = std::conditional_t<
      SignatureTraits<Fn>::template IsCompatible<Callable>::value,
      Fn,
      typename SelectMatchingFn<Callable, Rest...>::type>;
};

```

This allows for `SelectMatchingFn<Callable, void(void), int(int), int(float), int(string)>` etc. etc.

It would then find the correct function signature using `std::conditional_t`.

Most of the rest of the month was spent on various way of creating allocators. I created allocator using static methods on Allocator types, as well as an instanced Allocator to back `std::pmr::memory_resource`.
