/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2016  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <string.h>

#include <perk/perk.h>
#include "perk_endian_impl.h"
#include "perk_reader_impl.h"

static int pe_read_optional_header_structs(const union pe_raw_opt_hdr * p, struct pe_meta_opt_hdr * m)
{
	unsigned int			i;
	struct pe_block *		pdir;
	const  unsigned char *		mark;

	struct pe_raw_opt_hdr_std *	astd;
	struct pe_raw_opt_hdr_vers *	avers;
	struct pe_raw_opt_hdr_align *	aalign;
	struct pe_raw_opt_hdr_img *	aimg;
	struct pe_raw_opt_hdr_ldr *	aldr;

	m->std.coh_magic = pe_read_short(p->opt_hdr_32.magic);

	switch (m->std.coh_magic) {
		case PE_MAGIC_PE32:
			astd	= (struct pe_raw_opt_hdr_std *)p;
			avers	= (struct pe_raw_opt_hdr_vers *)&p->opt_hdr_32.major_os_ver;
			aalign	= (struct pe_raw_opt_hdr_align *)&p->opt_hdr_32.section_align;
			aimg	= (struct pe_raw_opt_hdr_img *)&p->opt_hdr_32.size_of_image;
			aldr	= (struct pe_raw_opt_hdr_ldr *)&p->opt_hdr_32.loader_flags;
			break;

		case PE_MAGIC_PE32_PLUS:
			astd	= (struct pe_raw_opt_hdr_std *)p;
			avers	= (struct pe_raw_opt_hdr_vers *)&p->opt_hdr_64.major_os_ver;
			aalign	= (struct pe_raw_opt_hdr_align *)&p->opt_hdr_64.section_align;
			aimg	= (struct pe_raw_opt_hdr_img *)&p->opt_hdr_64.size_of_image;
			aldr	= (struct pe_raw_opt_hdr_ldr *)&p->opt_hdr_64.loader_flags;
			break;

		default:
			return PERK_ERR_BAD_IMAGE_TYPE;
	}

	/* std */
	m->std.coh_major_linker_ver		= astd->coh_major_linker_ver[0];
	m->std.coh_minor_linker_ver		= astd->coh_minor_linker_ver[0];

	m->std.coh_size_of_code			= pe_read_long(astd->coh_size_of_code);
	m->std.coh_size_of_inited_data		= pe_read_long(astd->coh_size_of_inited_data);
	m->std.coh_size_of_uninited_data	= pe_read_long(astd->coh_size_of_uninited_data);
	m->std.coh_entry_point			= pe_read_long(astd->coh_entry_point);
	m->std.coh_base_of_code			= pe_read_long(astd->coh_base_of_code);

	/* vers */
	m->vers.coh_major_os_ver		= pe_read_short(avers->coh_major_os_ver);
	m->vers.coh_minor_os_ver		= pe_read_short(avers->coh_minor_os_ver);
	m->vers.coh_major_image_ver		= pe_read_short(avers->coh_major_image_ver);
	m->vers.coh_minor_image_ver		= pe_read_short(avers->coh_minor_image_ver);
	m->vers.coh_major_subsys_ver		= pe_read_short(avers->coh_major_subsys_ver);
	m->vers.coh_minor_subsys_ver		= pe_read_short(avers->coh_minor_subsys_ver);

	m->vers.coh_win32_ver			= pe_read_long(avers->coh_win32_ver);

	/* align */
	m->align.coh_section_align		= pe_read_long(aalign->coh_section_align);
	m->align.coh_file_align			= pe_read_long(aalign->coh_file_align);

	/* img */
	m->img.coh_size_of_image		= pe_read_long(aimg->coh_size_of_image);
	m->img.coh_size_of_headers		= pe_read_long(aimg->coh_size_of_headers);
	m->img.coh_checksum			= pe_read_long(aimg->coh_checksum);

	m->img.coh_subsystem			= pe_read_short(aimg->coh_subsystem);
	m->img.coh_dll_characteristics		= pe_read_short(aimg->coh_dll_characteristics);

	/* ldr */
	m->ldr.coh_loader_flags			= pe_read_long(aldr->coh_loader_flags);
	m->ldr.coh_rva_and_sizes		= pe_read_long(aldr->coh_rva_and_sizes);

	/* dirs */
	if (m->ldr.coh_rva_and_sizes > 0x10)
		return PERK_ERR_BAD_IMAGE_TYPE;

	if (m->ldr.coh_rva_and_sizes < 0x10)
		memset(&m->dirs,0,sizeof(m->dirs));

	mark = p->opt_hdr_64.export_tbl;
	pdir = &m->dirs.export_tbl;

	for (i=0; i<m->ldr.coh_rva_and_sizes; i++) {
		pdir[i].rva  = pe_read_long(&mark[i*8]);
		pdir[i].size = pe_read_long(&mark[i*8+4]);
	}

	return 0;
}

int pe_read_optional_header(const union pe_raw_opt_hdr * p, struct pe_meta_opt_hdr * m)
{
	int ret;

	if ((ret = pe_read_optional_header_structs(p,m)))
		return ret;

	switch (m->std.coh_magic) {
		case PE_MAGIC_PE32:
			m->mem.base_of_data		= pe_read_long(p->opt_hdr_32.base_of_data);
			m->mem.image_base		= pe_read_long(p->opt_hdr_32.image_base);
			m->mem.size_of_stack_reserve	= pe_read_long(p->opt_hdr_32.size_of_stack_reserve);
			m->mem.size_of_stack_commit	= pe_read_long(p->opt_hdr_32.size_of_stack_commit);
			m->mem.size_of_heap_reserve	= pe_read_long(p->opt_hdr_32.size_of_heap_reserve);
			m->mem.size_of_heap_commit	= pe_read_long(p->opt_hdr_32.size_of_heap_commit);
			break;

		case PE_MAGIC_PE32_PLUS:
			m->mem.base_of_data		= (uint64_t)-1;
			m->mem.image_base		= pe_read_quad(p->opt_hdr_64.image_base);
			m->mem.size_of_stack_reserve	= pe_read_quad(p->opt_hdr_64.size_of_stack_reserve);
			m->mem.size_of_stack_commit	= pe_read_quad(p->opt_hdr_64.size_of_stack_commit);
			m->mem.size_of_heap_reserve	= pe_read_quad(p->opt_hdr_64.size_of_heap_reserve);
			m->mem.size_of_heap_commit	= pe_read_quad(p->opt_hdr_64.size_of_heap_commit);
			break;
	}

	return 0;
}
