ifeq ($(OS_BINFMT),PE)
include $(PROJECT_DIR)/sysinfo/os/pe.mk
endif

ifeq ($(OS_BINFMT),ELF)
include $(PROJECT_DIR)/sysinfo/os/elf.mk
endif
