SRC := registry.c util.c init.c

SUBMODULES := 1

ifneq (,$(filter registry_storage%,$(USEMODULE)))
  DIRS += storage
endif

include $(RIOTBASE)/Makefile.base
