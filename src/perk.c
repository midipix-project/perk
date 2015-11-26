#include <stdio.h>
#include <perk/perk.h>
#include <perk/perk_output.h>

#ifndef PERK_DRIVER_FLAGS
#define PERK_DRIVER_FLAGS	PERK_DRIVER_VERBOSITY_ERRORS|PERK_DRIVER_VERBOSITY_USAGE
#endif

static void perk_perform_unit_actions(struct pe_unit_ctx * uctx)
{
	uint64_t flags = uctx->cctx.fmtflags;

	if (flags & PERK_OUTPUT_EXPORT_SYMS)
		uctx->cctx.status = pe_output_export_symbols(uctx->meta,&uctx->cctx,0);
}

static int perk_exit(struct pe_driver_ctx * dctx, int status)
{
	pe_free_driver_ctx(dctx);
	return (status < 0) ? 2 : status;
}

static int perk_main(int argc, const char ** argv, const char ** envp)
{
	int			ret;
	struct pe_driver_ctx *	dctx;
	struct pe_unit_ctx *	uctx;
	const char **		unit;

	if ((ret = pe_get_driver_ctx(argv,envp,PERK_DRIVER_FLAGS,&dctx)))
		return (ret == PERK_USAGE) ? 0 : 2;

	for (unit=dctx->units; *unit; unit++) {
		if (!(pe_get_unit_ctx(dctx,*unit,&uctx))) {
			perk_perform_unit_actions(uctx);
			ret = uctx->cctx.status ? uctx->cctx.status : ret;
			pe_free_unit_ctx(uctx);
		}
	}

	return perk_exit(dctx,ret);
}

int main(int argc, const char ** argv, const char ** envp)
{
	return perk_main(argc,argv,envp);
}
