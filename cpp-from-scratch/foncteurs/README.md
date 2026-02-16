# Foncteur

## Définition: c'est quoi un foncteur ?
**Un foncteur = un object appelable => un object pour lequel `operator()`est défini**
On appelle généralement `operator()`function call operator.
```cpp
struct Add{
  int operator()(int a, int b) const {return a + b;}
};

Add add;
int x = add(2,3);
```
Pouquoi c'est puissant:
- l'object peut porter un état (paramètres, mémoire, compteur)
- il peut être **inliné** (souvent plus performant qu'un pointeur de fonction)
- s'intègre parfaitement avec la STL (`std::sort`, `std::for_each`,`etc`)

## C: Pointeurs de fonction ('analogue "callable")
En C, pas de classes => pas de `operator()`.   
Le mécanisme standard:
- une fonction
- un pointeur de fonction
- souvent un `void*` `ctx`pour simuler l'état.
```c
#include <stdio.h>
int add(int a, int b){ return a + b;}

int main(void)
{
  int (*f)(int a, int b) = add;
  printf("%d\n",f(2,3)); // 5
}
```
Pour simuler un "foncteur avec état" en C: callback + context.

## Foncteur avec état (le vrai intérêt)
```cpp
struct Threashold{
  int t;
  explicit Threshold(int t_) : t(t_){}
  bool operator()(int x) const { return x > t;}
};
```
- Ici `t` est capturé dans l'object: c'est une "fonction paramétrée" mais stockée.

## Foncteurs et STL: exemple concret
`std::sort` avec un comparateur
```cpp
#include <algorithm>
#include <vector>

struct ByAbs{
  bool operator()(int a, int b) const{
    return std::abs(a) < std::abs(b);
  }
};

int main(){
  std::vector<int> v{3, -1, -7, 2};
  std::sort(v.begin(),v.end(),ByAbs{})
}
```
La STL attend un callable: fonction, lambda, foncteur, etc.

## Lambda vs foncteur: c'est quoi la différence ?
Une lambda est (presque) juste un foncteur générer par le compilateur.
```cpp
auto cmp = [](int a, int b){ return a < b;}
```
C'est équivalent à un type anonyme avec `operator()`.
**Lambda avec capture = foncteur avec état**
```cpp
int t = 10;
auto ge = [t](int x){ return x >=t;}
```

## La grande fammille des "callables" en C++
En C++ (moderne), "callable" peut être:
1. fonction: `int`f(int)
2. pointeur de fonction: `int (*)(int)`
3. foncteur: `struct X{ int operator() (int);}`
4. lambda
5. pointeur vers méthode: `&T::methode`
6. `std::function(type-erasure, polymorphisme)

## `std::function`: quand l'utiliser (et quand éviter)
`std::function<R(Arg...)>`peut contenir n'importe quel callable compatible.
```cpp
#include <functional>

std::function<int(int,int)> op;
op = [](int a, int b){ return a + b;};
int x = op(2,3);
```
Avantages:
- interface uniforme (pratique pour API, callabacks)
- stockage hétérogène (peut remplacer lambda/foncteur/fonction)
Inconvéninants:
- peut allouer (heap) selon la taille (même si SBO existe)
- overhead d'indirection: -> moins perf que templates/foncteurs direct

## mini exercice:
```cpp
#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <functional>
/**
 * @brief Foncteur est un object callable dont operator() est défini.
 */
struct Multiply{
  int k {};
  int operator()(int x) const { return k * x;}
};

template <typename T, typename Pred>
std::vector<T> filter(const std::vector<T>& v, Pred pred)
{
  std::vector<T> out;
  out.reserve(v.size());
  std::copy_if(v.begin(),v.end(), std::back_inserter(out), pred);
  return out;
}

int main()
{
  {
    // First question
    Multiply mul{2};
    // Vector of size 10
    std::vector<int> my_vec(10);
    // Fill the vector with sequentially increasing values starting from 0
    std::iota(my_vec.begin(),my_vec.end(),0);
    for(const auto& element: my_vec)
    {
      std::cout << element << " ";
    }
    std::cout << std::endl;
    std::transform(my_vec.begin(),my_vec.end(),my_vec.begin(), mul);
    for(const auto& element: my_vec)
    {
      std::cout << element << " ";
    }
    std::cout << std::endl;    
  }
  {
    // Second question
    const int t = 3;
    // Vector of size 10
    std::vector<int> my_vec(10);
    // Fill the vector with sequentially increasing values starting from 0
    std::iota(my_vec.begin(),my_vec.end(),0);
    for(const auto& element: my_vec)
    {
      std::cout << element << " ";
    }
    std::cout << std::endl;
    auto my_lambda = [t](int x){ return t * x;};
    std::transform(my_vec.begin(),my_vec.end(),my_vec.begin(), my_lambda);
    for(const auto& element: my_vec)
    {
      std::cout << element << " ";
    }
    std::cout << std::endl;    
  }
  {
    // Third question
    const int t = 3;
    // Vector of size 10
    std::vector<int> my_vec(10);
    // Fill the vector with sequentially increasing values starting from 0
    std::iota(my_vec.begin(),my_vec.end(),0);
    for(const auto& element: my_vec)
    {
      std::cout << element << " ";
    }
    std::cout << std::endl;

    auto is_even = [](int x){ return (x % 2) == 0; };
    auto evens = filter(my_vec, is_even);

    for(const auto& element: evens)
    {
      std::cout << element << " ";
    }
    std::cout << std::endl;    
  }
  return 0;
}
```