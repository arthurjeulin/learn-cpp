#include <iostream>
#include <random> // since c++11

int main()
{
  // Non-deterministics random-number
  // create a random seed
  std::random_device rd;
  std::mt19937 mt(rd());
  // Create a uniform distribution between 1 to 6
  std::uniform_int_distribution uniform(0,1);
  for(size_t i = 0; i < 20; i++)
  {
    // call uniform distribution
    std::cout << "Generate: " << uniform(mt) << "\n";
  }
  return 0;
}