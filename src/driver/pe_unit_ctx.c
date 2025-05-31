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
#include <perk/perk_arbits.h>
#include "perk_ar_impl.h"
#include "perk_driver_impl.h"
#include "perk_errinfo_impl.h"

static int pe_lib_free_unit_ctx_impl(struct pe_unit_ctx_impl * ctx, int ret)
{
	struct pe_image_meta ** pmeta;
	if (ctx) {
		for (pmeta=ctx->objmeta; pmeta && *pmeta; pmeta++)
			pe_meta_free_image_meta(*pmeta);

		pe_ar_free_archive_meta(ctx->armeta);
		pe_meta_free_image_meta(ctx->meta);
		pe_raw_unmap_raw_image(&ctx->map);
		free(ctx->usrobjmeta);
		free(ctx->objmeta);
		free(ctx);
	}

	return ret;
}

static int pe_lib_create_object_vector_or_verify_archive(
	const struct pe_driver_ctx *    dctx,
	struct pe_unit_ctx_impl *       ctx)
{
	struct pe_archive_meta *        actx;
	struct pe_archive_meta_impl *   ictx;
	struct pe_driver_ctx_impl *     idctx;
	bool                            fvector;
	bool                            fstrict;
	bool                            fpurearch;
	struct pe_image_meta *          meta;
	struct pe_image_meta **         objmeta;
	const struct pe_image_meta **   usrobjmeta;
	size_t                          veclen;
	struct pe_raw_image             map;
	struct ar_meta_member_info **   pmember;
	struct pe_error_info **         errinfp;
	enum pe_abi                     m_abi;
	enum pe_subtype                 m_subtype;

	fvector   = (dctx->cctx->drvflags & PERK_DRIVER_AR_OBJECT_VECTOR);
	fstrict   = (dctx->cctx->drvflags & PERK_DRIVER_AR_STRICT_PE);
	fpurearch = (dctx->cctx->drvflags & PERK_DRIVER_AR_STRICT_PE_ARCH);

	if (!fvector && !fstrict && !fpurearch)
		return 0;

	actx   = ctx->armeta;
	ictx   = pe_archive_meta_ictx(actx);
	idctx  = pe_get_driver_ictx(dctx);
	veclen = ictx->nentries + 1;

	if (fvector && !(ctx->objmeta = calloc(veclen,sizeof(meta))))
		return PERK_BUFFER_ERROR(dctx);

	if (fvector && !(ctx->usrobjmeta = calloc(veclen,sizeof(*usrobjmeta))))
		return PERK_BUFFER_ERROR(dctx);

	if (!actx->a_memberv)
		return 0;

	objmeta    = ctx->objmeta;
	usrobjmeta = ctx->usrobjmeta;

	for (pmember=actx->a_memberv; *pmember; pmember++) {
		errinfp = idctx->errinfp;

		switch (pmember[0]->ar_member_attr) {
			case AR_MEMBER_ATTR_ARCHIVE:
				return PERK_CUSTOM_ERROR(dctx,
					PERK_ERR_AR_NESTED_ARCHIVE);

			case AR_MEMBER_ATTR_ARMAP:
			case AR_MEMBER_ATTR_NAMESTRS:
			case AR_MEMBER_ATTR_LINKINFO:
				break;

			default:
				map.map_addr = pmember[0]->ar_object_data;
				map.map_size = pmember[0]->ar_object_size;

				if (!pe_meta_get_image_meta(dctx,&map,&meta)) {
					if (fpurearch && (objmeta == ctx->objmeta)) {
						m_abi     = meta->m_abi;
						m_subtype = meta->m_subtype;

					} else if (fpurearch) {
						if ((meta->m_abi != m_abi) || (meta->m_subtype != m_subtype))
							return PERK_CUSTOM_ERROR(dctx,
								PERK_ERR_AR_MIXED_PE_MEMBERS);
					}

					if (fvector) {
						*objmeta++ = meta;
						*usrobjmeta++ = meta;
					} else {
						pe_meta_free_image_meta(meta);
					};


				} else if (fstrict || fpurearch) {
					return PERK_CUSTOM_ERROR(dctx,
						PERK_ERR_AR_NON_PE_MEMBERS);

				} else {
					 idctx->errinfp = errinfp;

					for (; *errinfp; )
						*errinfp++ = 0;
				}

				break;
		}
	}

	return 0;
}

int pe_lib_get_unit_ctx(
	const struct pe_driver_ctx *	dctx,
	const char *			path,
	struct pe_unit_ctx **		pctx)
{
	struct pe_unit_ctx_impl *	ctx;
	int				prot;
	char *                          mark;
	size_t                          siglen;

	if (!dctx)
		return PERK_CUSTOM_ERROR(
			dctx,PERK_ERR_NULL_CONTEXT);

	else if (!(ctx = calloc(1,sizeof(*ctx))))
		return PERK_BUFFER_ERROR(dctx);

	pe_driver_set_ectx(
		dctx,0,path);

	prot = (dctx->cctx->drvflags & PERK_DRIVER_MAP_WRITE_ACCESS)
		? PROT_READ | PROT_WRITE
		: PROT_READ;

	if (pe_raw_map_raw_image(dctx,-1,path,prot,&ctx->map))
		return pe_lib_free_unit_ctx_impl(ctx,
			PERK_NESTED_ERROR(dctx));

	if (ctx->map.map_size < (siglen = sizeof(struct ar_raw_signature)))
		return pe_lib_free_unit_ctx_impl(ctx,
			PERK_CUSTOM_ERROR(
				dctx,
				PERK_ERR_INVALID_IMAGE));

	if (!strncmp((mark = ctx->map.map_addr),AR_SIGNATURE,siglen)) {
		ctx->armap.map_addr = ctx->map.map_addr;
		ctx->armap.map_size = ctx->map.map_size;

		if (pe_ar_get_archive_meta(dctx,&ctx->armap,&ctx->armeta))
			return pe_lib_free_unit_ctx_impl(ctx,
				PERK_NESTED_ERROR(dctx));

		if (pe_lib_create_object_vector_or_verify_archive(dctx,ctx) < 0)
			return pe_lib_free_unit_ctx_impl(ctx,
				PERK_NESTED_ERROR(dctx));

	} else if (pe_meta_get_image_meta(dctx,&ctx->map,&ctx->meta)) {
		return pe_lib_free_unit_ctx_impl(ctx,
			PERK_NESTED_ERROR(dctx));
	}

	ctx->path	  = path;
	ctx->uctx.path	  = &ctx->path;
	ctx->uctx.meta	  = ctx->meta;
	ctx->uctx.armeta  = ctx->armeta;
	ctx->uctx.objmeta = ctx->usrobjmeta;

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
