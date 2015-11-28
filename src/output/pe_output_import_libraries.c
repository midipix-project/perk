#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <perk/perk.h>
#include <perk/perk_output.h>
#include "perk_output_impl.h"

enum impsym_prefix_tag {
	IMPSYM_PREFIX_DEFAULT,
	IMPSYM_PREFIX_YAML
};

static const char * const impsym_prefix_arr[] = {
	"--> ",
	"- "
};

static const char * pretty_impsym_prefix(const struct pe_common_ctx * cctx)
{
	if (cctx->fmtflags & PERK_PRETTY_YAML)
		return impsym_prefix_arr[IMPSYM_PREFIX_YAML];
	else
		return impsym_prefix_arr[IMPSYM_PREFIX_DEFAULT];
}

int pe_output_import_libraries(
	const struct pe_image_meta *	m,
	const struct pe_common_ctx *	cctx,
	FILE *				fout)
{
	FILE *		ftmp;
	int		i,j;
	const char *	impsym_prefix;

	if (!m->summary.num_of_implibs)
		return 0;

	if (!(fout = pe_output_prolog(cctx,fout,&ftmp)))
		return -1;

	impsym_prefix = pretty_impsym_prefix(cctx);

	for (i=0; i<m->summary.num_of_implibs; i++) {
		if (cctx->fmtflags & PERK_OUTPUT_IMPORT_SYMS)
			fprintf(fout,"%s:\n",m->idata[i].name);
		else
			fprintf(fout,"%s\n",m->idata[i].name);

		if (cctx->fmtflags & PERK_OUTPUT_IMPORT_SYMS)
			for (j=0; j<m->idata[i].count; j++)
				if (m->idata[i].items[j].name)
					fprintf(fout,"%s%s\n",
						impsym_prefix,m->idata[i].items[j].name);
	}

	return pe_output_epilog(0,ftmp);
}
