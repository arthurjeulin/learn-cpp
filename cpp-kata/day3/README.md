## Day3
Theme: **tri+déduplication** fusion de détections, suppressions de doublons d'IDs, nettoyage de logs.
Algorithmes ciblés:
- `std::ranges::sort`(avec **projection**)
- `std::ranges::unique`(avec **projection**)
Bonus:
- `std::ranges::equal_range`(binary search sur séquence triée) ou`std::ranges::stables_sort` vs `sort`

### Exercice 1 - Sort By ID (projection)
Contexte: 
- Reçois des **Detections** possiblement en désordre.
- On les tries par `id` croissant.
À implémenter
```cpp
void sort_by_id(std::vector<Detection>& dets);
```
Contraintes:
- Utilise `std::ranges::sort`
- Utilise une projection (`&Detectoin::id`) plutôt qu'un comparator manuel.

### Exercice 2 - Unique By ID (erase idom)
Contexte:
- Après fusion capteurs, on peut avoir plusieurs `Detection` avec le même `id`(ex replays, double publish)
- Objectif: supprimer les doubons par `par id` (en gardant la première occurance dans l'ordre après tri)
A implémenter
```cpp
void unique_by_id_sorted(std::vector<Detection>& dets);
```
Containtes
- Préconditions: `dets`est déjà triée par `id`
- Utilise `std::ranges::unique` avec projection `&Detection::id`
- Puis fais le `erase`correct sur le subrange retourné.
Piège fréquents:
- `unique`ne supprime pas: il "compacte" et retourne une sous-plage à supprimer.
- Si oublie de trier avant, les doublons non adjacent ne partent pas.

### Bonus - Indexer rapidement les détections d'un ID (equal_range)
Contexte robotique:
- Après tri par `id`, on veut récupérer toutes les détections ayant un `id`donnée, **sans scan complet**
A Implémenter:
```cpp
[[nodiscard]] std::span<const Detection>
equal_rangeid(std::span<const Detection> dets_sorted_by_id,int id);
```
Contraintes:
- Précondition: trié par `id`
- Utilise `std::ranges::equal_range` avec projection `&Detection::id`
- Retourne une vue `span` sur la plage des éléments avec cet `id`(ou span vide)

---
### `std::ranges::sort`
In C++20, `std::ranges::sort` (defined in `<algorithm>`) introduces projections, a powerfull way to ransform or select data from element before they are compared.

#### Core Concept
**A projection is a callable (lambda, function, or member pointer) applied to each element by the algorithms. The result of this projection is then passed to the comparator.**
- **default**: The default projection is `std::identity` which does nothing.
- **Order of Arguments**: `std::ranges::sort(range,comparator,projection)`
```cpp
struct Player{ 
  std::string name; 
  int score;
};
// Sort by score ascending ( default comparator is std::ranges::less)
std::ranges::sort(players,{}, &Player::score)
```
- **Custom Transformations** Use a lambda to sort based on a derived value like absolute value or string length.
```cpp
std::vector<int> nums = {-5,2,-1,10};
// Sort by absolute value
std::ranges::sort(nums,{}, [](int n){ return std::abs(n);});
```
- **Changing Order**: To sort in **descending** order while using a projection, replace the default comparator with `std::ranges::greater{}`
```cpp
std::ranges::sort(player,std::ranges::greater{},&Player::score);
```

#### Limitations
- **No Parallelism** Unlike `std::sort`, the`std::ranges`versions do no currently support execution poliicies for parallel sorting.

---
### `std::ranges::unique`
`std::ranges::unique` removes consecutive duplicate elements from a range, returning a subranges (`std::ranges::subrange`) that represents the new, shortened ranges.  
It requires a sorted range to remove all duplicates. The original container size remains unchanged, so `container.erase()`is  used to remove the trailing "moved-from" elements.

#### Basic Usage (Erase-Remove Idiom)
```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>

int main(){
  std::vector<int> vec = {1,1,2,2,3,3,4,4,2,1};
  // 1. Sort the vector so duplicates are adjacent
  std::ranges::sort(vec); // sorted vector
  // 2. Use std::ranges::unique
  // Return a subrange pointing to the new end
  auto [new_end, last] = std::ranges::unique(vec);
  // 3. Erase the trailing duplicates
  vec.erase(new_end,last);
  // Output: 1 2 3 4
  for(int i : vec){
    std::cout << i << " ";
  }
}
```
#### Custom Comparisoin & Projections
You can use a **projection** to specify which part of an object to compare (e.g a specific member variable) and a **predicate**s to define what "equal" means.
```cpp
struct Player{
  std::string name;
  int score;
};

int main(){
  std::vector<Player> player {
    {"Alice",10},{"Bob",10}, {"Charlie",20}
  };
  // Unique by score only, using a projection
  auto [new_end, last] = std::ranges::unique(players,
    std::equal_to<>{}, // Comparaison predicate
    &Player::score    // Projection: only look at 'score'
  );
  player.erase(new_end,last);
  // Result: Alice(10)n Charlie(20) -- Bob was removed as a consecutive duplicate score.
}
```
---
### `std::ranges::equal_range`

`std::ranges::equal_range` algorithms find the subrange containing all elements equivalent to a given value withing a sorted range. The function returns a `std::ranges::subrange` object, which behaves like a pair of iterators (begin and end)

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <string>

// A simple struct to hold data
struct S {
    int number;
    char name;

    // Constructor
    S(int number, char name) : number(number), name(name) {}

    // Required for default comparison (operator<)
    bool operator<(const S& other) const {
        return number < other.number;
    }
};

int main() {
    // The range must be sorted or partitioned with respect to the value being searched for.
    std::vector<S> vec = { {1,'A'}, {2,'B'}, {2,'C'}, {2,'D'}, {3,'F'}, {4,'G'} };

    // Value to search for (only the 'number' field is used for comparison with default operator<)
    const S value_to_find(2, '?'); 

    // Use std::ranges::equal_range
    // It returns a std::ranges::subrange (which behaves like a pair of iterators)
    auto range = std::ranges::equal_range(vec, value_to_find);

    // Check if the range is not empty and print the found elements
    if (!std::ranges::empty(range)) {
        std::cout << "Elements with number " << value_to_find.number << ": ";
        for (const auto& s : range) {
            std::cout << s.name << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "No elements found." << std::endl;
    }

    // Example using a custom projection and comparator
    std::cout << "Elements with name 'D': ";
    // Search by projecting the 'name' field and using the 'char' value for comparison
    auto range_by_name = std::ranges::equal_range(vec, 'D', std::ranges::less{}, &S::name);
    for (const auto& s : range_by_name) {
        std::cout << "{ " << s.number << ", '" << s.name << "' } ";
    }
    std::cout << std::endl;

    return 0;
}
```
other example
```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <string>

struct Employee {
    int id;
    std::string name;
};

int main() {
    // The vector must be sorted according to the same criteria used for equal_range.
    std::vector<Employee> employees = {
        {101, "Alice"},
        {102, "Bob"},
        {102, "Charlie"},
        {103, "David"},
        {103, "Eve"},
        {105, "Frank"}
    };

    int target_id = 102;

    // Use std::ranges::equal_range to find all employees with the target_id.
    // The arguments are:
    // 1. The range to search (employees).
    // 2. The value to search for (target_id).
    // 3. The comparator (std::less<>{}) - default is fine here as we compare ints.
    // 4. The projection (&Employee::id) to compare the 'id' member.
    auto range = std::ranges::equal_range(employees, target_id, std::less<>{}, &Employee::id);

    if (!std::ranges::empty(range)) {
        std::cout << "Employees found with ID " << target_id << ":\n";
        // Iterate over the resulting subrange using a range-based for loop
        for (const auto& emp : range) {
            std::cout << "- " << emp.name << " (ID: " << emp.id << ")\n";
        }
    } else {
        std::cout << "No employees found with ID " << target_id << ".\n";
    }

    return 0;
}
