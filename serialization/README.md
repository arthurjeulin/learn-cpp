## Introduction à la sérialization
### Pourquoi protobub ?
**Sérialiser = transformer un object C++ (structure mémoire) en suite d'octects (bytes)**
- pour `stocker`(fichier, DB, logs binaires)
- pour `envoyer`(réseau, IPC, Kafka...)
- pour `rejouer`des données (replay robot,dataset,etc).  

Protobuf fournit:
- un format binaire compact et rapide
- un schéma `.proto`(contrat) => génération automatique de code
- une compatibilité bien gérée (forward/backward) si reste bien les règles.

### Modèle mental protobuf: "schéma + tags + encodage"
Un message protobuf est composé de champs identifiés par un numéro de tag:
```proto
message Pose{
  double x = 1;
  double y = 2;
  double yaw = 3;
}
```
- `1,2,3`sont critiques: ce sont les identifiants dans le format binaire.
- le nom x peut changer sans casser la compatibilité, le **tag non**
- le type influence l'encodage (varint, fixed32/64,length-delimited...)