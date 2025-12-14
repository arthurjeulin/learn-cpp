### Compilation

### C++ Name Mangling
Name mangling (also called name decoration) is the process compilers use to generate unique names (symbols) for functions, variables, classes, and other program entitites in the object code to solve varioys problems caused by the need to resolve unique names..
This is essential for features like function overloading, namespaces, and templates, as the linker requires a unique identifier for each symbol.
#### Key Characteristics
Compiler-Specific: 
- The C++ standard does not define a universal name mangling scheme, different compilers use different conventions.
- This is intentional to prevent linking incompatible code compiled with different Application Binary Interfaces (ABI)
ABI-Defined: Mangling rules are typically part of a platform's C++ ABI (Application Binary Interface)
- Itanium C++ ABI: Widely used by GCC (GNU Compilier Collection) and Clang on Linux / Unix systems, where all mangled names start with `_Z`
- Microsoft C++ ABI: Used by the Microsoft Visual C++ (MSVC) compiler, where all mangled names start with `?`
Information Encoding: The mangled name encode various details about the symbol's signature, including:
- The original anme
- namespace or class names (scope)
- parameter type and tiehr order
- return type (sometimes)
- special qualifier like `const`or `volatile`and reference (& or &&) or pointer(*) indicators

#### Common Examples (Itanium ABI)
The Itanium ABI uses length-prefix identifiers(e.g `3Foo`for `Foo`) and specific letter codes for types:
| C++ Type / Element | Mangling Code | Example                      | Mangled Name |
|--------------------|--------------|------------------------------|--------------|
| `int`              | `i`          | `void func(int)`             | `_Z4funci`   |
| `double`           | `d`          | `void func(double)`          | `_Z4funcd`   |
| `bool`             | `b`          | `void func(bool)`            | `_Z4funcb`   |
| `char`             | `c`          | `void func(char)`            | `_Z4funcc`   |
| Pointer            | `P` (prefix) | `int* ptr`                   | `Pi`         |
| Reference          | `R` (prefix) | `int& ref`                   | `Ri`         |
| Namespace `ns`     | `N2nsE`      | `namespace ns { void f(); }` | `_ZN2ns1fE`  |