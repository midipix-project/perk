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
	const struct pe_image_meta *	m,
	const struct pe_common_ctx *	cctx,
	FILE *				fout)
{
	FILE *		ftmp;
	int		i,j;

	if (!m->summary.num_of_implibs)
		return 0;

	if (!(fout = pe_output_prolog(cctx,fout,&ftmp)))
		return -1;

	if ((pretty_header(cctx,fout)) < 0)
		return pe_output_epilog(-1,ftmp);

	for (i=0; i<m->summary.num_of_implibs; i++) {
		if ((pretty_implib_header(cctx,m->idata[i].name,fout)) < 0)
			return pe_output_epilog(-1,ftmp);

		if (cctx->fmtflags & PERK_OUTPUT_IMPORT_SYMS)
			for (j=0; j<m->idata[i].count; j++)
				if (m->idata[i].items[j].name)
					if ((pretty_implib_item(
							cctx,
							m->idata[i].items[j].name,
							fout)) < 0)
						return pe_output_epilog(-1,ftmp);

	}

	return pe_output_epilog(0,ftmp);
}
