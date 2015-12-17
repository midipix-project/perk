OS		= midipix
OS_APP_PREFIX	=
OS_APP_SUFFIX	=
OS_LIB_PREFIX	= lib
OS_LIB_SUFFIX	= .so
OS_IMPLIB_EXT	= .lib.a
OS_LIBDEF_EXT	= .so.def
OS_ARCHIVE_EXT	= .a

LDFLAGS_CONFIG	+= -mout-implib

install-implib: shared-implib
		mkdir -p $(DESTDIR)/./$(PREFIX)/./$(LIBDIR)
		cp $(SHARED_IMPLIB) $(DESTDIR)/./$(PREFIX)/./$(LIBDIR)
