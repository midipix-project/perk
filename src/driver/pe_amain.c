/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2016  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <perk/perk.h>
#include <perk/perk_output.h>
#include "perk_driver_impl.h"

#ifndef PERK_DRIVER_FLAGS
#define PERK_DRIVER_FLAGS	PERK_DRIVER_VERBOSITY_ERRORS \
				| PERK_DRIVER_VERBOSITY_USAGE
#endif

static const char vermsg[] = "%s%s%s (git://midipix.org/perk): "
			     "version %s%d.%d.%d%s.\n"
			     "[commit reference: %s%s%s]\n";

static const char * const pe_ver_color[6] = {
		"\x1b[1m\x1b[35m","\x1b[0m",
		"\x1b[1m\x1b[32m","\x1b[0m",
		"\x1b[1m\x1b[34m","\x1b[0m"
};

static const char * const pe_ver_plain[6] = {
		"","",
		"","",
		"",""
};

static ssize_t pe_version(struct pe_driver_ctx * dctx)
{
	const struct pe_source_version * verinfo;
	const char * const * verclr;

	verinfo = pe_source_version();
	verclr  = isatty(STDOUT_FILENO) ? pe_ver_color : pe_ver_plain;

	return fprintf(stdout,vermsg,
			verclr[0],dctx->program,verclr[1],
			verclr[2],verinfo->major,verinfo->minor,
			verinfo->revision,verclr[3],
			verclr[4],verinfo->commit,verclr[5]);
}

static ssize_t pe_paragraph_break(struct pe_unit_ctx * uctx, int * fpara)
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

static void pe_perform_unit_actions(
	const struct pe_driver_ctx *	dctx,
	struct pe_unit_ctx *		uctx)
{
	int      fpara = 0;
	uint64_t flags = uctx->cctx->fmtflags;

	if (flags & PERK_OUTPUT_EXPORT_SYMS) {
		uctx->status = pe_output_export_symbols(dctx,uctx->meta,0);
		uctx->nerrors += !!uctx->status;
		fpara += uctx->meta->summary.nexpsyms;
	}

	if ((flags & PERK_OUTPUT_IMPORT_LIBS) || (flags & PERK_OUTPUT_IMPORT_SYMS)) {
		pe_paragraph_break(uctx,&fpara);
		uctx->status = pe_output_import_libraries(dctx,uctx->meta,0);
		uctx->nerrors += !!uctx->status;
		fpara += (uctx->meta->summary.nimplibs > 0);
	}
}

static int pe_exit(struct pe_driver_ctx * dctx, int nerrors)
{
	pe_free_driver_ctx(dctx);
	return nerrors ? 2 : 0;
}

int pe_main(int argc, char ** argv, char ** envp)
{
	int			ret;
	struct pe_driver_ctx *	dctx;
	struct pe_unit_ctx *	uctx;
	const char **		unit;

	if ((ret = pe_get_driver_ctx(argv,envp,PERK_DRIVER_FLAGS,&dctx)))
		return (ret == PERK_USAGE) ? !--argc : 2;

	if (dctx->cctx->drvflags & PERK_DRIVER_VERSION)
		if ((pe_version(dctx)) < 0)
			return pe_exit(dctx,2);

	for (unit=dctx->units; *unit; unit++) {
		if (!(pe_get_unit_ctx(dctx,*unit,&uctx))) {
			pe_perform_unit_actions(dctx,uctx);
			ret += uctx->nerrors;
			pe_free_unit_ctx(uctx);
		}
	}

	return pe_exit(dctx,ret);
}
