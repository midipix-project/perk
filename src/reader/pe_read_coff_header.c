/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2016  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <string.h>

#include <perk/perk.h>
#include "perk_endian_impl.h"
#include "perk_reader_impl.h"

int pe_read_coff_header(const struct pe_raw_coff_file_hdr * p, struct pe_meta_coff_file_hdr * m)
{
	if ((p->cfh_signature[0] != 'P') || (p->cfh_signature[1] != 'E')
			|| p->cfh_signature[2] || p->cfh_signature[3])
		return PERK_ERR_BAD_COFF_HEADER;

	m->cfh_signature[0] = p->cfh_signature[0];
	m->cfh_signature[1] = p->cfh_signature[1];
	m->cfh_signature[2] = p->cfh_signature[2];
	m->cfh_signature[3] = p->cfh_signature[3];

	m->cfh_machine		= pe_read_short(p->cfh_machine);
	m->cfh_num_of_sections	= pe_read_short(p->cfh_num_of_sections);

	m->cfh_time_date_stamp	= pe_read_long(p->cfh_time_date_stamp);
	m->cfh_ptr_to_sym_tbl	= pe_read_long(p->cfh_ptr_to_sym_tbl);
	m->cfh_num_of_syms	= pe_read_long(p->cfh_num_of_syms);

	m->cfh_size_of_opt_hdr	= pe_read_short(p->cfh_size_of_opt_hdr);
	m->cfh_characteristics	= pe_read_short(p->cfh_characteristics);

	return 0;
}
