
AM_INCLUDES = -I$(top_srcdir)/lib

CLEANFILES = .\#* *.bak *~

if BUILD_WITH_MSVC
else
noinst_LTLIBRARIES = libNutsnBoltsMisc.la
endif

SourceFiles = \
	SoEvent.cpp \
	SoEvent.h

libNutsnBoltsMisc_la_SOURCES = $(SourceFiles)

misc.lst: $(libNutsnBoltsMisc_la_OBJECTS)
	@echo "Linking $@..."; \
	rm -f misc.lst; \
	for file in $(libNutsnBoltsMisc_la_OBJECTS); do \
	  echo $$file >> $@; \
	done

if BUILD_WITH_MSVC
all-local: misc.lst
endif

