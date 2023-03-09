# TODO

- namespaces are predefined as enums, but can be set to a struct that is then this namespace?
- sys namespace struct with fields pointing to each sys schema
- schema contains fields that contain functions for set get commit export

- make registry_path optional and move it outside registry_core
  - if a storage facility needs paths, make path a requirement of that storage facility
  - for the storage facility api (interface) we use pointers to the exact items (&namespace, &schema, &instance) instead of the path integers. pointers are also smaller. so it is an advantage.
  - commit function implementations will be generated. A commit function of a group contains .commig() calls for all its direct children etc.
  - registry_sys und registry_app structs werden einfach direct generiert. Für den app namespace muss entsprechend ne sinnvolle ID einfach festgelegt werden.

## Known issues

- Commit by path cannot only commit a certain parameter. It can only commit a whole instance.
