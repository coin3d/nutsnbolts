
DEFAULT_INCLUDES = -I$(top_builddir)/lib -I$(top_srcdir)/lib

CLEANFILES = .\#* *.bak *.ilk *.lib *.exp *.pdb *~

BUILT_SOURCES = viewer.cpp

viewer.cpp: $(top_srcdir)/apps/viewer.cpp.in
	@sim_ac_prog_sogui_config@ --ac-subst $(top_srcdir)/apps/viewer.cpp.in  > viewer.cpp
	@sim_ac_prog_sogui_config@ --build viewer viewer.cpp -lNutsnBolts0

