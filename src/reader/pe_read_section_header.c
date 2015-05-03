#include <endian.h>
#include <string.h>

#include <perk/perk.h>
#include "perk_impl.h"

int pe_read_section_header(const struct pe_sec_hdr * p, struct pe_meta_sec_hdr * m)
{
	/* name: meta struct conveniently contains null termination */
	memset(m,0,sizeof(m->name));
	memcpy(m,p,sizeof(p->name));

	/* todo: long name support */
	m->long_name = 0;

	#if (BYTE_ORDER == LITTLE_ENDIAN)

	memcpy(&m->virtual_size,&p->virtual_size,sizeof(*p)-sizeof(p->name));

	#else

	m->virtual_size		= pe_read_long(p->virtual_size);
	m->virtual_addr		= pe_read_long(p->virtual_addr);
	m->size_of_raw_data	= pe_read_long(p->size_of_raw_data);
	m->ptr_to_raw_data	= pe_read_long(p->ptr_to_raw_data);
	m->ptr_to_relocs	= pe_read_long(p->ptr_to_relocs);
	m->ptr_to_line_nums	= pe_read_long(p->ptr_to_line_nums);

	m->num_of_relocs	= pe_read_short(p->num_of_relocs);
	m->num_of_line_nums	= pe_read_short(p->num_of_line_nums);

	m->characteristics	= pe_read_long(p->characteristics);

	#endif

	return 0;
}
