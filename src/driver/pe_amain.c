/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2025  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <perk/perk.h>
#include <perk/perk_output.h>
#include "perk_driver_impl.h"
#include "perk_dprintf_impl.h"
#include "perk_ar_impl.h"

#ifndef PERK_DRIVER_FLAGS
#define PERK_DRIVER_FLAGS	PERK_DRIVER_VERBOSITY_ERRORS \
				| PERK_DRIVER_VERBOSITY_UTILITY \
				| PERK_DRIVER_AR_OBJECT_VECTOR
#endif

static const char vermsg[] = "%s%s%s (https://git.foss21.org/perk): "
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

static int pe_exit(struct pe_driver_ctx * dctx, int ret)
{
	pe_output_error_vector(dctx);
	pe_lib_free_driver_ctx(dctx);
	return ret;
}

int pe_main(char ** argv, char ** envp, const struct pe_fd_ctx * fdctx)
{
	int			ret;
	int			fdout;
	uint64_t		flags;
	struct pe_driver_ctx *	dctx;
	const char **		unit;
	const char *            posname;
	const char *            arname;
	const char **           arfiles;
	uint64_t                arflags;

	flags = PERK_DRIVER_FLAGS;
	fdout = fdctx ? fdctx->fdout : STDOUT_FILENO;

	if ((ret = pe_lib_get_driver_ctx(argv,envp,flags,fdctx,&dctx)))
		return (ret == PERK_USAGE)
			? !argv || !argv[0] || !argv[1]
			: PERK_ERROR;

	if (dctx->cctx->drvflags & PERK_DRIVER_VERSION)
		if ((pe_version(dctx,fdout)) < 0)
			return pe_exit(dctx,PERK_ERROR);

	switch (dctx->cctx->cmd) {
		case PERK_CMD_PERK:
			for (unit=dctx->units; *unit; unit++)
				pe_cmd_perk(dctx,*unit);
			break;

		case PERK_CMD_AR:
			arflags = dctx->cctx->drvflags;

			if (arflags & AR_POSNAME_MASK) {
				posname = dctx->units[0];
				arname  = posname ? dctx->units[1]  : 0;
				arfiles = arname  ? &dctx->units[2] : 0;
			} else {
				posname = 0;
				arname = dctx->units[0];
				arfiles = arname  ? &dctx->units[1] : 0;
			}

			pe_cmd_ar(dctx,arflags,posname,arname,arfiles);

			break;

		case PERK_CMD_NM:
			for (unit=dctx->units; *unit; unit++)
				pe_cmd_nm(dctx,*unit);
			break;

		case PERK_CMD_SIZE:
			for (unit=dctx->units; *unit; unit++)
				pe_cmd_size(dctx,*unit);
			break;

		default:
			break;
	}

	return pe_exit(dctx,dctx->errv[0] ? PERK_ERROR : PERK_OK);
}
