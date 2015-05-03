#include <endian.h>
#include <string.h>

#include <perk/perk.h>
#include "perk_impl.h"

int pe_read_coff_header(const struct pe_coff_file_hdr * p, struct pe_meta_coff_file_hdr * m)
{
	if ((p->signature[0] != 'P') || (p->signature[1] != 'E')
			|| *(uint16_t *)&p->signature[2])
		return PERK_BAD_COFF_HEADER;

	#if (BYTE_ORDER == LITTLE_ENDIAN)

	memcpy(m,p,sizeof(*p));

	#else

	m->signature[0] = p->signature[0];
	m->signature[1] = p->signature[1];
	m->signature[2] = p->signature[2];
	m->signature[3] = p->signature[3];

	m->machine		= pe_read_short(p->machine);
	m->num_of_sections	= pe_read_short(p->num_of_sections);

	m->time_date_stamp	= pe_read_long(p->time_date_stamp);
	m->ptr_to_sym_tbl	= pe_read_long(p->ptr_to_sym_tbl);
	m->num_of_syms		= pe_read_long(p->num_of_syms);

	m->size_of_opt_hdr	= pe_read_short(p->size_of_opt_hdr);
	m->characteristics	= pe_read_short(p->characteristics);

	#endif

	return 0;
}
