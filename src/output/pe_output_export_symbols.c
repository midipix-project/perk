#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <perk/perk.h>
#include <perk/perk_output.h>
#include "perk_output_impl.h"

static int pretty_header(const struct pe_common_ctx * cctx, FILE * fout)
{
	return (cctx->fmtflags & PERK_PRETTY_YAML)
		? fputs("exports:\n",fout)
		: 0;
}

static int pretty_export_item(const struct pe_common_ctx * cctx, const char * name, FILE * fout)
{
	if (cctx->fmtflags & PERK_PRETTY_YAML)
		return fprintf(fout,"- %s\n",name);
	else
		return fprintf(fout,"%s\n",name);
}

int pe_output_export_symbols(
	const struct pe_image_meta *	m,
	const struct pe_common_ctx *	cctx,
	FILE *				fout)
{
	FILE *		ftmp;
	uint32_t 	offset;
	uint32_t *	symrva;
	int		i;

	if (!m->hedata)
		return 0;

	if (!(fout = pe_output_prolog(cctx,fout,&ftmp)))
		return -1;

	if ((pretty_header(cctx,fout)) < 0)
		return pe_output_epilog(-1,ftmp);

	offset	= m->hedata->virtual_addr - m->hedata->ptr_to_raw_data;
	symrva	= (uint32_t *)((uintptr_t)m->image.addr + (m->edata.name_ptr_rva - offset));

	for (i=0; i<m->edata.num_of_name_ptrs; i++)
		if ((pretty_export_item(
				cctx,
				(char *)((uintptr_t)m->image.addr + symrva[i] - offset),
				fout)) < 0)
			return pe_output_epilog(-1,ftmp);

	return pe_output_epilog(0,ftmp);
}
