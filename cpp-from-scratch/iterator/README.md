## Introduction au Iterator from cpp from sratch
### Iterator library
Itetors are a generalization of pointers that allow a C++ program to work with different data-structures (for example **containers** and **ranges**) in a uniform manner.
The iterator library provides definition for iterators, as weel as iterator traits, adaptors, and utility function.  
Since iterator are an absraction of pointers, their semantics are a generalization of most of the semantics of pointers in C++.
This ensures that every **function template** that takes iterators works as well with regular pointers.


## Simple explication
**Iterator provide the general way fo traversing your standard template library containers.**
Use with STL algorithms.
We can't use every single kind of iterator with very single standard template algorithm.
- `std::sort` on `std::array`
- `std::sort`cannot on `std::unorderd map` container


```cpp
#include <array>
#include <iostream>
#include <numeric>

int main()
{
  // create an array we want to iterate over
  std::array<int,10> my_vector;
  // populate my_vector from 0 to 9
  std::iota(my_vector.begin(),my_vector.end(),0);
  // use iterator to go through the entire std::array
  // we are going to use iterator to get bound of `my_vector`
  for(auto it = my_vector.begin(); it!=my_vector.end(); ++it)
  {
    // like a pointer we use dereference operator to access the value underneath
    std::cout << *it << " ";
  }
  std::cout << std::endl;
  // use iterator to go through the entire std::array in reverse order
  // we are going to use iterator to get bound of `my_vector`
  for(auto it = my_vector.rbegin(); it!=my_vector.rend(); ++it)
  {
    // like a pointer we use dereference operator to access the value underneath
    std::cout << *it << " ";
  }
  std::cout << std::endl;
  return 0;
}
```

## Iterator
### Pourquoi les iterators existent
**Un iterator est une abstraction de pointeur:** c'est object qui référence une position dans une séquence et permet de parcourir cette séquence.

L'idée clé de la STL:
- **containers**(vector,list,map...) stockent les données.
- **algorithms**(sort,find,transform...) font le boulot.
- **iterators** sont le contract qui connect les deux.
Un algorithm STL ne "connait" pas `std::vector`, il connait juste un couple (`first,last`) d'iterators.

### Vocabulaire essentiel: begin/end [first,last)
- `c.begin()`: iterator sur le premier élément
- `c.end()`: iterator sur "one past the last"
- La plage standard est **semi-ouverte**: [first,last)
Ça implique:
- `end()`ne se déréférence jamais
- boucle canonique:
```cpp
for(auto it = c.begin(); it!=c.end();++it)
{
  // *it valid
}
```
### Opérations de base (et pièges)
#### Déréférencement
- `*it`: référence l'élément courant
- `it->member`: accès membre si l'élément est une struct/class
- `*end()`est Undefined Behavoir.
#### Incrément / décrément
- `++it`: avancement (pré-increment recommandé)
- `it++`: avance mais peut créer une copie (souvent plus cher, parfois identique)

### Les catégories d'iterators
La STL classe les iterators selon ce qu'ils supportent. Plus tu montes, plus tu as d'opérations.
#### Input iterator (lecture, 1 passage)
- Parcours une seule fois
- Exemple: flux(`istream_iterator`)
Cas typique: lis un flux -> ne peux pas revenir en arrière

#### Output iterator (écriture, 1 passage)
- écrire, pas relire
- Exemple: `std::back_insert_iterator`(via `std::back_inserter`)

#### Forward iterator (lecture/écriture, multi-pass)
- tu peux faire plusieurs passages
- Exemple: iteerator de `std::forward_list`

#### Bidirectionnal iterator (forward + arrière)
- `--it` possible
- Exemple: `std::list`,`std::map`, `std::set`

#### Random access iterator (indexation, arithmétique)
- `it +n`,`it -n`,`it[n]`, comparaision <
- Exemple: `std::vector`, `std::deque`, `std::array` 

#### Contiguous iterator (mémoire contiguë)
- Comme random access + garantie contiguë
- Permet d'obtenir un pointeur brut, meilleur pour perf/SIMD
- Exemple: `std::vector`,`std::array`,`std::span`

### Pourquoi la catégorie compte (complexité & compatibilité)
Beaucoup d'algorithms exigent un minimum.
Exemples:
- `std::sort`:demande **random access** (donc OK `std::vector`/`std::deque`, pas list)
- `std::reverse`demande **bidirectional**
- `std::ranges::sort`pareil: random access

### `const_iterator`vs `iterator`
- `iterator:`peut modifier l'élément via `*it = `
- `const_iterator`: lecture seul

### Invalidation des iterators
Chaque container a des règles.
#### `std::vector`
- `reallocation` (souvent quand `push_back` dépasse la capacité)
  - **invalide** tous les iterators/pointers/references
- `erase`
  - invalide les iterators à partir de la position effacé jusqu'à `end()`
Pattern pro:
- faire `reserve()` si on connait une taille approximative.
- utiliser le `retour`de **erase** (il renvoie le **next valid**)
#### `std::deque`
- invalide plus facilement que vector selon operation (**push front / back, etc**)
- règle plus complexes

#### `std::list`/ `std::forward_list`
- `insertion`/ `erase` n'invalide que les itérateurs vers les éléments supprimés
- très stable pour iterators, mais moins cache-friendly

#### `std::map` / `std::set`
- `insertion` n'invalide pas les itérators.
- `erase`invalide juste l'élément effacé.

### `std::iterator_traits`(comment la STL "découvre" la capacités)
- Les algos utilisent `std::iterator_traits<It>`pour récupérer:
  - `value_type`
  - `difference_type`
  - `iterator_category` (concepts en C++20)
C++20 favorise maintenant les **concepts** (`std::radom_access_ierator, etc) pluôt que les vieilles catégories.

### iterators et algorithmes: patterns indispensables
#### trouver
```cpp
auto it = std::find(v.cbegin(),v.cend(),42);
if (it != v.cend()) { /* found*/}
```
#### transformer
```cpp
std::ranges::transform(in,std::back_inserter(out),
                        [](int x){return x*x;});
```
#### Filter: erase-remove idiom (vector)
```cpp
v.erase(std::remove_if(v.begin(), v.end(),
                       [](int x){ return x < 0; }),
        v.end());
```
```cpp
auto [first, last] = std::ranges::remove_if(v, [](int x){ return x < 0; });
v.erase(first, last);
```
### Reverse iterators
- `rbegin()`/ `rend()` parcourent à l'envers
Piège classique :
  - `it.base()` d’un reverse iterator pointe sur l’élément suivant (car logique [first,last))