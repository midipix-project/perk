/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2021  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

#define ARGV_DRIVER

#include <perk/perk.h>
#include <perk/perk_output.h>
#include "perk_version.h"
#include "perk_driver_impl.h"
#include "perk_synopsis_impl.h"
#include "argv/argv.h"

/* package info */
static const struct pe_source_version pe_src_version = {
	PERK_TAG_VER_MAJOR,
	PERK_TAG_VER_MINOR,
	PERK_TAG_VER_PATCH,
	PERK_GIT_VERSION
};

/* perk command names */
static const char * const perk_cmd_name[PERK_CMD_CAP] = {
	[PERK_CMD_PERK]     = "perk",
	[PERK_CMD_AR]       = "ar",
};

/* perk command options */
static const struct argv_option * perk_cmd_options[PERK_CMD_CAP] = {
	[PERK_CMD_DEFAULT]  = pe_default_options,
	[PERK_CMD_PERK]     = pe_perk_options,
	[PERK_CMD_AR]       = pe_ar_options,
};

/* default fd context */
static const struct pe_fd_ctx pe_default_fdctx = {
	.fdin  = STDIN_FILENO,
	.fdout = STDOUT_FILENO,
	.fderr = STDERR_FILENO,
	.fdcwd = AT_FDCWD,
	.fddst = AT_FDCWD,
	.fdlog = (-1),
};

struct pe_driver_ctx_alloc {
	struct argv_meta *		meta;
	struct pe_driver_ctx_impl	ctx;
	uint64_t			guard;
	const char *			units[];
};

static uint32_t pe_argv_flags(uint32_t flags)
{
	uint32_t ret = ARGV_CLONE_VECTOR;

	if (flags & PERK_DRIVER_VERBOSITY_NONE)
		ret |= ARGV_VERBOSITY_NONE;

	if (flags & PERK_DRIVER_VERBOSITY_ERRORS)
		ret |= ARGV_VERBOSITY_ERRORS;

	if (flags & PERK_DRIVER_VERBOSITY_STATUS)
		ret |= ARGV_VERBOSITY_STATUS;

	return ret;
}

static int pe_driver_usage(
	int				fdout,
	const char *			program,
	const char *			arg,
	const struct argv_option **	optv,
	struct argv_meta *		meta,
	const char *                    cmdarg,
	enum pe_cmd                     cmd)
{
	char header [2048];
	char cmdname[128];

	snprintf(cmdname,sizeof(cmdname),"%s%s%s",
		program,
		cmdarg ? " --cmd="           : "",
		cmdarg ? perk_cmd_name[cmd]  : "");

	switch (cmd) {
		case PERK_CMD_AR:
			snprintf(header,sizeof(header),
				PERK_AR_CMD_SYNOPSIS,
				cmdname,cmdname,cmdname,cmdname,
				cmdname,cmdname,cmdname,cmdname,
				cmdname,cmdname,cmdname,cmdname,
				cmdname,cmdname,cmdname);
			break;

		case PERK_CMD_PERK:
			snprintf(header,sizeof(header),
				PERK_PERK_CMD_SYNOPSIS,
				cmdname,cmdname,cmdname);
			break;

		default:
			snprintf(header,sizeof(header),
				PERK_DEFAULT_CMD_SYNOPSIS,
				program,program,program,program);
			break;
	}

	argv_usage(fdout,header,optv,arg);
	argv_free(meta);

	return PERK_USAGE;
}

static struct pe_driver_ctx_impl * pe_driver_ctx_alloc(
	struct argv_meta *		meta,
	const struct pe_fd_ctx *	fdctx,
	const struct pe_common_ctx *	cctx,
	size_t				nunits)
{
	struct pe_driver_ctx_alloc *	ictx;
	size_t				size;
	struct argv_entry *		entry;
	const char **			units;
	int				elements;

	size =  sizeof(struct pe_driver_ctx_alloc);
	size += (nunits+1)*sizeof(const char *);

	if (!(ictx = calloc(1,size)))
		return 0;

	memcpy(&ictx->ctx.fdctx,fdctx,sizeof(*fdctx));
	memcpy(&ictx->ctx.cctx,cctx,sizeof(*cctx));

	elements = sizeof(ictx->ctx.erribuf) / sizeof(*ictx->ctx.erribuf);

	ictx->ctx.errinfp  = &ictx->ctx.erriptr[0];
	ictx->ctx.erricap  = &ictx->ctx.erriptr[--elements];

	ictx->meta = meta;

	for (entry=meta->entries,units=ictx->units; entry->fopt || entry->arg; entry++)
		if (!entry->fopt)
			*units++ = entry->arg;

	ictx->ctx.ctx.units = ictx->units;
	ictx->ctx.ctx.errv  = ictx->ctx.errinfp;
	return &ictx->ctx;
}

static int pe_cmd_from_program(const char * program)
{
	const char *	dot;
	const char *	hyphen;
	const char *	mark;

	dot    = strrchr(program,'.');
	hyphen = strrchr(program,'-');

	if (hyphen > dot)
		mark = ++hyphen;
	else if (dot > hyphen)
		mark = ++dot;
	else
		mark = program;

	if (!strcmp(mark,"perk")) {
		return PERK_CMD_PERK;

	} else if (!strcmp(mark,"ar")) {
		return PERK_CMD_AR;

	} else {
		return PERK_CMD_DEFAULT;
	}
}

static int pe_cctx_update(
	const char *                    program,
	const struct argv_option **     optv,
	struct argv_meta *              meta,
	const struct pe_fd_ctx *        fdctx,
	struct pe_common_ctx *          cctx,
	size_t *                        nunits,
	const char *			cmdarg)
{
	struct argv_entry *		entry;
	const char *			pretty;

	pretty = 0;

	/* get options, count units */
	for (entry=meta->entries; entry->fopt || entry->arg; entry++) {
		if (entry->fopt) {
			switch (entry->tag) {
				case TAG_HELP:
					return pe_driver_usage(
						fdctx->fdout,
						program,entry->arg,
						optv,0,cmdarg,cctx->cmd);
					break;

				case TAG_CMD:
					if (*nunits)
						return pe_driver_usage(
							fdctx->fderr,
							program,0,
							optv,0,cmdarg,cctx->cmd);

					cctx->cmd = pe_cmd_from_program(entry->arg);
					cmdarg    = entry->arg;
					break;

				case TAG_VERSION:
					cctx->drvflags |= PERK_DRIVER_VERSION;
					break;

				case TAG_PRETTY:
					pretty = entry->arg;
					break;

				case TAG_VERBOSE:
					cctx->fmtflags |= PERK_PRETTY_VERBOSE;
					break;

				case TAG_CATEGORY:
					cctx->fmtflags |= PERK_OUTPUT_IMAGE_CATEGORY;
					break;

				case TAG_SECTIONS:
					cctx->fmtflags |= PERK_OUTPUT_IMAGE_SECTIONS;
					break;

				case TAG_SYMBOLS:
					cctx->fmtflags |= PERK_OUTPUT_IMAGE_SYMBOLS;
					break;

				case TAG_STRINGS:
					cctx->fmtflags |= PERK_OUTPUT_IMAGE_STRINGS;
					break;

				case TAG_EXPSYMS:
					cctx->fmtflags |= PERK_OUTPUT_EXPORT_SYMS;
					break;

				case TAG_IMPLIBS:
					cctx->fmtflags |= PERK_OUTPUT_IMPORT_LIBS;
					break;

				case TAG_IMPSYMS:
					cctx->fmtflags |= PERK_OUTPUT_IMPORT_SYMS;
					break;

				case TAG_DSOLIBS:
					cctx->fmtflags |= PERK_OUTPUT_MDSO_LIBS;
					break;

				case TAG_DSOSYMS:
					cctx->fmtflags |= PERK_OUTPUT_MDSO_SYMS;
					break;

				case TAG_HDRDUMP:
					if (!entry->arg) {
						cctx->hdrdump = 0;
						cctx->hdrdump = ~cctx->hdrdump;
					} else if (!strcmp(entry->arg,"dos")) {
						cctx->hdrdump  = PERK_HDRDUMP_IMAGE_DOS_HEADER;
					} else if (!strcmp(entry->arg,"image.dos")) {
						cctx->hdrdump  = PERK_HDRDUMP_IMAGE_DOS_HEADER;
					} else if (!strcmp(entry->arg,"coff")) {
						cctx->hdrdump  = PERK_HDRDUMP_COFF_IMAGE_HEADER;
						cctx->hdrdump |= PERK_HDRDUMP_COFF_OBJECT_HEADER;
						cctx->hdrdump |= PERK_HDRDUMP_COFF_OPT_HEADER;
					} else if (!strcmp(entry->arg,"coff.image")) {
						cctx->hdrdump  = PERK_HDRDUMP_COFF_IMAGE_HEADER;
					} else if (!strcmp(entry->arg,"coff.obj")) {
						cctx->hdrdump  = PERK_HDRDUMP_COFF_OBJECT_HEADER;
					} else if (!strcmp(entry->arg,"coff.object")) {
						cctx->hdrdump  = PERK_HDRDUMP_COFF_OBJECT_HEADER;
					} else if (!strcmp(entry->arg,"coff.opt")) {
						cctx->hdrdump  = PERK_HDRDUMP_COFF_OPT_HEADER;
					} else if (!strcmp(entry->arg,"coff.optional")) {
						cctx->hdrdump  = PERK_HDRDUMP_COFF_OPT_HEADER;
					} else if (!strcmp(entry->arg,"sectbl")) {
						cctx->hdrdump  = PERK_HDRDUMP_SECTION_TABLE;
					} else if (!strcmp(entry->arg,"section.table")) {
						cctx->hdrdump  = PERK_HDRDUMP_SECTION_TABLE;
					} else if (!strcmp(entry->arg,"exphdr")) {
						cctx->hdrdump  = PERK_HDRDUMP_EXPORT_HEADER;
					} else if (!strcmp(entry->arg,"export.header")) {
						cctx->hdrdump  = PERK_HDRDUMP_EXPORT_HEADER;
					} else if (!strcmp(entry->arg,"imptbl")) {
						cctx->hdrdump  = PERK_HDRDUMP_IMPORT_TABLE;
					} else if (!strcmp(entry->arg,"import.table")) {
						cctx->hdrdump  = PERK_HDRDUMP_IMPORT_TABLE;
					}
					break;
			}
		} else {
			(*nunits)++;
		}
	}

	if (pretty && !strcmp(pretty,"yaml")) {
		cctx->fmtflags |= PERK_PRETTY_YAML;

	} else if (pretty && !strcmp(pretty,"posix")) {
		cctx->fmtflags |= PERK_PRETTY_POSIX;

	} else if (pretty && !strcmp(pretty,"hexdata")) {
		cctx->fmtflags |= PERK_PRETTY_HEXDATA;

	} else if (pretty && !strcmp(pretty,"dlltool")) {
		cctx->fmtflags |= PERK_PRETTY_DLLTOOL;
	}

	return 0;
}

static int pe_lib_get_driver_ctx_fail(struct argv_meta * meta)
{
	argv_free(meta);
	return -1;
}

int pe_lib_get_driver_ctx(
	char **				argv,
	char **				envp,
	uint32_t			flags,
	const struct pe_fd_ctx *	fdctx,
	struct pe_driver_ctx ** 	pctx)
{
	struct pe_driver_ctx_impl *	ctx;
	struct pe_common_ctx		cctx;
	const struct argv_option *	optv[PERK_OPTV_ELEMENTS];
	struct argv_meta *		meta;
	size_t				nunits;
	const char *			program;
	const char *                    cmdarg;
	char **				parg;
	char **				pargcap;
	char **				cmdargv;
	char *				cmdmark;
	char                            stckarg[64];
	struct argv_ctx			actx = {ARGV_VERBOSITY_NONE,
                                               ARGV_MODE_SCAN,
                                               0,0,0,0,0,0,0,0};


	(void)envp;

	/* fdctx */
	if (!fdctx)
		fdctx = &pe_default_fdctx;

	/* cctx */
	memset(&cctx,0,sizeof(cctx));

	program       = argv_program_name(argv[0]);\
	cctx.cmd      = pe_cmd_from_program(program);
	cctx.drvflags = flags;
	nunits	      = 0;
	cmdarg        = 0;

	/* missing arguments? */
	argv_optv_init(perk_cmd_options[cctx.cmd],optv);

	if (!argv[1] && (flags & PERK_DRIVER_VERBOSITY_UTILITY))
		return pe_driver_usage(
			fdctx->fderr,
			program,0,
			optv,0,0,cctx.cmd);

	/* historic ar usage (vector will be cloned, so stack var is fine) */
	if (cctx.cmd == PERK_CMD_AR) {
		if (argv && argv[0] && argv[1] && (argv[1][0] != '-')) {
			if (strlen(argv[1]) < (sizeof(stckarg) - 1)) {
				stckarg[0] = '-';
				strcpy(&stckarg[1],argv[1]);
				argv[1] = stckarg;
			}
		}
	}

	/* initial argv scan: ... --cmd=xxx ... */
	argv_scan(argv,optv,&actx,0);

	/* position of last base perk argument */
	if (actx.erridx && actx.unitidx)
		pargcap = &argv[actx.unitidx];

	else if (actx.erridx)
		pargcap = &argv[actx.erridx];

	else
		for (pargcap=argv; *pargcap; pargcap++)
			(void)0;

	/* scan for --cmd */
	for (parg=argv, cmdargv=0; (parg<pargcap) && !cmdargv; parg++) {
		if (!strcmp(*parg,"--cmd") && parg[1]) {
				cmdargv = &parg[2];
				cmdmark = parg[2];
				cmdarg  = parg[1];
		} else if (!strncmp(*parg,"--cmd=",6)) {
				cmdargv = &parg[1];
				cmdmark = parg[1];
				cmdarg  = &parg[0][6];
		}
	}

	/* invalid perk arguments? */
	if (!actx.erridx) {
		(void)0;

	} else if (&argv[actx.erridx] >= cmdargv) {
		(void)0;

	} else {
		if (flags & PERK_DRIVER_VERBOSITY_ERRORS)
			argv_get(
				argv,optv,
				ARGV_VERBOSITY_ERRORS,
				fdctx->fderr);
		return -1;
	}

	/* process argv entries preceding --cmd */
	if (cmdargv) {
		*cmdargv = 0;

		if (!(meta = argv_get(
				argv,optv,
				pe_argv_flags(flags),
				fdctx->fderr)))
			return -1;

		if (pe_cctx_update(
				program,optv,meta,
				fdctx,&cctx,&nunits,
				cmdarg)) {
			argv_free(meta);
			return -1;
		}

		argv_free(meta);

		*cmdargv-- = cmdmark;
		*cmdargv   = argv[0];
		argv       = cmdargv;
	}

	/* set option vector by command */
	if (cctx.cmd == PERK_CMD_DEFAULT) {
		argv_optv_init(pe_default_options,optv);

	} else if (cctx.cmd == PERK_CMD_PERK) {
		argv_optv_init(pe_perk_options,optv);

	} else if (cctx.cmd == PERK_CMD_AR) {
		argv_optv_init(pe_ar_options,optv);
	}

	/* process the selected tool's command-line arguments */
	if (!(meta = argv_get(
			argv,optv,
			pe_argv_flags(flags),
			fdctx->fderr)))
		return -1;

	if (pe_cctx_update(
			program,optv,meta,
			fdctx,&cctx,&nunits,
			cmdarg)) {
		argv_free(meta);
		return -1;
	}

	/* utility mode and no action to take? */
	if (cctx.drvflags & PERK_DRIVER_VERBOSITY_UTILITY)
		if (!nunits && !(cctx.drvflags & PERK_DRIVER_VERSION))
			return pe_driver_usage(
				fdctx->fdout,
				program,0,optv,meta,
				cmdarg,cctx.cmd);

	/* context allocation */
	if (!(ctx = pe_driver_ctx_alloc(meta,fdctx,&cctx,nunits)))
		return pe_lib_get_driver_ctx_fail(meta);

	ctx->ctx.program	= program;
	ctx->ctx.cctx		= &ctx->cctx;

	*pctx = &ctx->ctx;
	return PERK_OK;
}

static void pe_lib_free_driver_ctx_impl(struct pe_driver_ctx_alloc * ictx)
{
	argv_free(ictx->meta);
	free(ictx);
}

void pe_lib_free_driver_ctx(struct pe_driver_ctx * ctx)
{
	struct pe_driver_ctx_alloc *	ictx;
	uintptr_t			addr;

	if (ctx) {
		addr = (uintptr_t)ctx - offsetof(struct pe_driver_ctx_impl,ctx);
		addr = addr - offsetof(struct pe_driver_ctx_alloc,ctx);
		ictx = (struct pe_driver_ctx_alloc *)addr;
		pe_lib_free_driver_ctx_impl(ictx);
	}
}

const struct pe_source_version * pe_source_version(void)
{
	return &pe_src_version;
}

int pe_lib_get_driver_fdctx(
	const struct pe_driver_ctx *	dctx,
	struct pe_fd_ctx *		fdctx)
{
	struct pe_driver_ctx_impl *	ictx;

	ictx = pe_get_driver_ictx(dctx);

	fdctx->fdin  = ictx->fdctx.fdin;
	fdctx->fdout = ictx->fdctx.fdout;
	fdctx->fderr = ictx->fdctx.fderr;
	fdctx->fdlog = ictx->fdctx.fdlog;
	fdctx->fdcwd = ictx->fdctx.fdcwd;
	fdctx->fddst = ictx->fdctx.fddst;

	return 0;
}

int pe_lib_set_driver_fdctx(
	struct pe_driver_ctx *	dctx,
	const struct pe_fd_ctx *	fdctx)
{
	struct pe_driver_ctx_impl *	ictx;

	ictx = pe_get_driver_ictx(dctx);

	ictx->fdctx.fdin  = fdctx->fdin;
	ictx->fdctx.fdout = fdctx->fdout;
	ictx->fdctx.fderr = fdctx->fderr;
	ictx->fdctx.fdlog = fdctx->fdlog;
	ictx->fdctx.fdcwd = fdctx->fdcwd;
	ictx->fdctx.fddst = fdctx->fddst;

	return 0;
}
