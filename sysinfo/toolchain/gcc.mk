ifeq ($(USER_CC)x,x)
	ifeq ($(CROSS_COMPILE)x,x)
		CC = $(CROSS_COMPILE)$(NATIVE_CC)
	else
		CC = $(CROSS_COMPILE)gcc
	endif
else
	CC	= $(USER_CC)
endif

ifeq ($(USER_CPP)x,x)
	CPP	= $(CROSS_COMPILE)cpp
else
	CPP	= $(USER_CPP)
endif

ifeq ($(USER_CXX)x,x)
	CXX	= $(CROSS_COMPILE)c++
else
	CXX	= $(USER_CXX)
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
