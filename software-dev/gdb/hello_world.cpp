#include <iostream>
#include <vector>

int main()
{
  std::vector<std::string> vector {"Hello", "World","!"};
  // ranged based for loop
  // for(const auto& world : vector)
  // {
  //   std::cout << world <<" ";
  // }
  for(size_t i = 0; i < vector.size() ; i++)
  {
    std::cout << vector[i] << " ";
  }
  std::cout << "\n";
}