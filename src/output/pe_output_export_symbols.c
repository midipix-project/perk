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
	if (cctx->fmtflags & PERK_PRETTY_YAML)
		return fputs("exports:\n",fout);
	else if (cctx->fmtflags & PERK_PRETTY_DLLTOOL)
		return fputs("EXPORTS\n",fout);
	return 0;
}

static int pretty_export_item(const struct pe_common_ctx * cctx, const char * name, FILE * fout)
{
	if (cctx->fmtflags & PERK_PRETTY_YAML)
		return fprintf(fout,"- %s\n",name);
	else
		return fprintf(fout,"%s\n",name);
}

int pe_output_export_symbols(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	m,
	FILE *				fout)
{
	char *				mark;
	uint32_t 			offset;
	uint32_t *			symrva;
	unsigned			i;
	const struct pe_common_ctx *	cctx = dctx->cctx;

	if (!m->h_edata)
		return 0;

	if (!fout)
		fout = stdout;

	if ((pretty_header(cctx,fout)) < 0)
		return PERK_FILE_ERROR(dctx);

	mark	= m->image.addr;
	offset	= m->h_edata->sh_virtual_addr - m->h_edata->sh_ptr_to_raw_data;
	symrva	= (uint32_t *)(mark + m->m_edata.eh_name_ptr_rva - offset);

	for (i=0; i<m->m_edata.eh_num_of_name_ptrs; i++)
		if ((pretty_export_item(
				cctx,
				&mark[symrva[i] - offset],
				fout)) < 0)
			return PERK_FILE_ERROR(dctx);

	return 0;
}
