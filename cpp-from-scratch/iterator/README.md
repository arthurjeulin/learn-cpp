## Introduction au Iterator
### Iterator library
Itetors are a generalization of pointers that allow a C++ program to work with different data-structures (for example **containers** and **ranges**) in a uniform manner.
The iterator library provides definition for iterators, as weel as iterator traits, adaptors, and utility function.  
Since iterator are an absraction of pointers, their semantics are a generalization of most of the semantics of pointers in C++.
This ensures that every **function template** that takes iterators works as well with regular pointers.


## Simple explication
**Iterator provide the general way fo traversing your standard template library containers.**
Use with STL algorithms.
We can't use every single kind of iterator with very single standard template algorithm.
- `std::sort` on `std::array`
- `std::sort`cannot on `std::unorderd map` container


```cpp
#include <array>
#include <iostream>
#include <numeric>

int main()
{
  // create an array we want to iterate over
  std::array<int,10> my_vector;
  // populate my_vector from 0 to 9
  std::iota(my_vector.begin(),my_vector.end(),0);
  // use iterator to go through the entire std::array
  // we are going to use iterator to get bound of `my_vector`
  for(auto it = my_vector.begin(); it!=my_vector.end(); ++it)
  {
    // like a pointer we use dereference operator to access the value underneath
    std::cout << *it << " ";
  }
  std::cout << std::endl;
  // use iterator to go through the entire std::array in reverse order
  // we are going to use iterator to get bound of `my_vector`
  for(auto it = my_vector.rbegin(); it!=my_vector.rend(); ++it)
  {
    // like a pointer we use dereference operator to access the value underneath
    std::cout << *it << " ";
  }
  std::cout << std::endl;
  return 0;
}
```