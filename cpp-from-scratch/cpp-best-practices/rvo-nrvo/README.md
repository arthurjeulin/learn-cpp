
## RVO et NRVO

### RVO (Return Value Optimization)

RVO est une `optimisation` (ou plus exactement une `élision` de copie/move) où le compilateur construit directement l’objet retourné dans la zone mémoire du résultat chez le caller, au lieu de :
	1.	**construire un objet temporaire dans la fonction**,
	2.	**puis le copier/déplacer vers l’objet de destination**.

Cela supprime donc un appel au constructeur de copie/move (et un destructeur de plus), ce qui peut améliorer les performances pour des types non triviaux.

Exemple :
```cpp
std::vector<int> get_vector()
{
  return std::vector<int>(1u << 20, 1);
}

void foo()
{
  auto vect = get_vector();
}
```
Ici, l’objet retourné peut être construit directement dans la mémoire de vect (dans foo).   
Le temporaire `“intermédiaire”` n’existe pas, et il n’y a ni move ni copie.

**Précision C++17+**
Depuis C++17, lorsqu’on retourne un `prvalue` (par exemple return std::vector<int>(...);), l’élision est garantie par le langage dans ce genre de cas (on parle souvent de `guaranteed copy elision`).
Autrement dit, ce n’est plus “juste une optimisation possible”, c’est le comportement attendu dans les cas concernés.

**Important : ce mécanisme ne “supprime pas” la construction — il supprime l’objet intermédiaire. L’objet final est quand même construit (une fois), mais directement au bon endroit.**

⸻

### NRVO (Named Return Value Optimization)

NRVO est le même principe, mais appliqué quand on retourne une variable locale nommée (un `lvalue`), par exemple result :
```cpp
std::vector<int> get_vector()
{
  std::vector<int> result(1u << 20, 1);
  return result;
}

void foo()
{
  auto vect = get_vector();
}
```
Avec NRVO, le compilateur peut construire result directement dans la mémoire de vect chez le caller, ce qui évite encore une copie/move.

Différence clé avec RVO
	•	RVO : retour d’un temporaire/prvalue (return T(...);)
	•	NRVO : retour d’un objet local nommé (return result;)

**Précision C++17+**
Contrairement au cas `prvalue`, NRVO n’est pas garanti par la norme : le compilateur “peut” l’appliquer.
S’il ne l’applique pas, alors en pratique on obtient généralement un move (si possible), pas une copie.

⸻

### Modèle mental (pseudo-ABI) de NRVO

Une façon de se représenter NRVO est : le caller réserve l’espace de l’objet de retour et passe implicitement une `“adresse de retour”` (un pointeur) à la fonction ; la fonction construit alors l’objet in-place à cette adresse.

Ce n’est pas du C++ source “réel”, mais une intuition de ce qui peut se passer à bas niveau.
```cpp
// Modèle mental : le caller fournit une adresse de retour
void get_vector_out(std::vector<int>* out)
{
  new (out) std::vector<int>(1'000'000, 1); // construction in-place
}

void foo()
{
  alignas(std::vector<int>) unsigned char storage[sizeof(std::vector<int>)];
  auto* out = std::launder(reinterpret_cast<std::vector<int>*>(storage));

  get_vector_out(out);

  auto& vect = *out;
  // ...

  out->~vector<int>(); // destruction explicite (car placement-new)
}
```
Remarques importantes :
- Ce code illustre le concept `“construction à l’adresse de retour”`, mais ce n’est pas la manière idiomatique d’écrire du C++.
- On utilise placement-new, donc on doit appeler le destructeur explicitement.
- En vrai C++, tu écris simplement return result; et tu laisses le compilateur appliquer NRVO/RVO.
