## RAII (Resource Acquicition Is Initialization)
**Resource Acquisition Is Initialization** is a C++ programming technique which binds the life cycle of a resource that must be acquired before use:
- allocated heap memory => from malloc or new, delete
- thread of execution
- open socket
- open file
- locked mutex
- disk space
- data base connection
- **anything that exist in limited supply**
to the lifetime of an object.

RAII guarantees that the resource is available to any function that may access the object (resource availability is class invariant, eliminating redundant runtime tests).  
It also guarantees that all resources are released when the lifetime of their controling objdct ends, in reverse order of acquisition.  
Lkewise, if resource acquicision fails (the constructor exits with an exception), all resources acquired by every fully-constructed member and base subobject are release in reverse order of initialization.  
This leverages the core language features (object lifetime, scope exit, order of initialization and stack unwinding) to eliminate resource leaks and guarantes exception safety.
RAII can be summarized as follows:
- encapsulate each resource into a class, where:
  - the constructor acquires the resource and establishes all class invariants or throws an exception if that cannot be done.
  - the destructor release the resource and never throws exceptions.
- always use the resource via an instance of RAII-class that either:
  - has automatic storage duration or temporary lifetime itself, or
  - has lifetime that is bounded by the lifetime of an automatic or temporary object.


memory_allocation.cpp
```cpp
#include <iostream>
#include <memory>

// A simple object with only constructor and destructor
struct Resource
{
  Resource(){}
  ~Resource(){}
};
```