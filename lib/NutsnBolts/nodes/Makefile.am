
AM_INCLUDES = -I$(top_srcdir)/lib -I$(top_builddir)/lib

CLEANFILES = .\#* *.bak *~ *.lst

if BUILD_WITH_MSVC
noinst_LIBRARIES = nodes.lst
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

nodes_lst_SOURCES = $(SourceFiles)

nodes.lst: $(nodes_lst_OBJECTS)
	@echo "Linking $@..."; \
	rm -f $@; \
	for obj in $(nodes_lst_OBJECTS); do \
	  echo $$obj >>$@; \
	done

# if BUILD_WITH_MSVC
# all-local: nodes.lst
# endif

