#!/usr/bin/make -f
################################################################################################
BUILDDIR = build/
APPNAME = amp
APPUINAME =
PKG_CONFIG ?=pkg-config
CURDIR = $(PWD)/
APPNAME=amp
APPUINAME=amp_ui
LV2NAME=eg-amp-x11gl.lv2
LIB_EXT=.so

################################################################################################
#https://caiorss.github.io/C-Cpp-Notes/compiler-flags-options.html
CFLAGS		?= -fomit-frame-pointer -O3 -fno-finite-math-only -DNDEBUG -Wall -fPIC -fvisibility=hidden -pthread `$(PKG_CONFIG) --cflags lv2` -DHAVE_LV2_1_8
LV2LDFLAGS	?=-Wl,-Bstatic -Wl,-Bdynamic
PUGLFLAGS	?=  -DPUGL_DISABLE_DEPRECATED -I$(CURDIR)pugl/include/ `$(PKG_CONFIG) --cflags cairo glib-2.0 pixman-1 uuid freetype2 libpng16`

LDFLAGS += \
-Wl,-O1 '-Wl,-rpath,$(CURDIR)pugl/build/' \
-Wl,-rpath-link,$(CURDIR)pugl/build/

LDFLAGS += \
-Wl,--start-group \
$(CURDIR)pugl/build/libpugl_x11-0.so.0.4.0 \
$(CURDIR)pugl/build/libpugl_x11_cairo-0.so.0.4.0 \
-lX11 -lXcursor -lXrandr -lXext -lm `$(PKG_CONFIG) --libs cairo` \
-Wl,--end-group

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
	$(CC) $(CFLAGS) $(PUGLFLAGS) -o $(BUILDDIR)$(APPUINAME)$(LIB_EXT) amp_ui.c $(LDFLAGS) -shared $(LV2LDFLAGS) 

# clean target definitions

install: 
	ln -s $(CURDIR)build ~/.lv2/$(LV2NAME)

uninstall:
	rm -Rf ~/.lv2/$(LV2NAME)

clean:
	rm -Rf $(BUILDDIR) ~/.lv2/$(LV2NAME)

.PHONY: clean all install uninstall