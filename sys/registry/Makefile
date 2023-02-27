SRC := core.c util.c

SUBMODULES := 1

ifneq (,$(filter registry_storage_facility_%,$(USEMODULE)))
  DIRS += storage_facility
endif

ifneq (,$(filter registry_schema_%,$(USEMODULE)))
  DIRS += schema
endif

include $(RIOTBASE)/Makefile.base
