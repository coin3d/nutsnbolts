
AM_INCLUDES = -I$(top_srcdir)/lib

CLEANFILES = .\#* *.bak *~

if BUILD_WITH_MSVC
else
noinst_LTLIBRARIES = libNutsnBoltsNavigation.la
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
	NbZoomMode.h \
	NbCenterMode.cpp \
	NbCenterMode.h

HeaderFiles = \
	NbNavigationInfo.h \
	NbNavigationMode.h \
	NbNavigationState.h \
	NbNavigationSystem.h \
	NbNavigationVehicle.h \
	NbIdleMode.h \
	NbRotateMode.h \
	NbPanMode.h \
	NbZoomMode.h \
	NbCenterMode.h

libNutsnBoltsNavigationincdir = $(includedir)/NutsnBolts/navigation
libNutsnBoltsNavigationinc_HEADERS = $(HeaderFiles)

libNutsnBoltsNavigation_la_SOURCES = $(SourceFiles)

navigation.lst: $(libNutsnBoltsNavigation_la_OBJECTS)
	@echo "Linking $@..."; \
	rm -f navigation.lst; \
	for file in $(libNutsnBoltsNavigation_la_OBJECTS); do \
	  echo $$file >> $@; \
	done

if BUILD_WITH_MSVC
all-local: navigation.lst
endif

