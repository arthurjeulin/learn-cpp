## Technics for cpp

### integer vs size_t
La difference principales entre `int i = 0`et `size_t i{}` dans un `for`tient à la nature du type (signé vs non signé),  \\
à la portabilité (taille du type) et aux interactions avec les taillles/indices (notamment `std::vector::size()`)
1. Le type: signé vs non signé
- `int` est signé: il peut représenter des valeurs négatives.
- `size_t` est non signé: il ne représente que des valeurs >= 0. 
  Il est conçu pour réprésenter des tailles et des indices.
2. Taille et portabilité
- `int` est générallement 32 bits.
- `size_t`a une taille suffisante pour représenter la taille maximal d'un object en mémoire sur la plateform 
  (souvent 64-bits sur un os-64 bit)
Concéquence: pour parcourir un conteneur potentiellement très brand, `size_t`est plus "naturel"