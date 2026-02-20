#include <iostream>
#include <algorithm>
#include <array>

// abbreviated template syntaxe
void print_container(const auto& container)
{
  for(const auto& element: container)
  {
    std::cout << element << " ";
  }
  std::cout << std::endl;
}

int main()
{
  // Array we want to sort
  std::array<int, 5> my_array{1,5,1,9,4};
  print_container(my_array);
  // sort my array, sort use random iterator type
  std::sort(my_array.begin(),my_array.end());
  print_container(my_array);

  //--------ranges version--------------
  // Array we want to sort
  std::array<int, 5> my_array_2{987,5,132,93,43};
  print_container(my_array_2);
  // Syntetic sugar: exact same operation but easier to write
  std::ranges::sort(my_array_2);
  print_container(my_array_2);
  return 0;
}