/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2025  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdio.h>

#include <perk/perk.h>
#include <perk/perk_output.h>
#include "perk_driver_impl.h"
#include "perk_dprintf_impl.h"
#include "perk_errinfo_impl.h"

int pe_output_pecoff_category(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta)
{
	int				fdout;
	struct pe_info_string		abi;
	struct pe_info_string		subtype;
	struct pe_info_string		subsystem;
	struct pe_info_string		framework;

	fdout = pe_driver_fdout(dctx);

	pe_info_get_image_abi      (meta,&abi);
	pe_info_get_image_subtype  (meta,&subtype);
	pe_info_get_image_subsystem(meta,&subsystem);
	pe_info_get_image_framework(meta,&framework);

	if (dctx->cctx->fmtflags & PERK_PRETTY_YAML) {
		if (pe_dprintf(
				fdout,
				"category:\n- %s:\n- %s:\n- %s:\n- %s:\n",
				abi.buffer,
				subtype.buffer,
				subsystem.buffer,
				framework.buffer) < 0)
			return PERK_FILE_ERROR(dctx);
	} else {
		if (pe_dprintf(
				fdout,
				"%s-%s-%s-%s\n",
				abi.buffer,
				subtype.buffer,
				subsystem.buffer,
				framework.buffer) < 0)
			return PERK_FILE_ERROR(dctx);
	}

	return 0;
}
