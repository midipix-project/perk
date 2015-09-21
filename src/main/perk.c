#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <perk/perk.h>
#include "perk_impl.h"

static int perk_parse_opts(struct perk_ctx * ctx)
{
	int		i;
	char *		ch;
	const char **	popt;

	for (i=1; i<ctx->argc; i++) {
		ch = ctx->argv[i];

		if (*ch == '-') {
			switch (*++ch) {
				case 'i':
					popt = &ctx->fname;
					break;
				case 'h':
					ctx->flags = PERK_HELP;
					return 0;
				default:
					ctx->status = PERK_BAD_OPT;
					return ctx->status;
			}

			while ((*++ch == '\t') || (*ch == ' '));

			if (!*ch) {
				if (++i < ctx->argc)
					*popt = ctx->argv[i];
				else
					ctx->status = PERK_BAD_OPT_VAL;
			} else
				*popt = ch;
		} else if (!ctx->fname)
			ctx->fname = ch;
		else
			ctx->status = PERK_BAD_OPT;

	}

	return ctx->status;
}


static int perk_map_input(struct perk_ctx * ctx)
{
	ctx->fd = open(ctx->fname,O_RDONLY | O_CLOEXEC);

	if (ctx->fd < 0) {
		ctx->status = PERK_IO_ERROR;
		return ctx->status;
	}

	ctx->status = pe_map_raw_image(ctx->fd,0,PROT_READ,&ctx->map);

	return ctx->status;
}


static int perk_exit(struct perk_ctx * ctx)
{
	if (ctx->map.addr)
		pe_unmap_raw_image(&ctx->map);

	return ctx->status;
}


static int perk_run(struct perk_ctx * ctx)
{
	struct pe_image_meta * meta;

	if (perk_map_input(ctx))
		return perk_exit(ctx);

	if ((ctx->status = pe_get_image_meta(&ctx->map,&meta)))
		return perk_exit(ctx);

	/* pre-alpha default output */
	pe_output_export_symbols(
		meta,
		PERK_OUTPUT_FORMAT_LIST | PERK_OUTPUT_FIELD_NAME,
		stdout);

	ctx->status = pe_free_image_meta(meta);

	return perk_exit(ctx);
}

static int perk_main(int argc, char ** argv, char ** envp)
{
	struct perk_ctx ctx = {argc,argv,envp};

	if (perk_parse_opts(&ctx))
		return ctx.status;
	else
		return perk_run(&ctx);
}

int main(int argc, char ** argv, char ** envp)
{
	return perk_main(argc,argv,envp);
}
