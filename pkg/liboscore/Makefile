PKG_NAME=liboscore
PKG_URL=https://gitlab.com/oscore/liboscore
PKG_VERSION=cd7c87f2f73fb75028f26a895908f2e1982d913f
PKG_LICENSE=BSD-3-Clause

include $(RIOTBASE)/pkg/pkg.mk

all:
	$(QQ)"$(MAKE)" -C $(PKG_SOURCE_DIR)/src -f $(RIOTBASE)/Makefile.base MODULE=$(PKG_NAME)
	$(QQ)"$(MAKE)" -C $(PKG_SOURCE_DIR)/backends/nanocoap/src -f $(RIOTBASE)/Makefile.base MODULE=$(PKG_NAME)_nanocoap
	$(QQ)"$(MAKE)" -C $(PKG_SOURCE_DIR)/backends/libcose/src -f $(RIOTBASE)/Makefile.base MODULE=$(PKG_NAME)_libcose

