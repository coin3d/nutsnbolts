
DEFAULT_INCLUDES = -I$(top_builddir)/lib -I$(top_srcdir)/lib -I$(top_builddir)/apps -I$(top_srcdir)/apps

CLEANFILES = .\#* *.bak *.ilk *.lib *.exp *.pdb *~ *.exe viewer.cpp *.obj

BUILT_SOURCES = viewer.cpp

viewer.cpp: $(top_srcdir)/apps/viewer.cpp.in
	if cmp -s $(top_srcdir)/apps/viewer.cpp.in viewer.cpp.in; then \
	  :; \
	else \
	  cp $(top_srcdir)/apps/viewer.cpp.in .; \
	  cp $(top_srcdir)/apps/UTMCamera.cpp .; \
	  cp $(top_srcdir)/apps/UTMCamera.h .; \
	  cp $(top_srcdir)/apps/UTMPosition.cpp .; \
	  cp $(top_srcdir)/apps/UTMPosition.h .; \
	  cp $(top_srcdir)/apps/UTMElement.cpp .; \
	  cp $(top_srcdir)/apps/UTMElement.h .; \
	  cp $(top_srcdir)/apps/Geometry.cpp .; \
	  cp $(top_srcdir)/apps/Geometry.h .; \
	  cp $(top_srcdir)/apps/GeometryKit.cpp .; \
	  cp $(top_srcdir)/apps/GeometryKit.h .; \
	  cp $(top_srcdir)/apps/GridKit.cpp .; \
	  cp $(top_srcdir)/apps/GridKit.h .; \
	  cp $(top_srcdir)/apps/LabelKit.cpp .; \
	  cp $(top_srcdir)/apps/LabelKit.h .; \
	  cp $(top_srcdir)/apps/LineKit.cpp .; \
	  cp $(top_srcdir)/apps/LineKit.h .; \
	  cp $(top_srcdir)/apps/PointKit.cpp .; \
	  cp $(top_srcdir)/apps/PointKit.h .; \
	  cp $(top_srcdir)/apps/PolygonKit.cpp .; \
	  cp $(top_srcdir)/apps/PolygonKit.h .; \
	  cp $(top_srcdir)/apps/SelectionKit.cpp .; \
	  cp $(top_srcdir)/apps/SelectionKit.h .; \
	  cp $(top_srcdir)/apps/VertexArrayShape.cpp .; \
	  cp $(top_srcdir)/apps/SmVertexArrayShape.h .; \
	fi
	@sim_ac_prog_sogui_config@ --ac-subst viewer.cpp.in
	CPPFLAGS="$(NUTSNBOLTS_EXTRA_CPPFLAGS)" \
	LDFLAGS="$(NUTSNBOLTS_EXTRA_LDFLAGS)" \
	  @sim_ac_prog_sogui_config@ --build viewer \
	    viewer.cpp Geometry.cpp GeometryKit.cpp GridKit.cpp LabelKit.cpp \
	    LineKit.cpp PointKit.cpp PolygonKit.cpp SelectionKit.cpp \
	    UTMCamera.cpp UTMPosition.cpp UTMElement.cpp \
	    VertexArrayShape.cpp \
	  $(NUTSNBOLTS_EXTRA_LIBS)

