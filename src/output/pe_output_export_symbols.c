#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <perk/perk.h>

int pe_output_export_symbols (const struct pe_image_meta * m, const struct pe_common_ctx * cctx, FILE * f)
{
	uint32_t 	offset;
	uint32_t *	symrva;
	int		fdout;
	int		i;

	if (!m->hedata)
		return 0;
	else if (f)
		fdout = -1;
	else if (!cctx)
		return -1;
	else if (cctx->fdout < 0) {
		f = stdout;
		fdout = -1;
	} else if ((fdout = dup(cctx->fdout)) < 0)
		return -1;
	else if (!(f = fdopen(fdout,"a"))) {
		close(fdout);
		return -1;
	}

	offset	= m->hedata->virtual_addr - m->hedata->ptr_to_raw_data;
	symrva	= (uint32_t *)((uintptr_t)m->image.addr + (m->edata.name_ptr_rva - offset));

	for (i=0; i<m->edata.num_of_name_ptrs; i++)
		fprintf(f,"%s\n",(char *)((uintptr_t)m->image.addr + symrva[i] - offset));

	if (fdout >= 0)
		fclose(f);

	return 0;
}
