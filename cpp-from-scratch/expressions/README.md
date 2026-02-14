## Expressions en C++ : types, catégories de valeurs, std::move, forwarding

En C++, chaque expression a :
1.	un type (`int`, `std::string`, `T`, `T&`, etc.)
2.	une catégorie de valeur (value category) : 
  elle décrit comment l’expression se comporte (a-t-elle une identité ? peut-on “réutiliser” ses ressources ?).

La catégorie de valeur détermine notamment à quels types de références l’expression peut se lier (`T&`, `T&&`).


### Les catégories de valeurs (C++11+)
Il existe 3 catégories “atomiques” importantes :
- lvalue
- xvalue
- prvalue

et 2 unions :
- glvalue = lvalue ∪ xvalue (g = “generalized”)
- rvalue = prvalue ∪ xvalue

#### `lvalue` (“locator value”)

Une lvalue a une identité : elle désigne un objet avec une adresse stable (tu peux prendre son adresse).

Exemples typiques (lvalue) :
```cpp
int x = 1;
x;          // lvalue
++x;        // lvalue (oui)
*ptr;       // lvalue (si ptr pointe vers un objet)
s[0];       // lvalue (référence sur un caractère)
```
Propriété utile : `&expr` est en général valide pour une lvalue.
```cpp
&x;     // ok
```
#### `prvalue` (“pure rvalue”)

Une prvalue est une valeur temporaire (souvent “sans identité” observable) : typiquement le résultat d’un calcul ou d’une construction temporaire.

Exemples (prvalue) :
```cpp
42;                 // prvalue
x + 1;              // prvalue
std::string("hi");  // prvalue (temporaire)
i++;                // prvalue (le résultat “ancien”)
```
Attention : depuis C++17, une prvalue n’est plus forcément “un objet temporaire matérialisé” immédiatement. Mais pour raisonner, tu peux retenir : prvalue = valeur produite.

#### `xvalue` (“expiring value”)

Une xvalue est une expression qui a une identité, mais dont les ressources peuvent être réutilisées (objet “sur le départ”, expiring).

Exemples (xvalue) :
```cpp
std::move(x);                 // xvalue
static_cast<T&&>(x);          // xvalue
f()           // si f retourne T&&, alors f() est xvalue
```

⸻

### Références : T&, const T&, T&& (et qui se lie à quoi)
#### Definition
Declares a named variable as a reference, that is, an alias to an already existing object or function.

#### Référence `lvalue` : `T&`

Se lie aux lvalues uniquement.
```cpp
int x = 0;
int& r = x;     // ok
int& r2 = 42;   // erreur
```
#### Référence `lvalue const` : `const T&`
Se lie à `lvalue` et `rvalue` (donc super pratique pour prendre un param “read-only” sans copie).
```cpp
const std::string& a = s;                 // ok (lvalue)
const std::string& b = std::string("x");  // ok (rvalue, lifetime étendu)
```
#### Référence `rvalue` : `T&&`

Se lie aux rvalues (`prvalue` ou `xvalue`). Typiquement utilisée pour déplacer.
```cpp
std::string make();        // déclaration (prototype of make function)
std::string&& rr = make(); // ok (rvalue)
// r à pour categorie lvalue mais son type est référence rvalue !
std::string&& rr2 = s;     // erreur (s est lvalue)
```
### std::move : ce que ça fait vraiment

`std::move` ne déplace rien.

`std::move(x)` fait essentiellement un cast :
```cpp
static_cast<std::remove_reference_t<decltype(x)>&&>(x)
```
Donc :
- si x est une lvalue, std::move(x) est une xvalue
- ça “autorise” les overloads/constructeurs de move à être sélectionnés.

Exemple :
```cpp
// a est une lvalue
// "hello" est une lvalue tableau en mémoire
// array-to-pointer en const char* et appelle constructor
// de std::string(const char*)
std::string a = "hello";
// std::move(a) => xvalue de type std::string&& => appelle constructor
// std::string(std::string&&)
std::string b = std::move(a); // b prend les ressources de a (move ctor)
```
Après ça, `a` est dans un état valide mais non spécifié (tu peux le détruire, lui réassigner une valeur, appeler certaines méthodes, mais tu ne dois pas supposer son contenu).

Règle pratique :

N’utilise `std::move` que si tu es d’accord pour considérer la variable comme “consommée” (tu ne dépends plus de sa valeur).

#### decltype(x) : attention, ce n’est pas “toujours le type de x”

`decltype(expr)` dépend de la forme de expr.

Règle clé :
- si `expr` est un nom non parenthésé d’une variable (ex: x), alors decltype(x) donne le type déclaré de x (en conservant const et & si x est une référence).
- si expr est une expression (ex: (x) parenthésé), alors decltype((x)) donne une référence selon la catégorie de valeur :
	-	lvalue → T&
	-	xvalue → T&&
	-	prvalue → T

Exemple :
```cpp
int i = 0;
decltype(i)   t1 = 0;   // int
decltype((i)) t2 = i;   // int&  (car (i) est une expression lvalue)
```
Autre exemple important :
```cpp
int& r = i;
decltype(r)   t1 = i;   // int& (type déclaré)
decltype((r)) t2 = i;   // int& (expression lvalue)
```

#### std::remove_reference_t<T> : qu’est-ce que ça fait ?

`std::remove_reference<T>::type` enlève `&` et `&&` si présents.
Le suffixe `_t` est juste un alias moderne pour écrire `.type`.

Concrètement :
- `std::remove_reference_t<int>` → `int`
- `std::remove_reference_t<int&>` → `int`
- `std::remove_reference_t<int&&>` → `int`
- `std::remove_reference_t<const int&>` → `const int` (note : le const reste)

Donc remove_reference enlève uniquement la “couche référence”, pas les qualifiers const/volatile.


#### Overload lvalue/rvalue : pourquoi ça existe

Un pattern classique :
```cpp
struct V {
  void set(std::string const& s) { data = s; }          // copie
  void set(std::string&& s)      { data = std::move(s);} // move
  std::string data;
};
```
- si tu passes une lvalue, on copie
- si tu passes un rvalue, on move

### Forwarding references (ex “universal references”) et “perfect forwarding”
#### Définition : quand T&& est une forwarding reference ?

`T&&` est une forwarding reference seulement si :
- `T` est un paramètre de template déduit (deduced) dans une fonction template (ou `auto&&`)
- et que c’est exactement la forme `T&&` (pas `std::vector<T>&&`, pas `const T&&`, etc.)

Exemples : forwarding refs
```cpp
template<class T>
void f(T&& x);   // x est forwarding reference

auto&& y = expr; // auto&& est forwarding reference
```
Contre-exemples : pas forwarding
```cpp
template<class T>
void g(const T&& x); // const T&& : pas forwarding

template<class T>
struct S {
  void h(T&& x);     // ici T n'est pas déduit par h, donc pas forwarding
};
```

forwarding => transfert   
Une forwarding reference (forme `T&&` avec `T` déduit) sert à écrire une fonction générique qui accepte à la fois des lvalues et des rvalues sans perdre l’info “`lvalue` vs `rvalue`”, afin de pouvoir :
- copier si l’appelant a passé une `lvalue`
- déplacer (move) si l’appelant a passé une `rvalue`

C’est exactement ce qu’on appelle perfect forwarding.

#### Pourquoi on en a besoin (le problème à résoudre)

Supposons que tu veuilles écrire un wrapper qui appelle une fonction target :
```cpp
void target(std::string const&); // copie / lecture
void target(std::string&&);      // move
```
Tu écris un wrapper naïf :
```cpp
template<class T>
void wrapper(T&& x)
{
  target(x); // BUG : x est une variable nommée => expression lvalue
}
```
Même si l’appelant fait `wrapper(std::string("hi"))`, à l’intérieur `x` est nommé, donc `x` est `lvalue`, et tu appelles toujours `target(const std::string&)` (donc tu copies / tu perds le move).

C’est exactement ce que corrige la forwarding reference.

---

#### Comment ça marche : `std::forward`

La bonne version :
```cpp
template<class T>
void wrapper(T&& x)
{
  target(std::forward<T>(x));
}
```
- si l’appelant passe une `lvalue` `U`, alors `T` déduit `U&` et `std::forward<T>(x)` renvoie une `lvalue` `U&`
- si l’appelant passe une `rvalue` U, alors `T` déduit `U` et std::forward<T>(x) renvoie une `xvalue` `U&&`

Donc forwarding ref + std::forward = préserver la catégorie d’origine.
#### A retenir
Dans :
```cpp
template<class T>
void wrapper(T&& x)
```
1) Le **type** de x:
- Si wrapper est appelé avec une `lvalue` u de type `U` :
	-	`T` est déduit comme `U&`
	-	donc `T&&` devient `U&` && → **reference collapsing** → `U&`
	-	type de x : `U&` (référence `lvalue`)
- Si wrapper est appelé avec une `rvalue` de type U :
	-	`T` est déduit comme `U`
	-	donc `T&&` reste `U&&`
	-	type de x : `U&&` (référence `rvalue`)

2) La **catégorie** de valeur de l’expression x

Quel que soit son type, `x` est une variable nommée dans le corps de la fonction. Donc l’expression `x` est toujours une `lvalue`.

C’est précisément pour ça qu’on écrit :
```cpp
target(std::forward<T>(x));
```
- `std::forward<T>(x)` recrée une expression `lvalue` ou `xvalue` selon le cas d’origine.

À retenir:
- Type de x : `U&` si on passe une `lvalue`, `U&&` si on passe une `rvalue`.
- Catégorie de l’expression x : toujours `lvalue` (parce que x est un nom).
 
---

4) Exemple d’utilisation concret (le plus fréquent) : factory / emplace

C’est le pattern de `std::make_unique`, `std::make_shared`, `std::vector::emplace_back`, etc.
```cpp
struct Widget
{
  Widget(std::string name, int id);
};

template<class... Args>
Widget make_widget(Args&&... args)
{
  return Widget(std::forward<Args>(args)...);
}

void demo()
{
  std::string name = "cam_front";

  auto w1 = make_widget(name, 1);                 // name est lvalue => copie
  auto w2 = make_widget(std::string("tmp"), 2);   // rvalue => move
}
```
Ici :
- `Args&&...` sont des forwarding references.
- `std::forward` transmet chaque argument avec la même catégorie que l’appelant

Sans `std::forward`, tu perds les moves et tu copies plus que nécessaire.

---

5) Exemple “ultra parlant” avec overload copy/move
```cpp
void sink(std::string const&) { /* copie/lecture */ }
void sink(std::string&&)      { /* move */ }

template<class T>
void pass_to_sink(T&& x)
{
  sink(std::forward<T>(x));
}

void demo()
{
  std::string s = "hello";

  pass_to_sink(s);                 // appelle sink(const&) (lvalue)
  pass_to_sink(std::string("x"));  // appelle sink(&&) (rvalue => xvalue)
}
```
#### Définition : quand T&& est une forwarding reference ?

`T&&` est une forwarding reference seulement si :
- `T` est un paramètre de template déduit (deduced) dans une fonction template (ou `auto&&`)
- et que c’est exactement la forme `T&&` (pas `std::vector<T>&&`, pas `const T&&`, etc.)

Exemples : forwarding refs
```cpp
template<class T>
void f(T&& x);   // x est forwarding reference

auto&& y = expr; // auto&& est forwarding reference
```
Contre-exemples : pas forwarding
```cpp
template<class T>
void g(const T&& x); // const T&& : pas forwarding

template<class T>
struct S {
  void h(T&& x);     // ici T n'est pas déduit par h, donc pas forwarding
};
```

#### Exemple complet : factory + forwarding
```cpp
#include <memory>
#include <string>
#include <utility>
#include <iostream>

struct Widget
{
  std::string name;

  Widget(const std::string& s) : name(s)
  {
    std::cout << "Widget(const std::string&): copy\n";
  }

  Widget(std::string&& s) : name(std::move(s))
  {
    std::cout << "Widget(std::string&&): move\n";
  }
};

template<class T, class... Args>
std::unique_ptr<T> make_obj(Args&&... args)
{
  // Args&&... = forwarding references (car Args est déduit)
  // std::forward<Args>(args)... préserve lvalue/rvalue argument par argument
  return std::make_unique<T>(std::forward<Args>(args)...);
}

int main()
{
  std::string s = "hello";

  auto a = make_obj<Widget>(s);                 // s est lvalue => copie
  auto b = make_obj<Widget>(std::string("x"));  // rvalue => move
}
```
- 	`Args&&...` = forwarding refs
- `std::forward` garde le move pour les rvalues (efficace), et copie pour les lvalues (correct)

#### Rappel

#### `class... Args` : un parameter pack

`template<class T, class... Args>`
- `Args...` est un pack de paramètres template : une liste de types de longueur variable.
- Ça permet à `make_obj` d’accepter n’importe quel nombre d’arguments, avec des types potentiellement tous différents, sans les écrire à la main.

	-	appel `make_obj<Foo>(1, 3.14, "x")`
	-	alors `Args...` devient la liste : (int, double, const char*)

---

### `Args&&... args` : deux choses séparées

Tu as exactement identifié les deux composants :

1) `Args&& `: **forwarding references** (parce que Args est déduit)

Dans une fonction template, `Args` est déduit depuis les arguments à l’appel, donc chaque paramètre `Args&&` est une forwarding reference.

Important :
- il n’y a pas “un seul `Args&&`”, il y en a un par argument après expansion.
- et chaque `Args` peut être déduit différemment :
  - si l’argument est une `lvalue` `U` → `Args` se déduit en `U&`
  - si l’argument est une `rvalue` `U` → `Args` se déduit en `U`

Donc le type final du paramètre devient :
- `lvalue` → `U& &&` → collapse → `U&`
- `rvalue` → `U&&`

2) `...` : expansion + nombre d’arguments
- Le `...` dans `Args&&... args` signifie : **répète ce pattern pour chaque type dans le pack Args...**.
- Autrement dit, il “auto-déduit” (plus exactement : accepte) le nombre d’arguments, et instancie autant de paramètres qu’il faut.

3) Exemple concret : 3 arguments mixtes
```cpp
std::string s = "hi";
make_obj<Widget>(s, std::string("tmp"), 42);
```
Déduction :
- argument 1 : `s` est `lvalue` `std::string` → Args1 = `std::string&`
- argument 2 : `std::string("tmp")` est `rvalue` `std::string` → Args2 = `std::string`
- argument 3 : `42` est `prvalue` `int` → Args3 = `int`

Donc :
`Args...` = (`std::string&`, `std::string`, `int`)
Les paramètres deviennent :
-	`Args1&&` → `std::string& &&` → `std::string&`
- `Args2&&` → `std::string&&`
- `Args3&&` → `int&&`

Et dans le corps, tu forwardes chacun :
```cpp
std::forward<Args1>(arg1) // lvalue
std::forward<Args2>(arg2) // xvalue
std::forward<Args3>(arg3) // xvalue
```
---

4) Pourquoi on a besoin de std::forward<Args>(args)...

Parce que args (les paramètres) sont nommés, donc les expressions args sont toujours des `lvalues` dans le corps, même si leur type est `U&&`.

Le rôle de forward est :
- si Args = U& → forward renvoie une lvalue (U&)
- 	si Args = U → forward renvoie une xvalue (U&&)

---

### Résumé pratique (cheat-sheet)

Cheat-sheet C++ — `value categories`, `références`, `std::move`, `std::forward`

#### Deux notions à ne jamais confondre
- **Type** (ex: `T`, `T&`, `T&&`) ≠ catégorie de valeur (`lvalue`/`prvalue`/`xvalue`).
- Toute variable nommée est une `lvalue` quand tu écris l’expression x, même si son type est `T&&`.


#### Catégories de valeurs (expressions)

**lvalue**: Expression qui désigne un objet (identité / adresse).
- Ex: `x`, `*p`, `a[i]`, `++x`
- `&expr` est généralement valide.

**prvalue**: Expression qui produit une valeur (`temporaire` “pur”), sans identité exploitable.
- Ex: `42`, `x + 1`, `T{...}`, `i++` (résultat “ancien”)

**xvalue**: Expression qui désigne un objet (identité), mais traité comme expiring (ressources réutilisables).
- Ex: `std::move(x)`, `static_cast<T&&>(x)`, `f()` si f retourne `T&&`
- Typiquement ce que tu passes pour déclencher un move.

Unions
- glvalue = lvalue ∪ xvalue (expressions avec identité)
- rvalue  = prvalue ∪ xvalue

⸻

#### Références (binding)
- `T&` : se lie aux lvalues uniquement.
- `const T&` : se lie aux lvalues + rvalues (lecture sans copie).
- `T&&` :
	-	`rvalue` reference si `T` est un type fixé (ex: `std::string&&`) → se lie aux `rvalues`.
	-	**forwarding reference** si `T` est **déduit** (ex: `template<class T> f(T&&)`) → accepte `lvalues` + `rvalues` via collapsing.

Rappel : dans `f(T&& x)`, l’expression `x` est toujours `lvalue` (car `x` est nommée).

---

#### `std::move` (strictement)
- `std::move(x)` = `static_cast<remove_reference_t<decltype(x)>&&>(x)`
- Ça ne déplace rien : c’est un cast qui produit une `xvalue`.
	•	Effet : permet de sélectionner des overloads/constructeurs move (`T(T&&)`, `operator=(T&&)`).

---

#### Forwarding / “transmission parfaite”

**But : transmettre un argument à une autre fonction en conservant sa nature lvalue/rvalue**.

Forwarding reference
```cpp
template<class T>
void wrapper(T&& x); // T est déduit => forwarding reference
```
`std::forward<T>`.  
À utiliser uniquement dans ce contexte.
`std::forward<T>(x)` renvoie :
- une `lvalue` si l’appelant a passé une `lvalue` (`T` = `U&`)
- une `xvalue` si l’appelant a passé une `rvalue` (`T` = `U`)

Pattern canonique :
```cpp
template<class T, class... Args>
std::unique_ptr<T> make_obj(Args&&... args)
{
  return std::make_unique<T>(std::forward<Args>(args)...);
}
```