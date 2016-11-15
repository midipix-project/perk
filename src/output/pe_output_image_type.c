/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2016  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <perk/perk.h>
#include <perk/perk_output.h>
#include "perk_errinfo_impl.h"

static const char * pretty_abi(const struct pe_unit_ctx * uctx)
{
	switch (uctx->meta->opt.std.magic) {
		case PE_MAGIC_PE32:
			return "PE32";

		case PE_MAGIC_PE32_PLUS:
			return "PE64";

		default:
			return "INTERNAL_ERROR";
	}
}

static const char * pretty_type(const struct pe_unit_ctx * uctx)
{
	if (uctx->meta->coff.characteristics & PE_IMAGE_FILE_DLL)
		return "dll";
	else
		return "exe";
}

int pe_output_image_type(
	const struct pe_driver_ctx *	dctx,
	const struct pe_unit_ctx *	uctx,
	FILE *				fout)
{
	struct pe_info_string		subsystem;
	struct pe_info_string		framework;
	const struct pe_image_meta *	meta = uctx->meta;

	if (!fout)
		fout = stdout;

	pe_get_image_subsystem(meta,&subsystem);
	pe_get_image_framework(meta,&framework);

	if (dctx->cctx->fmtflags & PERK_PRETTY_YAML) {
		if (fprintf(fout,"%s:\n- %s:\n- %s:\n- %s:\n- %s:\n",
				*uctx->path,
				pretty_abi(uctx),
				pretty_type(uctx),
				subsystem.buffer,
				framework.buffer) < 0)
			return PERK_FILE_ERROR(dctx);
	} else {
		if (fprintf(fout,"%s-%s-%s-%s\n",
				pretty_abi(uctx),
				pretty_type(uctx),
				subsystem.buffer,
				framework.buffer) < 0)
			return PERK_FILE_ERROR(dctx);
	}

	return 0;
}
