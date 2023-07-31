SRC := registry.c util.c

SUBMODULES := 1

ifneq (,$(filter registry_storage%,$(USEMODULE)))
  DIRS += storage
endif

ifneq (,$(filter registry_namespace_sys%,$(USEMODULE)))
  DIRS += namespace_sys
endif

ifneq (,$(filter registry_namespace_tests%,$(USEMODULE)))
  DIRS += namespace_tests
endif

include $(RIOTBASE)/Makefile.base
