
AM_INCLUDES = -I$(top_srcdir)/lib -I$(top_builddir)/lib

CLEANFILES = .\#* *.bak *~

if BUILD_WITH_MSVC
else
noinst_LTLIBRARIES = libNutsnBoltsNodes.la
endif

SourceFiles = \
	NbViewerNavigationMode.cpp \
	NbViewerNavigationMode.h

HeaderFiles = \
	NbViewerNavigationMode.h

libNutsnBoltsNodesincdir = $(includedir)/NutsnBolts/nodes
libNutsnBoltsNodesinc_HEADERS = $(HeaderFiles)

libNutsnBoltsNodes_la_SOURCES = $(SourceFiles)

nodes.lst: $(libNutsnBoltsNodes_la_OBJECTS)
	@echo "Linking $@..."; \
	rm -f $@; \
	for i in $(libNutsnBoltsNodes_la_OBJECTS); do \
	  echo $$i >>$@; \
	done

if BUILD_WITH_MSVC
all-local: libnodes.lst
endif

