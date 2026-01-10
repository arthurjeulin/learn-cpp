
## Template
### Abbreviated template syntax
En C++20, "abbreviated template syntax" désigne la possibilité d'écrire des fonctions template sans écrire `template<...>`, en utilisant directement des `concepts`ou `auto` dans la signature.
1. La forme la plus simple: auto paramètre
Avant **C++20**, pour un template génératique:
```cpp
template <class T>
void f(T x);
```
En **C++20**
```cpp
void f(auto x);
```
C'est équivalent à une fonction template avec un paramètre de type déduit.

### Concepts
Often, times when we are defining linke a function template we don't intend for it to be used for all possible data types.
For example create a template for differente kind of `integer`number. Like 8-bit, 16-bit, 32-bit and 64-bit but no floating point type for user-defined structure.

**The `concepts`library provides definitions of fundamental library concepts that can be used to perform compile-time validation of template arguments and perform function dispatch based on properties of type.**
These concepts provide a foundation for equational reasoning in programs.


```cpp
#include <iostream>
/**
 * @brief used abbreviate template syntax with from c++20
 */
void print(auto value)
{
  std::cout << "Printing integral value: " << value << "\n";
}
int main()
{
  print(10);
  return 0;
}
```

```bash
g++ concepts.cpp -std=c++20 -o out && ./out
```
Now add `concepts` to limite the usage of the template function to integral only
```cpp
#include <iostream>
#include <concepts>

/**
 * @brief used abbreviate template syntax with from c++20
 * @note use concepts to contrain the template to integer number only !
 */
void print(std::integral auto value)
{
  std::cout << "Printing integral value: " << value << "\n";
}
int main()
{
  print(10.1);
  return 0;
}
```
We get the compilation error:
```bash
arthur@Mac concepts-library % g++ concepts.cpp -std=c++20 -o out && ./out
concepts.cpp:14:3: error: no matching function for call to 'print'
   14 |   print(10.1);
      |   ^~~~~
concepts.cpp:8:6: note: candidate template ignored: constraints not satisfied [with value:auto = double]
    8 | void print(std::integral auto value)
      |      ^
concepts.cpp:8:12: note: because 'double' does not satisfy 'integral'
    8 | void print(std::integral auto value)
      |            ^
/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/__concepts/arithmetic.h:30:20: note: because 'is_integral_v<double>' evaluated to false
   30 | concept integral = is_integral_v<_Tp>;
      |                
```
Correct the error:
```cpp
#include <iostream>
#include <concepts>

/**
 * @brief used abbreviate template syntax with from c++20
 * @note use concepts to contrain the template to integer number only !
 */
void print(std::integral auto value)
{
  std::cout << "Printing integral value: " << value << "\n";
}
int main()
{
  print(10);
  return 0;
}
```