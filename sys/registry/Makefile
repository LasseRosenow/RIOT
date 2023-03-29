SRC := registry.c util.c

SUBMODULES := 1

ifneq (,$(filter registry_path%,$(USEMODULE)))
  DIRS += path
endif

ifneq (,$(filter registry_storage%,$(USEMODULE)))
  DIRS += storage
endif

ifneq (,$(filter registry_sys_namespace%,$(USEMODULE)))
  DIRS += sys_namespace
endif

ifneq (,$(filter registry_cli%,$(USEMODULE)))
  CFLAGS += -DCONFIG_REGISTRY_ENABLE_META_NAME
endif

include $(RIOTBASE)/Makefile.base
