
DEFAULT_INCLUDES = -I$(top_builddir) -I$(top_srcdir)/lib -I$(top_builddir)/lib

CLEANFILES = .\#* *.bak *~ *.lst

if BUILD_WITH_MSVC
noinst_LIBRARIES = misc.lst
else
noinst_LTLIBRARIES = libNutsnBoltsMisc.la
endif

SourceFiles = \
	SoEvent.cpp \
	SoEvent.h \
	NbNamedEvent.cpp NbNamedEvent.h

HeaderFiles = \
	NbNamedEvent.h

libNutsnBoltsMiscincdir = $(includedir)/NutsnBolts/misc
libNutsnBoltsMiscinc_HEADERS = $(HeaderFiles)

libNutsnBoltsMisc_la_SOURCES = $(SourceFiles)

misc_lst_SOURCES = $(SourceFiles)

misc.lst: $(misc_lst_OBJECTS)
	@echo "Linking $@..."; \
	rm -f $@; \
	for obj in $(misc_lst_OBJECTS); do \
	  echo $$obj >> $@; \
	done
