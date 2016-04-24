AS		= llvm-mc -filetype=obj
AR		= llvm-ar
LD		= lld
NM		= llvm-nm
OBJDUMP		= llvm-objdump
RANLIB		= llvm-ranlib
SIZE		= llvm-size
STRIP		= $(CROSS_COMPILE)strip
STRINGS		= $(CROSS_COMPILE)strings


ADDR2LINE	= $(CROSS_COMPILE)addr2line
COV		= $(CROSS_COMPILE)gcov
CXXFILT		= $(CROSS_COMPILE)c++filt
ELFEDIT		= $(CROSS_COMPILE)elfedit
OBJCOPY		= $(CROSS_COMPILE)objcopy
READELF		= $(CROSS_COMPILE)readelf
DLLTOOL		= $(CROSS_COMPILE)dlltool
