/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2016  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdio.h>

#include <perk/perk.h>
#include <perk/perk_output.h>
#include "perk_errinfo_impl.h"

int pe_output_image_category(
	const struct pe_driver_ctx *	dctx,
	const struct pe_unit_ctx *	uctx,
	FILE *				fout)
{
	struct pe_info_string		abi;
	struct pe_info_string		subtype;
	struct pe_info_string		subsystem;
	struct pe_info_string		framework;
	const struct pe_image_meta *	meta = uctx->meta;

	if (!fout)
		fout = stdout;

	pe_get_image_abi      (meta,&abi);
	pe_get_image_subtype  (meta,&subtype);
	pe_get_image_subsystem(meta,&subsystem);
	pe_get_image_framework(meta,&framework);

	if (dctx->cctx->fmtflags & PERK_PRETTY_YAML) {
		if (fprintf(fout,"%s:\n- %s:\n- %s:\n- %s:\n- %s:\n",
				*uctx->path,
				abi.buffer,
				subtype.buffer,
				subsystem.buffer,
				framework.buffer) < 0)
			return PERK_FILE_ERROR(dctx);
	} else {
		if (fprintf(fout,"%s-%s-%s-%s\n",
				abi.buffer,
				subtype.buffer,
				subsystem.buffer,
				framework.buffer) < 0)
			return PERK_FILE_ERROR(dctx);
	}

	return 0;
}