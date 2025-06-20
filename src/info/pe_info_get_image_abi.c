/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2025  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <string.h>

#include <perk/perk.h>
#include <perk/perk_consts.h>
#include <perk/perk_meta.h>

static const char * const pe_abi_str[PE_ABI_CAP] = {
	[PE_ABI_UNSUPPORTED]    = "UNSUPPORTED",
	[PE_ABI_PE32]           = "PE32",
	[PE_ABI_PE64]           = "PE64",
};

int pe_info_get_image_abi(const struct pe_image_meta * m, struct pe_info_string * infostr)
{
	int abi;

	if (((abi = m->m_abi) < 0) || (abi >= PE_ABI_CAP))
		abi = PE_ABI_UNSUPPORTED;

	if (infostr)
		strcpy(infostr->buffer,pe_abi_str[abi]);

	return abi;
}
