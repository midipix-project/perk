#include <endian.h>
#include <string.h>

#include <perk/perk.h>
#include "perk_reader_impl.h"

int pe_read_export_header(const struct pe_export_hdr * p, struct pe_meta_export_hdr * m)
{
	#if (BYTE_ORDER == LITTLE_ENDIAN)

	memcpy(m,p,sizeof(*p));

	#else

	m->export_flags		= pe_read_long(p->export_flags);
	m->time_date_stamp	= pe_read_long(p->time_date_stamp);

	m->major_ver		= pe_read_short(p->major_ver);
	m->minor_ver		= pe_read_short(p->minor_ver);

	m->name_rva		= pe_read_long(p->name_rva);
	m->ordinal_base		= pe_read_long(p->ordinal_base);
	m->addr_tbl_entries	= pe_read_long(p->addr_tbl_entries);
	m->num_of_name_ptrs	= pe_read_long(p->num_of_name_ptrs);
	m->export_addr_tbl_rva	= pe_read_long(p->export_addr_tbl_rva);
	m->name_ptr_rva		= pe_read_long(p->name_ptr_rva);
	m->ordinal_tbl_rva	= pe_read_long(p->ordinal_tbl_rva);

	#endif

	return 0;
}
