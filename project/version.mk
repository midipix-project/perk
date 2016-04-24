include $(PROJECT_DIR)/project/tagver.mk

CFLAGS_VERSION	+= -D$(VER_NAMESPACE)_TAG_VER_MAJOR=$(VER_MAJOR)
CFLAGS_VERSION	+= -D$(VER_NAMESPACE)_TAG_VER_MINOR=$(VER_MINOR)
CFLAGS_VERSION	+= -D$(VER_NAMESPACE)_TAG_VER_PATCH=$(VER_PATCH)

VER_XYZ		= .$(VER_MAJOR).$(VER_MINOR).$(VER_PATCH)
VER_SONAME	= .$(VER_MAJOR)

# libfoo.so (common)
install-solink:		install-lib
			rm -f $@.tmp
			ln -s $(DSO_VER) $@.tmp
			mv $@.tmp $(DESTDIR)$(LIBDIR)/$(DSO_SOLINK)

$(SHARED_SOLINK):	$(SHARED_LIB)
			rm -f $@.tmp
			ln -s $(DSO_VER) $@.tmp
			mv $@.tmp $@

# libfoo.so.x (symlink)
ifeq ($(OS_SONAME),symlink)
$(SHARED_SONAME):	$(SHARED_LIB)
			rm -f $@.tmp
			ln -s $(DSO_VER) $@.tmp
			mv $@.tmp $@

install-soname:		install-lib
			rm -f $@.tmp
			ln -s $(DSO_VER) $@.tmp
			mv $@.tmp $(DESTDIR)$(LIBDIR)/$(DSO_SONAME)
endif


# libfoo.so.x (copy)
ifeq ($(OS_SONAME),copy)
install-soname:		install-lib
			cp $(SHARED_LIB) $(DESTDIR)$(LIBDIR)/$(DSO_SONAME)

$(SHARED_SONAME):	$(SHARED_LIB)
			cp $(SHARED_LIB) $(SHARED_SONAME)
endif
