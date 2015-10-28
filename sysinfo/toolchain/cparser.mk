ifeq ($(CROSS_COMPILE)x,x)
	CROSS_HOST 	=
	CROSS_HOST_SPEC =
else
	CROSS_HOST 	= $(HOST)
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


AS 		= $(CROSS_COMPILE)as
AR 		= $(CROSS_COMPILE)ar
LD 		= $(CROSS_COMPILE)ld
NM 		= $(CROSS_COMPILE)nm
OBJDUMP 	= $(CROSS_COMPILE)objdump
RANLIB 		= $(CROSS_COMPILE)ranlib
SIZE 		= $(CROSS_COMPILE)size
STRIP 		= $(CROSS_COMPILE)strip
STRINGS 	= $(CROSS_COMPILE)strings


ADDR2LINE 	= $(CROSS_COMPILE)addr2line
COV 		= $(CROSS_COMPILE)gcov
CXXFILT 	= $(CROSS_COMPILE)c++filt
ELFEDIT 	= $(CROSS_COMPILE)elfedit
OBJCOPY 	= $(CROSS_COMPILE)objcopy
READELF 	= $(CROSS_COMPILE)readelf


CFLAGS_PIC 	= -fPIC
