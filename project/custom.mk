ifeq ($(OS_BINFMT),PE)
include $(PROJECT_DIR)/sysinfo/os/pe.mk
endif

ifeq ($(OS_BINFMT),ELF)
include $(PROJECT_DIR)/sysinfo/os/elf.mk
endif



ifeq ($(ALL_STATIC),yes)

package-app:	static-app
app:		PACKAGE_APP = $(STATIC_APP)
app-tag:	PACKAGE_APP = $(STATIC_APP)


else ifeq ($(ALL_SHARED),yes)

package-app:	shared-app
app:		PACKAGE_APP = $(SHARED_APP)
app-tag:	PACKAGE_APP = $(SHARED_APP)


else

package-app:	default-app
app:		PACKAGE_APP = $(DEFAULT_APP)
app-tag:	PACKAGE_APP = $(DEFAULT_APP)

endif
