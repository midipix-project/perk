#ifndef PE_DRIVER_IMPL_H
#define PE_DRIVER_IMPL_H

#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

#include <perk/perk.h>
#include <perk/perk_meta.h>
#include "perk_dprintf_impl.h"
#include "argv/argv.h"

#define PERK_OPTV_ELEMENTS 64

extern const struct argv_option pe_default_options[];

enum app_tags {
	TAG_HELP,
	TAG_VERSION,
	TAG_PRETTY,
	TAG_CATEGORY,
	TAG_SECTIONS,
	TAG_SYMBOLS,
	TAG_STRINGS,
	TAG_EXPSYMS,
	TAG_IMPLIBS,
	TAG_IMPSYMS,
	TAG_DSOLIBS,
	TAG_DSOSYMS,
	TAG_HDRDUMP,
};

struct pe_driver_ctx_impl {
	struct pe_common_ctx        cctx;
	struct pe_driver_ctx        ctx;
	struct pe_fd_ctx            fdctx;
	const struct pe_unit_ctx *  euctx;
	const char *                eunit;
	struct pe_error_info **     errinfp;
	struct pe_error_info **     erricap;
	struct pe_error_info *      erriptr[64];
	struct pe_error_info        erribuf[64];
};

struct pe_unit_ctx_impl {
	const char *		path;
	struct pe_raw_image	map;
	struct pe_image_meta *	meta;
	struct pe_unit_ctx	uctx;
};


static inline struct pe_driver_ctx_impl * pe_get_driver_ictx(
	const struct pe_driver_ctx * dctx)
{
        uintptr_t addr;

        if (dctx) {
                addr = (uintptr_t)dctx - offsetof(struct pe_driver_ctx_impl,ctx);
                return (struct pe_driver_ctx_impl *)addr;
        }

        return 0;
}

static inline void pe_driver_set_ectx(
	const struct pe_driver_ctx * dctx,
	const struct pe_unit_ctx *   uctx,
	const char *                 unit)
{
	struct pe_driver_ctx_impl *  ictx;

	ictx        = pe_get_driver_ictx(dctx);
	ictx->euctx = uctx;
	ictx->eunit = unit;
}

static inline int pe_driver_fdin(const struct pe_driver_ctx * dctx)
{
	struct pe_fd_ctx fdctx;
	pe_get_driver_fdctx(dctx,&fdctx);
	return fdctx.fdin;
}

static inline int pe_driver_fdout(const struct pe_driver_ctx * dctx)
{
	struct pe_fd_ctx fdctx;
	pe_get_driver_fdctx(dctx,&fdctx);
	return fdctx.fdout;
}

static inline int pe_driver_fderr(const struct pe_driver_ctx * dctx)
{
	struct pe_fd_ctx fdctx;
	pe_get_driver_fdctx(dctx,&fdctx);
	return fdctx.fderr;
}

static inline int pe_driver_fdlog(const struct pe_driver_ctx * dctx)
{
	struct pe_fd_ctx fdctx;
	pe_get_driver_fdctx(dctx,&fdctx);
	return fdctx.fdlog;
}

static inline int pe_driver_fdcwd(const struct pe_driver_ctx * dctx)
{
	struct pe_fd_ctx fdctx;
	pe_get_driver_fdctx(dctx,&fdctx);
	return fdctx.fdcwd;
}

static inline int pe_driver_fddst(const struct pe_driver_ctx * dctx)
{
	struct pe_fd_ctx fdctx;
	pe_get_driver_fdctx(dctx,&fdctx);
	return fdctx.fddst;
}

#endif
