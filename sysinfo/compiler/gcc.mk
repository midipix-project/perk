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


CFLAGS_PIC	= -fPIC
