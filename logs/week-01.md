Some notes:

What did I learn:

- Templates are powerful and can help reduce allocations, but can also increase overhead due to dynamic dispatch and virtual table lookup.
- C++ can be extremely unsafe (when I hadn't written that method yet but it was working anyways)
- The C++ destructor is extremely powerful and allows for fine-grain control over cleanup
- std::string_view is really handy replacement for a const string

Difficulties:

- The C++ standard library is huge and can be daunting to navigate
- Operator overloading can be wildly complex with a lot of operators to consider with move and copy variants, assignment variants, etc. It is a lot to try to learn at once
- Understanding Lvalues and Rvalues syntax, I get the concept, but not good at picking up on it while reading code
-

How does this help me get to lead C++ developer:

- Understanding the C++ standard library is going to help me know what tools to grab for and when. Learning this stuff will allow me to translate my dev experience into C++.
- Understanding C++'s memory and ownership model will allow me to make fewer mistakes when writing C++ code
- Getting experience with the common tools to manage memory and objects in a C++ program is what is going to land me jobs, so all of this practice is going to help no matter what.
