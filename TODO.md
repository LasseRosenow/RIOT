# TODO

## Mandatory

- [ ] Update documentation
  - [ ] Code comments
  - [ ] Readme.md

- [ ] XFA()
  - [ ] Automatically add schemas to a namespace on compile time
    - Problem: Cannot get the XFA array size at compile time => cannot apply schema_len
  - [x] Automatically add namespaces to the registry on compile time
  - [x] Add storage instances to the registry on compile time

- [x] Add min-value, max-value and allowed values
  - [x] implementation
  - [x] With a flag to disable for extremely constrained devices

- [ ] Unit tests
  - [x] get, set
    - [x] min and max values of type
    - [x] zero values
    - [x] constraints: min-value
    - [x] constraints: max-value
    - [x] constraints: allowed-values
    - [x] constraints: forbidden-values
  - [x] commit
  - [x] export
  - [x] load
  - [x] save
  - [x] int_path
  - [ ] string_path
  - [x] storage_vfs
  - [ ] storage_heap

- [ ] Initial values
  - dafür sorgen, dass klar ist, dass schema instances von modules initialisiert werden müssen
  - dafür sorgen dass klar ist, dass module die initialwerte erst annehmen, wenn sie dafür aufgefordert werden, um falsches nutzen von initialwerten zu verhindern, falls neue ausm speicher geladen werden können.
  - dafür z.B. sagen, dass module die daten erst benutzen, nachdem einmal commit_cb ausgeführt wurde.
  - registry_init() führt dann z.B. einmal commit_cb auf instance ebene aus oder es muss halt manuell gemacht werden who knows egal.

## Uncertain

- [ ] Use XFA() to automatically add instances at compile time? => Could not be added dynamically anymore

- [ ] commit_cb and export_cb at most on the instance level (not on the parameter/group level)?
  - wenn ein driver der meinung ist, dass das auf parameter level sinnvoll ist, kann er checks in der commit_cb method implementieren dafür
  - maybe last_change timestamp für params einführen, damit der driver einfach sehen kann, was sich seit dem letzten commit verändert hat?

- [ ] think about implementing a backwards commit to read for example sensor data
  - could also be implicit by requiring the driver to call registry_set() and registry_commit()
  - maybe introduce permission field (READ, WRITE) etc. since sensor data should not be written to
    - this should have a good name since internally write is still allowed for the driver for example
    - this is also good to expose const values like the build version number etc.

## Optional/Future

- [ ] Python code generator (registry_gen) to generate namespaces from json/yaml files
  - located in RIOT/dist/tools
  - make it a makefile target, that calls the codegen script
  - name field must be unique but also within the full path perspective and not schema wide

- [ ] Implement string_path
  - name field must be unique but also within the full path perspective and not schema wide

- [ ] Mutex
  - Mutex in export um instancen? oder einfach abhängig vom namespace, damit atomar immer die werte garantiert werden können, die zusammengehören
  - Mutex bei zugriff auf schema instance auch im modul z.B.
  - entweder durch so ein globales registry_lock() registry_release()
  - oder commit_cb generell automatisch ein lock. eig. gute idee

- [ ] Generated Typed API
  - sys namespace struct with fields pointing to each sys schema
  - schema contains fields that contain functions for set get commit export
  - a commit function of a group contains .commit() calls for all its direct children etc.
