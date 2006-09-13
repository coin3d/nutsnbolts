
DEFAULT_INCLUDES = -I$(top_builddir)/lib -I$(top_srcdir)/lib

CLEANFILES = .\#* *.bak *~ *.lst

if BUILD_WITH_MSVC
noinst_LIBRARIES = navigation.lst
else
noinst_LTLIBRARIES = libNutsnBoltsNavigation.la
endif

SourceFiles = \
	NbNavigationControl.cpp \
	NbNavigationControl.h \
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
	NbPickMode.cpp \
	NbPickMode.h \
	NbZoomMode.cpp \
	NbZoomMode.h \
	NbCenterMode.cpp \
	NbCenterMode.h \
	NbOrthoCenterMode.cpp \
	NbOrthoCenterMode.h \
	NbRollMode.cpp \
	NbRollMode.h \
	NbPitchMode.cpp \
	NbPitchMode.h \
	NbYawMode.cpp \
	NbYawMode.h \
        extra.cpp

HeaderFiles = \
	NbNavigationControl.h \
	NbNavigationMode.h \
	NbNavigationState.h \
	NbNavigationSystem.h \
	NbNavigationVehicle.h \
	NbIdleMode.h \
	NbRotateMode.h \
	NbPanMode.h \
	NbPickMode.h \
	NbZoomMode.h \
	NbCenterMode.h \
	NbOrthoCenterMode.h \
	NbRollMode.h \
	NbPitchMode.h \
	NbYawMode.h \
	NbSubMode.h

libNutsnBoltsNavigationincdir = $(includedir)/NutsnBolts/navigation
libNutsnBoltsNavigationinc_HEADERS = $(HeaderFiles)

libNutsnBoltsNavigation_la_SOURCES = $(SourceFiles)

navigation_lst_SOURCES = $(SourceFiles)

navigation.lst: $(navigation_lst_OBJECTS)
	@echo "Linking $@..."; \
	rm -f $@; \
	for obj in $(navigation_lst_OBJECTS); do \
	  echo $$obj >> $@; \
	done
