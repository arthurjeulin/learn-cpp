#include <iostream>
#include <functional>
#include <numbers>

struct Plancher{
  int operator()(double d) const{
    return static_cast<int>(d);
  }
};

int plafond(double d){
  return Plancher{}(d) + 1;
}

int main()
{
  std::function<int(double)> fct = plafond;
  std::cout << fct(std::numbers::pi) << std::endl;
  fct = Plancher{};
  std::cout << fct(std::numbers::pi) << std::endl;
}