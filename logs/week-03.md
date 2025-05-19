## Theme of the week

Precise control at scope boundaries.

## What did I learn?

- A clean pattern for transactions - Wrapping a value and providing a current working copy, and a previous copy as well as the real copy. I have used this pattern in libraries in other languages, and I know they aren't implemented the same, but the core ideas carry over.
- RAII rollback is an excellent use case for destructors. If we forget to commit, the rollback is automatic : making code more robust with less effort.
- When and why to return references (`T&`) instead of copies (`T`): critical for memory efficiency and preserving intent.
- I practiced implementing explicit move/copy logic across multiple transaction types (`ScopedTransaction`, `Transaction`, `UndoableComponent`, etc.). I also learned a little more on how to build proper default constructors.
- I explored how transactions group together into a `TransactionGroup`, enabling scoped control over multiple independent values with a single commit or rollback.

## What was the most difficult thing to learn?

- Copy semantics vs. memory safety: especially when move-only or expensive types are involved. Getting types to line up properly in C++ can be a pain.
- Handling constructor overloads and making sure the types involved were properly initialized, this forced me to think about the state of each object during move/copy operations. Having a proper default constructor is also great for classes that will frequently end up in containers.
- Debugging allocation bugs and tracing lifetimes through verbose output was hard but rewarding.
- Understanding when a destructor should or should not act (e.g. rollback behavior in `Transaction<T>`). This boiled down to: do I manually run a `.rollback()` method in the desctructor? Not for all types of transactions. I built a 'passive' one that will just do nothing on the destructor and only saving on a `.commit()`.
- Making sure transactional state stayed in sync without leaking or aliasing ownership.

## How does this help me get to lead C++ developer?

- This week pushed me to design systems that are safe by default, where rollback and error paths are naturally handled by RAII.
- These systems mirror real-world needs in game engines: undo stacks in editors, atomic gameplay updates, transactional asset editing, etc.
- I’m becoming more fluent in template-based architecture, memory management, and scope-controlled lifetimes.
- I’m also building a deeper understanding of copy/move tradeoffs and how to architect around them in modular, reusable ways.
