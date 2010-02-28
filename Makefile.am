
if ENABLE_SOGUI
AppsDir = apps
else
AppsDir =
endif

EXTRA_DIST_FILES = \
	NutsnBolts.pc.in \
	cfg/gendsp.sh.in


SUBDIRS = bin lib $(AppsDir)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA=NutsnBolts.pc

CLEANFILES = .\#* *.pdb *~
DISTCLEANFILES = NutsnBolts.pc