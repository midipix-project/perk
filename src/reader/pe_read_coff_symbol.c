/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2016  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <string.h>

#include <perk/perk.h>
#include "perk_endian_impl.h"
#include "perk_reader_impl.h"

int pe_read_coff_symbol(
	const struct pe_coff_symbol *		p,
	struct pe_meta_coff_symbol *		m,
	const struct pe_meta_coff_file_hdr *	coff,
	void *					base)
{
	uint32_t	roffset;
	char *		mark;
	unsigned	bias = 0;

	m->long_name		= 0;
	m->value		= pe_read_long(p->value);
	m->section_number	= pe_read_short(p->section_number);
	m->type			= pe_read_short(p->type);
	m->storage_class	= p->storage_class[0];
	m->num_of_aux_symbols	= p->num_of_aux_symbols[0];

	memset(m->name,0,sizeof(m->name));

	if (p->storage_class[0] == PE_IMAGE_SYM_CLASS_FILE)
		if (p->num_of_aux_symbols[0])
			if (!p[1].value[0])
				bias = 1;

	p += bias;

	if (!bias && (p->storage_class[0] == PE_IMAGE_SYM_CLASS_FILE)
			&& p->num_of_aux_symbols[0]) {
		memcpy(m->name,p[1].name,sizeof(*p));

	} else if (p->name[0]) {
		memcpy(m->name,p->name,sizeof(p->name));

	} else if (!p->name[1] && !p->name[2] && !p->name[3]) {
		mark	= (char *)base;
		roffset	= pe_read_long(&p->name[4]);

		if (roffset < coff->size_of_string_tbl)
			m->long_name = mark + coff->ptr_to_string_tbl + roffset;
	}

	return 0;
}
