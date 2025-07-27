## GDB
### Tutorial
Run the program with error
```bash
g++ 0_error.cpp -o 0_error -g
./0_error
double free or corruption (!prev)
Aborted (core dumped)
```
start gdb
```bash
#start gdb
gdb ./0_error
# make the program run
run
# backtrace the error
# show all the step until the error in the program
backtrace or bt
(gdb) bt
#0  __pthread_kill_implementation (threadid=281474842450464, signo=signo@entry=6, no_tid=no_tid@entry=0)
    at ./nptl/pthread_kill.c:44
#1  0x0000fffff7b6f244 in __pthread_kill_internal (signo=6, threadid=<optimized out>)
    at ./nptl/pthread_kill.c:78
#2  0x0000fffff7b2a67c in __GI_raise (sig=sig@entry=6) at ../sysdeps/posix/raise.c:26
#3  0x0000fffff7b17130 in __GI_abort () at ./stdlib/abort.c:79
#4  0x0000fffff7b63300 in __libc_message (action=action@entry=do_abort, fmt=fmt@entry=0xfffff7c443b8 "%s\n")
    at ../sysdeps/posix/libc_fatal.c:156
#5  0x0000fffff7b7956c in malloc_printerr (str=str@entry=0xfffff7c3f7d0 "double free or corruption (!prev)")
    at ./malloc/malloc.c:5664
#6  0x0000fffff7b7b694 in _int_free (av=0xfffff7c8bb10 <main_arena>, p=0xaaaaaaac5ea0, 
    have_lock=<optimized out>) at ./malloc/malloc.c:4591
#7  0x0000fffff7b7dc74 in __GI___libc_free (mem=<optimized out>) at ./malloc/malloc.c:3391
#8  0x0000aaaaaaaa16e0 in main () at 0_error.cpp:29
```
Debugging
```bash
# in gdb
# we want to stop before the error arise
break 0_error.cpp:29
# information about the breakpoint
info breakpoints
# rerun the program
run
# print information about the program (line before and after)
list
# make the program continue the breakpoint
continue
```

Compile 1_error
```bash
```bash
g++ 1_error.cpp -o 1_error -g
./1_error
```

Run gdb
```bash
gdb ./1_error
# run the program
run
# print value of variable
print(divisor)
# add breakpoint
break 1_error.cpp:21
# add commands for the break point
commands
run
end
# get info on the breakpoint
info breakpoints
# we can test patch without recompile the code 
break 1_error.cpp:21 if divisor==0
# add a commands
commands
set divisor=1
print("Patching divisor!")
continue
end
```

#### GDB Text User Interface
- build in debug
```bash
g++ 0_error.cpp -g -O2  -o 0_error 
```
à faire dans un terminal hors vscode
- press ctrl + x puis ctrl + a ou ctrl + x and ctrl + 1 
- ctrl + x and ctrl + 2 
```bash
gdb ./0_errorr
ctrl + x and ctrl + a 
layout asm : to check the assemby code 
layout regs: show the register 
layout src: montre le fichier
```
- start: go to start of main
- step: go to another statement
ctrl + x and ctrl + o: to move in the user interface
- add a vector to ligne 24: b 24
- continue: go until the next breakpoint
- stepi: move at instruction level

### Debug a C++ project in VS Code 
- debug configuration is define in launch.json in .vscode
compile with debug
```bash
g++ hello_world.cpp -g -o hello_world
```
- add a new configuration for the hello_world program
    - Run / configuration / GDB
    - créer un fichier json avec toutes les specificités
    - press F5
- debugging panel
    - continue: run the program until the next break point F5
    - step over: to go to the next line of code (skip function call) F10
    - step into: to go into a funcion F11
    - step out: go out of a function: shift + F11
    - restart
    - stop
    - next (ou n) : exécute la ligne courante et passe à la suivante (ne rentre pas dans les appels de fonction)
    - info locals    # voir les variables de cette frame
    - delete 1       # supprime le breakpoint numéro 1

### Debug Samples
run the diagram
```bash
/opt/rtmaps/bin/rtmaps_runtime /opt/rtmaps/packages/rtmaps_input_reader/samples/test-multiplier-2.rtd --run --console
```
gdb
```bash
ps aux | grep rtmaps_runtime

sudo gdb -p 1773732
break maps_multiplier_2.cpp:100
continue # to to the breakpoint
info sharedlibrary
info sources

break maps_multiplier_2.cpp:Birth
break maps_multiplier_2.cpp:Core
break maps_multiplier_2.cpp:MyFunction
```
dgb with diagram
```bash
## Setup to run a diagram
sudo gdb --args /opt/rtmaps/bin/rtmaps_runtime /opt/rtmaps/packages/rtmaps_input_reader/samples/test-multiplier-2.rtd --run --console --no-X11
## Setup breakpoint
break maps_multiplier_2.cpp:65
break MAPSmultiplier2::Birth
## Run the diagram until the breakpoint
run
```
commande gdb:
- next     # exécute la ligne en cours sans entrer dans les appels
- step     # entre dans la fonction appelée
- finish   # exécute jusqu’à la fin de la fonction
- continue # jusqu’au prochain breakpoint
layout src         # affichage source
layout split       # source + assembleur
Ctrl + X puis A    # activer/désactiver TUI
- start: go to the beginning of the main fonction
- list: get context where we are 
Set a watchpoint: expression we want to watch overtime
- watch p[24]
info wathcpoints: get information about watchpoints 
- continue: continue the program 
will stop when the value watch changed
- print(variable): print the value of a variable 
- set a breakpoint: b 32 (il faut que le fichier soit ouvert)
- delete a breakpoint: d

### Debug Samples
run the diagram
```bash
/opt/rtmaps/bin/rtmaps_runtime /opt/rtmaps/packages/rtmaps_input_reader/samples/test-multiplier-2.rtd --run --console
```
gdb
```
ps aux | grep rtmaps_runtime

sudo gdb -p 1773732
break maps_multiplier_2.cpp:100
continue # to to the breakpoint
info sharedlibrary
info sources

break maps_multiplier_2.cpp:Birth
break maps_multiplier_2.cpp:Core
break maps_multiplier_2.cpp:MyFunction
```
dgb with diagram
```bash
## Setup to run a diagram
sudo gdb --args /opt/rtmaps/bin/rtmaps_runtime /opt/rtmaps/packages/rtmaps_input_reader/samples/test-multiplier-2.rtd --run --console --no-X11
## Setup breakpoint
break maps_multiplier_2.cpp:65
break MAPSmultiplier2::Birth
break maps_multiplier_2.cpp:72
## Run the diagram until the breakpoint
run
```
# GDB - Fiche Référence pour Développement C++ avec RTMaps

## 1. Lancer GDB
- Avec arguments :
```bash
gdb --args /opt/rtmaps/bin/rtmaps_runtime /home/mic-733ao/test-multiplier-2.rtd --run --console --no-X11
```
- **Attacher à un processus existant** :
```bash
ps aux | grep rtmaps_runtime
sudo gdb -p <PID>
```
## 2. Variables et structures
- Voir les variables
    info locals : variables locales
    info args : arguments de la fonction
    print var ou p var
    display var : affiche à chaque arrêt
    set var = val : modifier une variable
- Suivre l’état
    watch var : interrompt quand la valeur change
    rwatch var : interrompt quand la variable est lue
    awatch var : quand lue ou modifiée
## 3. Inspection à l'executation
- Contrôle de l’exécution
  - run : démarre le programme
  - continue (c) : reprend jusqu’au prochain breakpoint
  - next (n) : exécute la ligne courante sans entrer dans les fonctions
  - step (s) : entre dans la fonction appelée
  - finish : termine la fonction en cours
  - until : exécute jusqu’à une ligne donnée
- Navigation dans le code
  - list : affiche le code source autour de la ligne courante
  - list NomFonction ou list fichier.cpp:ligne
  - frame, up, down : naviguer dans la pile d’appel
## 4. Breakpoints et navigation
- posser un breakpoint classique
```bash
break fichier.cpp:ligne
break NomClass::NomMethod
```
- Breakpoint conditionnel
```bash
break fichier.cpp:42 fi compteur == 5
```
- breakpoint temporaire
```bash
tbreak fichier.cpp:42
```
- Gérer les breakpoints
```bash
info breakpoints
delete 1
disable 2
enable 2
```
## 5. Threads (Multithread RTMaps)

- `info threads` : liste tous les threads
- `thread <n>` : changer de thread
- `bt` : pile d’appel du thread courant

---
## 6. Librairies Dynamiques (Composants RTMaps)

- `break MAPSmultiplier2::Birth` → pending si .so pas encore chargé
- `info sharedlibrary` : liste des bibliothèques chargées
---
## 7. Mode TUI (interface texte)
- layout src         # affichage source
- layout split       # source + assembleur
- Ctrl + X puis A    # activer/désactiver TUI
---
commande gdb:
- next ou n     # exécute la ligne en cours sans entrer dans les appels
- step     # entre dans la fonction appelée
- finish   # exécute jusqu’à la fin de la fonction
- continue # jusqu’au prochain breakpoint
- clear: cleras all breakpoints

### GDB – Navigation dans la pile d'appels (`frame`, `up`, `down`)
Quand vous déboguez un programme C++ avec GDB, il est souvent crucial de naviguer dans la **pile d'appels** (call stack) pour comprendre le contexte d'exécution.
Voici trois commandes fondamentales :
---
#### `frame`

Affiche le **cadre courant** de la pile (fonction active).  
Cela permet de voir où on est dans le code, les arguments, et le fichier source.
#### Exemple :
```
(gdb) frame
#2  0x000055555555517d in MaClasse::maFonction(int x) at fichier.cpp:42
```
---
####  `up`
Monte **vers la fonction appelante** dans la pile.  
Cela vous montre **qui a appelé** la fonction courante.

##### Exemple :
```
(gdb) up
#1  0x0000555555555010 in main() at main.cpp:12
```
---
#### `down`

Descend **dans la fonction appelée**, si vous êtes monté avec `up`.  
Cela vous permet de **revenir au niveau inférieur** de la pile.
#### Exemple :
```
(gdb) down
#2  0x000055555555517d in MaClasse::maFonction(int x) at fichier.cpp:42
```
---
#### Astuce
- Combinez `backtrace` (ou `bt`) pour voir toute la pile, puis naviguez avec `frame`, `up` et `down`.
- Utile dans RTMaps : en cas de crash ou comportement suspect d’un composant, utilisez ces commandes pour remonter la chaîne d’appel et identifier la source du problème.
#### Pour aller plus loin
- `info sources`, `info functions`, `info address var`
- `define` / `document` : créer des commandes personnalisées
- `catch throw`, `catch catch` : surveillance d'exceptions C++
- `set logging on` : journaliser la session GDB

set confirm off
    But : Désactive les confirmations interactives de GDB.
    Effet : GDB ne demande plus confirmation pour des actions destructrices (ex: recharger un symbole, supprimer un breakpoint).
    Utile pour : les scripts GDB ou une utilisation non-interactive (automatisée).
Exemple :
Sans set confirm off, GDB demanderait :
Really delete all breakpoints? (y or n)
set pagination off
    But : Désactive la pagination lors de l'affichage long.
    Effet : GDB affiche tout d’un coup sans s’arrêter à --More--.
    Utile pour : les scripts, logs ou intégrations dans des éditeurs (comme VSCode).
Exemple :
Sans cette commande, un backtrace long serait affiché page par page. Avec cette commande, tout s'affiche directement.
- set print pretty on
    But : Affiche les structures (comme les std::vector, std::map, etc.) avec un format lisible.
    Effet : Meilleure lisibilité pour les objets complexes.
    Utile pour : l’exploration de structures C++ pendant le debug.
Exemple :
Avec pretty on, un std::map est affiché clé/valeur ligne par ligne au lieu d’une seule ligne compacte.


#### VScode RTMaps diagram debug
- verifier que les fichier sources et binaire sont bien mappé
launch.json test
```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "RTMaps - test-multiplier-2",
      "type": "cppdbg",
      "request": "launch",
      "program": "/opt/rtmaps/bin/rtmaps_runtime",
      "args": [
        "/home/mic-733ao/test-multiplier-2.rtd",
        "--run",
        "--console",
        "--no-X11"
      ],
      "stopAtEntry": false,
      "cwd": "${workspaceFolder}",
      "environment": [],
      "externalConsole": false,
      "MIMode": "gdb",
      "miDebuggerPath": "/usr/bin/gdb",
      "setupCommands": [
        {
          "description": "Enable pretty printing",
          "text": "-enable-pretty-printing",
          "ignoreFailures": true
        },
        {
          "description": "Allow breakpoints in shared libraries",
          "text": "set breakpoint pending on"
        }
      ],
      "preLaunchTask": "",
      "logging": {
        "moduleLoad": true,
        "trace": false,
        "engineLogging": false
      },
      "symbolSearchPath": "",
      "miDebuggerArgs": "",
      "sourceFileMap": {
        // "/opt/rtmaps/packages": "/opt/rtmaps/packages/rtmaps_input_reader/samples"
      }
    }
  ]
}
```
- ajouter des breakpoints pour arrêter le programme
Dans le vscode debugger
-exec + command gdb usual
-exec next
-exec continue
-exec print inElt.Data(0)
-exec print outGuard.Data(0)
-exec set variable nom = nouvelle_valeur
-exec set inElt.Data(0) = 1