## Introduction à `std::function`
### Historique introduciton à partir du C/C++
Imaginons que vous souhaitez manipuler une indirection vers une fonction de manière telle que celle-ci puisse correspondre tantôt à une fonction `f()`, tantôt veres une fonction `g()`dans la mesure où les deux soit de la même signature.

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
Il est possible par exemple de passer des **pointeurs de fonctions** en paramètre à une fonction, ou de retourner un **pointeur de fonctions**.  

Un exemple d'utilisation de fonction prenant en paramètre une fonction sera **appliquer()**, qui prendre un paramètre `std::vector<T>` et une fonction `T (*)(T)`, applique cette fonction à chaque élément du vecteur et retourne le vecteur résultant.  
En pratique n'écrivez pas `appliquer()`, il existe déjà `std::for_each()` et `std::tranform()` dans `<algorithm>` qui sont bien meilleurs et beaucoup plus flexibles.  
```cpp
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <numeric>

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
  const std::size_t vector_size = 10;
  std::vector<int> v(vector_size);
  std::iota(v.begin(),v.end(),1);

  auto u = appliquer(v,negation);
  std::cout << "Before: ";
  std::copy(std::begin(v),std::end(v), std::ostream_iterator<int>{std::cout, " "});
  std::cout << std::endl;
  std::cout << "After ";
  std::copy(std::begin(u),std::end(u), std::ostream_iterator<int>{std::cout, " "});
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
### Utiliser `std::funcction`pour une méthode d'instance
Il est possible de capturer un pointeur sur une méthode de classe (méthode **static**) mais ceci ne représente pas varitablement un défi, puisqu'une telle méthode n'est qu'u fond qu'un fonction globale déguissé avec contrôle d'accès en prime:
```cpp
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
```
Remarquez ici que **fct** peut pointer à la fois sur **f** et sur **X::f** puisque les deux fonctions sont de même signature.  
Toutefois, dans le cas d'une méthode d'instance, un **this** est requis.  
Normalement, ce **this** est passé de manière silencieuse à la fonction:
**Ceci**:
```cpp
#include <iostream>

class X{
  int val;
public:
  X(int val): val{val}{}
  int f() const { return val;} 
};

int main()
{
  X x{3};
  std::cout << x.f() << std::endl;
  return 0;
}
```
***... est en fait cela**
```cpp
#include <iostream>

class X{
  int val;
public:
  X(int val): val{val}{}
  int f() const { return val;} 
};

int appeler(const X &x, int (X::*fct)()const)
{
  return (x.*fct)();
}
int main()
{
  X x{3};
  std::cout << appeler(x,&X::f) << std::endl;
  return 0;
}
```

La syntaxe est plus complexe, mais met en relief qu'il faut une instance (un**this**) pour solliciter une méthode d'instance.  
Il est possible d'encapsuler une méthode d'instance dans un `std::function` étant donnée une signature adéquate.
```cpp
#include <iostream>
#include <functional>

class X{
    int val;
  public:
    X(int val) : val{val}{}
    int f() const { return val;}
}

int main()
{
  X x{3};
  std::function<int(const X&)> fct = &X::f;
  std::cout << fct(x) << std::endl;
}
```

**Les pointeurs vers membres (pointer-to-member)** et les opérateurs `.* /->*`.  
Le compilateur ne "transforme" pas littéralement le premier code en le second code, mais conceptuellement, l'appel d'une méthode à **besoin d'un object + d'un membre (fonction membre) - et un pointeur vers membre encode "quel membre de X"
1) `int(X::*fct)() const`
- `int(...)() const`: une fonction qui retourne `int`ne comprend aucun paramètre et est `const`
- `(X::*)`: **pointeur vers un membre de X**
Donc:
```cpp
int (X::*fct) () const
```
- **est un pointeur vers une fonction membre de X de signature: 
```cpp
int X::f() const
```
- **Important**: **ce n'est PAS un pointeur de fonction classique**

Un pointeur vers fonction membre **(X::*)** pointe vers quelque chose qui a besoin d’un objet **X** (car il y a un this implicite). Donc il n’est pas directement appelable sans objet.

2) `x.*fct`: c'est quoi ?
- `.*`est l'opérateur: **apply pointer-to-member-to-object**
- `x`est un object (ou une référence sur un objet) de type X.
- `fct`est un pointeur vers membre X.
Alors:
```cpp
x.*fct
```
- signifie: **le membre de x désigné par fct**

#### Cas pointeur vers données membre
```cpp
class X {
  int val;
public:
  X(int v) : val{v} {}
  int f() const { return val; }
};

int main() {
  X x{3};

  int X::*p = &X::val;     // pointeur vers membre-donnée
  int v = x.*p;            // lit x.val

  std::cout << v << "\n";
}
```
- `p`ne pointe pas vers une adresse mémoire **global**
- il encode **où est val dans un X** (souvent un offset, mais pas obliqé)
- `x.*p`signifie: **prends l'adresse de x, applique la description p, et accès au champ**

#### Vocabulaire
- `int (X::*fct)()`: est un pointeur vers fonction membre (**pointer-to-member function**)

8) Bonus : équivalent “haut niveau” avec `std::invoke`
```cpp
#include <functional>
return std::invoke(fct, x); // appelle fct sur x
```
`std::invoke`sait gérer:
- pointeurs vers fonctions membres
- pointeurs vers données membres
- lambdas/ fonctuers
- pointeurs de fonction.