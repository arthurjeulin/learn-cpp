#include <iostream>

struct X{
  static int f(int n){
    return n + 1;
  }
};

int f(int n){
  return n * 2;
}

int main(){
  auto fct = f; // f est int(*)(int)
  std::cout << fct(3) << std::endl;; // retournera 6
  fct = &X::f; // &X::f est un int (*)(int)
  std::cout << fct(3) << std::endl; // retournera 4
  return 0;
}