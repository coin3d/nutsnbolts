
DEFAULT_INCLUDES = -I$(top_builddir)/lib -I$(top_srcdir)/lib

CLEANFILES = .\#* *.bak *.ilk *.lib *.exp *.pdb *~

noinst_PROGRAMS = viewer

viewer_SOURCES = viewer.cpp
viewer_LDADD =  -lsowin1 -L../lib/NutsnBolts -lnutsnbolts0

# ../lib/NutsnBolts/libNutsnBolts.la
# viewer_LDADD =

