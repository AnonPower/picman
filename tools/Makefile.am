## Process this file with automake to produce Makefile.in

libgimpbase = $(top_builddir)/libgimpbase/libgimpbase-$(GIMP_API_VERSION).la

if WITH_PDBGEN
D_pdbgen = pdbgen
else
D_pdbgen =
endif

SUBDIRS = $(D_pdbgen)

if OS_WIN32

bin_PROGRAMS = \
	gimptool-2.0 \
	gimp-debug-resume

gimp_debug_resume_SOURCES = gimp-debug-resume.c

else

bin_PROGRAMS = gimptool-2.0

endif

noinst_PROGRAMS = test-clipboard

EXTRA_PROGRAMS = \
	kernelgen

gimptool_2_0_SOURCES = \
	gimptool.c

gimptool_2_0_LDADD = \
	$(libgimpbase)			\
	$(GTK_LIBS)

kernelgen_SOURCES = kernelgen.c

test_clipboard_SOURCES = test-clipboard.c

test_clipboard_LDADD = \
	$(GTK_LIBS)

AM_CPPFLAGS = \
	-DGIMP_APP_VERSION=\"@GIMP_APP_VERSION@\"	\
	-DLOCALEDIR=\""$(gimplocaledir)"\"		\
	-DPREFIX=\""$(prefix)"\"			\
	-DEXEC_PREFIX=\""$(exec_prefix)"\"		\
	-DBINDIR=\""$(bindir)"\"			\
	-DSBINDIR=\""$(sbindir)"\"			\
	-DLIBEXECDIR=\""$(libexecdir)"\"		\
	-DDATADIR=\""$(datadir)"\"			\
	-DDATAROOTDIR=\""$(datarootdir)"\"		\
	-DSYSCONFDIR=\""$(sysconfdir)"\"		\
	-DSHAREDSTATEDIR=\""$(sharedstatedir)"\"	\
	-DLOCALSTATEDIR=\""$(localstatedir)"\"		\
	-DLIBDIR=\""$(libdir)"\"			\
	-DINFODIR=\""$(infodir)"\"			\
	-DMANDIR=\""$(mandir)"\"			\
	-DGIMPPLUGINDIR=\""$(gimpplugindir)"\"		\
	-DGIMPDATADIR=\""$(gimpdatadir)"\"		\
	-DCC=\""$(CC)"\"				\
	-DGIMPDIR=\""$(gimpdir)"\"			\
	-DGIMP_PLUGIN_VERSION=\""$(GIMP_PLUGIN_VERSION)"\"

INCLUDES = \
	-I$(top_srcdir)	\
	$(GTK_CFLAGS)	\
	-I$(includedir)

EXTRA_DIST = \
	defcheck.py	\
	gimp-mkenums	\
	gimppath2svg.py
