SRC := registry.c util.c

SUBMODULES := 1

ifneq (,$(filter registry_path%,$(USEMODULE)))
  DIRS += path
endif

ifneq (,$(filter registry_storage%,$(USEMODULE)))
  DIRS += storage
endif

ifneq (,$(filter registry_schema%,$(USEMODULE)))
  DIRS += schema
endif

include $(RIOTBASE)/Makefile.base
