## Stack
Un call stack (en français, pile d'appels) est une structure de données utilisée par le programme (et le runtime/OS) pour suivre les fonctions en cours d'exécution.
#### À quoi ça sert 
À chaque fois qu'une fonction en appelle une autre, le programme empile une frame (cadre d'appel) qui contient typiquement:
- l'addresse de retour(où reprendre la fin de la fonction)
- les arguments (ou leurs référecences, selon ABI / optimisations)
- variables locales
l'état necéssaire pour restaurer l'exécution (regirstres sauvegardés)
```cpp
void c(){}
void b(){c();}
void a(){b();}
int main()
{
  a();
  return 0;
}
```
Pendant l'exécution de c(), la pile ressemble conceptuellement à
- main()
- a()
- b()
- c() <- sommet de la pile (fonction courante)
Important:
- **Debug**: "strack trace": liste des appels empilés menant à l'erreur.
- **crash / segfaul**: le call stack aide à retroucher la chaîne d'appels.
- **récursion**: chaque appel récursif ajoute une frame -> torp profond = stack overflow
- **sécurité/perf**: la pile a une taille limité (sourchent quelques Mb / thread) et l'allocation des variables locales est très rapide.