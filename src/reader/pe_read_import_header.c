#include <endian.h>
#include <string.h>

#include <perk/perk.h>
#include "perk_reader_impl.h"

int pe_read_import_header(const struct pe_import_hdr * p, struct pe_meta_import_hdr * m)
{
	#if (BYTE_ORDER == LITTLE_ENDIAN)

	memcpy(m,p,sizeof(*p));

	#else

	m->import_lookup_tbl_rva	= pe_read_long(p->import_lookup_tbl_rva);
	m->time_date_stamp		= pe_read_long(p->time_date_stamp);
	m->forwarder_chain		= pe_read_long(p->forwarder_chain);
	m->name_rva			= pe_read_long(p->name_rva);
	m->import_addr_tbl_rva		= pe_read_long(p->import_addr_tbl_rva);

	#endif

	m->name  = 0;
	m->aitems = 0;

	return 0;
}

int pe_read_import_lookup_item(
	const union pe_import_lookup_item * p,
	struct pe_meta_import_lookup_item * m,
	uint32_t magic)
{
	switch (magic) {
		case PE_MAGIC_PE32:
			m->u.import_lookup_entry_64 = pe_read_long(p->import_lookup_entry_32);
			return 0;

		case PE_MAGIC_PE32_PLUS:
			m->u.import_lookup_entry_64 = pe_read_quad(p->import_lookup_entry_64);
			return 0;

		default:
			return PERK_BAD_IMAGE_TYPE;
	}
}
