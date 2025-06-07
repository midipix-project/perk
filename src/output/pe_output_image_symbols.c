/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2021  SysDeer Technologies, LLC        */
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

int pe_output_image_symbols(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta)
{
	unsigned			i;
	int				fdout;
	char *				mark;
	struct pe_raw_coff_symbol *	symtbl;
	struct pe_meta_coff_symbol	symrec;
	const char * 			dash = "";

	fdout = pe_driver_fdout(dctx);

	if (dctx->cctx->fmtflags & PERK_PRETTY_YAML) {
		if (pe_dprintf(fdout,"symbols:\n") < 0)
			return PERK_FILE_ERROR(dctx);

		dash = "- ";
	}

	mark   = (char *)meta->r_image.map_addr;
	symtbl = (struct pe_raw_coff_symbol *)(mark + meta->m_coff.cfh_ptr_to_sym_tbl);

	for (i=0; i<meta->m_coff.cfh_num_of_syms; i++) {
		pe_read_coff_symbol(
			&symtbl[i],&symrec,
			&meta->m_coff,meta->r_image.map_addr);

		if (pe_dprintf(fdout,"%s%s\n",dash,symrec.cs_name) < 0)
			return PERK_FILE_ERROR(dctx);

		i += symtbl[i].cs_num_of_aux_recs[0];
	}

	return 0;
}
