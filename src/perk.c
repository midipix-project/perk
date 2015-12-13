#include <stdio.h>
#include <unistd.h>
#include <perk/perk.h>
#include <perk/perk_output.h>
#include "perk_version.h"

#ifndef PERK_DRIVER_FLAGS
#define PERK_DRIVER_FLAGS	PERK_DRIVER_VERBOSITY_ERRORS \
				| PERK_DRIVER_VERBOSITY_USAGE
#endif

static const char vermsg[] = "%s (git://midipix.org/perk): commit %s.\n";

static ssize_t perk_version(struct pe_driver_ctx * dctx)
{
	char	buf[512];
	size_t	len;

	if (dctx->cctx->ioctx->fdout >= 0) {
		len = sprintf(buf,vermsg,dctx->program,PERK_GIT_VERSION);
		return write(dctx->cctx->ioctx->fdout,buf,len);
	} else
		return fprintf(stdout,vermsg,dctx->program,PERK_GIT_VERSION);
}

static ssize_t perk_paragraph_break(struct pe_unit_ctx * uctx, int * fpara)
{
	if (*fpara) {
		*fpara = 0;

		if (uctx->cctx->ioctx->fdout >= 0)
			return write(uctx->cctx->ioctx->fdout,"\n",1);
		else
			return fputc('\n',stdout);
	} else
		return 0;
}

static void perk_perform_unit_actions(struct pe_unit_ctx * uctx)
{
	int      fpara = 0;
	uint64_t flags = uctx->cctx->fmtflags;

	if (flags & PERK_OUTPUT_EXPORT_SYMS) {
		uctx->status = pe_output_export_symbols(uctx->meta,uctx->cctx,0);
		uctx->nerrors += !!uctx->status;
		fpara += uctx->meta->summary.nexpsyms;
	}

	if ((flags & PERK_OUTPUT_IMPORT_LIBS) || (flags & PERK_OUTPUT_IMPORT_SYMS)) {
		perk_paragraph_break(uctx,&fpara);
		uctx->status = pe_output_import_libraries(uctx->meta,uctx->cctx,0);
		uctx->nerrors += !!uctx->status;
		fpara += (uctx->meta->summary.nimplibs > 0);
	}
}

static int perk_exit(struct pe_driver_ctx * dctx, int nerrors)
{
	pe_free_driver_ctx(dctx);
	return nerrors ? 2 : 0;
}

int perk_main(int argc, const char ** argv, const char ** envp)
{
	int			ret;
	struct pe_driver_ctx *	dctx;
	struct pe_unit_ctx *	uctx;
	const char **		unit;

	if ((ret = pe_get_driver_ctx(argv,envp,PERK_DRIVER_FLAGS,&dctx)))
		return (ret == PERK_USAGE) ? !--argc : 2;

	if (dctx->cctx->drvflags & PERK_DRIVER_VERSION)
		if ((perk_version(dctx)) < 0)
			return perk_exit(dctx,2);

	for (unit=dctx->units; *unit; unit++) {
		if (!(pe_get_unit_ctx(dctx,*unit,&uctx))) {
			perk_perform_unit_actions(uctx);
			ret += uctx->nerrors;
			pe_free_unit_ctx(uctx);
		}
	}

	return perk_exit(dctx,ret);
}

#ifndef PERK_IN_A_BOX

int main(int argc, const char ** argv, const char ** envp)
{
	return perk_main(argc,argv,envp);
}

#endif
