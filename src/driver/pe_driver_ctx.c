#include <stdint.h>
#include <fcntl.h>
#include <perk/perk.h>
#include <perk/perk_output.h>
#include "argv/argv.h"

enum app_tags {
	TAG_HELP,
	TAG_VERSION,
	TAG_OUTPUT,
	TAG_EXPSYMS,
	TAG_IMPLIBS,
	TAG_IMPSYMS,
};

static const struct argv_option options[] = {
	{"version",	'v',TAG_VERSION,ARGV_OPTARG_NONE,	0,0,
								"show version information"},

	{"help",	'h',TAG_HELP,	ARGV_OPTARG_OPTIONAL,	"short|long",0,
								"show usage information "
								"[listing %s options only]"},

	{"output",	'o',TAG_OUTPUT,	ARGV_OPTARG_REQUIRED,	0,"<file>",
								"write output to %s"},

	{"expsyms",	'e',TAG_EXPSYMS,ARGV_OPTARG_NONE,	0,0,
								"print exported symbols" },

	{"implibs",	'i',TAG_IMPLIBS,ARGV_OPTARG_NONE,	0,0,
								"list direct dependency libraries"},

	{"impsyms",	'I',TAG_IMPSYMS,ARGV_OPTARG_NONE,	0,0,
								"list direct dependency libraries "
								"along with required symbols"},
	{0}
};

struct pe_driver_ctx_impl {
	struct argv_meta *	meta;
	struct pe_symbol_ctx	symctx;
	struct pe_output_ctx	outctx;
	struct pe_linker_ctx	lnkctx;
	struct pe_server_ctx	srvctx;
	struct pe_driver_ctx	ctx;
	uint64_t		guard;
	const char *		units[];
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

static struct pe_driver_ctx * pe_driver_ctx_alloc(struct argv_meta * meta, size_t nunits)
{
	struct pe_driver_ctx_impl *	ictx;
	size_t				size;
	struct argv_entry *		entry;
	const char **			units;

	size =  sizeof(struct pe_driver_ctx_impl);
	size += (nunits+1)*sizeof(const char *);

	if (!(ictx = calloc(size,1)))
		return 0;

	ictx->meta		= meta;
	ictx->ctx.cctx.symctx	= &ictx->symctx;
	ictx->ctx.cctx.outctx	= &ictx->outctx;
	ictx->ctx.cctx.lnkctx	= &ictx->lnkctx;
	ictx->ctx.cctx.srvctx	= &ictx->srvctx;

	for (entry=meta->entries,units=ictx->units; entry->fopt || entry->arg; entry++)
		if (!entry->fopt)
			*units++ = entry->arg;

	ictx->ctx.units = ictx->units;
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
	struct pe_driver_ctx *	ctx;
	struct argv_meta *	meta;
	struct argv_entry *	entry;
	size_t			nunits;
	uint64_t		dflags;
	uint64_t		fflags;
	const char *		program;
	const char *		output;
	int			fdout;
	char			header[512];

	if (!(meta = argv_get(argv,options,pe_argv_flags(flags))))
		return -1;

	dflags	= 0;
	fflags	= 0;
	output	= 0;
	nunits	= 0;
	program = argv_program_name(argv[0]);

	/* get options, count units */
	for (entry=meta->entries; entry->fopt || entry->arg; entry++) {
		if (entry->fopt) {
			switch (entry->tag) {
				case TAG_HELP:
					if (flags & PERK_DRIVER_VERBOSITY_USAGE) {
						snprintf(header,sizeof(header),
							"Usage: %s [options] <file>...\n" "Options:\n",
							program);

						argv_usage(stdout,header,options,entry->arg);
						argv_free(meta);
						return PERK_USAGE;
					}

				case TAG_VERSION:
					dflags |= PERK_DRIVER_VERSION;
					break;

				case TAG_OUTPUT:
					output = entry->arg;
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

	ctx->program		= program;
	ctx->cctx.drvflags	= dflags;
	ctx->cctx.fmtflags	= fflags;
	ctx->cctx.output	= output;
	ctx->cctx.fdout		= output ? fdout : -1;

	ctx->cctx.fdin		= -1;
	ctx->cctx.fderr		= -1;
	ctx->cctx.fdlog		= -1;
	ctx->cctx.fdsrc		= AT_FDCWD;
	ctx->cctx.fddst		= AT_FDCWD;
	ctx->cctx.fdtmp		= AT_FDCWD;

	*pctx = ctx;
	return PERK_OK;
}

static void pe_driver_close_fds(struct pe_common_ctx * cctx)
{
	if (cctx->status && cctx->output)
		unlinkat(cctx->fddst,cctx->output,0);

	if (cctx->fdout >= 0)
		close(cctx->fdout);
}

static void pe_free_driver_ctx_impl(struct pe_driver_ctx_impl * ictx)
{
	if (ictx->symctx.append)
		free(ictx->symctx.append);

	if (ictx->symctx.exclude)
		free(ictx->symctx.exclude);

	pe_driver_close_fds(&ictx->ctx.cctx);
	argv_free(ictx->meta);
	free(ictx);
}

void pe_free_driver_ctx(struct pe_driver_ctx * ctx)
{
	struct pe_driver_ctx_impl *	ictx;
	uintptr_t			addr;

	if (ctx) {
		addr = (uintptr_t)ctx - offsetof(struct pe_driver_ctx_impl,ctx);
		ictx = (struct pe_driver_ctx_impl *)addr;
		pe_free_driver_ctx_impl(ictx);
	}
}
