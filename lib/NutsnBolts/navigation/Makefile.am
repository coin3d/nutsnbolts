
INCLUDES = -I$(top_srcdir)/lib

CLEANFILES = .\#* *.bak *~

if BUILD_WITH_MSVC
lib_LIBRARIES = navigation.lst
else
lib_LTLIBRARIES = libNutsnBoltsNavigation.la
endif

SourceFiles = \
	NbNavigationInfo.cpp \
	NbNavigationInfo.h \
	NbNavigationMode.cpp \
	NbNavigationMode.h \
	NbNavigationState.cpp \
	NbNavigationState.h \
	NbNavigationSystem.cpp \
	NbNavigationSystem.h \
	NbNavigationVehicle.cpp \
	NbNavigationVehicle.h \
	NbIdleMode.cpp \
	NbIdleMode.h \
	NbRotateMode.cpp \
	NbRotateMode.h \
	NbPanMode.cpp \
	NbPanMode.h \
	NbZoomMode.cpp \
	NbZoomMode.h

HeaderFiles = \
	NbNavigationInfo.h \
	NbNavigationMode.h \
	NbNavigationState.h \
	NbNavigationSystem.h \
	NbNavigationVehicle.h \
	NbIdleMode.h \
	NbRotateMode.h \
	NbPanMode.h \
	NbZoomMode.h

libNutsnBoltsNavigation_la_SOURCES = $(SourceFiles)

libNutsnBoltsNavigationincdir = $(includedir)/NutsnBolts/navigation
libNutsnBoltsNavigationinc_HEADERS = $(HeaderFiles)

navigation_lst_SOURCES = $(SourceFiles)

navigation.lst: $(navigation_lst_OBJECTS) $(navigation_lst_DEPENDENCIES)
	rm -f navigation.lst; \
	for file in $(navigation_lst_OBJECTS); do \
	  echo $$file >> navigation.lst; \
	done

