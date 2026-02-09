## C++ Best Pratictes: Always Const
**optimisation à garder en tête**:
- initialization direct = > Construire le bon object du premier coup.
### initilizatoin vs default construct + assign
```cpp
#include <string>
// By forgetting const, we call the constructor then do assignment
std::string s;
s = "Some simple string for initialization";
```
1. constructeur par défault de `std::string`(souvent construit une string vide)
2. `operator=` depuis un const char* (alloue / copier si nécessaire)
Call the constructor with initialization
```cpp
#include <string>
// Now the strings constructor perform initialization
const std::string s = "Some simple string for initialization";
```
1. construction direct à partir du littéral ( constructor `std::string` (const char*))
Another example
```cpp
#include <string>
#include <benchmark/benchmark.h>

static void baseBench(benchmark::State &s)
{
  const int i = rand();
  while (s.KeepRunning())
  {
    // By forgetting const, we call the constructor then do the assignment
    std::string s;
    switch (i% 4)
    {
      case 0:
        s = "The string from case 0!";
        break;
      case 1:
        s = "The string from case 1!";
        break;
      case 2:
        s = "The string from case 2!";
        break;
      case 3:
        s = "The string from case 3!";
        break;
    }
  }
}
```
Fix proposiiton

```cpp
static void baseBench(benchmark::State &s)
{
  const int i = rand();
  while (s.KeepRunning())
  {
    // By forgetting const, we call the constructor then do the assignment
    const std::string s = [&]()
    {
      switch (i% 4)
      {
        case 0:
          return "The string from case 0!";
        case 1:
          return "The string from case 1!";
        case 2:
          return "The string from case 2!";
        case 3:
          return "The string from case 3!";
        default:
          return "Some string to shut the compiler up"
      }
    }(); 
  }
}
```

**NRVO: Named Return Value Optimization is a C++ compiler optimization that eliminates unnecessary copy/move operations by constructing a name, local object directly in the caller's stack frame.**