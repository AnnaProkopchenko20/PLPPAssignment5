1 What is an interpreter? It is a program that directly executes code line by line without compiling it first.
2. What is a declarative programming language? In declarative language you specify the result and not how to get that result.
3. What is the functional programming paradigm, and what are its key principles? Functional paradign is biul on a concept of high-order functons, avoiding state in program, pure lower-order functions.
4. Explain the concept of immutability in functional programming and its benefits. Once smth immutable is created it cannot be changed.
5. How can you create pure functions in Python/Haskell/your language? Just put all of the variables function result depends on in parameters.
6. What are higher-order functions? Functions that take as argument other functions.
7. Describe the role of I/O operations in functional programming and how they are typically 
handled. Examples of i/o operations are reading from files/databeses/user input. This is not goof for functional programming because its stresses the immutability and pure functions and these kind of outpus can change. As far as I understand in functional programming when working with changig data you copy changes and create new immutable object, minimize functions working with such data(encapsulation of sorts).
8. Compare and contrast interpreted languages with compiled languages. Interpreted: execute code line by line, not platform dependent, usually slower because interprets code at runtime Compiled: compiles into binaries and then gets executed, platform-dependent, quite fast.

About this project
It has Interpreter class that has one public method available (start()). It takes line, interprets it, returns result(0 if successful, detailed error, or int expression result). Can have variables and functions but has limitatins of only integer numbers. The inerpreting works in tree steps 1 identify whether it is an expession/variable function definition 2 if expression just parse and calculate if definition save variable/ function and its param names and their definition in dictionaries. 
For testing i wrote the edge cases(unary minus, global/ non-global variables, recursion, different blankspace and function combinations) i wanted to cover and some base cases. Ued ai to help me generate more test cases so I could pick the most pretty ones.
After countless hours of debugging, which made me sad and my code dirty, I know respect people, who write interpreters, like a lot.
