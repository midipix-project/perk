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
	unsigned			i,bias;
	uint32_t			roffset;
	struct pe_coff_symbol *		symtbl;
	char				buf[24];
	char *				mark;
	const char *			name;
	const char * 			dash = "";

	if (!fout)
		fout = stdout;

	if (dctx->cctx->fmtflags & PERK_PRETTY_YAML) {
		if (fputs("symbols:\n",fout) < 0)
			return PERK_FILE_ERROR(dctx);

		dash = "- ";
	}

	mark   = (char *)meta->image.addr;
	symtbl = (struct pe_coff_symbol *)(mark + meta->coff.ptr_to_sym_tbl);

	for (i=0,bias=0; i<meta->coff.num_of_syms; i++,bias=0) {
		if (symtbl[i].storage_class[0] == PE_IMAGE_SYM_CLASS_FILE)
			if (symtbl[i].num_of_aux_symbols[0])
				if (!symtbl[i+1].value[0])
					bias = 1;

		i += bias;

		if (!bias && (symtbl[i].storage_class[0] == PE_IMAGE_SYM_CLASS_FILE)
				&& symtbl[i].num_of_aux_symbols[0]) {
			memset(buf,0,sizeof(buf));
			memcpy(buf,symtbl[i+1].name,sizeof(*symtbl));
			name = buf;

		} else if (symtbl[i].name[0]) {
			memset(buf,0,sizeof(buf));
			memcpy(buf,symtbl[i].name,sizeof(symtbl->name));
			name = buf;

		} else if (!symtbl[i].name[1] && !symtbl[i].name[2] && !symtbl[i].name[3]) {
			roffset = pe_read_long(&symtbl[i].name[4]);
			name    = (roffset < meta->coff.size_of_string_tbl)
					? mark + meta->coff.ptr_to_string_tbl + roffset
					: 0;
		} else {
			name = 0;
		}

		i -= bias;
		i += symtbl[i].num_of_aux_symbols[0];


		if (name) {
			if (fprintf(fout,"%s%s\n",dash,name) < 0)
				return PERK_FILE_ERROR(dctx);
		}
	}

	return 0;
}
