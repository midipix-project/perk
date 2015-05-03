#include <endian.h>
#include <string.h>

#include <perk/perk.h>
#include "perk_impl.h"

int pe_read_dos_header(const struct pe_image_dos_hdr * p, struct pe_meta_image_dos_hdr * m)
{
	if ((p->dos_magic[0] != 'M') || (p->dos_magic[1] != 'Z'))
		return PERK_BAD_DOS_HEADER;

	#if (BYTE_ORDER == LITTLE_ENDIAN)

	memcpy(m,p,sizeof(*p));

	#else

	int i;

	m->dos_magic[0] = p->dos_magic[0];
	m->dos_magic[1] = p->dos_magic[1];

	m->dos_cblp	= pe_read_short(p->dos_cblp);
	m->dos_cp	= pe_read_short(p->dos_cp);
	m->dos_crlc	= pe_read_short(p->dos_crlc);
	m->dos_cparhdr	= pe_read_short(p->dos_cparhdr);
	m->dos_minalloc	= pe_read_short(p->dos_minalloc);
	m->dos_maxalloc	= pe_read_short(p->dos_maxalloc);
	m->dos_ss	= pe_read_short(p->dos_ss);
	m->dos_sp	= pe_read_short(p->dos_sp);
	m->dos_csum	= pe_read_short(p->dos_csum);
	m->dos_ip	= pe_read_short(p->dos_ip);
	m->dos_cs	= pe_read_short(p->dos_cs);
	m->dos_lfarlc	= pe_read_short(p->dos_lfarlc);
	m->dos_ovno	= pe_read_short(p->dos_ovno);


	for (i=0; i<4; i++)
		m->dos_res[i] = pe_read_short(&p->dos_res[2*i]);

	m->dos_oemid	= pe_read_short(p->dos_oemid);
	m->dos_oeminfo	= pe_read_short(p->dos_oeminfo);

	for (i=0; i<10; i++)
		m->dos_res2[i] = pe_read_short(&p->dos_res2[2*i]);

	m->dos_lfanew	= pe_read_long(p->dos_lfanew);

	#endif

	return 0;
}
