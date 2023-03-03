SRC := core.c util.c

SUBMODULES := 1

ifneq (,$(filter registry_storage_%,$(USEMODULE)))
  DIRS += storage
endif

ifneq (,$(filter registry_schema_%,$(USEMODULE)))
  DIRS += schema
endif

include $(RIOTBASE)/Makefile.base
