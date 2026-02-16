## Introduction à `std::function`
### Historique introduciton à partir du C/C++
Imaginons que vous souhaitez manipuler une indirection vers une fonction de manière telle que celle-ci puisse correspondre tantôt à une fonciton `f()`, tantôt veres une fonction `g()`dans la mesure où les deux soit de la même signature.

La solution à ce problème est le recours à des **pointeurs de fonctions**.  
```cpp
#include <iostream>

int f(){ return 3; }
int g(){ return 4; }

int main()
{
  int (*ptr_function)() = f;
  std::cout << ptr_function() << std::endl;
  ptr_function = g;
  std::cout << ptr_function() << std::endl;
}
```
- deux fonctions `f()`et `g()`de même signature (fonction nullaire - sans paramètres qui retourne `int`)
- Ces fonctions sont appeléés dans le `main()`à travers un seul même `pointeur de fonction` `ptr_function`.
- **utiliser le nom seul d'une fonction `f`au lieu de `f()`signifie prendre son adresse alors que l'appeler donne accès au résultat de son exécution**

Syntaxe de la déclaration de `ptr_function`
```cpp
type (*nom)(params)
```
- `type`est le type de retourn de la fonction pointée.
- `param`est la signature des paramètres.
**Notez les parenthèses autour du `nom`**
```cpp
// Pointeur sur une fonction recevant un const std::string et retournant une std::string
std:string (*ptr)(const std::string&);
```
**Attention**
```cpp
// Fonction recevant une const std::string et retournant un std::string*
std::string *p(const std::string&);
```
### Pointeurs de fonctions et `using`
Depuis C++ 11, un nouvelle syntaxe est possible pour définir un type, en particulier dans le cas des **pointeurs de fonctions**.  
Cette syntaxe est puisssante, surtout lorsque jointe à des templates.
```cpp
using ptr_function = int(*)(int);
```
Exemple:
```cpp
#include <iostream>

int f(int n){ return n +1;}
int g(int n){ return -n;}

// Create a type ptr_function
using ptr_function = int (*)(int);

int main()
{
  ptr_function my_function = f;
  std::cout << my_function(2) << std::endl;
  my_function = g;
  std::cout << my_function(2) << std::endl;
  return 0;
}
```

### Pointeurs de fonctions en paramètres

Les **pointeurs de fonctions** font partie intégrante du langage **C** depuis toujours.  
Il est posssible par exemple de passer des **pointeurs de fonctions** en paramètre à unf contion, ou de retourner un **pointeur de fonctions**.  

Un exemple d'utilisation de fonction prenant en paramètre une fonction sera **appliquer()**, qui prendre un paramètre `std::vector<T>` et une fonction `T (*)(T)`, applique cette fonction à chaque élément du vecteur et retourne le vecteur résultant.  
En pratique n'écrivez pas `appliquer()`, il existe déjà `std::for_each()` et `std::tranform()` dans `<algorithm>` qui sont bien meilleurs et beaucoup plus flexibles.  
```cpp
template <class T>
std::vector<T> appliquer(std::vector<T> v, T(*f)(T))
{
  for(auto& val: v)
  {
    val = f(val);
  }
  return v;
}

```
```cpp
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>

template <class T>
std::vector<T> appliquer(std::vector<T> v, T(*f)(T))
{
  for(auto& val: v)
  {
    val = f(val);
  }
  return v;
}

int negation(int n) { return -n;}

int main()
{
  std::vector<int> v;
  for(std::size_t i = 0; i < 10; i++)
  {
    v.push_back(i+1);
  }
  auto u = appliquer(v,negation);
  std::cout << "Before: ";
  std::copy(begin(v),std::end(v), std::ostream_iterator<int>{std::cout, " "});
  std::cout << std::endl;
  std::cout << "After ";
  std::copy(begin(u),std::end(u), std::ostream_iterator<int>{std::cout, " "});
  std::cout << std::endl;
}
```

### Le type `std::function` de C++11
Exemple:
```cpp
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
```