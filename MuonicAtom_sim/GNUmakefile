# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := MuonicAtom
G4TARGET := $(name)
G4EXLIB := true

# ======= ih root ============
ROOTINC       = -I$(ROOTSYS)/include
ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --glibs) -lMinuit -lHtml
ROOTLIBS      := $(filter-out -lNew,$(ROOTSYS))
CPPFLAGS += $(ROOTINC)
EXTRALIBS += $(ROOTLIBS)
CPPFLAGS += -g

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/binmake.gmk

visclean:
	rm -f g4*.prim g4*.eps g4*.wrl
	rm -f .DAWN_*

