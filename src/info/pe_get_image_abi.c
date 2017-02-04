/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2016  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <string.h>

#include <perk/perk.h>
#include <perk/perk_meta.h>

static const char * const pe_abi_str[PE_ABI_CAP] = {
	[PE_ABI_UNSUPPORTED]    = "UNSUPPORTED",
	[PE_ABI_PE32]           = "PE32",
	[PE_ABI_PE64]           = "PE64",
};

int pe_get_image_abi(const struct pe_image_meta * m, struct pe_info_string * infostr)
{
	int abi;

	if (m->aobj) {
		switch (m->coff.cfh_machine) {
			case PE_IMAGE_FILE_MACHINE_I386:
				abi = PE_ABI_PE32;
				break;

			case PE_IMAGE_FILE_MACHINE_IA64:
			case PE_IMAGE_FILE_MACHINE_AMD64:
				abi = PE_ABI_PE64;
				break;

			default:
				abi = PE_ABI_UNSUPPORTED;
				break;
		}
	} else {
		switch (m->opt.oh_std.coh_magic) {
			case PE_MAGIC_PE32:
				abi = PE_ABI_PE32;
				break;

			case PE_MAGIC_PE32_PLUS:
				abi = PE_ABI_PE64;
				break;

			default:
				abi = PE_ABI_UNSUPPORTED;
				break;
		}
	}

	if (infostr)
		strcpy(infostr->buffer,pe_abi_str[abi]);

	return abi;
}
