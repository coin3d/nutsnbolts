
if ENABLE_SOGUI
AppsDir = apps
else
AppsDir =
endif

EXTRA_DIST_FILES = cfg/gendsp.sh.in

SUBDIRS = bin lib $(AppsDir)

NutsnBolts.pc: config.status $(srcdir)/NutsnBolts.pc.in
	@$(SHELL) config.status --file=$@:NutsnBolts.pc.in

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA=NutsnBolts.pc

CLEANFILES = .\#* *.pdb *~
DISTCLEANFILES = NutsnBolts.pc