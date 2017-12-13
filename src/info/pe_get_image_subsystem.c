/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2017  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <string.h>

#include <perk/perk.h>
#include <perk/perk_meta.h>

static const char * const pe_subsystem_str[0x10] = {
	[PE_IMAGE_SUBSYSTEM_UNKNOWN]                  = "unknown",
	[PE_IMAGE_SUBSYSTEM_NATIVE]                   = "native",
	[PE_IMAGE_SUBSYSTEM_WINDOWS_GUI]              = "windows",
	[PE_IMAGE_SUBSYSTEM_WINDOWS_CUI]              = "console",
	[PE_IMAGE_SUBSYSTEM_POSIX_CUI]                = "posix",
	[PE_IMAGE_SUBSYSTEM_WINDOWS_CE_GUI]           = "wince",
	[PE_IMAGE_SUBSYSTEM_EFI_APPLICATION]          = "efi_app",
	[PE_IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER]  = "efi_driver",
	[PE_IMAGE_SUBSYSTEM_EFI_ROM]                  = "efi_rom",
	[PE_IMAGE_SUBSYSTEM_XBOX]                     = "xbox"
};

int pe_get_image_subsystem(const struct pe_image_meta * m, struct pe_info_string * infostr)
{
	int subsystem;

	if (m->r_obj)
		subsystem = 0;

	else if (m->m_opt.oh_img.coh_subsystem >= 0x10)
		subsystem = -1;

	else if (!pe_subsystem_str[m->m_opt.oh_img.coh_subsystem])
		subsystem = -1;

	else
		subsystem = m->m_opt.oh_img.coh_subsystem;

	if ((subsystem < 0) && infostr) {
		strcpy(infostr->buffer,"INVALID");
		return subsystem;
	}

	if (infostr)
		strcpy(infostr->buffer,pe_subsystem_str[subsystem]);

	return subsystem;
}
