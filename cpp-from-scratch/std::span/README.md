## Introduction à `std::span`

### C'est quoi `std::span`
`std::span<T>` est une **vue non propriétaire** sur une séquence contiguë de T en mémoire.
- **NON-propriétaire**: il ne possède pas les données, il ne fait pas d'allocation.
- Il stock typiquement:
  - un pointeur `T*`
  - une taille `std::size_t`
- C'est l'équivalent moderne de `(ptr,len)` mais **type-safe**.  
Utile pour passer des buffers (points, détections, frames) **sans copie**

```cpp
// Au lieu
std::vector<int> extract_ids(const std::vector<Detection>& dets);
// faire
std::vector<int> extract_ids(std::span<const Detection> dets);
```
---
### Contigu vs non-contigu (règle d'or)
span fonctionne seulement avec des données continguës
- **Ok**: `std::vector`,`std:array`, `std::string`, `T[]`, `new T[n]`
- **Pas OK**: `std::list`, `std::map`, `std::deque`(pas garanti contigu),`std::forward_list`

### `Const-correctness` `std::span<T>`vs `std::span<const T>`
- `std::span<T>`: on meut modifier les éléments.
- `std::span<const T>`: lecture seule.
---
### cpp from scratch `std::span`
We want the abstraction of a container but we don't want ownership of the underline memory.  
Example: have a look to a subset of vector. But we don't want to know the memory inside a new container.

### cppreference definition
The class template span describes an object that can refer to a contiguous sequence of objects with the first element of the sequence at poisiton zero.

A span can either have a static extent, in which case the number of element in the sequence is known at compile-time and encoded in the type or a dynamic extent.
If a span has **dynamic** extent, a typical implementation hold two members: 
- a pointer to `T`
- a size
A sapn with `static extent` may have only one member:
- a pointer to `T`

### Example
```cpp
#include <iostream>
#include <span>
#include <vector>
#include <numeric>

/**
  @brief implement a simple function that print out a subset of our vector
  */
template <typename T>
void print_sub(const std::span<T> container)
{
  for(const auto& element: container)
  {
    std::cout << element << " ";
  }
  std::cout << std::endl;
}


int main()
{
  std::size_t vector_size{20};
  std::vector<int> my_vec(vector_size);
  std::iota(my_vec.begin(),my_vec.end(),0);
  // conversion implicite de std::vector<int> en std::span<int>
  print_sub<int>(my_vec);
  return 0;
}
```
Concrètement `std::span`va contenir seulement
- **data** = `my_vector.data()`
- **size** = `my_vec.size()`
Donc pas de copie des éléments, juste (pointeur + taille).
**Avantage**: La fonction accepte toute séquence contiguë :
-	`std::vector<T>`
- `std::array<T, N>`
-	`T[N]`
- `T* + size`
- un autre `std::span<T>`
- une “slice” (subspan) d’un buffer
**généralité de l’interface + ça exprime “je veux un buffer contigu”.**