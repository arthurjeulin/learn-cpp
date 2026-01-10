## Pseudo-random number generation
**The random number library provides classes that generate random and pseudo-random numbers.**

first example:
```cpp
#include <iostream>
#include <random> // since c++11

int main()
{
  // Non-deterministics random-number
  // random_device: non-deterministic random generator using harware entropy source
  std::random_device rd;
  for(size_t i = 0; i < 10; i++)
  {
    std::cout << "Generate: " << rd() << "\n";
  }
  return 0;
}
```
Run
```bash
g++ random.cpp -std=c++20 -o out && out && ./out
```
Example with Merseene twister engine
```cpp
#include <iostream>
#include <random> // since c++11

int main()
{
  // Non-deterministics random-number
  std::mt19937 mt;
  for(size_t i = 0; i < 10; i++)
  {
    std::cout << "Generate: " << mt() << "\n";
  }
  return 0;
}
```

Example with number from 0 to 9
```cpp
#include <iostream>
#include <random> // since c++11

int main()
{
  // Non-deterministics random-number
  std::random_device rd;
  for(size_t i = 0; i < 50; i++)
  {
    std::cout << "Generate: " << rd() % 10 << "\n";
  }
  return 0;
}
```
Use random device for generating the seed for mt
```cpp
#include <iostream>
#include <random> // since c++11

int main()
{
  // Non-deterministics random-number
  // create a random seed
  std::random_device rd;
  std::mt19937 mt(rd());
  for(size_t i = 0; i < 20; i++)
  {
    std::cout << "Generate: " << mt() % 10 << "\n";
  }
  return 0;
}
```