## Introduction to `constexpr` specifier since C++11
**The `constexpr`specifier declares that is possible to evaluate the value of the function or variable at compile time**.
This is use to move computation from runtime to compile time !

Example:

```cpp
#include <iostream>

/**
 * @brief Function that calculates the factorial of some integer n
 */
constexpr int integer_fact(int n)
{
  if(n>1)
  {
    return n*integer_fact(n-1);
  }
  else
  {
    return 1;
  }
}
int main()
{
  constexpr int result = integer_fact(5);
  std::cout << "Compute Factorial of: " << result << "\n";
  return 0;
}
```
Run
```bash
g++ constexpr.cpp -o out --std=c++20 && ./out
```
Check the assembly of our language
```bash
# d to dissassebme a program
# c demangle name
objdump -dC out
```
Resultat:
```bash
arthur@Mac-4721 constexpr % objdump -dC ./out                            

./out:  file format mach-o arm64

Disassembly of section __TEXT,__text:

00000001000031f0 <_main>:
1000031f0: d10083ff     sub     sp, sp, #0x20
1000031f4: a9017bfd     stp     x29, x30, [sp, #0x10]
1000031f8: 910043fd     add     x29, sp, #0x10
1000031fc: 52800008     mov     w8, #0x0                ; =0
100003200: b90007e8     str     w8, [sp, #0x4]
100003204: b81fc3bf     stur    wzr, [x29, #-0x4]
100003208: 52800f08     mov     w8, #0x78               ; =120 <== Resultat de l'appel à integer_fact
10000320c: b90003e8     str     w8, [sp]
100003210: b9000be8     str     w8, [sp, #0x8]
100003214: b0000000     adrp    x0, 0x100004000 <_strlen+0x100004000>
100003218: f9402000     ldr     x0, [x0, #0x40]
10000321c: 90000001     adrp    x1, 0x100003000 <_strlen+0x100003000>
100003220: 913bb021     add     x1, x1, #0xeec
100003224: 9400000a     bl      0x10000324c <std::__1::basic_ostream<char, std::__1::char_traits<char>>& std::__1::operator<<[abi:ne180100]<std::__1::char_traits<char>>(std::__1::basic_ostream<char, std::__1::char_traits<char>>&, char const*)>
100003228: b94003e1     ldr     w1, [sp]
10000322c: 940002f9     bl      0x100003e10 <_strlen+0x100003e10>
100003230: 90000001     adrp    x1, 0x100003000 <_strlen+0x100003000>
100003234: 913c0c21     add     x1, x1, #0xf03
100003238: 94000005     bl      0x10000324c <std::__1::basic_ostream<char, std::__1::char_traits<char>>& std::__1::operator<<[abi:ne180100]<std::__1::char_traits<char>>(std::__1::basic_ostream<char, std::__1::char_traits<char>>&, char const*)>
10000323c: b94007e0     ldr     w0, [sp, #0x4]
100003240: a9417bfd     ldp     x29, x30, [sp, #0x10]
100003244: 910083ff     add     sp, sp, #0x20
100003248: d65f03c0     ret
```
In C++, the `operator()` is known as the function **call operator**. 
```cpp
#include <iostream>
#include <random>
/**
 * @brief Function that calculates the factorial of some integer n
 */
constexpr int integer_fact(int n)
{
  if(n>1)
  {
    return n*integer_fact(n-1);
  }
  else
  {
    return 1;
  }
}
int main()
{
  // Create a random device
  std::random_device rd;
  int result_random = integer_fact(rd()%6);
  constexpr int result = integer_fact(5);
  std::cout << "Compute Factorial of: " << result << "\n";
  std::cout << "Compote Factoriel of: " << result_random << "\n";
  return 0;
}
```