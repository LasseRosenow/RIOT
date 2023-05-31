# TODO

- namespaces are predefined as enums, but can be set to a struct that is then this namespace?
- sys namespace struct with fields pointing to each sys schema
- schema contains fields that contain functions for set get commit export

- make registry_path optional and move it outside registry_core
  - if a storage facility needs paths, make path a requirement of that storage facility
  - for the storage facility api (interface) we use pointers to the exact items (&namespace, &schema, &instance) instead of the path integers. pointers are also smaller. so it is an advantage.
  - commit function implementations will be generated. A commit function of a group contains .commig() calls for all its direct children etc.
  - registry_sys und registry_app structs werden einfach direct generiert. Für den app namespace muss entsprechend ne sinnvolle ID einfach festgelegt werden.
  - get und set müssen nicht abstrahiert werden denke ich, weil es ja registry_get gibt, was path nutzen kann.
  - commit und export könnte nice sein, wenn path da die statische infrastruktur einfach nutzen könnte, anstelle sachen doppelt zu implementieren.
  - woher hat man bei get und set überhaupt den instance pointer? lwm2m z.B. gibt einem ja nur ne ID oder nicht? vielleicht lieber instance_id nehmen?

## Known issues

- Commit by path cannot only commit a certain parameter. It can only commit a whole instance.

## Michel

- [ ] Mutex
  - Mutex in export um instancen? oder einfach abhängig vom namespace, damit atomar immer die werte garantiert werden können, die zusammengehören
  - Mutex bei zugriff auf schema instance auch im modul z.B.
  - entweder durch so ein globales registry_lock() registry_release()
  - oder commit_cb generell automatisch ein lock. eig. gute idee

- [x] IDs aus Registry Core entfernen
  - z.B. ein switch case generieren, welches einem für einen pfad den jeweiligen pointer gibt.
  - metadaten wie "name" und "description" aus registry core entfernen vielleicht. entweder durch define, oder durch extra modul ... (dafür braucht man dann halt wieder so n switch oder so), dafür kann man dann auch so sachen machen wie: "Gebe mir pointer von parameter mit name x.

- [x] Statisches zugriffsstruct und das schema trennen und nicht so vermischt haben.
  - statisches zugriffs struct soll lieber optionaler typisierter zugriff sein. macht registry core noch schlanker. die pointer bekommt man ja sowieso durch die variable names

- [ ] Storage
  - Den pointer basierten storage nur so machen, dass er "alles auf einmal liest", dafür extrem wenig speichert, aber auch probleme mit neuer compilation hat, da er nichtmal merkt, dass er kaputt ist.
  - Pfad basiertes intefrace so lassen und auch direktes laden erlauben

- [ ] Initial values
  - dafür sorgen, dass klar ist, dass schema instances von modules initialisiert werden müssen
  - dafür sorgen dass klar ist, dass module die initialwerte erst annehmen, wenn sie dafür aufgefordert werden, um falsches nutzen von initialwerten zu verhindern, falls neue ausm speicher geladen werden können.
  - dafür z.B. sagen, dass module die daten erst benutzen, nachdem einmal commit_cb ausgeführt wurde.
  - registry_init() führt dann z.B. einmal commit_cb auf instance ebene aus oder es muss halt manuell gemacht werden who knows egal.

DAS BASIS EXPORT/COMMIT IST NUR AUF INSTANCE LEVEL!!!

- wenn ein driver der meinung ist, dass das auf parameter level sinnvoll ist, kann er checks in der commit_cb method implementieren dafür
- maybe last_change timestamp für params einführen, damit der driver einfach sehen kann, was sich seit dem letzten commit verändert hat?
- der recursive kram wandert zurück zu path oder so I guess

## Kevin Feedback

- Python code generator in RIOT/dist/tools registry_gen or something
  - and create makefile target?, that calls that script
- name field must be unique but also within the full path perspective and not schema wide
- maybe just add min-value, max-value and allowed values etc. should be so easy isn't it?
  - But with flag to disable for weak processors etc.
- think about implementing a backwards commit to read for example sensor data

## Storage rework again

- Die Zugriffs-API und die Store-API müssen nicht zwangsläufig zusammengeklebt sein?
- Man möchte vielleicht via path zugreifen, aber speicher sparen, also mit pointer speichern
- Man möchte vielleicht via pointer zugreifen, aber systemupdates überleben, also mit path speichern
- wenn registry_storage und registry_path an sind => registry_path_storage api anmachen?
- cli nutzt optional die registry_path_storage api, aber wie es am ende gespeichert wird, ist ihr egal
- die apis müssen mit beiden speichermethoden klarkommen

- neue struktur z.B
  - core
    - storage (registry.h und IS_USED(REGISTRY_STORAGE))
  - path
    - path_storage (registry_path.h und IS_USED(REGISTRY_STORAGE))
  - keine ahnung iwo müssen noch die impls hin, also VFS, VFS_PATH usw.
