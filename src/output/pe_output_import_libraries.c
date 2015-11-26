#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <perk/perk.h>
#include <perk/perk_output.h>
#include "perk_output_impl.h"

int pe_output_import_libraries(
	const struct pe_image_meta *	m,
	const struct pe_common_ctx *	cctx,
	FILE *				fout)
{
	FILE *	ftmp;
	int	i,j;

	if (!m->summary.num_of_implibs)
		return 0;

	if (!(fout = pe_output_prolog(cctx,fout,&ftmp)))
		return -1;

	for (i=0; i<m->summary.num_of_implibs; i++) {
		if (cctx->fmtflags & PERK_OUTPUT_IMPORT_SYMS)
			fprintf(fout,"%s:\n",m->idata[i].name);
		else
			fprintf(fout,"%s\n",m->idata[i].name);

		if (cctx->fmtflags & PERK_OUTPUT_IMPORT_SYMS)
			for (j=0; j<m->idata[i].count; j++)
				if (m->idata[i].items[j].name)
					fprintf(fout,"==> %s\n",
						m->idata[i].items[j].name);
	}

	return pe_output_epilog(0,ftmp);
}
