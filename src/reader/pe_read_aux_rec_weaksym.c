/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2025  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <perk/perk.h>
#include <perk/perk_consts.h>
#include <perk/perk_structs.h>
#include "perk_endian_impl.h"
#include "perk_reader_impl.h"

int pe_read_aux_rec_weaksym(
	const struct pe_raw_coff_symbol *       p,
	struct pe_meta_aux_rec_weaksym *	m,
	int                                     idx)
{
	const void *                            addr;
	const struct pe_raw_aux_rec_weaksym *   arec;

	addr = &p[++idx];
	arec = (struct pe_raw_aux_rec_weaksym *)addr;

	m->aux_tag_index        = pe_read_long(arec->aux_tag_index);
	m->aux_characteristics  = pe_read_long(arec->aux_characteristics);

	return 0;
}
