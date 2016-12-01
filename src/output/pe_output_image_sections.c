/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2016  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdio.h>

#include <perk/perk.h>
#include <perk/perk_output.h>
#include "perk_errinfo_impl.h"

int pe_output_image_sections(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta,
	FILE *				fout)
{
	int		i;
	const char * 	dash = "";

	if (!fout)
		fout = stdout;

	if (dctx->cctx->fmtflags & PERK_PRETTY_YAML) {
		if (fputs("sections:\n",fout) < 0)
			return PERK_FILE_ERROR(dctx);

		dash = "- ";
	}

	for (i=0; i<meta->coff.cfh_num_of_sections; i++)
		if (fprintf(fout,"%s%s\n",
				dash,
				meta->sectbl[i].long_name
					? meta->sectbl[i].long_name
					: meta->sectbl[i].name) < 0)
			return PERK_FILE_ERROR(dctx);

	return 0;
}
