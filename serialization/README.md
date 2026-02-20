## Introduction à la sérialization
### Pourquoi protobub ?
**Sérialiser = transformer un object C++ (structure mémoire) en suite d'octects (bytes)**
- pour `stocker`(fichier, DB, logs binaires)
- pour `envoyer`(réseau, IPC, Kafka...)
- pour `rejouer`des données (replay robot,dataset,etc).  

- Sérialiser = **transformer un objet en mémoire (structures, champs) en une représentation transmissible/stockable (souvent une suite d’octets).**
-	Désérialiser = **reconstruire un objet (ou un équivalent logique) à partir de cette représentation.**

Avec protobuf, le “patron” c’est le schéma `.proto` + les tags de champs.  
C’est le contrat qui permet à deux programmes (ou deux versions du même programme) de se comprendre.

**Important** : ce n’est pas “du code” qu’on sérialise, c’est des données (état).

Protobuf fournit:
- un format binaire compact et rapide
- un schéma `.proto`(contrat) => génération automatique de code
- une compatibilité bien gérée (forward/backward) si reste bien les règles.

Mais en pratique, quand les devs disent “sérialisation”, ils veulent souvent dire :
-	une représentation structurée (schéma, champs) d’objets/données (ex: protobuf, JSON),
-	avec des propriétés : compatibilité de versions, validation, parsing, etc.

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


### Proto3
- Syntaxe simple.
-	Champs scalaires ont une valeur par défaut (0, “”, false).
- “Presence” (savoir si un champ a été explicitement défini) :
-  Pour les scalaires : `optional` (proto3 moderne) permet has_*().

### Installation
```bash
sudo apt-get install
sudo apt install protobuf-compiler libprotobuf-dev
protoc --version
```

### Exemple .proto
```proto
syntax = "proto3";

package coast.perception;

message Header {
  uint64 timestamp_ns = 1;
  string frame_id = 2;
  uint32 seq = 3;
}

message Vec3 {
  double x = 1;
  double y = 2;
  double z = 3;
}

message Quaternion {
  double x = 1;
  double y = 2;
  double z = 3;
  double w = 4;
}

message Pose {
  Vec3 position = 1;
  Quaternion orientation = 2;
}

enum ObjectClass {
  OBJECT_CLASS_UNSPECIFIED = 0;
  PERSON = 1;
  CAR = 2;
  TRUCK = 3;
  ANIMAL = 4;
}

message BoundingBox2D {
  float xmin = 1;
  float ymin = 2;
  float xmax = 3;
  float ymax = 4;
}

message Detection {
  ObjectClass cls = 1;
  float score = 2;
  BoundingBox2D bbox = 3;
  optional uint32 track_id = 4; // presence utile
}

message FrameDetections {
  Header header = 1;
  Pose ego_pose = 2;
  repeated Detection detections = 3;
  map<string, string> metadata = 4;
}
```
### Génération C++
Commande typique
```bash
protoc -I=proto --cpp_out=gen proto/frame_detections.proto
```
Ça génère :
- frame_detections.pb.h
- frame_detections.pb.cc

Compile et link avec protobuf runtime.

### Utilisation C++ : créer, remplir, sérialiser, désérialiser
**Constuire et remplir**
```cpp
#include "frame_detections.pb.h"
#include <string>
#include <vector>

using coast::perception::FrameDetections;
using coast::perception::Detection;
using coast::perception::ObjectClass;

FrameDetections build_msg()
{
  FrameDetections msg;

  auto* header = msg.mutable_header();
  header->set_timestamp_ns(123456789ULL);
  header->set_frame_id("cam_front");
  header->set_seq(42);

  auto* pose = msg.mutable_ego_pose();
  pose->mutable_position()->set_x(1.0);
  pose->mutable_position()->set_y(2.0);
  pose->mutable_position()->set_z(0.0);
  pose->mutable_orientation()->set_w(1.0);

  auto* det = msg.add_detections();
  det->set_cls(ObjectClass::PERSON);
  det->set_score(0.93f);
  det->mutable_bbox()->set_xmin(100.0f);
  det->mutable_bbox()->set_ymin(50.0f);
  det->mutable_bbox()->set_xmax(200.0f);
  det->mutable_bbox()->set_ymax(300.0f);
  det->set_track_id(7);

  (*msg.mutable_metadata())["site"] = "waycross";
  return msg;
}
```
**Sérialiser en bytes**
```cpp
std::string serialize_to_string(const FrameDetections& msg)
{
  std::string out;
  out.reserve(msg.ByteSizeLong());
  if (!msg.SerializeToString(&out)) {
    throw std::runtime_error("SerializeToString failed");
  }
  return out;
}
```
**Sésérialiser**
```cpp
FrameDetections parse_from_string(const std::string& bytes)
{
  FrameDetections msg;
  if (!msg.ParseFromString(bytes)) {
    throw std::runtime_error("ParseFromString failed");
  }
  return msg;
}
```

### C'est quoi gRPC
**gRPC** est un framework RPC (Remote Procedure Call) qui utilise souvent protobug pour définir:
- des **messages** (input/output)
- des **services** (fonctions distantes)
`.proto`
```proto
service Perception {
  rpc Send(SmallMessage) returns (Ack);
}
```

### Définition de `framing` (réseau/stream)
**framing= la manière de découper un flux d'octects continu (TCP, fichier append-only,pipe,...) en message discrets.**
- TCP te livre une suite de bytes sans notion de "message"
- un `send()` côté émetteur différent un `read()` côté récepteur (on peut recevoir la moitié, ou 3 messages d'un coup).
- Donc ajoute une structure: typiquement `[length][payload]` ou un délimiteur, ou un en-tête.
Exemples de framing:
- `length-prefix`: `uint32 size + size byte`
- `varint length-prefix`: taille encodée en varint (protobuf-style)
- `delimiter`: `\n` (ok pour text, fragile pour binaire)
- `header fixe` : `magic` + `version` + `type` + `length` + `CRC` + `payload`