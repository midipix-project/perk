OS		= mingw
OS_APP_PREFIX	=
OS_APP_SUFFIX	= .exe
OS_LIB_PREFIX	= lib
OS_LIB_SUFFIX	= .dll
OS_IMPLIB_EXT	= .dll.a
OS_LIBDEF_EXT	= .def
OS_ARCHIVE_EXT	= .a
OS_SONAME	= copy

CFLAGS_PIC	=
LDFLAGS_CONFIG	+= -Wl,--out-implib,$(SHARED_IMPLIB)

install-implib:	shared-implib
		mkdir -p $(DESTDIR)$(LIBDIR)
		cp $(SHARED_IMPLIB) $(DESTDIR)$(LIBDIR)
