## Introduction à `std::optional`

`std::optional<T>` (C++17) est un wrapper qui représente une valeur de type T **présente** ou **absente**.
C'est l'outil standard pour modéliser "peut échouer / peut ne rien revnoyer" **sans**:
- valeur sentienelle (-1, **nullptr**, "").
- paramètre out + bool de succès.
- exceptions (quand tu ne veux pas en lancer).

### Idée en une phrase
Un `std::optional<int>`est soit:
- **vide** => `std::nullopt`
- contient un **int**

### Exemple minimal
```cpp
#include <optional>
#include <string>

std::optional<int> parse_int(const std::string& s)
{
    try {
        size_t idx = 0;
        int v = std::stoi(s, &idx);
        if (idx != s.size()) return std::nullopt; // caractères en trop
        return v;                                  // valeur présente
    } catch (...) {
        return std::nullopt;                       // échec
    }
}
```
Usage:
```cpp
auto v = parse_int("123");
if (v) {
    // v contient une valeur
    int x = *v;           // accès (attention: UB si vide)
} else {
    // v est vide
}
```

### Test / accéder proprement
tester présence:
  - `if(opt){...}`
  - `if(opt.value())`(lève `std::bad_optional_access`si vide)
Accéder:
  - `*opt`(rapide, à ne pas faire si tu sais que c'est présent)
  - `opt.value()`(lève `std::bad_optional_access`si vide)
  - `opt.value_or(default)`(retourne la valeur ou un défault)
Exemple:
```cpp
int port = parse_int(env_port_str).value_or(8080);
```
### Construction et remise à vide
```cpp
std::optional<std::string> name; // vide
name = "arthur";                 // contient une valeur
name.reset();                    // redevient vide
name = std::nullopt;             // idem
```
### Règle de design
**retour de fonction** excellent pour "résutat optionnel"
  - `std::optional<T>`quand l'abscence est "normal"
  - `std::expected<T,E>` (C++23) expliquer pourquoi ça échoue

### Piège fréquents
- Ne pas faire `*opt` sans test -> **Undefined Behavoir**
- `value_or(...)`évalue l'argument par valeur (donc peut coûter). Si C'est cher, préférer `if(opt)`
- `std::optional<T&>`n'existe pas (par design) => Utilise `std::reference_wrapper`