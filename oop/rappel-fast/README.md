## Object-Oriented Programming


#### `static`
En C++ orienté object, `static`appliqué à un attribut ou une méthode signifie principalement: **il appartient à la classe, pas à chaque instance**
#### Attribut Statique (static data member)
Definition: **Un attribut statique est une variable membre partagée par toutes les instances de la classe.**
Il n'existe qu'en seul exemplaire, indépendamment du nombre d'objects crée.
À quoi ça sert:
 - État global par classe: compteur d'instance, numéro de série, cache partagé, statistiques, singleton-like state.
 - Constantes de classe: (souvant avec `static constexpr`).
 - Données communes à tous les objects (et non propres à une isntance).

```cpp
#include <iostream>
#include <string>

class Car {
public:
    Car(std::string name) : name_(std::move(name)) {
        ++alive_count_;
    }

    ~Car() {
        --alive_count_;
    }

    static int alive_count() {  // accès contrôlé au compteur
        return alive_count_;
    }

private:
    std::string name_;
    static int alive_count_;     // partagé entre toutes les Car
};

// Définition (obligatoire si pas constexpr/inline)
int Car::alive_count_ = 0;

int main() {
    std::cout << Car::alive_count() << "\n"; // 0

    Car a("A");
    Car b("B");

    std::cout << Car::alive_count() << "\n"; // 2
}
```
#### Méthode statique (static member function)
Definition: **Une méthode statique est une fonction membre rattachée à la classe.**
Elle n'a pas de pointeur implicite this donc:
- elle peut être apellée **sans object**: `ClassName::method()`
- elle ne peut accéder directement qu'aux membres statiques (pas aux membres d'instances).
À quoi sert:
- Fonction utilitaires liées au type (helpers cohérens avec la classe).
- Factory methods: créer des objets via une méthode nommée (`create(...)`,`from_json()`)
- Accès contrôlé à un état statique (encaspulation d'un compteur, d'un cache, etc)
Exemple (factory)
```cpp
#include <string>

class Connection {
public:
    static Connection from_env() {
        // exemple simplifié : construire avec une "config"
        return Connection("tcp://127.0.0.1:9000");
    }

    const std::string& endpoint() const { return endpoint_; }

private:
    explicit Connection(std::string endpoint) : endpoint_(std::move(endpoint)) {}
    std::string endpoint_;
};

int main() {
    Connection c = Connection::from_env(); // pas besoin d'instance existante
}
```
#### Divers
1) Où est “définie” int Car::alive_count_ = 0; ? Segment .data / .bss et init du BSS
“Définie” au sens C++ (ODR / linkage)
```cpp
int Car::alive_count_ = 0;
```
Fournis la définition (au sens **One Definition Rule**) de la variable statique de classe.
Concrètement :  
- Elle doit apparaître dans exactement une **translation unit** (un seul .cpp) en C++ “classique”.
- Si dans un header inclus par plusieurs .cpp, tu auras typiquement une **erreur de multiple definition à l’édition de liens (link)**.
Exception moderne (C++17+) :
```cpp
inline static int alive_count_ = 0;
```
**Dans la classe (header) est autorisé car inline lève le problème ODR.**

Où se retrouve-t-elle en mémoire à l’exécution ? .bss vs .data

`alive_count_` a une durée de vie statique (**static storage duration**) : 
- elle existe pendant toute la durée du programme.
Si sa valeur initiale est zéro, le linker la place généralement dans le **segment .bss** **(Block Started by Symbol)** car `.bss` représente les données zéro-initialisées et ne “coûte” pas d’espace dans le fichier binaire (le binaire stocke juste la taille à réserver).
En C++, une variable à storage duration statique est zero-initialized par défaut. Donc elle vaut 0 au démarrage.
Si l’initialiseur est non nul (ex: = 42), elle ira généralement dans `.data` car le binaire doit stocker la valeur initiale.

Important : le = 0 explicite peut quand même finir en .bss selon le toolchain et le format binaire (ELF/PE/Mach-O).
La distinction est une optimisation de layout, pas une règle C++.

Comment le segment `.bss` est initialisé ?

Le principe est :
- Le loader de l’OS réserve un bloc mémoire pour l’image du programme.
- Il met à zéro la zone `.bss` (ou mappe des pages déjà à zéro).
- Puis le runtime exécute les initialisations dynamiques **(constructeurs d’objets statiques non triviaux)**, avant main().

En C++ standard, on parle de :
- zero-initialization : toutes les variables à storage statique sont d’abord mises à zéro (conceptuellement).
- puis constant initialization si applicable,
- puis dynamic initialization si nécessaire (constructeurs, code).
```cpp
int Car::alive_count_ = 0;
```
tombe dans le cas simple : zéro-init/const-init, donc pas de code compliqué à exécuter.

⸻

2) Dans l’exemple, pourquoi std::move(name) ?

```cpp
Car(std::string name) : name_(std::move(name)) {}
```
Ici, la signature prend name par valeur. 
Donc on a déjà un objet std::string name local dans le constructeur.
- Sans std::move, on ferait **une copie vers name_**.
- Avec std::move, on fait un move **(transfert du buffer interne de la string)** vers `name_`, ce qui évite souvent une **allocation/copieuse**.

Ce pattern **“take by value then move”** est courant quand tu veux une API simple qui optimise bien les rvalues :
- Appel avec rvalue : 
```cpp
Car("A")
```
Le paramètre name est construit efficacement, puis move vers name_ (très performant).
Appel avec lvalue :
```cpp
std::string s = "A";
Car c(s);
```
Là, on paie **une copie** pour construire le paramètre name **(car s est lvalue)**, puis un move vers `name_`. 
Donc : 1 copie + 1 move.

**Alternative classique** : 
- prendre `const std::string&` (évite la copie du paramètre pour les **lvalues**, mais copie quand même vers le membre) ou surcharger const& et &&, ou utiliser forwarding.
Le “by value + move” est un compromis très propre et souvent excellent.

----------------------------

1) Le loader de l’OS, c’est quoi ?

Le loader (chargeur) est la partie du système d’exploitation (et/ou du runtime) qui prépare et démarre un programme exécutable.

- Concrètement, quand tu fais `./app`, l’OS ne “lit pas juste un fichier et exécute”.
- Il doit créer un processus (un nouvel espace mémoire virtuel + un thread initial), puis mapper le contenu de l’exécutable et des bibliothèques en mémoire, régler les protections, et sauter au point d’entrée.

Sur Linux, il y a souvent deux étages :
- le loader noyau (kernel): crée le process, mappe les segments ELF, prépare la pile, etc.
- le dynamic loader/linker (ex: ld-linux.so via ld.so) si l’exécutable est dynamiquement lié :
  - il charge les .so, fait les relocations, résout des symboles, etc.

Sur Windows, le PE loader fait un rôle similaire, et sur macOS c’est dyld / Mach-O.

⸻

2) “L’image du programme”, c’est quoi ?

Le mot “image” veut dire : **la représentation complète du programme telle qu’elle existe en mémoire.**

Il y a deux idées :
- L’image sur disque (fichier exécutable)
  Le binaire (ELF/PE/Mach-O) contient des sections/segments :
	- `.text` : code machine
	- `.rodata` : constantes en lecture seule
	- `.data` : données initialisées non nulles
	- `.bss`` : données zéro-initialisées (plutôt “taille à réserver”)
	- tables de symboles, relocations, etc.

- L’image en mémoire (process image)
Quand le loader démarre ton programme, il construit l’image mémoire du process, typiquement composée de régions :
	•	segments mappés de l’exécutable (.text, .rodata, .data, .bss),
	•	bibliothèques dynamiques (libstdc++.so, libc.so, etc.),
	•	heap (allocations new/malloc),
	•	stack (pile du thread principal),
	•	TLS (thread-local storage),
	•	zones du runtime (loader, vDSO, etc.).

Chaque région a des droits (R/W/X).
Exemple important :
- `.text` est souvent RX (read+execute), `.data RW`, .`rodata R`.

⸻

Ce que fait le loader, étape par étape (vision pratique)
À haut niveau :
- Création du process
	•	nouvel espace d’adressage virtuel
	•	création du thread principal
- Chargement / mapping des segments
	•	lecture des “program headers” (ELF) ou équivalent
	•	mapping mémoire (souvent via mécanisme de type mmap)
	•	.bss est réservé et mis à zéro (pages zéro)
- Préparation de l’environnement d’exécution
	•	construction de la stack initiale avec argv, envp, et infos auxiliaires (auxv)
	•	mise en place du point d’entrée
- Si dynamique : lancement du dynamic loader
	•	charge les .so
	•	effectue les relocations (adresses à ajuster)
	•	résout les symboles (parfois lazy binding)
- Transfert au runtime C/C++
	•	exécution des init (constructeurs d’objets statiques, __attribute__((constructor)), etc.)
	•	puis appel de main()
