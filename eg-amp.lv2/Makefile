#!/usr/bin/make -f
################################################################################################
BUILDDIR = build/
APPNAME = amp
APPUINAME =
PKG_CONFIG ?=pkg-config
CURDIR = $(PWD)/
APPNAME=amp
APPUINAME=
LIB_EXT=.so

################################################################################################
#https://caiorss.github.io/C-Cpp-Notes/compiler-flags-options.html
CFLAGS ?= -fomit-frame-pointer -O3 -fno-finite-math-only -DNDEBUG -Wall -fPIC -fvisibility=hidden -pthread `$(PKG_CONFIG) --cflags lv2` -DHAVE_LV2_1_8
LV2LDFLAGS=-Wl,-Bstatic -Wl,-Bdynamic
CFLAGS_UI ?= 


default: all

all: $(BUILDDIR)manifest.ttl $(BUILDDIR)$(APPNAME).ttl $(BUILDDIR)$(APPNAME)$(LIB_EXT)

$(BUILDDIR)manifest.ttl: manifest.ttl
	@mkdir -p $(BUILDDIR)
	cp manifest.ttl $(BUILDDIR)manifest.ttl

$(BUILDDIR)$(APPNAME).ttl: $(APPNAME).ttl
	@mkdir -p $(BUILDDIR)
	cp $(APPNAME).ttl $(BUILDDIR)$(APPNAME).ttl

$(BUILDDIR)$(APPNAME)$(LIB_EXT): amp.c
	@mkdir -p $(BUILDDIR);
	$(CXX) $(CFLAGS) -o $(BUILDDIR)$(APPNAME)$(LIB_EXT) amp.c -shared $(LV2LDFLAGS) -lm

# clean target definitions

clean:
	rm -Rf $(BUILDDIR)

.PHONY: clean all install uninstall
