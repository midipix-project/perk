#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <perk/perk.h>
#include "perk_output_impl.h"

int pe_output_export_symbols (const struct pe_image_meta * m, const struct pe_common_ctx * cctx, FILE * fout)
{
	FILE *		ftmp;
	uint32_t 	offset;
	uint32_t *	symrva;
	int		i;

	if (!m->hedata)
		return 0;

	if (!(fout = pe_output_prolog(cctx,fout,&ftmp)))
		return -1;

	offset	= m->hedata->virtual_addr - m->hedata->ptr_to_raw_data;
	symrva	= (uint32_t *)((uintptr_t)m->image.addr + (m->edata.name_ptr_rva - offset));

	for (i=0; i<m->edata.num_of_name_ptrs; i++)
		fprintf(fout,"%s\n",(char *)((uintptr_t)m->image.addr + symrva[i] - offset));

	return pe_output_epilog(0,ftmp);
}
