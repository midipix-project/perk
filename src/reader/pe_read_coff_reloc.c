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

int pe_read_coff_reloc(
	const struct pe_raw_coff_reloc *	p,
	struct pe_meta_coff_reloc *		m)
{
	m->rel_rva  = pe_read_long(p->rel_rva);
	m->rel_sym  = pe_read_long(p->rel_sym);
	m->rel_type = pe_read_long(p->rel_type);

	return 0;
}
