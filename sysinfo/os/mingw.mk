OS 		= mingw
OS_APP_PREFIX 	=
OS_APP_SUFFIX 	= .exe
OS_LIB_PREFIX 	= lib
OS_LIB_SUFFIX 	= .dll
OS_IMPLIB_EXT 	= .dll.a
OS_LIBDEF_EXT 	= .def
OS_ARCHIVE_EXT 	= .a

CFLAGS_PIC 	=
LDFLAGS_CONFIG	+= -Wl,--mout-implib,$(SHARED_IMPLIB)

install-implib:	shared-implib
		mkdir -p $(DESTDIR)/./$(PREFIX)/./$(LIB_DIR)
		cp $(SHARED_IMPLIB) $(DESTDIR)/./$(PREFIX)/./$(LIB_DIR)
