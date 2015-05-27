#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <perk/perk.h>

int pe_output_export_symbols (const struct pe_image_meta * m, uint32_t flags, FILE * f)
{
	uint32_t 	offset;
	uint32_t *	symrva;
	int		i;

	if (!m->hedata) return 0;

	offset	= m->hedata->virtual_addr - m->hedata->ptr_to_raw_data;
	symrva	= (uint32_t *)((uintptr_t)m->image.addr + (m->edata.name_ptr_rva - offset));

	for (i=0; i<m->edata.num_of_name_ptrs; i++)
		fprintf(f,"%s\n",(char *)((uintptr_t)m->image.addr + symrva[i] - offset));

	return 0;
}
