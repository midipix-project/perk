/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2016  Z. Gilboa                        */
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
#include "argv/argv.h"

/* package info */
static const struct pe_source_version pe_src_version = {
	PERK_TAG_VER_MAJOR,
	PERK_TAG_VER_MINOR,
	PERK_TAG_VER_PATCH,
	PERK_GIT_VERSION
};

struct pe_driver_ctx_alloc {
	struct argv_meta *		meta;
	struct pe_driver_ctx_impl	ctx;
	uint64_t			guard;
	const char *			units[];
};

static uint32_t pe_argv_flags(uint32_t flags)
{
	uint32_t ret = 0;

	if (flags & PERK_DRIVER_VERBOSITY_NONE)
		ret |= ARGV_VERBOSITY_NONE;

	if (flags & PERK_DRIVER_VERBOSITY_ERRORS)
		ret |= ARGV_VERBOSITY_ERRORS;

	if (flags & PERK_DRIVER_VERBOSITY_STATUS)
		ret |= ARGV_VERBOSITY_STATUS;

	return ret;
}

static int pe_driver_usage(
	const char *			program,
	const char *			arg,
	const struct argv_option *	options,
	struct argv_meta *		meta)
{
	char header[512];

	snprintf(header,sizeof(header),
		"Usage: %s [options] <file>...\n" "Options:\n",
		program);

	argv_usage(stdout,header,options,arg);
	argv_free(meta);

	return PERK_USAGE;
}

static struct pe_driver_ctx_impl * pe_driver_ctx_alloc(
	struct argv_meta *		meta,
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

	if (cctx)
		memcpy(&ictx->ctx.cctx,cctx,sizeof(*cctx));

	elements = sizeof(ictx->ctx.erribuf) / sizeof(*ictx->ctx.erribuf);

	ictx->ctx.errinfp  = &ictx->ctx.erriptr[0];
	ictx->ctx.erricap  = &ictx->ctx.erriptr[--elements];

	ictx->meta		= meta;
	ictx->ctx.cctx.lnkctx	= &ictx->ctx.lnkctx;
	ictx->ctx.cctx.srvctx	= &ictx->ctx.srvctx;
	ictx->ctx.cctx.ioctx	= &ictx->ctx.ioctx;

	for (entry=meta->entries,units=ictx->units; entry->fopt || entry->arg; entry++)
		if (!entry->fopt)
			*units++ = entry->arg;

	ictx->ctx.ctx.units = ictx->units;
	ictx->ctx.ctx.errv  = ictx->ctx.errinfp;
	return &ictx->ctx;
}

static int pe_get_driver_ctx_fail(struct argv_meta * meta)
{
	argv_free(meta);
	return -1;
}

int pe_get_driver_ctx(
	char **			argv,
	char **			envp,
	uint32_t		flags,
	struct pe_driver_ctx ** pctx)
{
	struct pe_driver_ctx_impl *	ctx;
	struct pe_common_ctx		cctx;
	const struct argv_option *	options;
	struct argv_meta *		meta;
	struct argv_entry *		entry;
	size_t				nunits;
	const char *			program;
	const char *			pretty;
	int				fdout;

	(void)envp;

	options = pe_default_options;

	if (!(meta = argv_get(argv,options,pe_argv_flags(flags))))
		return -1;

	pretty	= 0;
	nunits	= 0;
	fdout	= 0;
	program = argv_program_name(argv[0]);
	memset(&cctx,0,sizeof(cctx));
	cctx.drvflags = flags;

	if (!argv[1] && (flags & PERK_DRIVER_VERBOSITY_USAGE))
		return pe_driver_usage(program,0,options,meta);

	/* get options, count units */
	for (entry=meta->entries; entry->fopt || entry->arg; entry++) {
		if (entry->fopt) {
			switch (entry->tag) {
				case TAG_HELP:
					if (flags & PERK_DRIVER_VERBOSITY_USAGE)
						return pe_driver_usage(program,entry->arg,options,meta);

				case TAG_VERSION:
					cctx.drvflags |= PERK_DRIVER_VERSION;
					break;

				case TAG_OUTPUT:
					cctx.output = entry->arg;
					break;

				case TAG_PRETTY:
					pretty = entry->arg;
					break;

				case TAG_IMGTYPE:
					cctx.fmtflags |= PERK_OUTPUT_IMAGE_TYPE;
					break;

				case TAG_EXPSYMS:
					cctx.fmtflags |= PERK_OUTPUT_EXPORT_SYMS;
					break;

				case TAG_IMPLIBS:
					cctx.fmtflags |= PERK_OUTPUT_IMPORT_LIBS;
					break;

				case TAG_IMPSYMS:
					cctx.fmtflags |= PERK_OUTPUT_IMPORT_SYMS;
					break;
			}
		} else
			nunits++;
	}

	if (cctx.output && ((fdout = open(cctx.output,
			O_CREAT|O_TRUNC|O_WRONLY|O_NOCTTY|O_NOFOLLOW,
			S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) < 0))
		return pe_get_driver_ctx_fail(meta);

	if (pretty && !strcmp(pretty,"yaml"))
		cctx.fmtflags |= PERK_PRETTY_YAML;
	else if (pretty && !strcmp(pretty,"dlltool"))
		cctx.fmtflags |= PERK_PRETTY_DLLTOOL;

	if (!(ctx = pe_driver_ctx_alloc(meta,&cctx,nunits)) && cctx.output)
		close(fdout);

	if (!ctx)
		return pe_get_driver_ctx_fail(meta);

	ctx->ctx.program	= program;
	ctx->ctx.cctx		= &ctx->cctx;

	ctx->ioctx.fdout	= cctx.output ? fdout : -1;
	ctx->ioctx.fdin		= -1;
	ctx->ioctx.fderr	= -1;
	ctx->ioctx.fdlog	= -1;
	ctx->ioctx.fdsrc	= AT_FDCWD;
	ctx->ioctx.fddst	= AT_FDCWD;
	ctx->ioctx.fdtmp	= AT_FDCWD;

	*pctx = &ctx->ctx;
	return PERK_OK;
}

static void pe_driver_close_fds(struct pe_common_ctx * cctx)
{
	if (cctx->ioctx->status && cctx->output)
		unlinkat(cctx->ioctx->fddst,cctx->output,0);

	if (cctx->ioctx->fdout >= 0)
		close(cctx->ioctx->fdout);
}

static void pe_free_driver_ctx_impl(struct pe_driver_ctx_alloc * ictx)
{
	pe_driver_close_fds(&ictx->ctx.cctx);
	argv_free(ictx->meta);
	free(ictx);
}

void pe_free_driver_ctx(struct pe_driver_ctx * ctx)
{
	struct pe_driver_ctx_alloc *	ictx;
	uintptr_t			addr;

	if (ctx) {
		addr = (uintptr_t)ctx - offsetof(struct pe_driver_ctx_impl,ctx);
		addr = addr - offsetof(struct pe_driver_ctx_alloc,ctx);
		ictx = (struct pe_driver_ctx_alloc *)addr;
		pe_free_driver_ctx_impl(ictx);
	}
}

const struct pe_source_version * pe_source_version(void)
{
	return &pe_src_version;
}
