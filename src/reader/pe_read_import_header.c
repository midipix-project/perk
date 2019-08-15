/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2017  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <string.h>

#include <perk/perk.h>
#include <perk/perk_consts.h>
#include <perk/perk_structs.h>
#include "perk_endian_impl.h"
#include "perk_reader_impl.h"

int pe_read_import_header(const struct pe_raw_import_hdr * p, struct pe_meta_import_hdr * m)
{
	m->ih_import_lookup_tbl_rva = pe_read_long(p->ih_import_lookup_tbl_rva);
	m->ih_time_date_stamp       = pe_read_long(p->ih_time_date_stamp);
	m->ih_forwarder_chain       = pe_read_long(p->ih_forwarder_chain);
	m->ih_name_rva              = pe_read_long(p->ih_name_rva);
	m->ih_import_addr_tbl_rva   = pe_read_long(p->ih_import_addr_tbl_rva);

	m->ih_name   = 0;
	m->ih_aitems = 0;

	return 0;
}

int pe_read_import_lookup(
	const unsigned char * p,
	struct pe_meta_import_lookup * m,
	uint32_t magic)
{
	switch (magic) {
		case PE_MAGIC_PE32:
			m->u.ii_import_lookup_entry_64 = pe_read_long(p);
			m->ii_flag = (uint32_t)m->u.ii_import_lookup_entry_64 >> 31;
			break;

		case PE_MAGIC_PE32_PLUS:
			m->u.ii_import_lookup_entry_64 = pe_read_quad(p);
			m->ii_flag = m->u.ii_import_lookup_entry_64 >> 63;
			break;

		default:
			return PERK_ERR_BAD_IMAGE_TYPE;
	}

	if (m->ii_flag) {
		m->ii_ordinal           = m->u.ii_import_lookup_entry_32 & 0xffff;
		m->ii_hint_name_tbl_rva = 0;
	} else {
		m->ii_ordinal           = 0;
		m->ii_hint_name_tbl_rva = m->u.ii_import_lookup_entry_32 & 0x7fffffff;
	}

	m->ii_hint = 0;
	m->ii_name = 0;

	return 0;
}
