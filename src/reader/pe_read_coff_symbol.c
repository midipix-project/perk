/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2025  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <string.h>

#include <perk/perk.h>
#include <perk/perk_consts.h>
#include <perk/perk_structs.h>
#include "perk_endian_impl.h"
#include "perk_reader_impl.h"

int pe_read_coff_symbol(
	const struct pe_raw_coff_symbol *	p,
	struct pe_meta_coff_symbol *		m,
	const struct pe_meta_coff_file_hdr *	coff,
	void *					base)
{
	uint32_t	roffset;
	char *		mark;
	unsigned	bias = 0;

	m->cs_name                = m->cs_name_buf;
	m->cs_value               = pe_read_long(p->cs_value);
	m->cs_section_number      = pe_read_short(p->cs_section_number);
	m->cs_type                = pe_read_short(p->cs_type);
	m->cs_storage_class       = p->cs_storage_class[0];
	m->cs_num_of_aux_recs     = p->cs_num_of_aux_recs[0];
	m->cs_aux_recs            = 0;

	memset(m->cs_name_buf,0,sizeof(m->cs_name_buf));

	if (m->cs_num_of_aux_recs)
		m->cs_aux_recs = &p[1].cs_name[0];

	if (p->cs_storage_class[0] == PE_IMAGE_SYM_CLASS_FILE)
		if (p->cs_num_of_aux_recs[0])
			if (!p[1].cs_value[0])
				bias = 1;

	p += bias;

	if (!bias && (p->cs_storage_class[0] == PE_IMAGE_SYM_CLASS_FILE)
			&& p->cs_num_of_aux_recs[0]) {
		memcpy(m->cs_name_buf,p[1].cs_name,sizeof(*p));

	} else if (p->cs_name[0]) {
		memcpy(m->cs_name_buf,p->cs_name,sizeof(p->cs_name));

	} else if (!p->cs_name[1] && !p->cs_name[2] && !p->cs_name[3]) {
		mark	= (char *)base;
		roffset	= pe_read_long(&p->cs_name[4]);

		if (roffset < coff->cfh_size_of_str_tbl)
			m->cs_name = mark + coff->cfh_ptr_to_str_tbl + roffset;
	}

	return 0;
}
