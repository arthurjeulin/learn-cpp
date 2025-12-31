#include <iostream>

class MyClass{};

class MyClass_2
{
  int x;
  int y;
};

int main(int argc, char** argv)
{
  std::cout << "Size Of MyClass: " << sizeof(MyClass) << "\n";     // Result 1
  std::cout << "Size of MyClass_2: " << sizeof(MyClass_2) << "\n"; // Result 8
  return 0;
}
/*
 * Deux objects distincs doivent avoir des adresses distincts, donc un type vide ne peut pas aovir sizeof==0
 */