/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2025  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <perk/perk.h>
#include <perk/perk_arbits.h>
#include "perk_ar_impl.h"
#include "perk_driver_impl.h"
#include "perk_errinfo_impl.h"
#include "perk_visibility_impl.h"

perk_hidden int pe_ar_parse_primary_armap_sysv_64(
	const struct pe_driver_ctx *    dctx,
	struct pe_archive_meta_impl *   m)
{
	struct ar_raw_armap_sysv_64 *   armap;
	struct ar_meta_member_info *    memberp;
	struct ar_meta_armap_common_64 *armapref;
	struct ar_meta_armap_ref_64 *   symrefs;
	uint64_t                        idx;
	uint64_t                        uref_hi;
	uint64_t                        uref_lo;
	uint64_t                        nsyms_hi;
	uint64_t                        nsyms_lo;
	uint64_t                        nsyms;
	uint64_t                        nstrs;
	const char *                    ch;
	const char *                    cap;
	unsigned char *                 uch;
	unsigned char                   (*mark)[0x08];

	armap   = &m->armaps.armap_sysv_64;
	memberp = m->memberv[0];

	mark = memberp->ar_object_data;

	armap->ar_num_of_syms = mark;
	uch = *mark++;

	armap->ar_first_ref_offset = mark;

	nsyms_hi = (uch[0] << 24) + (uch[1] << 16) + (uch[2] << 8) + uch[3];
	nsyms_lo = (uch[4] << 24) + (uch[5] << 16) + (uch[6] << 8) + uch[7];

	nsyms = (nsyms_hi << 32) + nsyms_lo;
	mark += nsyms;

	if (memberp->ar_object_size < (sizeof(*mark) + (nsyms * sizeof(*mark))))
		return PERK_CUSTOM_ERROR(
			dctx,
			PERK_ERR_AR_INVALID_ARMAP_NUMBER_OF_SYMS);

	m->symstrs = (const char *)mark;

	cap  = memberp->ar_object_data;
	cap += memberp->ar_object_size;

	if ((cap == m->symstrs) && nsyms)
		return PERK_CUSTOM_ERROR(
			dctx,
			PERK_ERR_AR_INVALID_ARMAP_STRING_TABLE);

	if (nsyms && !m->symstrs[0])
		return PERK_CUSTOM_ERROR(
			dctx,
			PERK_ERR_AR_INVALID_ARMAP_STRING_TABLE);

	for (ch=&m->symstrs[1],nstrs=0; ch<cap; ch++) {
		if (!ch[0] && !ch[-1] && (nstrs < nsyms))
			return PERK_CUSTOM_ERROR(
				dctx,
				PERK_ERR_AR_INVALID_ARMAP_STRING_TABLE);

		if (!ch[0] && ch[-1])
			nstrs++;
	}

	if (nstrs != nsyms)
		return PERK_CUSTOM_ERROR(
			dctx,
			PERK_ERR_AR_INVALID_ARMAP_STRING_TABLE);

	if (cap[-1])
		return PERK_CUSTOM_ERROR(
			dctx,
			PERK_ERR_AR_INVALID_ARMAP_STRING_TABLE);

	if (!(m->symstrv = calloc(nsyms + 1,sizeof(const char *))))
		return PERK_SYSTEM_ERROR(dctx);

	if (!(m->armaps.armap_symrefs_64 = calloc(nsyms + 1,sizeof(*symrefs))))
		return PERK_SYSTEM_ERROR(dctx);

	mark    = armap->ar_first_ref_offset;
	symrefs = m->armaps.armap_symrefs_64;

	for (idx=0,uch=*mark; idx<nsyms; idx++,uch=*++mark) {
		uref_hi = (uch[0] << 24) + (uch[1] << 16) + (uch[2] << 8) + uch[3];
		uref_lo = (uch[4] << 24) + (uch[5] << 16) + (uch[6] << 8) + uch[7];

		symrefs[idx].ar_member_offset = (uref_hi << 32) + uref_lo;
	}

	armap->ar_string_table = m->symstrv;

	armapref = &m->armaps.armap_common_64;
	armapref->ar_member         = memberp;
	armapref->ar_symrefs        = symrefs;
	armapref->ar_armap_sysv     = armap;
	armapref->ar_armap_attr     = AR_ARMAP_ATTR_SYSV | AR_ARMAP_ATTR_BE_64;
	armapref->ar_num_of_symbols = nsyms;
	armapref->ar_size_of_refs   = nsyms * sizeof(*mark);
	armapref->ar_size_of_strs   = cap - m->symstrs;
	armapref->ar_string_table   = m->symstrs;

	m->armaps.armap_nsyms = nsyms;

	m->armeta.a_armap_primary.ar_armap_common_64 = armapref;

	return 0;
}
