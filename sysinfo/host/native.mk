include $(PROJECT_DIR)/sysinfo/os/$(NATIVE_OS).mk

OS		= $(NATIVE_OS)
HOST_BITS	= $(NATIVE_OS_BITS)
HOST_UNDERSCORE = $(NATIVE_OS_UNDERSCORE)

ifeq ($(OS),linux)
	ifeq ($(HOST_BITS),32)
		ARCH = i386
	else ifeq ($(HOST_BITS),64)
		ARCH = x86_64
	endif
endif

ifeq ($(OS),midipix)
	ifeq ($(HOST_BITS),32)
		ARCH = nt32
	else ifeq ($(HOST_BITS),64)
		ARCH = nt64
	endif
endif

ifeq ($(OS),mingw)
	ifeq ($(HOST_BITS),32)
		ARCH = w32
	else ifeq ($(HOST_BITS),64)
		ARCH = w64
	endif
endif

ifeq ($(OS),bsd)
	ifeq ($(HOST_BITS),32)
		ARCH = bsd32
	else ifeq ($(HOST_BITS),64)
		ARCH = bsd64
	endif
endif

ifeq ($(OS),darwin)
	ifeq ($(HOST_BITS),32)
		ARCH = dw32
	else ifeq ($(HOST_BITS),64)
		ARCH = dw64
	endif
endif
