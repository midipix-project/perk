DSO_REF_VER      = $(IMPLIB_VER)
DSO_REF_SONAME   = $(IMPLIB_SONAME)
DSO_REF_SOLINK   = $(IMPLIB_SOLINK)

LDFLAGS_IMPLIB	+= -Wl,--output-def
LDFLAGS_IMPLIB	+= -Wl,$(IMPLIB_DEF)
LDFLAGS_SHARED	+= $(LDFLAGS_IMPLIB)

LDFLAGS_SONAME	+= -Wl,-soname
LDFLAGS_SONAME	+= -Wl,$(DSO_SONAME)
LDFLAGS_SHARED	+= $(LDFLAGS_SONAME)

DSO_LIBPATH	?= loader
PE_SUBSYSTEM	?= windows
LDFLAGS_COMMON	+= -Wl,--subsystem=$(PE_SUBSYSTEM)

implib:			implib-ver package-implib-soname package-implib-solink

implib-ver:		shared-lib $(IMPLIB_VER)

implib-soname:		shared-lib $(IMPLIB_SONAME)

implib-solink:		shared-lib $(IMPLIB_SOLINK)

$(IMPLIB_DEF):		shared-lib

install-implib:		install-implib-ver \
			package-install-implib-soname \
			package-install-implib-solink

install-implib-ver:	implib-ver
			mkdir -p $(DESTDIR)$(LIBDIR)
			cp $(IMPLIB_VER) $(DESTDIR)$(LIBDIR)

clean-implib:
			rm -f $(SHARED_LIB)
			rm -f $(IMPLIB_DEF)
			rm -f $(IMPLIB_VER)
			rm -f $(IMPLIB_SONAME)
			rm -f $(IMPLIB_SOLINK)


ifeq ($(OS_IMPLIB_TOOL),mdso)

$(IMPLIB_VER):		$(IMPLIB_DEF)
			$(MDSO) -m $(HOST_BITS) -i $(IMPLIB_VER) -n $(DSO_VER) -l $(DSO_LIBPATH) $<

else ifeq ($(OS_IMPLIB_TOOL),dlltool)

$(IMPLIB_VER):		$(IMPLIB_DEF)
			$(DLLTOOL) -l $(IMPLIB_VER) -d $(IMPLIB_DEF) -D $(DSO_VER)

endif



ifeq ($(AVOID_VERSION),yes)

package-implib-soname:
package-implib-solink:
package-install-implib-soname:
package-install-implib-solink:

else

package-implib-soname:		implib-soname
package-implib-solink:		implib-solink
package-install-implib-soname:	install-implib-soname
package-install-implib-solink:	install-implib-solink


ifeq ($(OS_IMPLIB_TOOL),mdso)

$(IMPLIB_SONAME):	$(IMPLIB_DEF)
			$(MDSO) -m $(HOST_BITS) -i $(IMPLIB_SONAME) -n $(DSO_SONAME) $(IMPLIB_DEF)

else ifeq ($(OS_IMPLIB_TOOL),dlltool)

$(IMPLIB_SONAME):	$(IMPLIB_DEF)
			$(DLLTOOL) -l $(IMPLIB_SONAME) -d $(IMPLIB_DEF) -D $(DSO_SONAME)

endif


$(IMPLIB_SOLINK):	$(IMPLIB_SONAME)
			rm -f $(IMPLIB_SOLINK).tmp
			ln -s $(IMP_SONAME) $(IMPLIB_SOLINK).tmp
			mv $(IMPLIB_SOLINK).tmp $(IMPLIB_SOLINK)

install-implib-soname:	implib-soname
			mkdir -p $(DESTDIR)$(LIBDIR)
			cp $(IMPLIB_SONAME) $(DESTDIR)$(LIBDIR)

install-implib-solink:	implib-soname
			mkdir -p $(DESTDIR)$(LIBDIR)
			rm -f $(IMPLIB_SOLINK).tmp
			ln -s $(IMP_SONAME) $(IMPLIB_SOLINK).tmp
			mv $(IMPLIB_SOLINK).tmp $(DESTDIR)$(LIBDIR)/$(IMP_SOLINK)

endif
