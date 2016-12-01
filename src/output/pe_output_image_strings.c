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

int pe_output_image_strings(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta,
	FILE *				fout)
{
	const char *	ch;
	const char *	mark;
	const char *	cap;
	const char * 	dash = "";

	if (!fout)
		fout = stdout;

	if (dctx->cctx->fmtflags & PERK_PRETTY_YAML) {
		if (fputs("strings:\n",fout) < 0)
			return PERK_FILE_ERROR(dctx);

		dash = "- ";
	}

	mark  = (char *)meta->image.addr;
	mark += meta->coff.cfh_ptr_to_str_tbl;

	cap   = mark + meta->coff.cfh_size_of_str_tbl;
	mark += sizeof(uint32_t);

	for (ch=mark; ch<cap; ) {
		if (fprintf(fout,"%s%s\n",dash,ch) < 0)
			return PERK_FILE_ERROR(dctx);

		ch += strlen(ch);

		for (; ch<cap && !*ch; )
			ch++;
	}

	return 0;
}
