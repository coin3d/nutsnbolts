
DEFAULT_INCLUDES = -I$(top_builddir)/lib -I$(top_srcdir)/lib

CLEANFILES = .\#* *.bak *.ilk *.lib *.exp *.pdb *~ *.exe viewer.cpp *.obj

BUILT_SOURCES = viewer.cpp

viewer.cpp: $(top_srcdir)/apps/viewer.cpp.in
	if cmp -s $(top_srcdir)/apps/viewer.cpp.in viewer.cpp.in; then \
	  :; \
	else \
	  cp $(top_srcdir)/apps/viewer.cpp.in viewer.cpp.in; \
	fi
	@sim_ac_prog_sogui_config@ --ac-subst viewer.cpp.in
	CPPFLAGS="$(NUTSNBOLTS_EXTRA_CPPFLAGS)" \
	LDFLAGS="$(NUTSNBOLTS_EXTRA_LDFLAGS)" \
	  @sim_ac_prog_sogui_config@ --build viewer viewer.cpp \
	  $(NUTSNBOLTS_EXTRA_LIBS)

