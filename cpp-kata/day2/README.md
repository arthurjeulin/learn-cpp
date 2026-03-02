### Jour 2: `copy_if`+ `count_if` (fenêtre temporeel / gating temps réel)
**Objectif**
- Simuler unf lux de points capteurs horodatés (`TimedPoint`)
  1. **Gater**(filter) les points dans une fenêtre `[t0,t1]`
  2. **Compter** ceux qui passent un second critère (ex: distance^2 < `r^2`) utile pour un "health check" rapide
Algorithms ciblés:
- `std::range::copy_if`
- `std::ranges::count_if`
#### Exercice 1 — Window gate (copy_if)
```cpp
[[nodiscard]] std::vector<TimedPoint>
filter_time_window(std::span<const TimedPoint> pts,
                   std::chrono::steady_clock::time_point t0,
                   std::chrono::steady_clock::time_point t1);
```

Spéc
- Garde les points tels que : t0 <= ts <= t1
- Retourne un `vector<TimedPoint>` (copie des éléments).
- Utilise `std::ranges::copy_if`.
ë
Pièges fréquents
- Fenêtre invalide (t1 < t0) : décide une politique. Pour le kata : retourne vide.
- Oublier `reserve(pts.size())`.
- Captures de `t0/t1`.

#### Exercice 2 — Count “close points” inside window (count_if)
```cpp
[[nodiscard]] std::size_t
count_points_within_radius(std::span<const TimedPoint> pts,
                           std::chrono::steady_clock::time_point t0,
                           std::chrono::steady_clock::time_point t1,
                           float radius_m);
```
Spéc
- Compte les points qui vérifient :
  - `t0 <= ts <= t1`
  - `x*x + y*y <= radius_m*radius_m`
- Utilise `std::ranges::count_if`.
- Pas de copie de vector ici.

Pièges
- Évite `sqrt` (compare en norme²).
- `radius_m négatif` : pour le kata, traite comme 0 (donc seuls points à l’origine passent).

#### Bonus (optionnel 5 min) — Downsample “1 point every N”
À implémenter
```cpp
[[nodiscard]] std::vector<TimedPoint>
take_every_n(std::span<const TimedPoint> pts, std::size_t n);
```
Spéc
- Si `n == 0` : retourne vide.
- Sinon garde indices `0, n, 2n, ...`
- Utilise ranges si tu veux (pas obligatoire), mais pas de boucle dans la fonction si possible.

Astuce : tu peux faire une `views::iota `+ `views::stride` (C++23) n’existe pas en C++20, donc ici tu peux :

- soit accepter une boucle (bonus only),
- soit faire `views::transform` sur un vecteur d’indices (overkill).
=> Pour le bonus, une boucle est acceptable si tu veux rester simple.