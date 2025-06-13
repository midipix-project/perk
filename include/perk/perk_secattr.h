#ifndef PERK_SECATTR_H
#define PERK_SECATTR_H

#include "perk_consts.h"

#define PE_SECTION_ATTR_BSS         (PE_IMAGE_SCN_CNT_UNINITIALIZED_DATA \
		                    |PE_IMAGE_SCN_MEM_READ               \
		                    |PE_IMAGE_SCN_MEM_WRITE)

#define PE_SECTION_ATTR_TEXT        (PE_IMAGE_SCN_CNT_CODE               \
		                    |PE_IMAGE_SCN_MEM_READ               \
		                    |PE_IMAGE_SCN_MEM_EXECUTE)

#define PE_SECTION_ATTR_DATA        (PE_IMAGE_SCN_CNT_INITIALIZED_DATA   \
		                    |PE_IMAGE_SCN_MEM_READ               \
		                    |PE_IMAGE_SCN_MEM_WRITE)

#define PE_SECTION_ATTR_TLS         (PE_IMAGE_SCN_CNT_INITIALIZED_DATA   \
		                    |PE_IMAGE_SCN_MEM_READ               \
		                    |PE_IMAGE_SCN_MEM_WRITE)

#define PE_SECTION_ATTR_DEBUG       (PE_IMAGE_SCN_CNT_INITIALIZED_DATA   \
		                    |PE_IMAGE_SCN_MEM_READ               \
		                    |PE_IMAGE_SCN_MEM_DISCARDABLE)

#define PE_SECTION_ATTR_DRECTIVE    (PE_IMAGE_SCN_LNK_INFO)

#define PE_SECTION_ATTR_CORMETA     (PE_IMAGE_SCN_LNK_INFO)

#define PE_SECTION_ATTR_IDLSYM      (PE_IMAGE_SCN_LNK_INFO)

#define PE_SECTION_ATTR_EDATA       (PE_IMAGE_SCN_CNT_INITIALIZED_DATA   \
		                    |PE_IMAGE_SCN_MEM_READ)

#define PE_SECTION_ATTR_IDATA       (PE_IMAGE_SCN_CNT_INITIALIZED_DATA   \
		                    |PE_IMAGE_SCN_MEM_READ               \
		                    |PE_IMAGE_SCN_MEM_WRITE)

#define PE_SECTION_ATTR_PDATA       (PE_IMAGE_SCN_CNT_INITIALIZED_DATA   \
		                    |PE_IMAGE_SCN_MEM_READ)

#define PE_SECTION_ATTR_RDATA       (PE_IMAGE_SCN_CNT_INITIALIZED_DATA   \
		                    |PE_IMAGE_SCN_MEM_READ)

#define PE_SECTION_ATTR_RELOC       (PE_IMAGE_SCN_CNT_INITIALIZED_DATA   \
		                    |PE_IMAGE_SCN_MEM_READ               \
		                    |PE_IMAGE_SCN_MEM_DISCARDABLE)

#define PE_SECTION_ATTR_RSRC        (PE_IMAGE_SCN_CNT_INITIALIZED_DATA   \
		                    |PE_IMAGE_SCN_MEM_READ)

#define PE_SECTION_ATTR_SBSS        (PE_IMAGE_SCN_CNT_UNINITIALIZED_DATA \
		                    |PE_IMAGE_SCN_MEM_READ               \
		                    |PE_IMAGE_SCN_MEM_WRITE              \
		                    |PE_IMAGE_SCN_GPREL)

#define PE_SECTION_ATTR_SDATA       (PE_IMAGE_SCN_CNT_INITIALIZED_DATA   \
		                    |PE_IMAGE_SCN_MEM_READ               \
		                    |PE_IMAGE_SCN_MEM_WRITE              \
		                    |PE_IMAGE_SCN_GPREL)

#define PE_SECTION_ATTR_VSDATA      (PE_IMAGE_SCN_CNT_INITIALIZED_DATA   \
		                    |PE_IMAGE_SCN_MEM_READ               \
		                    |PE_IMAGE_SCN_MEM_WRITE)

#define PE_SECTION_ATTR_SRDATA      (PE_IMAGE_SCN_CNT_INITIALIZED_DATA   \
		                    |PE_IMAGE_SCN_MEM_READ               \
		                    |PE_IMAGE_SCN_GPREL)

#define PE_SECTION_ATTR_XDATA       (PE_IMAGE_SCN_CNT_INITIALIZED_DATA   \
		                    |PE_IMAGE_SCN_MEM_READ)

#define PE_SECTION_ATTR_SXDATA      (PE_IMAGE_SCN_LNK_INFO)

#endif
