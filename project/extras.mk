CFLAGS_SHARED_ATTR	+= -DPERK_PRE_ALPHA -DPERK_EXPORT
CFLAGS_STATIC_ATTR	+= -DPERK_PRE_ALPHA -DPERK_STATIC
CFLAGS_APP_ATTR		+= -DPERK_APP

CFLAGS_CONFIG          += $(CFLAGS_ATTR_VISIBILITY_HIDDEN)

src/driver/pe_driver_ctx.o:	version.tag
src/driver/pe_driver_ctx.lo:	version.tag
