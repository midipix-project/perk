/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2025  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdio.h>
#include <string.h>

#include <perk/perk.h>
#include <perk/perk_structs.h>
#include <perk/perk_output.h>
#include "perk_reader_impl.h"
#include "perk_driver_impl.h"
#include "perk_dprintf_impl.h"
#include "perk_errinfo_impl.h"

static int pe_output_symbol_names(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta,
	int                             fdout)
{
	struct pe_meta_coff_symbol *	symrec;

	for (symrec=meta->m_symtbl; symrec->cs_name; symrec++)
		if (pe_dprintf(fdout,"%s\n",symrec->cs_name) < 0)
			return PERK_FILE_ERROR(dctx);

	return 0;
}

static int pe_output_symbol_names_yaml(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta,
	int                             fdout)
{
	struct pe_meta_coff_symbol *	symrec;

	if (pe_dprintf(fdout,"  - Symbols:\n") < 0)
		return PERK_FILE_ERROR(dctx);

	for (symrec=meta->m_symtbl; symrec->cs_name; symrec++)
		if (pe_dprintf(fdout,"    - [ symbol: %s ]\n",symrec->cs_name) < 0)
			return PERK_FILE_ERROR(dctx);

	return 0;
}

int pe_output_image_symbols(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta)
{
	int fdout = pe_driver_fdout(dctx);

	if (dctx->cctx->fmtflags & PERK_PRETTY_YAML) {
		if (pe_output_symbol_names_yaml(dctx,meta,fdout) < 0)
			return PERK_NESTED_ERROR(dctx);

	} else {
		if (pe_output_symbol_names(dctx,meta,fdout) < 0)
			return PERK_NESTED_ERROR(dctx);
	}

	return 0;
}
