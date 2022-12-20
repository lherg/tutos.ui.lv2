#!/usr/bin/make -f

# An adaptation of https://github.com/x42/convoLV2.git Makefile
# these can be overridden using make variables. e.g.
#   make CXXFLAGS=-O2
#   make install DESTDIR=$(CURDIR)/debian/tutoLV2 PREFIX=/usr
#
PREFIX ?= /usr/local
LV2DIR ?= $(PREFIX)/lib/lv2

OPTIMIZATIONS ?= -msse -msse2 -mfpmath=sse -ffast-math -fomit-frame-pointer -O3 -fno-finite-math-only -DNDEBUG
CXXFLAGS ?= $(OPTIMIZATIONS) -Wall

PKG_CONFIG?=pkg-config
STRIP ?= strip

BUILDGTK ?= yes

###############################################################################
BUILDDIR=build/
SRCDIR=src/

LV2NAME=amp
LV2UINAME=amp_ui
BUNDLE=lv2

targets=

UNAME=$(shell uname)
ifeq ($(UNAME),Darwin)
  LV2LDFLAGS=-dynamiclib
  LIB_EXT=.dylib
  STRIPFLAGS=-u -r -arch all -s lv2syms
  UISTRIPFLAGS=-u -r -arch all -s lv2uisyms
  targets+=lv2syms lv2uisyms
else
  LV2LDFLAGS=-Wl,-Bstatic -Wl,-Bdynamic
  LIB_EXT=.so
  STRIPFLAGS=-s
  UISTRIPFLAGS=-s
endif

ifneq ($(XWIN),)
  CC=$(XWIN)-gcc
  CXX=$(XWIN)-g++
  STRIP=$(XWIN)-strip
  LV2LDFLAGS=-Wl,-Bstatic -Wl,-Bdynamic -Wl,--as-needed -lpthread
  LIB_EXT=.dll
  BUILDGTK=no
  override LDFLAGS += -static-libgcc -static-libstdc++
else
  override CXXFLAGS += -fPIC -fvisibility=hidden -pthread
endif

# check for build-dependencies

ifeq ($(shell $(PKG_CONFIG) --exists lv2 || echo no), no)
  $(error "LV2 SDK was not found")
endif

ifeq ($(shell $(PKG_CONFIG) --atleast-version=1.4 lv2 || echo no), no)
  $(error "LV2 SDK needs to be version 1.4 or later")
endif

ifneq ($(shell $(PKG_CONFIG) --exists sndfile samplerate\
        && echo yes), yes)
  $(error "libsndfile and libsamplerate are required")
endif

CLV2UI=
ifneq ($(BUILDGTK), no)
  ifeq ($(shell $(PKG_CONFIG) --exists glib-2.0 gtk+-2.0 || echo no), no)
    $(warning "The optional plugin GUI requires glib-2.0 and gtk+-2.0")
    $(warning "call  make BUILDGTK=no  to disable the GUI.")
    $(error "Aborting build.")
  endif
	CLV2UI=ui:ui clv2:ui;
endif

# add library dependent flags and libs

override CXXFLAGS +=`$(PKG_CONFIG) --cflags glib-2.0 lv2 sndfile samplerate`
override LOADLIBES +=`$(PKG_CONFIG) --libs sndfile samplerate` -lm

ifeq ($(shell $(PKG_CONFIG) --atleast-version=1.8.1 lv2 && echo yes), yes)
	override CXXFLAGS += -DHAVE_LV2_1_8
endif


GTKCFLAGS = `$(PKG_CONFIG) --cflags gtk+-2.0`
GTKLIBS   = `$(PKG_CONFIG) --libs gtk+-2.0`

CAIROFLAGS = `$(PKG_CONFIG) --cflags cairo`
CAIROLIBS   = `$(PKG_CONFIG) --libs cairo`

targets+= $(BUILDDIR)$(LV2NAME)$(LIB_EXT)
targets+= $(BUILDDIR)$(LV2UINAME)$(LIB_EXT)

ifneq ($(BUILDGTK), no)
	targets+=$(BUILDDIR)$(LV2UINAME)$(LIB_EXT)
endif

# build target definitions

default: all

all: $(BUILDDIR)manifest.ttl $(BUILDDIR)$(LV2NAME).ttl $(targets)

lv2syms:
	echo "_lv2_descriptor" > lv2syms

lv2uisyms:
	echo "_lv2ui_descriptor" > lv2uisyms

$(BUILDDIR)manifest.ttl: manifest.ttl.in
	@mkdir -p $(BUILDDIR)
	sed "s/@LV2NAME@/$(LV2NAME)/;s/@LV2GUI@/$(LV2GUI)/;s/@LIB_EXT@/$(LIB_EXT)/" \
	 manifest.ttl.in > $(BUILDDIR)manifest.ttl

$(BUILDDIR)$(LV2NAME).ttl: $(LV2NAME).ttl
	@mkdir -p $(BUILDDIR)
	cp $(LV2NAME).ttl $(BUILDDIR)$(LV2NAME).ttl

$(BUILDDIR)$(LV2NAME)$(LIB_EXT): amp.c
	@mkdir -p $(BUILDDIR);
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) \
	  -o $(BUILDDIR)$(LV2NAME)$(LIB_EXT) amp.c \
	  -shared $(LV2LDFLAGS) $(LDFLAGS) $(LOADLIBES)
	$(STRIP) $(STRIPFLAGS) $(BUILDDIR)$(LV2NAME)$(LIB_EXT)

$(BUILDDIR)$(LV2UINAME)$(LIB_EXT): amp.c
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(GTKCFLAGS) $(CAIROFLAGS)\
	  -o $(BUILDDIR)$(LV2UINAME)$(LIB_EXT) amp.c \
	  -shared $(LV2LDFLAGS) $(LDFLAGS) $(LOADLIBES) $(GTKLIBS) $(CAIROLIBS)
	$(STRIP) $(STRIPFLAGS) $(BUILDDIR)$(LV2UINAME)$(LIB_EXT)


# clean target definitions

clean:
	rm -f $(BUILDDIR)manifest.ttl $(BUILDDIR)$(LV2NAME).ttl \
		$(BUILDDIR)$(LV2NAME)$(LIB_EXT) $(BUILDDIR)$(LV2UINAME)$(LIB_EXT) \
		lv2syms lv2uisyms
	rm -rf $(BUILDDIR)*.dSYM
	-test -d $(BUILDDIR) && rmdir $(BUILDDIR) || true

.PHONY: clean all install uninstall
