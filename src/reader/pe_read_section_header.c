/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2016  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <string.h>

#include <perk/perk.h>
#include "perk_endian_impl.h"
#include "perk_reader_impl.h"

int pe_read_section_header(const struct pe_sec_hdr * p, struct pe_meta_sec_hdr * m)
{
	/* name: meta struct conveniently contains null termination */
	memset(m,0,sizeof(m->name));
	memcpy(m,p,sizeof(p->name));

	m->long_name		= 0;
	m->virtual_size		= pe_read_long(p->virtual_size);
	m->virtual_addr		= pe_read_long(p->virtual_addr);
	m->size_of_raw_data	= pe_read_long(p->size_of_raw_data);
	m->ptr_to_raw_data	= pe_read_long(p->ptr_to_raw_data);
	m->ptr_to_relocs	= pe_read_long(p->ptr_to_relocs);
	m->ptr_to_line_nums	= pe_read_long(p->ptr_to_line_nums);

	m->num_of_relocs	= pe_read_short(p->num_of_relocs);
	m->num_of_line_nums	= pe_read_short(p->num_of_line_nums);

	m->characteristics	= pe_read_long(p->characteristics);

	return 0;
}
