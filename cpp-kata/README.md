## cpp kata

### Jour 1: Exercice

#### `find_if`- `first high-confidence detection`
**Contexte** reçoit une framde de détection triée (ou pas) dans l'ordre d'arrivée.
**But**: trouver la première détection dont le score >= threshold
**À Implémenter**
```cpp
std::optional<Detection> first_detection_above(std::span<const Detection> dets, float threshold);
```
**Règle**
- Retourne `std::nullopt` si aucune
- Sinon retourne une copie de la première détection matchée
- Utilise std::ranges::find_if(pas de boucle manuelle)