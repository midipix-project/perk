/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2017  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <perk/perk.h>
#include <perk/perk_output.h>
#include "perk_errinfo_impl.h"

static int pretty_header(const struct pe_common_ctx * cctx, FILE * fout)
{
	return (cctx->fmtflags & PERK_PRETTY_YAML)
		? fputs("imports:\n",fout)
		: 0;
}

static int pretty_implib_header(const struct pe_common_ctx * cctx, const char * name, FILE * fout)
{
	if ((cctx->fmtflags & PERK_PRETTY_YAML) && (cctx->fmtflags & PERK_OUTPUT_IMPORT_SYMS))
		return fprintf(fout,"  %s:\n",name);
	else if (cctx->fmtflags & PERK_PRETTY_YAML)
		return fprintf(fout,"- %s:\n",name);
	else if (cctx->fmtflags & PERK_OUTPUT_IMPORT_SYMS)
		return fprintf(fout,"%s:\n",name);
	else
		return fprintf(fout,"%s\n",name);
}

static int pretty_implib_item(const struct pe_common_ctx * cctx, const char * name, FILE * fout)
{
	if (cctx->fmtflags & PERK_PRETTY_YAML)
		return fprintf(fout,"  - %s\n",name);
	else
		return fprintf(fout,"%s\n",name);
}

int pe_output_import_libraries(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	m,
	FILE *				fout)
{
	int				i;
	unsigned			j;
	const struct pe_common_ctx *	cctx = dctx->cctx;

	if (!m->mstats.nimplibs)
		return 0;

	if (!fout)
		fout = stdout;

	if ((pretty_header(cctx,fout)) < 0)
		return PERK_FILE_ERROR(dctx);

	for (i=0; i<m->mstats.nimplibs; i++) {
		if ((pretty_implib_header(cctx,m->idata[i].ih_name,fout)) < 0)
			return PERK_FILE_ERROR(dctx);

		if (cctx->fmtflags & PERK_OUTPUT_IMPORT_SYMS)
			for (j=0; j<m->idata[i].ih_count; j++)
				if (m->idata[i].ih_items[j].ii_name)
					if ((pretty_implib_item(
							cctx,
							m->idata[i].ih_items[j].ii_name,
							fout)) < 0)
						return PERK_FILE_ERROR(dctx);
	}

	return 0;
}
