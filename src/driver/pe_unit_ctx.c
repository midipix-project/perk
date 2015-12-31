#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include <perk/perk.h>
#include "perk_driver_impl.h"

static int pe_free_unit_ctx_impl(struct pe_unit_ctx_impl * ctx, int status)
{
	if (ctx) {
		pe_free_image_meta(ctx->meta);
		pe_unmap_raw_image(&ctx->map);
		free(ctx);
	}

	return status;
}

int pe_get_unit_ctx(
	const struct pe_driver_ctx *	dctx,
	const char *			path,
	struct pe_unit_ctx **		pctx)
{
	struct pe_unit_ctx_impl *	ctx;
	int				prot;

	if (!dctx || !(ctx = calloc(1,sizeof(*ctx))))
		return -1;

	prot = (dctx->cctx->actflags & PERK_ACTION_MAP_READWRITE)
		? PROT_READ | PROT_WRITE
		: PROT_READ;

	if (pe_map_raw_image(dctx->cctx->ioctx->fdin,path,prot,&ctx->map))
		return pe_free_unit_ctx_impl(ctx,-1);

	if (pe_get_image_meta(&ctx->map,&ctx->meta))
		return pe_free_unit_ctx_impl(ctx,-1);

	memcpy(&ctx->cctx,dctx->cctx,
		sizeof(ctx->cctx));

	ctx->path	= path;
	ctx->ioctx.prot	= prot;

	ctx->uctx.path	= &ctx->path;
	ctx->uctx.map	= &ctx->map;
	ctx->uctx.meta	= ctx->meta;
	ctx->uctx.cctx	= &ctx->cctx;

	*pctx = &ctx->uctx;
	return 0;
}

void pe_free_unit_ctx(struct pe_unit_ctx * ctx)
{
	struct pe_unit_ctx_impl *	ictx;
	uintptr_t			addr;

	if (ctx) {
		addr = (uintptr_t)ctx - offsetof(struct pe_unit_ctx_impl,uctx);
		ictx = (struct pe_unit_ctx_impl *)addr;
		pe_free_unit_ctx_impl(ictx,0);
	}
}
