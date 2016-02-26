ifeq ($(CROSS_COMPILE)x,x)
	CROSS_HOST	=
	CROSS_HOST_SPEC =
else ifeq ($(CROSS_HOST)x,x)
	CROSS_HOST	= $(HOST)
	CROSS_HOST_SPEC = --target=$(HOST)
else
	CROSS_HOST_SPEC = --target=$(CROSS_HOST)
endif


ifeq ($(USER_CC)x,x)
	CC	= $(NATIVE_CC) $(CROSS_HOST_SPEC)
else
	CC	= $(USER_CC) $(CROSS_HOST_SPEC)
endif

ifeq ($(USER_CPP)x,x)
	CPP	= $(NATIVE_CC) $(CROSS_HOST_SPEC) -E
else
	CPP	= $(USER_CPP) $(CROSS_HOST_SPEC) -E
endif

ifeq ($(USER_CXX)x,x)
	CXX	= $(NATIVE_CC)++ $(CROSS_HOST_SPEC)
else
	CXX	= $(USER_CXX) $(CROSS_HOST_SPEC)
endif


CFLAGS_PIC	= -fPIC
