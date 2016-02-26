ifeq ($(CROSS_COMPILE)x,x)
	CROSS_HOST	=
	CROSS_HOST_SPEC =
else
	CROSS_HOST	= $(HOST)
	CROSS_HOST_SPEC = --target=$(HOST)
endif


ifeq ($(USER_CC)x,x)
	CC	= $(NATIVE_CC) $(CROSS_HOST_SPEC) -Wno-experimental -integrated-cpp
else
	CC	= $(USER_CC) $(CROSS_HOST_SPEC) -Wno-experimental -integrated-cpp
endif

ifeq ($(USER_CPP)x,x)
	CPP	= $(NATIVE_CC) $(CROSS_HOST_SPEC) -Wno-experimental -integrated-cpp -E
else
	CPP	= $(USER_CPP) $(CROSS_HOST_SPEC) -Wno-experimental -integrated-cpp -E
endif

ifeq ($(USER_CXX)x,x)
	CXX	= $(NATIVE_CC) $(CROSS_HOST_SPEC) -Wno-experimental -integrated-cpp -std=c++
else
	CXX	= $(USER_CXX) $(CROSS_HOST_SPEC) -Wno-experimental -integrated-cpp -std=c++
endif


CFLAGS_PIC	= -fPIC
