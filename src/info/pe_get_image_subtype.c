/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2017  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <string.h>

#include <perk/perk.h>
#include <perk/perk_consts.h>
#include <perk/perk_meta.h>

/* todo: object, unrecognized */

static const char * const pe_subtype_str[PE_SUBTYPE_CAP] = {
	[PE_SUBTYPE_UNRECOGNIZED]   = "UNRECOGNIZED",
	[PE_SUBTYPE_DLL]            = "dll",
	[PE_SUBTYPE_EXE]            = "exe",
	[PE_SUBTYPE_OBJ]            = "obj",
};

int pe_get_image_subtype(const struct pe_image_meta * m, struct pe_info_string * infostr)
{
	int subtype;

	if (((subtype = m->m_subtype) < 0) || (subtype >= PE_SUBTYPE_CAP))
		subtype = PE_SUBTYPE_UNRECOGNIZED;

	if (infostr)
		strcpy(infostr->buffer,pe_subtype_str[subtype]);

	return subtype;
}
