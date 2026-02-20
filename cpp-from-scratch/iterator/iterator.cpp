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
  for(auto it = my_vector.cbegin(); it!=my_vector.cend(); ++it)
  {
    // like a pointer we use dereference operator to access the value underneath
    std::cout << *it << " ";
  }
  std::cout << std::endl;
  // use iterator to go through the entire std::array in reverse order
  // we are going to use iterator to get bound of `my_vector`
  for(auto it = my_vector.crbegin(); it!=my_vector.crend(); ++it)
  {
    // like a pointer we use dereference operator to access the value underneath
    std::cout << *it << " ";
  }
  std::cout << std::endl;
  return 0;
}