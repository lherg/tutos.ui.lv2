#!/usr/bin/make -f
################################################################################################
BUILDDIR = build/
APPNAME = amp
APPUINAME =
PKG_CONFIG ?=pkg-config
CURDIR = $(PWD)/
APPNAME=amp
APPUINAME=amp_ui
LV2NAME=eg-amp-gtk2.lv2
LIB_EXT=.so

################################################################################################
#https://caiorss.github.io/C-Cpp-Notes/compiler-flags-options.html
CFLAGS ?= -fomit-frame-pointer -O3 -fno-finite-math-only -DNDEBUG -Wall -fPIC -fvisibility=hidden -pthread `$(PKG_CONFIG) --cflags lv2` -DHAVE_LV2_1_8
LV2LDFLAGS=-Wl,-Bstatic -Wl,-Bdynamic
GTKCFLAGS = `$(PKG_CONFIG) --cflags gtk+-2.0`
GTKLIBS   = `$(PKG_CONFIG) --libs gtk+-2.0`

default: all

all: $(BUILDDIR)manifest.ttl $(BUILDDIR)$(APPNAME).ttl $(BUILDDIR)$(APPNAME)$(LIB_EXT) $(BUILDDIR)$(APPUINAME)$(LIB_EXT)

$(BUILDDIR)manifest.ttl: manifest.ttl
	@mkdir -p $(BUILDDIR)
	cp manifest.ttl $(BUILDDIR)manifest.ttl

$(BUILDDIR)$(APPNAME).ttl: $(APPNAME).ttl
	@mkdir -p $(BUILDDIR)
	cp $(APPNAME).ttl $(BUILDDIR)$(APPNAME).ttl

$(BUILDDIR)$(APPNAME)$(LIB_EXT): amp.c
	@mkdir -p $(BUILDDIR);
	$(CC) $(CFLAGS) -o $(BUILDDIR)$(APPNAME)$(LIB_EXT) amp.c -shared $(LV2LDFLAGS) -lm

$(BUILDDIR)$(APPUINAME)$(LIB_EXT): amp_ui.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(GTKCFLAGS) -o $(BUILDDIR)$(APPUINAME)$(LIB_EXT) amp_ui.c -shared $(LV2LDFLAGS) -lm $(GTKLIBS)

# clean target definitions

install: 
	ln -s $(CURDIR)build ~/.lv2/$(LV2NAME)

clean:
	rm -Rf $(BUILDDIR) ~/.lv2/$(LV2NAME)

.PHONY: clean all install uninstall





















