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