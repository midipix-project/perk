/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2021  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdio.h>

#include <perk/perk.h>
#include <perk/perk_output.h>
#include "perk_driver_impl.h"
#include "perk_dprintf_impl.h"
#include "perk_errinfo_impl.h"

int pe_output_image_sections(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta)
{
	int		i;
	int		fdout;
	const char * 	dash = "";

	fdout = pe_driver_fdout(dctx);

	if (dctx->cctx->fmtflags & PERK_PRETTY_YAML) {
		if (pe_dprintf(fdout,"sections:\n") < 0)
			return PERK_FILE_ERROR(dctx);

		dash = "- ";
	}

	for (i=0; i<meta->m_coff.cfh_num_of_sections; i++)
		if (pe_dprintf(
				fdout,
				"%s%s\n",
				dash,
				meta->m_sectbl[i].sh_long_name
					? meta->m_sectbl[i].sh_long_name
					: meta->m_sectbl[i].sh_name) < 0)
			return PERK_FILE_ERROR(dctx);

	return 0;
}
