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