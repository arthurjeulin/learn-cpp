# Cheat code of random stuff to remember


RVO : optimisation où le compilateur construit directement l’objet retourné dans la mémoire du caller, évitant toute copie/move lors d’un return d’un temporaire.  
NRVO : même optimisation, mais appliquée quand on return une variable locale nommée (le “named return value”).

4) Best practices “Always const” (copier/coller)
### Règle A — “const par défaut” pour les lectures

Dès qu’une variable est un résultat que tu ne modifies plus, mets const :
```cpp
const auto frame_id = ctx.frame_id();
const auto timestamp_ns = msg.ts_ns();
```
Ça réduit les erreurs (modif involontaire) et rend l’intention claire.

### Règle B — “initialize, don’t assign”

```cpp
std::vector<int> v(n);
MyType obj(arg1, arg2);
```
plutôt que :
```cpp
std::vector<int> v;
v = std::vector<int>(n);
```
Encore plus important pour tes types “ressource” (buffers, images, vecteurs, strings).

### Règle C — paramètres : const& pour éviter copies (mais pas dogmatique)
- Gros objets : `const T&`
- Petits scalaires : passer par valeur (`int`, `double`, `pointers`)
- Si tu veux “prendre possession” : passer par valeur + std::move (pattern moderne)

Exemple :
```cpp
void set_name(std::string name) { name_ = std::move(name); } // accepte lvalue/rvalue efficacement
```
### Règle D — `const` + `constexpr` pour constantes compile-time
```cpp
static constexpr int k_max_clients = 8;
static constexpr std::string_view k_topic = "camera_front";
```
### Règle E — éviter “const” qui bloque un move utile

Piège classique :
```cpp
const std::string s = make_string();
return s;  // NRVO souvent ok, mais si pas de NRVO => move interdit car s est const
```
En pratique, les compilateurs font souvent NRVO, mais règle simple :
- ne mets pas `const` sur une variable que tu comptes `std::move` ou transférer.

⸻

### std::vector(1u << 20) vs std::vector(1048576) : perf / style

**Performance :**
- Aucune différence significative en pratique.
- `1u << 20` est une constante compile-time, tout comme `1048576`.
- Le compilateur produit la même valeur.

**Style / lisibilité :**
- `1u << 20` exprime clairement “1 Mi éléments” (puissance de 2). Très utile quand tu raisonnes en buffers, pages, tailles GPU, ring buffers, etc.
- `1048576` est opaque sans commentaire.

Attention au type :
- `1u << 20` est un `unsigned int`. Pour les tailles, je préfère souvent un size_t :
```cpp
constexpr std::size_t n = std::size_t{1} << 20;
std::vector<int> v(n);
```
ou si tu aimes les séparateurs :
```cpp
std::vector<int> v(1'048'576);
```