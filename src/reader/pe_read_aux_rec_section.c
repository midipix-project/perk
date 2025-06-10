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

int pe_read_aux_rec_section(
	const struct pe_raw_coff_symbol *       p,
	struct pe_meta_aux_rec_section *	m,
	int                                     idx)
{
	const void *                            addr;
	const struct pe_raw_aux_rec_section *   arec;

	addr = &p[++idx];
	arec = (struct pe_raw_aux_rec_section *)addr;

	m->aux_size             = pe_read_long(arec->aux_size);
	m->aux_num_of_relocs    = pe_read_short(arec->aux_num_of_relocs);
	m->aux_num_of_line_nums = pe_read_short(arec->aux_num_of_line_nums);
	m->aux_check_sum        = pe_read_long(arec->aux_check_sum);
	m->aux_number           = pe_read_short(arec->aux_number);
	m->aux_selection        = arec->aux_selection[0];

	return 0;
}
