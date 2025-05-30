/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2021  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include <perk/perk.h>
#include "perk_driver_impl.h"
#include "perk_errinfo_impl.h"

static int pe_lib_free_unit_ctx_impl(struct pe_unit_ctx_impl * ctx, int ret)
{
	if (ctx) {
		pe_meta_free_image_meta(ctx->meta);
		pe_raw_unmap_raw_image(&ctx->map);
		free(ctx);
	}

	return ret;
}

int pe_lib_get_unit_ctx(
	const struct pe_driver_ctx *	dctx,
	const char *			path,
	struct pe_unit_ctx **		pctx)
{
	struct pe_unit_ctx_impl *	ctx;
	int				prot;

	if (!dctx)
		return PERK_CUSTOM_ERROR(
			dctx,PERK_ERR_NULL_CONTEXT);

	else if (!(ctx = calloc(1,sizeof(*ctx))))
		return PERK_BUFFER_ERROR(dctx);

	pe_driver_set_ectx(
		dctx,0,path);

	prot = (dctx->cctx->actflags & PERK_ACTION_MAP_READWRITE)
		? PROT_READ | PROT_WRITE
		: PROT_READ;

	if (pe_raw_map_raw_image(dctx,-1,path,prot,&ctx->map))
		return pe_lib_free_unit_ctx_impl(ctx,
			PERK_NESTED_ERROR(dctx));

	if (pe_meta_get_image_meta(dctx,&ctx->map,&ctx->meta))
		return pe_lib_free_unit_ctx_impl(ctx,
			PERK_NESTED_ERROR(dctx));

	ctx->path	= path;
	ctx->uctx.path	= &ctx->path;
	ctx->uctx.map	= &ctx->map;
	ctx->uctx.meta	= ctx->meta;

	*pctx = &ctx->uctx;
	return 0;
}

void pe_lib_free_unit_ctx(struct pe_unit_ctx * ctx)
{
	struct pe_unit_ctx_impl *	ictx;
	uintptr_t			addr;

	if (ctx) {
		addr = (uintptr_t)ctx - offsetof(struct pe_unit_ctx_impl,uctx);
		ictx = (struct pe_unit_ctx_impl *)addr;
		pe_lib_free_unit_ctx_impl(ictx,0);
	}
}
