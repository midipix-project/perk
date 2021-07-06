/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2021  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <string.h>

#include <perk/perk.h>
#include <perk/perk_consts.h>
#include <perk/perk_structs.h>
#include "perk_endian_impl.h"
#include "perk_reader_impl.h"

int pe_read_export_header(const struct pe_raw_export_hdr * p, struct pe_meta_export_hdr * m)
{
	m->eh_export_flags          = pe_read_long(p->eh_export_flags);
	m->eh_time_date_stamp       = pe_read_long(p->eh_time_date_stamp);

	m->eh_major_ver             = pe_read_short(p->eh_major_ver);
	m->eh_minor_ver             = pe_read_short(p->eh_minor_ver);

	m->eh_name_rva              = pe_read_long(p->eh_name_rva);
	m->eh_ordinal_base          = pe_read_long(p->eh_ordinal_base);
	m->eh_addr_tbl_entries      = pe_read_long(p->eh_addr_tbl_entries);
	m->eh_num_of_name_ptrs      = pe_read_long(p->eh_num_of_name_ptrs);
	m->eh_export_addr_tbl_rva   = pe_read_long(p->eh_export_addr_tbl_rva);
	m->eh_name_ptr_rva          = pe_read_long(p->eh_name_ptr_rva);
	m->eh_ordinal_tbl_rva       = pe_read_long(p->eh_ordinal_tbl_rva);

	return 0;
}
