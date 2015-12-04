#include <endian.h>
#include <string.h>

#include <perk/perk.h>
#include "perk_reader_impl.h"

int pe_read_optional_header(const union pe_opt_hdr * p, struct pe_meta_opt_hdr * m)
{
	m->std.magic = pe_read_short(p->opt_hdr_32.magic);
	memset(&m->dirs,0,sizeof(m->dirs));

	#if (BYTE_ORDER == LITTLE_ENDIAN)

	memcpy(&m->std,p,sizeof(struct pe_meta_opt_hdr_std));

	switch (m->std.magic) {
		case PE_MAGIC_PE32:
			memcpy(&m->vers,&p->opt_hdr_32.major_os_ver,sizeof(struct pe_meta_opt_hdr_vers));
			memcpy(&m->align,&p->opt_hdr_32.section_align,sizeof(struct pe_meta_opt_hdr_align));
			memcpy(&m->img,&p->opt_hdr_32.size_of_image,sizeof(struct pe_meta_opt_hdr_img));
			memcpy(&m->ldr,&p->opt_hdr_32.loader_flags,sizeof(struct pe_meta_opt_hdr_ldr));

			if (m->ldr.rva_and_sizes > 0x10)
				return PERK_BAD_IMAGE_TYPE;
			else
				memcpy(&m->dirs,&p->opt_hdr_32.export_tbl,sizeof(struct pe_meta_opt_hdr_dirs));

			break;

		case PE_MAGIC_PE32_PLUS:
			memcpy(&m->vers,&p->opt_hdr_64.major_os_ver,sizeof(struct pe_meta_opt_hdr_vers));
			memcpy(&m->align,&p->opt_hdr_64.section_align,sizeof(struct pe_meta_opt_hdr_align));
			memcpy(&m->img,&p->opt_hdr_64.size_of_image,sizeof(struct pe_meta_opt_hdr_img));
			memcpy(&m->ldr,&p->opt_hdr_64.loader_flags,sizeof(struct pe_meta_opt_hdr_ldr));

			if (m->ldr.rva_and_sizes > 0x10)
				return PERK_BAD_IMAGE_TYPE;
			else
				memcpy(&m->dirs,&p->opt_hdr_64.export_tbl,sizeof(struct pe_meta_opt_hdr_dirs));

			break;

		default:
			return PERK_BAD_IMAGE_TYPE;
	};

	#else

	struct pe_opt_hdr_std *		astd;
	struct pe_opt_hdr_vers *	avers;
	struct pe_opt_hdr_align *	aalign;
	struct pe_opt_hdr_img *		aimg;
	struct pe_opt_hdr_ldr *		aldr;
	struct pe_opt_hdr_dirs *	adirs;
	size_t				sdirs;

	astd = (struct pe_opt_hdr_std *)p;

	switch (m->std.magic) {
		case PE_MAGIC_PE32:
			avers	= (struct pe_opt_hdr_vers *)&p->opt_hdr_32.major_os_ver;
			aalign	= (struct pe_opt_hdr_align *)&p->opt_hdr_32.section_align;
			aimg	= (struct pe_opt_hdr_img *)&p->opt_hdr_32.size_of_image;
			aldr	= (struct pe_opt_hdr_ldr *)&p->opt_hdr_32.loader_flags;
			adirs	= (struct pe_opt_hdr_dirs *)&p->opt_hdr_32.export_tbl;
			break;

		case PE_MAGIC_PE32_PLUS:
			avers	= (struct pe_opt_hdr_vers *)&p->opt_hdr_64.major_os_ver;
			aalign	= (struct pe_opt_hdr_align *)&p->opt_hdr_64.section_align;
			aimg	= (struct pe_opt_hdr_img *)&p->opt_hdr_64.size_of_image;
			aldr	= (struct pe_opt_hdr_ldr *)&p->opt_hdr_64.loader_flags;
			adirs	= (struct pe_opt_hdr_dirs *)&p->opt_hdr_64.export_tbl;
			break;

		default:
			return PERK_BAD_IMAGE_TYPE;
	}

	/* std */
	m->std.major_linker_ver			= astd->major_linker_ver[0];
	m->std.minor_linker_ver			= astd->minor_linker_ver[0];

	m->std.size_of_code			= pe_read_long(astd->size_of_code);
	m->std.size_of_initialized_data		= pe_read_long(astd->size_of_initialized_data);
	m->std.size_of_uninitialized_data	= pe_read_long(astd->size_of_uninitialized_data);
	m->std.entry_point			= pe_read_long(astd->entry_point);
	m->std.base_of_code			= pe_read_long(astd->base_of_code);

	/* vers */
	m->vers.major_os_ver			= pe_read_short(avers->major_os_ver);
	m->vers.minor_os_ver			= pe_read_short(avers->minor_os_ver);
	m->vers.major_image_ver			= pe_read_short(avers->major_image_ver);
	m->vers.minor_image_ver			= pe_read_short(avers->minor_image_ver);
	m->vers.major_subsys_ver		= pe_read_short(avers->major_subsys_ver);
	m->vers.minor_subsys_ver		= pe_read_short(avers->minor_subsys_ver);

	m->vers.win32_ver			= pe_read_long(avers->win32_ver);

	/* align */
	m->align.section_align			= pe_read_long(aalign->section_align);
	m->align.file_align			= pe_read_long(aalign->file_align);

	/* img */
	m->img.size_of_image			= pe_read_long(aimg->size_of_image);
	m->img.size_of_headers			= pe_read_long(aimg->size_of_headers);
	m->img.checksum				= pe_read_long(aimg->checksum);

	m->img.subsystem			= pe_read_short(aimg->subsystem);
	m->img.dll_characteristics		= pe_read_short(aimg->dll_characteristics);

	/* ldr */
	m->ldr.loader_flags			= pe_read_long(aldr->loader_flags);
	m->ldr.rva_and_sizes			= pe_read_long(aldr->rva_and_sizes);

	#endif

	switch (m->std.magic) {
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
	};

	return 0;
}
