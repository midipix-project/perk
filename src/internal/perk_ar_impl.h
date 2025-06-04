/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2025  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#ifndef SLIBTOOL_AR_IMPL_H
#define SLIBTOOL_AR_IMPL_H

#include "argv/argv.h"
#include <perk/perk.h>
#include <perk/perk_arbits.h>

/* decimal values in archive header are right padded with ascii spaces */
#define AR_DEC_PADDING (0x20)

/* archive file members are right padded as needed with ascii newline */
#define AR_OBJ_PADDING (0x0A)

/* initial number of elements in the transient, on-stack vector */
# define AR_STACK_VECTOR_ELEMENTS   (0x200)

/* bit mask to indicate that the first non-option argument is <posname> */
#define AR_POSNAME_MASK   (PERK_DRIVER_AR_POSITION_AFTER \
	                  |PERK_DRIVER_AR_POSITION_BEFORE)

#define AR_UPDATE_MASK    (PERK_DRIVER_AR_UPDATE_MEMBERS \
	                  |PERK_DRIVER_AR_REPLACE_MEMBERS)

#define AR_ACTION_MASK    (PERK_DRIVER_AR_LIST_MEMBERS  \
	                  |PERK_DRIVER_AR_MOVE_MEMBERS   \
	                  |PERK_DRIVER_AR_UPDATE_MEMBERS  \
	                  |PERK_DRIVER_AR_DELETE_MEMBERS  \
	                  |PERK_DRIVER_AR_APPEND_MEMBERS  \
	                  |PERK_DRIVER_AR_REPLACE_MEMBERS \
	                  |PERK_DRIVER_AR_EXTRACT_MEMBERS \
	                  |PERK_DRIVER_AR_PRINT_MEMBERS)

extern const struct argv_option pe_ar_options[];

struct ar_armaps_impl {
	struct ar_meta_armap_ref_32 *   armap_symrefs_32;
	struct ar_meta_armap_ref_64 *   armap_symrefs_64;
	struct ar_raw_armap_bsd_32      armap_bsd_32;
	struct ar_raw_armap_bsd_64      armap_bsd_64;
	struct ar_raw_armap_sysv_32     armap_sysv_32;
	struct ar_raw_armap_sysv_64     armap_sysv_64;
	struct ar_meta_armap_common_32  armap_common_32;
	struct ar_meta_armap_common_64  armap_common_64;
	uint64_t                        armap_nsyms;
};

struct pe_archive_meta_impl {
	const struct pe_driver_ctx *    dctx;
	struct pe_archive_ctx *         actx;
	size_t                          ofmtattr;
	size_t                          nentries;
	void *                          hdrinfov;
	char *                          namestrs;
	const char *                    symstrs;
	const char **                   symstrv;
	const char **                   mapstrv;
	off_t *                         offsetv;
	struct ar_meta_symbol_info *    syminfo;
	struct ar_meta_symbol_info **   syminfv;
	struct ar_meta_member_info **   memberv;
	struct ar_meta_member_info *    members;
	struct ar_armaps_impl           armaps;
	struct pe_txtfile_ctx *         nminfo;
	struct pe_archive_meta          armeta;
};

struct ar_meta_member_info * pe_archive_member_from_offset(
	struct pe_archive_meta_impl *   meta,
	off_t                           offset);

int pe_ar_parse_primary_armap_bsd_32(
	const struct pe_driver_ctx *    dctx,
	struct pe_archive_meta_impl *   m);

int pe_ar_parse_primary_armap_bsd_64(
	const struct pe_driver_ctx *    dctx,
	struct pe_archive_meta_impl *   m);

int pe_ar_parse_primary_armap_sysv_32(
	const struct pe_driver_ctx *    dctx,
	struct pe_archive_meta_impl *   m);

int pe_ar_parse_primary_armap_sysv_64(
	const struct pe_driver_ctx *    dctx,
	struct pe_archive_meta_impl *   m);

int pe_update_mapstrv(
	const struct pe_driver_ctx *    dctx,
	struct pe_archive_meta_impl *   m);

int pe_ar_update_syminfo(
	struct pe_archive_ctx *         actx);

int pe_ar_update_syminfo_ex(
	struct pe_archive_ctx *         actx,
	int                             fdout);

static inline struct pe_archive_meta_impl * pe_archive_meta_ictx(const struct pe_archive_meta * meta)
{
	uintptr_t addr;

	if (meta) {
		addr = (uintptr_t)meta - offsetof(struct pe_archive_meta_impl,armeta);
		return (struct pe_archive_meta_impl *)addr;
	}

	return 0;
}

#endif
