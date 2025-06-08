/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2025  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <string.h>
#include <stdbool.h>

#include <perk/perk.h>
#include <perk/perk_consts.h>
#include <perk/perk_meta.h>

static const char * const pe_framework_str[PE_FRAMEWORK_CAP] = {
	[PE_FRAMEWORK_UNKNOWN]  = "unknown",
	[PE_FRAMEWORK_FREESTD]  = "freestd",
	[PE_FRAMEWORK_PSXSCL]   = "psxscl",
	[PE_FRAMEWORK_MIDIPIX]  = "midipix",
	[PE_FRAMEWORK_CYGONE]   = "cygone",
	[PE_FRAMEWORK_CYGWIN]   = "cygwin",
	[PE_FRAMEWORK_MINGW]    = "mingw",
	[PE_FRAMEWORK_MSYS]     = "msys",
	[PE_FRAMEWORK_SUACON]   = "suacon",
	[PE_FRAMEWORK_WINCON]   = "wincon",
	[PE_FRAMEWORK_WINCLI]   = "wincli",
	[PE_FRAMEWORK_WIN32]    = "win32",
};

int pe_info_get_image_framework(const struct pe_image_meta * m, struct pe_info_string * infostr)
{
	int framework;

	if (((framework = m->m_framework) < 0) || (framework >= PE_FRAMEWORK_CAP))
		framework = PE_FRAMEWORK_UNKNOWN;

	if (infostr)
		strcpy(infostr->buffer,pe_framework_str[framework]);

	return framework;
}
