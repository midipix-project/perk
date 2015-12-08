#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

#define ARGV_DRIVER

#include <perk/perk.h>
#include <perk/perk_output.h>
#include "perk_driver_impl.h"
#include "perk_impl.h"
#include "argv/argv.h"

extern const struct argv_option pe_default_options[];

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

static struct pe_driver_ctx_impl * pe_driver_ctx_alloc(struct argv_meta * meta, size_t nunits)
{
	struct pe_driver_ctx_alloc *	ictx;
	size_t				size;
	struct argv_entry *		entry;
	const char **			units;

	size =  sizeof(struct pe_driver_ctx_alloc);
	size += (nunits+1)*sizeof(const char *);

	if (!(ictx = calloc(size,1)))
		return 0;

	ictx->meta		= meta;
	ictx->ctx.cctx.symctx	= &ictx->ctx.symctx;
	ictx->ctx.cctx.outctx	= &ictx->ctx.outctx;
	ictx->ctx.cctx.lnkctx	= &ictx->ctx.lnkctx;
	ictx->ctx.cctx.srvctx	= &ictx->ctx.srvctx;
	ictx->ctx.cctx.ioctx	= &ictx->ctx.ioctx;

	for (entry=meta->entries,units=ictx->units; entry->fopt || entry->arg; entry++)
		if (!entry->fopt)
			*units++ = entry->arg;

	ictx->ctx.ctx.units = ictx->units;
	return &ictx->ctx;
}

int pe_get_driver_ctx_fail(struct argv_meta * meta)
{
	argv_free(meta);
	return -1;
}

int pe_get_driver_ctx(
	const char **		argv,
	const char **		envp,
	uint32_t		flags,
	struct pe_driver_ctx ** pctx)
{
	struct pe_driver_ctx_impl *	ctx;
	const struct argv_option *	options;
	struct argv_meta *		meta;
	struct argv_entry *		entry;
	size_t				nunits;
	uint64_t			dflags;
	uint64_t			fflags;
	const char *			program;
	const char *			output;
	const char *			pretty;
	int				fdout;

	options = pe_default_options;

	if (!(meta = argv_get(argv,options,pe_argv_flags(flags))))
		return -1;

	dflags	= 0;
	fflags	= 0;
	output	= 0;
	pretty	= 0;
	nunits	= 0;
	fdout	= 0;
	program = argv_program_name(argv[0]);

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
					dflags |= PERK_DRIVER_VERSION;
					break;

				case TAG_OUTPUT:
					output = entry->arg;
					break;

				case TAG_PRETTY:
					pretty = entry->arg;
					break;

				case TAG_EXPSYMS:
					fflags |= PERK_OUTPUT_EXPORT_SYMS;
					break;

				case TAG_IMPLIBS:
					fflags |= PERK_OUTPUT_IMPORT_LIBS;
					break;

				case TAG_IMPSYMS:
					fflags |= PERK_OUTPUT_IMPORT_SYMS;
					break;
			}
		} else
			nunits++;
	}

	if (output && ((fdout = open(output,
			O_CREAT|O_TRUNC|O_WRONLY|O_NOCTTY|O_NOFOLLOW,
			S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) < 0))
		return pe_get_driver_ctx_fail(meta);

	if (!(ctx = pe_driver_ctx_alloc(meta,nunits)) && output)
		close(fdout);

	if (!ctx)
		return pe_get_driver_ctx_fail(meta);

	if (pretty && !strcmp(pretty,"yaml"))
		fflags |= PERK_PRETTY_YAML;

	ctx->ctx.program	= program;
	ctx->cctx.drvflags	= dflags;
	ctx->cctx.fmtflags	= fflags;
	ctx->cctx.output	= output;
	ctx->ioctx.fdout	= output ? fdout : -1;

	ctx->ioctx.fdin		= -1;
	ctx->ioctx.fderr	= -1;
	ctx->ioctx.fdlog	= -1;
	ctx->ioctx.fdsrc	= AT_FDCWD;
	ctx->ioctx.fddst	= AT_FDCWD;
	ctx->ioctx.fdtmp	= AT_FDCWD;

	ctx->ctx.cctx		= &ctx->cctx;

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
	if (ictx->ctx.symctx.append)
		free(ictx->ctx.symctx.append);

	if (ictx->ctx.symctx.exclude)
		free(ictx->ctx.symctx.exclude);

	pe_driver_close_fds(&ictx->ctx.cctx);
	argv_free(ictx->meta);
	free(ictx);
}

void pe_free_driver_ctx(struct pe_driver_ctx * ctx)
{
	struct pe_driver_ctx_alloc *	ictx;
	uintptr_t			addr;

	if (ctx) {
		addr = (uintptr_t)ctx - offsetof(struct pe_driver_ctx_alloc,ctx);
		addr = addr - offsetof(struct pe_driver_ctx_impl,ctx);
		ictx = (struct pe_driver_ctx_alloc *)addr;
		pe_free_driver_ctx_impl(ictx);
	}
}
