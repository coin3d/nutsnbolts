
SUBDIRS = misc navigation nodes

DEFAULT_INCLUDES = -I$(top_builddir)/lib -I$(top_srcdir)/lib

CLEANFILES = .\#* *.bak *.pdb *.exp *.dll *~

BUILT_SOURCES = Basic.h

if BUILD_WITH_MSVC
lib_LIBRARIES = nutsnbolts@NB_MAJOR_VERSION@.lib
else
lib_LTLIBRARIES = libNutsnBolts.la
endif

SourceFiles = \
	NutsnBolts.cpp \
	NutsnBolts.h \
	NbSceneManager.cpp \
	NbSceneManager.h

HeaderFiles = \
	Basic.h \
	NutsnBolts.h \
	NbSceneManager.h

libNutsnBolts_la_SOURCES = $(SourceFiles)

libNutsnBoltsincdir = $(includedir)/NutsnBolts
libNutsnBoltsinc_HEADERS = $(HeaderFiles)

libNutsnBolts_la_LIBADD = \
	misc/libNutsnBoltsMisc.la \
	navigation/libNutsnBoltsNavigation.la \
	nodes/libNutsnBoltsNodes.la

libNutsnBolts_la_LDFLAGS = \
	-no-undefined \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

nutsnbolts@NB_MAJOR_VERSION@_lib_SOURCES = $(SourceFiles)
nutsnbolts@NB_MAJOR_VERSION@_lib_LIBADD = \
	misc/misc.lst \
	navigation/navigation.lst \
	nodes/nodes.lst

nutsnbolts@NB_MAJOR_VERSION@.lib: Makefile \
	$(nutsnbolts@NB_MAJOR_VERSION@_lib_OBJECTS) \
	$(nutsnbolts@NB_MAJOR_VERSION@_lib_DEPENDENCIES)
	@basename="nutsnbolts@NB_MAJOR_VERSION@"; \
	rm -f "$$basename.dll" "$$basename.lib"; \
	echo "Linking $$basename.lib..."; \
	echo $(CC) $(LDFLAGS) $(LIBFLAGS) \
	  $(nutsnbolts@NB_MAJOR_VERSION@_lib_OBJECTS) \
	  $(nutsnbolts@NB_MAJOR_VERSION@_lib_LIBADD) $(LIBS); \
	$(CC) $(LDFLAGS) $(LIBFLAGS) \
	  $(nutsnbolts@NB_MAJOR_VERSION@_lib_OBJECTS) \
	  $(nutsnbolts@NB_MAJOR_VERSION@_lib_LIBADD) $(LIBS);

# **************************************************************************

if BUILD_WITH_MSVC
all-local: nutsnbolts@NB_MAJOR_VERSION@.lib

install-exec-local: NutsnBolts@NB_MAJOR_VERSION@@SUFFIX@.lib
	@basename="NutsnBolts@NB_MAJOR_VERSION@@SUFFIX@"; \
	for ext in dll pdb; do \
	  file="$$basename.$$ext"; \
	  if test -s "$$file"; then \
	    $(mkinstalldirs) "$(DESTDIR)$(bindir)"; \
	    echo $(INSTALL_PROGRAM) "$$file" "$(DESTDIR)$(bindir)/$$file"; \
	    $(INSTALL_PROGRAM) "$$file" "$(DESTDIR)$(bindir)/$$file" || exit 1; \
	  fi \
	done
else
install-exec-local:
endif

install-data-local:
	@basename="NutsnBolts@NB_MAJOR_VERSION@@SUFFIX@"; \
	for ext in lib; do \
	  file="$$basename.$$ext"; \
	  if test -s "$$file"; then \
	    $(mkinstalldirs) "$(DESTDIR)$(libdir)"; \
	    echo $(INSTALL_PROGRAM) "$$file" "$(DESTDIR)$(libdir)/$$file"; \
	    $(INSTALL_PROGRAM) "$$file" "$(DESTDIR)$(libdir)/$$file" || exit 1; \
	  fi \
	done

uninstall-local:
	@basename="NutsnBolts@NB_MAJOR_VERSION@@SUFFIX@"; \
	for file in \
	  "$(DESTDIR)$(bindir)/$$basename.dll" \
	  "$(DESTDIR)$(bindir)/$$basename.pdb" \
	  "$(DESTDIR)$(libdir)/$$basename.lib"; do \
	  if test -f "$$file"; then \
	    echo " rm -f" "$$file"; \
	    rm -f "$$file"; \
	  fi \
	done; \
	incbasedir="$(DESTDIR)$(includedir)/NutsnBolts"; \
	for dir in \
	  "$$incbasedir/navigation" \
	  "$$incbasedir/nodekits" \
	  "$$incbasedir/nodes" \
	  "$$incbasedir/misc" \
	  "$$incbasedir"; do \
	  if test -d "$$dir"; then \
	    echo " rmdir" "$$dir"; \
	    rmdir "$$dir"; \
	  fi \
	done

install-libNutsnBoltsincHEADERS: $(libNutsnBoltsinc_HEADERS)
	@$(NORMAL_INSTALL)
	@if test x"$(libNutsnBoltsinc_HEADERS)" = x""; then :; else \
	  echo " $(mkinstalldirs) $(DESTDIR)$(libNutsnBoltsincdir)"; \
	  $(mkinstalldirs) $(DESTDIR)$(libNutsnBoltsincdir); \
	fi
	@list='$(libNutsnBoltsinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  src="$$d$$p"; dst="$(DESTDIR)$(libNutsnBoltsincdir)/$$f"; \
	  if cmp -s "$$src" "$$dst"; then :; else \
	    echo " $(INSTALL_HEADER) $$src $$dst"; \
	    $(INSTALL_HEADER) "$$src" "$$dst" || exit 1; \
	  fi \
	done
	@list='$(ObsoletedHeaders)'; for f in $$list; do \
	  file="$(DESTDIR)$(libNutsnBoltsincdir)/$$f"; \
	  if test -x "$$file"; then \
	    echo " rm -f $$file"; \
	    rm -f "$$file"; \
	  fi \
	done; \
	:

