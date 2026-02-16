## Qualifier

### Definition
Qualifier in C++ (`const`,`volatile`,`mutable`,`restrict`) are keywords that modify the properties of data types, controlling how variables and pointers are treated by the compiler to improve safety, performance, and maintainability.  
They allow for `const correctness`, `optimization of memory access`, and `specific behavior` for member function.

#### Key C++ Type Qualifier
- `const`: Specifies that a variable's value cannot be changed after initialization ensuring immutability.
- `volatile`: Tells the compiler that a variable's value may change unexpectedly (e.g via ahrdware or another thread), preventing improper optimization.
- `mutable`: Allows a member of a `const`object to me modified, often used for caching or mutexes.
- `restrict`: (compiler-specific/upcoming standard): A hint that a pointer is the only way to access the object it point to, enabling optimization.

### Related Keywords
- `constexpr`: used for compile-time evaluation
- `constinit`: Ensures a variable is initialized at compile time.

#### `const-correctness`
`const-correctness` est un idiome / bonne pratique idiomatique en C++.  
C'est un ensemble de conventions d'écriture qui exploitent le système de type `const`