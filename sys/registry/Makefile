SRC := registry.c util.c

SUBMODULES := 1

ifneq (,$(filter registry_storage%,$(USEMODULE)))
  DIRS += storage
endif

ifneq (,$(filter registry_namespace_%,$(USEMODULE)))
  DIRS += namespace
endif

include $(RIOTBASE)/Makefile.base
