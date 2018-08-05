/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2017  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <perk/perk.h>
#include <perk/perk_output.h>
#include "perk_driver_impl.h"
#include "perk_dprintf_impl.h"

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

static ssize_t pe_version(struct pe_driver_ctx * dctx, int fdout)
{
	const struct pe_source_version * verinfo;
	const char * const * verclr;

	verinfo = pe_source_version();
	verclr  = isatty(fdout) ? pe_ver_color : pe_ver_plain;

	return pe_dprintf(
			fdout,vermsg,
			verclr[0],dctx->program,verclr[1],
			verclr[2],verinfo->major,verinfo->minor,
			verinfo->revision,verclr[3],
			verclr[4],verinfo->commit,verclr[5]);
}

static void pe_perform_unit_actions(
	const struct pe_driver_ctx *	dctx,
	struct pe_unit_ctx *		uctx)
{
	uint64_t flags = dctx->cctx->fmtflags;

	if (flags & PERK_OUTPUT_IMAGE_CATEGORY)
		pe_output_image_category(dctx,uctx->meta);

	if (flags & PERK_OUTPUT_IMAGE_SECTIONS)
		pe_output_image_sections(dctx,uctx->meta);

	if (flags & PERK_OUTPUT_IMAGE_SYMBOLS)
		pe_output_image_symbols(dctx,uctx->meta);

	if (flags & PERK_OUTPUT_IMAGE_STRINGS)
		pe_output_image_strings(dctx,uctx->meta);

	if (flags & PERK_OUTPUT_EXPORT_SYMS)
		pe_output_export_symbols(dctx,uctx->meta);

	if ((flags & PERK_OUTPUT_IMPORT_LIBS) || (flags & PERK_OUTPUT_IMPORT_SYMS))
		pe_output_import_libraries(dctx,uctx->meta);

	if ((flags & PERK_OUTPUT_MDSO_LIBS) || (flags & PERK_OUTPUT_MDSO_SYMS))
		pe_output_mdso_libraries(dctx,uctx->meta);
}

static int pe_exit(struct pe_driver_ctx * dctx, int ret)
{
	pe_output_error_vector(dctx);
	pe_free_driver_ctx(dctx);
	return ret;
}

int pe_main(int argc, char ** argv, char ** envp, const struct pe_fd_ctx * fdctx)
{
	int			ret;
	int			fdout;
	uint64_t		flags;
	struct pe_driver_ctx *	dctx;
	struct pe_unit_ctx *	uctx;
	const char **		unit;

	flags = PERK_DRIVER_FLAGS;
	fdout = fdctx ? fdctx->fdout : STDOUT_FILENO;

	if ((ret = pe_get_driver_ctx(argv,envp,flags,fdctx,&dctx)))
		return (ret == PERK_USAGE)
			? !--argc
			: PERK_ERROR;

	if (dctx->cctx->drvflags & PERK_DRIVER_VERSION)
		if ((pe_version(dctx,fdout)) < 0)
			return pe_exit(dctx,PERK_ERROR);

	for (unit=dctx->units; *unit; unit++) {
		if (!(pe_get_unit_ctx(dctx,*unit,&uctx))) {
			pe_perform_unit_actions(dctx,uctx);
			pe_free_unit_ctx(uctx);
		}
	}

	return pe_exit(dctx,dctx->errv[0] ? PERK_ERROR : PERK_OK);
}
