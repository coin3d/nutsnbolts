
INCLUDES = -I$(top_srcdir)/lib

CLEANFILES = .\#* *.bak *~

if BUILD_WITH_MSVC
lib_LIBRARIES = misc.lst
else
lib_LTLIBRARIES = libNutsnBoltsMisc.la
endif

SourceFiles = \
	SoEvent.cpp \
	SoEvent.h

libNutsnBoltsMisc_la_SOURCES = $(SourceFiles)

misc_lst_SOURCES = $(SourceFiles)

misc.lst: $(misc_lst_OBJECTS) $(misc_lst_DEPENDENCIES)
	rm -f misc.lst; \
	for file in $(misc_lst_OBJECTS); do \
	  echo $$file >> misc.lst; \
	done

