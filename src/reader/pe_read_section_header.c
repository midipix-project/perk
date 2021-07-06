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

int pe_read_section_header(const struct pe_raw_sec_hdr * p, struct pe_meta_sec_hdr * m)
{
	/* name: meta struct conveniently contains null termination */
	memset(m,0,sizeof(m->sh_name));
	memcpy(m,p,sizeof(p->sh_name));

	m->sh_long_name         = 0;
	m->sh_virtual_size      = pe_read_long(p->sh_virtual_size);
	m->sh_virtual_addr      = pe_read_long(p->sh_virtual_addr);
	m->sh_size_of_raw_data  = pe_read_long(p->sh_size_of_raw_data);
	m->sh_ptr_to_raw_data   = pe_read_long(p->sh_ptr_to_raw_data);
	m->sh_ptr_to_relocs     = pe_read_long(p->sh_ptr_to_relocs);
	m->sh_ptr_to_line_nums  = pe_read_long(p->sh_ptr_to_line_nums);

	m->sh_num_of_relocs     = pe_read_short(p->sh_num_of_relocs);
	m->sh_num_of_line_nums  = pe_read_short(p->sh_num_of_line_nums);

	m->sh_characteristics   = pe_read_long(p->sh_characteristics);

	return 0;
}
