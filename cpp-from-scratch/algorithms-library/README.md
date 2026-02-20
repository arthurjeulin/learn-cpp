# Introduction à Algorithms library
The algorithms library defines function for a variety of purpose (e.g seraching, sorting, counting, manipulating) that operate on ranges of elements.
Note that a range is defined as `[first, last)` where last refers to the element **past** the last element to inspect or modify.
## Introduction à `std::transform`

**std::transform applique une fonction à une plage d'éléments et écrit le résultat dans une plage de sortie**

C’est le “map” fonctionnel de la STL.

### Signatures principales

Unaire (1 entrée → 1 sortie) :
```cpp
std::transform(first, last, d_first, unary_op);
```
- lit `[first, last)`
- écrit à partir de d_first
- `unary_op(x)` doit retourner une valeur assignable dans *d_first

Binaire (2 entrées → 1 sortie) :
```cpp
std::transform(first1, last1, first2, d_first, binary_op);
```
-	lit `[first1, last1)` et `[first2, first2 + (last1-first1))`
- écrit à partir de `d_first`
- `binary_op(a, b)`.

### Règles importantes (les points qui font mal)
1.	La plage de sortie doit avoir la place
transform ne resize pas. Donc :
 - soit tu écris `in-place` (entrée = sortie),
 - soit tu pré-alloues (size >= n),
 - soit tu utilises un inserte `std::back_inserter` dans certains cas.
2.	In-place OK si `d_first` est dans la même plage et ne casse pas l’itération.
Exemple standard :
```cpp
std::transform(v.begin(), v.end(), v.begin(), f);
```
**Ça marche car chaque élément est lu puis écrasé.**

3.	Types : `unary_op` peut changer le type, mais la sortie doit accepter.
Ex : vector<int> -> vector<double> OK si sortie est vector<double>.
4.	Complexité : exactement N appels à l’opérateur (unary/binary).

### Exemples “pro”

Map vers un nouveau vecteur :
```cpp
std::vector<int> in{1,2,3};
std::vector<int> out(in.size());

std::transform(in.begin(), in.end(), out.begin(),
               [](int x){ return x * x; });
```
Accumuler 2 vecteurs élément-par-élément :
```cpp
std::transform(a.begin(), a.end(), b.begin(), out.begin(),
               std::plus<>{});
```
Construire sans connaître la taille (avec back_inserter) :
```cpp
std::vector<int> out;
out.reserve(in.size());
std::transform(in.begin(), in.end(), std::back_inserter(out),
               f);
```
**Note: back_inserter marche si le conteneur supporte push_back. Sinon, on pré-alloue.**

⸻

## Introduction à `std::iota`

### Intuition

**Remplit une plage avec des valeurs séquentielles croissantes en commençant par une valeur initiale.**
```cpp
std::iota(first, last, value);
```
### Sémantique précise
- Affecte : `*first = value;` `++value;` `++first;` et ainsi de suite.
- **Le type de value doit supporter ++value.**

### Cas d’usage typiques
 1. Créer des indices
```cpp
std::vector<int> idx(n);
std::iota(idx.begin(), idx.end(), 0); // 0..n-1
```
 2.	Permutations / tri indirect
Tu tries les indices selon une clé sans bouger les données originales :
```cpp
std::vector<size_t> idx(n);
std::iota(idx.begin(), idx.end(), 0);

std::ranges::sort(idx, [&](size_t i, size_t j){
  return values[i] < values[j];
});
```

 3.	Initialiser un range
iota est parfait pour des tests, benchmarks, datasets synthétiques.

 4. Pièges
	-	Si tu mets un type non adapté (ex `std::string`), ça ne compile pas car `++` n’existe pas.
	-	Overflow possible si ton type est trop petit (ex uint8_t).

⸻

### Les algos STL “pratiques” à connaître en C++20

Je te donne une liste orientée usage quotidien. 
En C++20, je te conseille de privilégier `std::ranges::...` quand possible (plus safe, compose mieux, évite les erreurs d’itérateurs).

1. Recherche / présence
	-	`std::ranges::find`, `find_if`, `find_if_not`
	-	`std::ranges::count`, `count_if`
	-	`std::ranges::any_of`, `all_of`, `none_of`
	-	`std::ranges::search` (sous-séquence)

2. Transformation / génération
	-	`std::transform` (ou `ranges::transform`)
	-	`std::iota`
	-	`std::generate`, `generate_n`
	-	`std::fill`, `fill_n`

3. Filtrage / copie conditionnelle
	-	`std::copy_if` (filtrage)
	-	`std::remove`, `remove_if` + `erase idiom`
	-	`std::unique` + `erase` (déduplication consécutive)

4. Tri / ordre
	-	`std::ranges::sort`
	-	`std::ranges::stable_sort`
	-	`std::ranges::partial_sort`
	-	`std::ranges::nth_element` (sélection O(n) moyenne)
	-	`std::ranges::is_sorted`, `is_sorted_until`

5. Agrégation / réduction
	-	`std::accumulate` (numeric)
	-	`std::reduce` (parallélisable, attention à l’associativité)
	-	`std::inner_product`
	-	`std::partial_sum`, `adjacent_difference`

6. Min/Max
	-	`std::ranges::min_element`, `max_element`, `minmax_element`
	-	`std::clamp`

7. Ensemble / set-like (sur ranges triées)
	-	`std::ranges::set_union`, `set_intersection`, etc.

8. Outils souvent oubliés mais “gold”
	-	`std::ranges::for_each`
	-	`std::ranges::partition` / stable_partition
	-	`std::ranges::rotate`, `reverse`, `shuffle`
	-	`std::ranges::equal`, `lexicographical_compare`

### Rappel `std::copy_if`
But: copier uniquement les éléments qui satisfont un prédicat
```cpp
std::copy_if(first,last,d_first,pred);
```
- lit `[first,last)`
- écrit à partir de `d_first`
- copie x si `pred(x)` est `true`
```cpp
std::vector<int> out;
std::copy_if(v.begin(), v.end(), std::back_inserter(out),
             [](int x){ return x % 2 == 0; });
```

### `for (const auto& x : v | std::views::filter(pred)) expliqué`
```cpp
for (const auto& x : v | std::views::filter(pred)) {
  out.push_back(x);
}
```
Décomposition:
- v est un `std::vector<T>`
- `std::views::filter(pred)`: crée une vue paresseuse (lazy): elle ne copie rien
- `v | filter(pred) = "pipeline"` : un range qui itère uniquement sur les éléments de v pour lesquels `pred(elem)` est vrai.


## Introduction à `std::sort`
Goal is to sort the element inside an array.
```cpp
#include <iostream>
#include <algorithm>
#include <array>

// abbreviated template syntaxe
void print_container(const auto& container)
{
  for(const auto& element: container)
  {
    std::cout << element << " ";
  }
  std::cout << std::endl;
}

int main()
{
  // Array we want to sort
  std::array<int, 5> my_array{1,5,1,9,4};
  print_container(my_array);
  // sort my array, sort use random iterator type
  std::sort(my_array.begin(),my_array.end());
  print_container(my_array);

  //--------ranges version--------------
  // Array we want to sort
  std::array<int, 5> my_array_2{987,5,132,93,43};
  print_container(my_array_2);
  // Syntetic sugar: exact same operation but easier to write
  std::ranges::sort(my_array_2);
  print_container(my_array_2);
  return 0;
}
```
### Constrained algorithms
C++20 provides **contrained** version of most algorithms in the namesace `std::ranges`.  
In these algorithms, a range can be specified as either an **iterator-sentinel** pair or as a single range argument, and projections and pointer-to-momenber callables are supported.  
Additionally, the return types of most algorithms have been change to return all potentialy usefull information computed during the execution of the algorithms.
```cpp
std::vector<int> v = {6,3,6,1,3,7};
std::ranges:sort(v); // constrained algorithm
```