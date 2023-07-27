SRC := registry.c util.c

SUBMODULES := 1

ifneq (,$(filter registry_storage%,$(USEMODULE)))
  DIRS += storage
endif

ifneq (,$(filter registry_namespace_sys%,$(USEMODULE)))
  DIRS += namespace_sys
endif

include $(RIOTBASE)/Makefile.base
