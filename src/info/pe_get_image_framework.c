/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2016  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <string.h>
#include <stdbool.h>

#include <perk/perk.h>
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

static bool pe_image_is_psxscl(const struct pe_image_meta * m)
{
	return (!m->summary.nimplibs
		&& !pe_get_expsym_by_name(m,"__psx_init",0));
}

static bool pe_image_is_cygwin(const struct pe_image_meta * m)
{
	int i;

	for (i=0; i<m->summary.nimplibs; i++)
		if (!(strcmp(m->idata[i].name,"cygwin1.dll")))
			return true;

	return false;
}

static bool pe_image_is_msys(const struct pe_image_meta * m)
{
	int i;

	for (i=0; i<m->summary.nimplibs; i++)
		if (!(strcmp(m->idata[i].name,"msys-2.0.dll")))
			return true;

	return false;
}

static bool pe_image_is_mingw(const struct pe_image_meta * m)
{
	return ((pe_get_named_section_index(m,".CRT") >= 0)
		&& (pe_get_named_section_index(m,".bss") >= 0)
		&& (pe_get_named_section_index(m,".tls") >= 0));
}

int pe_get_image_framework(const struct pe_image_meta * m, struct pe_info_string * infostr)
{
	int framework;

	if (pe_get_named_section_index(m,".midipix") >= 0)
		framework = PE_FRAMEWORK_MIDIPIX;

	else if (pe_get_named_section_index(m,".freestd") >= 0)
		framework = PE_FRAMEWORK_FREESTD;

	else if (pe_get_named_section_index(m,".cygheap") >= 0)
		framework = PE_FRAMEWORK_CYGONE;

	else if (pe_image_is_psxscl(m))
		framework = PE_FRAMEWORK_PSXSCL;

	else if (pe_image_is_cygwin(m))
		framework = PE_FRAMEWORK_CYGWIN;

	else if (pe_image_is_msys(m))
		framework = PE_FRAMEWORK_MSYS;

	else if (pe_image_is_mingw(m))
		framework = PE_FRAMEWORK_MINGW;

	else if (m->opt.oh_img.coh_subsystem == PE_IMAGE_SUBSYSTEM_POSIX_CUI)
		framework = PE_FRAMEWORK_SUACON;

	else if (m->opt.oh_img.coh_subsystem == PE_IMAGE_SUBSYSTEM_WINDOWS_CUI)
		framework = PE_FRAMEWORK_WINCON;

	else if (m->opt.oh_img.coh_subsystem == PE_IMAGE_SUBSYSTEM_WINDOWS_GUI)
		framework = PE_FRAMEWORK_WIN32;

	else
		framework = PE_FRAMEWORK_UNKNOWN;

	if (infostr)
		strcpy(infostr->buffer,pe_framework_str[framework]);

	return framework;
}
