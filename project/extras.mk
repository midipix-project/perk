CFLAGS_SHARED_ATTR	+= -DPERK_PRE_ALPHA -DPERK_EXPORT
CFLAGS_STATIC_ATTR	+= -DPERK_PRE_ALPHA -DPERK_STATIC
CFLAGS_APP_ATTR		+= -DPERK_APP

CFLAGS_CONFIG          += $(CFLAGS_ATTR_VISIBILITY_HIDDEN)

src/driver/pe_driver_ctx.o:	version.tag
src/driver/pe_driver_ctx.lo:	version.tag

install-app-extras:
	mkdir -p $(DESTDIR)$(BINDIR)

	rm -f bin/$(NICKNAME)-ar$(OS_APP_SUFFIX).tmp
	ln -s ./$(NICKNAME)$(OS_APP_SUFFIX) bin/$(NICKNAME)-ar$(OS_APP_SUFFIX).tmp
	mv bin/$(NICKNAME)-ar$(OS_APP_SUFFIX).tmp     $(DESTDIR)$(BINDIR)/$(NICKNAME)-ar$(OS_APP_SUFFIX)

	rm -f bin/$(NICKNAME)-nm$(OS_APP_SUFFIX).tmp
	ln -s ./$(NICKNAME)$(OS_APP_SUFFIX) bin/$(NICKNAME)-nm$(OS_APP_SUFFIX).tmp
	mv bin/$(NICKNAME)-nm$(OS_APP_SUFFIX).tmp     $(DESTDIR)$(BINDIR)/$(NICKNAME)-nm$(OS_APP_SUFFIX)

	rm -f bin/$(NICKNAME)-size$(OS_APP_SUFFIX).tmp
	ln -s ./$(NICKNAME)$(OS_APP_SUFFIX) bin/$(NICKNAME)-size$(OS_APP_SUFFIX).tmp
	mv bin/$(NICKNAME)-size$(OS_APP_SUFFIX).tmp     $(DESTDIR)$(BINDIR)/$(NICKNAME)-size$(OS_APP_SUFFIX)
