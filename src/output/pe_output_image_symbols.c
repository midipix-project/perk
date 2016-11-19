/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2016  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdio.h>
#include <string.h>

#include <perk/perk.h>
#include <perk/perk_output.h>
#include "perk_reader_impl.h"
#include "perk_errinfo_impl.h"

int pe_output_image_symbols(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta,
	FILE *				fout)
{
	unsigned			i;
	char *				mark;
	struct pe_raw_coff_symbol *	symtbl;
	struct pe_meta_coff_symbol	symrec;
	const char * 			dash = "";

	if (!fout)
		fout = stdout;

	if (dctx->cctx->fmtflags & PERK_PRETTY_YAML) {
		if (fputs("symbols:\n",fout) < 0)
			return PERK_FILE_ERROR(dctx);

		dash = "- ";
	}

	mark   = (char *)meta->image.addr;
	symtbl = (struct pe_raw_coff_symbol *)(mark + meta->coff.ptr_to_sym_tbl);

	for (i=0; i<meta->coff.num_of_syms; i++) {
		pe_read_coff_symbol(
			&symtbl[i],&symrec,
			&meta->coff,meta->image.addr);

		if (fprintf(fout,"%s%s\n",
				dash,
				symrec.long_name
					? symrec.long_name
					: symrec.name) < 0)
			return PERK_FILE_ERROR(dctx);

		i += symtbl[i].num_of_aux_symbols[0];
	}

	return 0;
}
