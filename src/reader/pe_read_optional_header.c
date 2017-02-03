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
	const  unsigned char *		etbl;

	struct pe_raw_opt_hdr_std *	astd;
	struct pe_raw_opt_hdr_vers *	avers;
	struct pe_raw_opt_hdr_align *	aalign;
	struct pe_raw_opt_hdr_img *	aimg;
	struct pe_raw_opt_hdr_ldr *	aldr;

	m->oh_std.coh_magic = pe_read_short(p->opt_hdr_32.coh_magic);

	switch (m->oh_std.coh_magic) {
		case PE_MAGIC_PE32:
			astd	= (struct pe_raw_opt_hdr_std *)p;
			avers	= (struct pe_raw_opt_hdr_vers *)&p->opt_hdr_32.coh_major_os_ver;
			aalign	= (struct pe_raw_opt_hdr_align *)&p->opt_hdr_32.coh_section_align;
			aimg	= (struct pe_raw_opt_hdr_img *)&p->opt_hdr_32.coh_size_of_image;
			aldr	= (struct pe_raw_opt_hdr_ldr *)&p->opt_hdr_32.coh_loader_flags;
			etbl	= p->opt_hdr_32.coh_export_tbl;
			break;

		case PE_MAGIC_PE32_PLUS:
			astd	= (struct pe_raw_opt_hdr_std *)p;
			avers	= (struct pe_raw_opt_hdr_vers *)&p->opt_hdr_64.coh_major_os_ver;
			aalign	= (struct pe_raw_opt_hdr_align *)&p->opt_hdr_64.coh_section_align;
			aimg	= (struct pe_raw_opt_hdr_img *)&p->opt_hdr_64.coh_size_of_image;
			aldr	= (struct pe_raw_opt_hdr_ldr *)&p->opt_hdr_64.coh_loader_flags;
			etbl	= p->opt_hdr_64.coh_export_tbl;
			break;

		default:
			return PERK_ERR_BAD_IMAGE_TYPE;
	}

	/* std */
	m->oh_std.coh_major_linker_ver		= astd->coh_major_linker_ver[0];
	m->oh_std.coh_minor_linker_ver		= astd->coh_minor_linker_ver[0];

	m->oh_std.coh_size_of_code		= pe_read_long(astd->coh_size_of_code);
	m->oh_std.coh_size_of_inited_data	= pe_read_long(astd->coh_size_of_inited_data);
	m->oh_std.coh_size_of_uninited_data	= pe_read_long(astd->coh_size_of_uninited_data);
	m->oh_std.coh_entry_point		= pe_read_long(astd->coh_entry_point);
	m->oh_std.coh_base_of_code		= pe_read_long(astd->coh_base_of_code);

	/* vers */
	m->oh_vers.coh_major_os_ver		= pe_read_short(avers->coh_major_os_ver);
	m->oh_vers.coh_minor_os_ver		= pe_read_short(avers->coh_minor_os_ver);
	m->oh_vers.coh_major_image_ver		= pe_read_short(avers->coh_major_image_ver);
	m->oh_vers.coh_minor_image_ver		= pe_read_short(avers->coh_minor_image_ver);
	m->oh_vers.coh_major_subsys_ver		= pe_read_short(avers->coh_major_subsys_ver);
	m->oh_vers.coh_minor_subsys_ver		= pe_read_short(avers->coh_minor_subsys_ver);

	m->oh_vers.coh_win32_ver		= pe_read_long(avers->coh_win32_ver);

	/* align */
	m->oh_align.coh_section_align		= pe_read_long(aalign->coh_section_align);
	m->oh_align.coh_file_align		= pe_read_long(aalign->coh_file_align);

	/* img */
	m->oh_img.coh_size_of_image		= pe_read_long(aimg->coh_size_of_image);
	m->oh_img.coh_size_of_headers		= pe_read_long(aimg->coh_size_of_headers);
	m->oh_img.coh_checksum			= pe_read_long(aimg->coh_checksum);

	m->oh_img.coh_subsystem			= pe_read_short(aimg->coh_subsystem);
	m->oh_img.coh_dll_characteristics	= pe_read_short(aimg->coh_dll_characteristics);

	/* ldr */
	m->oh_ldr.coh_loader_flags		= pe_read_long(aldr->coh_loader_flags);
	m->oh_ldr.coh_rva_and_sizes		= pe_read_long(aldr->coh_rva_and_sizes);

	/* dirs */
	if (m->oh_ldr.coh_rva_and_sizes > 0x10)
		return PERK_ERR_BAD_IMAGE_TYPE;

	if (m->oh_ldr.coh_rva_and_sizes < 0x10)
		memset(&m->oh_dirs,0,sizeof(m->oh_dirs));

	mark = etbl;
	pdir = &m->oh_dirs.coh_export_tbl;

	for (i=0; i<m->oh_ldr.coh_rva_and_sizes; i++) {
		pdir[i].dh_rva  = pe_read_long(&mark[i*8]);
		pdir[i].dh_size = pe_read_long(&mark[i*8+4]);
	}

	return 0;
}

int pe_read_optional_header(const union pe_raw_opt_hdr * p, struct pe_meta_opt_hdr * m)
{
	int ret;

	if ((ret = pe_read_optional_header_structs(p,m)))
		return ret;

	switch (m->oh_std.coh_magic) {
		case PE_MAGIC_PE32:
			m->oh_mem.coh_base_of_data          = pe_read_long(p->opt_hdr_32.coh_base_of_data);
			m->oh_mem.coh_image_base            = pe_read_long(p->opt_hdr_32.coh_image_base);
			m->oh_mem.coh_size_of_stack_reserve = pe_read_long(p->opt_hdr_32.coh_size_of_stack_reserve);
			m->oh_mem.coh_size_of_stack_commit  = pe_read_long(p->opt_hdr_32.coh_size_of_stack_commit);
			m->oh_mem.coh_size_of_heap_reserve  = pe_read_long(p->opt_hdr_32.coh_size_of_heap_reserve);
			m->oh_mem.coh_size_of_heap_commit   = pe_read_long(p->opt_hdr_32.coh_size_of_heap_commit);
			break;

		case PE_MAGIC_PE32_PLUS:
			m->oh_mem.coh_base_of_data          = (uint64_t)-1;
			m->oh_mem.coh_image_base            = pe_read_quad(p->opt_hdr_64.coh_image_base);
			m->oh_mem.coh_size_of_stack_reserve = pe_read_quad(p->opt_hdr_64.coh_size_of_stack_reserve);
			m->oh_mem.coh_size_of_stack_commit  = pe_read_quad(p->opt_hdr_64.coh_size_of_stack_commit);
			m->oh_mem.coh_size_of_heap_reserve  = pe_read_quad(p->opt_hdr_64.coh_size_of_heap_reserve);
			m->oh_mem.coh_size_of_heap_commit   = pe_read_quad(p->opt_hdr_64.coh_size_of_heap_commit);
			break;
	}

	return 0;
}
