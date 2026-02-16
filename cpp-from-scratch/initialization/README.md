## Initialization

### Rappel
1. Trois familles d'initialization (syntaxe)
  1. Parentheses-init (**direct init**)
```cpp
T x(a,b);
```
 - appelle des constructeurs **classique** (overload resolution standard)
  2. Copy-init (**avec =**)
```cpp
T x = expr;
```
 - conceptuallement **copie**, mais en C++ moderne ça déclenche très souvent **copy elision** (pas de copie réelle).
  3. List-init (**avec {}**) (la plus importante)
```cpp
T x{a,b};     // direct-list-init
T w = {a,b};  // copy-list-init
```
Propriétés clés:
- interdit les narrowing conversions (troncature / approx implicite)
- favorise **initializer_list** si un constructeur existe (priorité particulière)
- pour les scalaire `{}`peut faire **value-initialization** = 0 ou séléctionner un constructeur.
2. Zéro / deafault : 3 notions à ne pas mélanger
  - **Default-initialization**: "pas d'init explicit (ex: `t x;` local)
    - pour un type fondamental local (`int`) => **indéterminé **(garbage)
    - pourune classe => appelle le constructeur par défaut (si disponible) sinon erreur.
  - **Value-initialization**: (ex: `T x{};)
    - pour un type fondamental -> zéro
    - pour une classe -> appelle le constructor par défaut, et les membres sont value-initialized si pertinent.
  - **Zero-initialization**: étape interne (ex: stockage statique, ou value init d'un fondamental)
    - `static int x;`=> zéro

3. Règle d'or pro
  - Pour les **scalaires** (`int`, `size_t`,`double`,`pointeurs`) => `{}`est un airbag, évite l'indéterminié
  - Pour les **classes** `{}`vs `()`a des impacts d'ovrload resolution.

### Les formes : Obj obj{}, Obj obj;, Obj obj = something, Obj obj(v0, v1)
```cpp
struct Obj{
  Obj();                // default constructor
  Obj(int);             // converting constructor
  explicit Obj(double);
  Obj(std::initializer_list<int>);
};
```
#### `Obj obj{};`(brace / list-init vide)
C'est **direct-list-initialization** avec liste vide
- Si `Obj()`existe => il est appelé
- S'il n'existe pas, mais si les membres peuvent être agrégés => ça peut faire une **aggregation initialization** (cas `struct`sans constructor)
- Avantage: **uniforme + évite** certains pièges pour les scalaires/membres.
**Important**: pour une classe avec constructor par défaut, `Obj obj{};`et `Obj obj;`se ressemblent, mais pas toujours identiques.

#### `Obj obj;`
C'est **default-initialization**
- Si `Obj`est une classe -> appelle `Obj()`si (disponible)
- Si `Obj`est un type fondamental (ou un membre fonamental non initialisé) => **indéterminé**
```cpp
std::size_t i; // indéterminé is local
```

#### `Obj obj = something;`(copy-init)
C'est **copy-initialization**
`something` peut-être:
  - un `Obj`=> en pratique pas de copie (copy elision fréquente)
  - un type convertible vers `Obj`=> appelle un constructeur de conversion (`Obj(int)` etc).
**Règle utile**: si le constructeur est `explicit`, `Obj obj = 3.14` **peut être interdit** (car **copy-init** n'autorise pas certains **explicit**)
Exemples
```cpp
Obj a = 42;     // Ok si Obj(int) non-explicit
Obj b = 3.14;   // interdit si Obj(double) est explicit
Obj c = Obj(1); // elision, généralement pas de copie
```

#### `Obj obj(value_0,value_1);` (direct init)
C'est **direct-initialization**
- Autorise les `explcit`
```cpp
Obj b(3.14); // OK même si explicit Obj(double)
```
- Overload resolution "classique" (sans les règles spéciales **initializer_list**)

#### Le point critique: `{}`vs`()`quand initialize_list exist
Si `Obj(std::initialize_list<int>)`alors
```cpp
Obj x{1};   // préfère initializer_list si viable
Obj y(1);   // appelle Obj(int)
```
En plus "dangereux"
```cpp
Obj a{1,2}; // initializer_list
Obj b(1,2); // cherche ctor (int,int) -> s’il n’existe pas : erreur
```

### Les cas scalaires: `std::size_t i{}`, `std::size_t{0}`, `std::size_t i = 0`

#### `std::size_t i{}`
- `i` est un objet nommé.
- `{}`vide sur un scalaire = **value-initialization**
- `i == 0`
Très bien pour éviter les variables non initialisées.

#### `std::size_t i = 0`
- **copy-initialization** depuis le littéral 0 (`int`)
- conversion implicite vers `size_t`(unsigned)
- `i==0`

### Narrowing: pourquoi `{}`est une arme de précision
```cpp
int a = 3.7;  // Ok (troncature) => a == 3
int b(3.7);   // Ok (troncature) => b == 3
int c{3.7};   // Erreur (narrowing)
// autre
std::size_t s = -1;   // OK mais énorme (wrap) -> piège
std::size_t t{-1};    // ERREUR (narrowing / conversion non sûre)
```
### Les "objects": classes, agrégats, membres, et la vraie différence `obj;` vs `obj{};`
#### Classe avec constructor par défaut
```cpp
struct A{
  int x; // non initialisé
  A(){}  // constructeur qui ne touche pas à x
};
A a;   // appelle A(), x indéterminé
A b{}; // appelle A(), x indéterminé aussi car (constructor vide)
```
#### Agrégat (pas de constructor **user-declared**)
```cpp
struct B{
  int x;
  int y;
};
B b;    // x,y indéterminés (local)
B c{};  // x=0, y=0 (value-init des membres)
B d{1}; // x=1, y=0
```
#### Membre avec initialiseur par défaut
```cpp
struct C{
  int x = 5;
  int y;
};
C c{}; // x=5, y=0
```


## Second part
### Deux aces différents: forme vs sémantique

**Axe A - formes (syntax categories): comment tu écris**
- default-initialization: `T x;`
- direct-initialization: `T x(args);`
- copy-initialization: `T x = expr;`
- direct-list-initialization: `T x{args};`
- copy-list-initialization: `T x = {args};`
- reference initialization: `T& r = x;`, `const T& r= expr;`, `T&& r = expr;` 

**Axe B - sémantiques (semantic effects): ce que ça fait réelement**
- zero-initialization
- value-initialization
- default-initialization
- aggregate-initialization
- list-initialization (règle `{}`: no narrowing + priorité **initializer_list**)

**A Retenir: `direct-list-initialization`est une forme. `value-initialization`est un effet (peut être déclenché par `{}`mais pas uniquement).**
### Tous les types d'initialization "pour un objet" avec exemples
#### `default-initialization`(forme)
```cpp
T x;
```
- classe: appelle le constructeur par défaut `T()`
- scalaire local (`int`,`size_t`): valeur indéterminée (garbage)
- statique: `static T x;` d'abord `zero-initialization`
#### `direct-initialization` (forme) = "parenthese-init"
```cpp
T x(a,b)
```
- appelle un constructeur `T(a,b)` via overload resolution "classique".
- autorise explicit.
#### `copy-initialization` (forme)
```cpp
T x = expr;
```
- conceptuellement "copie / conversion"
- en pratique peut-être élidé (pas de copie réelle).
#### `direct-list-initialization` (forme)
```cpp
T x{a,b};
```
**Règle fortes**
- interdit le narrowing
- si `std::initialize_list` viable, il est privilégié
- sinon cherche constructor normaux
#### `copy-list-initialization` (forme)
```cpp
T x = {a,b};
```
- même règle `{}`(no narrowing + priorité init_list)
```cpp
struct E { explicit E(int) {} };

E a{1};     // OK (direct-list-init)
E b = {1};  // interdit (copy-list-init + ctor explicit)
```
#### `value-initialization` (sémantique, pas une forme)
C'est l'effet typique de:
```cpp
T c{}; // direct-list-initialization
T();   // direct-initialization (temporaire)
new T()
```
Effet:
- scalaire: zero
- classe: appelle le constructor par défaut (et / ou zero-init seon les règles)
Exemple:
```cpp
int a{};            // 0
std::size_t s{};    // 0
struct A { int x; };
A a{};              // agrégat => x=0
```
Attention
```cpp
T t();  // ce n'est PAS un objet, c'est une déclaration de fonction !
```
#### `Zero-initalization` (sémantique)
Arrive notamment:
- pour stockage statique: `static int x;`
- avant certaines `value-initialization`selon type
```cpp
static int g; // g == 0
```
#### `Aggregate initialization` (sémantique + forme souvent `{}`)
Quand T est un agrégat (struct sans constructors user-declared, pas de private, etc)
```cpp
struct B{int x; int y;};
B b;    // default-init => x,y indetermines (local)
B b{};  // aggregate + value-init => x=0,y=0
B b{1}; // x=1, y=0
```

### `std::initialize_list`vs "constucteur normal"
Règle de sélection
```cpp
#include <initialize_list>
struct V{
  V(int,int){/*normal ctor*/}
  V(std::initializer_list<int>) { /*init_list ctor*/}
}
V a(1,2);     // appelle V(int,int)
V b{1,2};     // appelle V(initialize_list<int>)
V c{1};       // appelle V(initialize_list<int>)
V d(1);       // pas de résolution => Erreur
```
Cas "vector-like"
```cpp
std::vector<int> v1(5,9); // 5 éléments valeur 9
std::vector<int> v2{5,9}; // 2 éléments: 5 et 9
```
## Introduction à `std::initializer_list<T>`

`std::initializer_list<T>` est un petit object qui représente une séquence de valeurs de type T crée par une `brace-list{...}`.
```cpp
{1,2,3}
```
le compilateur peut construire un `std::initializer_list<int>` qui cpointe vers un tableau temporaire contenant `1,2,3`.  
Permet une syntaxe naturelle de `liste de valurs`:
- `std::vector<int> v{1,2,3}`;
- `std::map<K,V> m {{k1,v1}, {k2,v2}}`;
Propriétés importantes:
- C'est léger: typiquement **(ptr,size)**
- Les éléments pointent vers un stockage temporaire dont la durée de vie est assez courte.
- On l'utilise surtout dans un constructeur ou une fonction qui consomme immédiatement la liste.

pour `{...}`, le langage a une règle :
- si un ctor `initializer_list` est viable, il est préféré.

Donc `{1,2}` ne veut pas dire **deux arguments** au sens classique, ça veut dire **une liste**.


