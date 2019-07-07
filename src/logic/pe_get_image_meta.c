/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2017  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <mdso/mdso_specs.h>
#include <mdso/mdso_structs.h>

#include <perk/perk.h>
#include "perk_reader_impl.h"
#include "perk_errinfo_impl.h"

static int pe_free_image_meta_impl(struct pe_image_meta * meta, int ret)
{
	int i;

	if (meta) {
		for (i=0; i<meta->m_stats.t_nimplibs; i++)
			free(meta->m_idata[i].ih_items);

		free(meta->m_idata);
		free(meta->m_sectbl);
		free(meta);
	}

	return ret;
}

void pe_free_image_meta(struct pe_image_meta * meta)
{
	pe_free_image_meta_impl(meta,0);
}

int pe_get_named_section_index(const struct pe_image_meta * m, const char * name)
{
	int i; for (i=0; i<m->m_coff.cfh_num_of_sections; i++)
		if (!(strcmp(name,m->m_sectbl[i].sh_name)))
			return i;

	return -1;
}

int pe_get_block_section_index(const struct pe_image_meta * m, const struct pe_block * block)
{
	int i;
	uint32_t low,high;

	if (m->r_obj)
		return -1;

	for (i=0; i<m->m_coff.cfh_num_of_sections; i++) {
		low  = m->m_sectbl[i].sh_virtual_addr;
		high = low + m->m_sectbl[i].sh_virtual_size;

		if ((block->dh_rva >= low) && (block->dh_rva + block->dh_size <= high))
			return i;
	}

	return -1;
}

int pe_get_roffset_from_rva(const struct pe_image_meta * m, uint32_t rva, uint32_t * roffset)
{
	int i;
	uint32_t low,high;

	for (i=0; i<m->m_coff.cfh_num_of_sections; i++) {
		low  = m->m_sectbl[i].sh_virtual_addr;
		high = low + m->m_sectbl[i].sh_virtual_size;

		if ((rva >= low) && (rva < high)) {
			*roffset = (rva - low) + m->m_sectbl[i].sh_ptr_to_raw_data;
			return 0;
		}
	}

	return -1;
}

int pe_get_rva_from_roffset(const struct pe_image_meta * m, uint32_t roffset, uint32_t * rva)
{
	int i;
	uint32_t low,high,ref;

	for (i=0, ref=~0; i<m->m_coff.cfh_num_of_sections; i++) {
		low  = m->m_sectbl[i].sh_ptr_to_raw_data;
		high = low + m->m_sectbl[i].sh_virtual_size;

		if ((roffset >= low) && (roffset < high)) {
			*rva = (roffset - low) + m->m_sectbl[i].sh_virtual_addr;
			return 0;
		} else if (ref > low) {
			ref = low;
		}
	}

	if (roffset < ref) {
		*rva = roffset;
		return 0;
	}

	return -1;
}

int pe_get_expsym_by_name(
	const struct pe_image_meta *	m,
	const char *			name,
	struct pe_expsym *		expsym)
{
	uint32_t 	offset;
	uint32_t *	symrva;
	const char *	sym;
	unsigned	i;

	if (m->r_obj || !m->h_edata)
		return -1;

	offset	= m->h_edata->sh_virtual_addr - m->h_edata->sh_ptr_to_raw_data;
	symrva	= (uint32_t *)((uintptr_t)m->r_image.map_addr + (m->m_edata.eh_name_ptr_rva - offset));

	for (i=0; i<m->m_edata.eh_num_of_name_ptrs; i++) {
		sym = (const char *)m->r_image.map_addr + symrva[i] - offset;

		if (!(strcmp(sym,name))) {
			if (expsym) {
				expsym->s_name    = sym;
				expsym->s_eaddr   = 0;
				expsym->s_maddr   = 0;
				expsym->s_roffset = 0;
			}

			return 0;
		}
	}

	return -1;
}

int pe_get_expsym_by_index(
	const struct pe_image_meta *	m,
	unsigned			index,
	struct pe_expsym *		expsym)
{
	uint32_t 	offset;
	uint32_t *	symrva;
	uintptr_t	symaddr;

	if (m->r_obj)
		return -1;

	if (index >= m->m_edata.eh_num_of_name_ptrs)
		return -1;

	if (expsym) {
		offset  = m->h_edata->sh_virtual_addr - m->h_edata->sh_ptr_to_raw_data;
		symrva  = (uint32_t *)((uintptr_t)m->r_image.map_addr + (m->m_edata.eh_name_ptr_rva - offset));
		symaddr = (uintptr_t)m->r_image.map_addr + symrva[index] - offset;

		expsym->s_name    = (const char *)symaddr;
		expsym->s_eaddr   = 0;
		expsym->s_maddr   = 0;
		expsym->s_roffset = 0;
	}

	return 0;
}

static void pe_detect_image_abi(struct pe_image_meta * m)
{
	int abi;

	if (m->r_obj) {
		switch (m->m_coff.cfh_machine) {
			case PE_IMAGE_FILE_MACHINE_I386:
				abi = PE_ABI_PE32;
				break;

			case PE_IMAGE_FILE_MACHINE_IA64:
			case PE_IMAGE_FILE_MACHINE_AMD64:
				abi = PE_ABI_PE64;
				break;

			default:
				abi = PE_ABI_UNSUPPORTED;
				break;
		}
	} else {
		switch (m->m_opt.oh_std.coh_magic) {
			case PE_MAGIC_PE32:
				abi = PE_ABI_PE32;
				break;

			case PE_MAGIC_PE32_PLUS:
				abi = PE_ABI_PE64;
				break;

			default:
				abi = PE_ABI_UNSUPPORTED;
				break;
		}
	}

	m->m_abi = abi;
}

static void pe_detect_image_subtype(struct pe_image_meta * m)
{
	int subtype;

	if (m->r_obj)
		subtype = PE_SUBTYPE_OBJ;

	else if (m->m_coff.cfh_characteristics & PE_IMAGE_FILE_DLL)
		subtype = PE_SUBTYPE_DLL;

	else
		subtype = PE_SUBTYPE_EXE;

	m->m_subtype = subtype;
}

static bool pe_image_is_psxscl(const struct pe_image_meta * m)
{
	return (!m->m_stats.t_nimplibs
		&& !pe_get_expsym_by_name(m,"__psx_init",0));
}

static bool pe_image_is_cygwin(const struct pe_image_meta * m)
{
	int i;

	for (i=0; i<m->m_stats.t_nimplibs; i++)
		if (!(strcmp(m->m_idata[i].ih_name,"cygwin1.dll")))
			return true;

	return false;
}

static bool pe_image_is_msys(const struct pe_image_meta * m)
{
	int i;

	for (i=0; i<m->m_stats.t_nimplibs; i++)
		if (!(strcmp(m->m_idata[i].ih_name,"msys-2.0.dll")))
			return true;

	return false;
}

static bool pe_image_is_mingw(const struct pe_image_meta * m)
{
	return ((pe_get_named_section_index(m,".CRT") >= 0)
		&& (pe_get_named_section_index(m,".bss") >= 0)
		&& (pe_get_named_section_index(m,".tls") >= 0));
}

static void pe_detect_image_framework(struct pe_image_meta * m)
{
	int framework;

	if (pe_get_named_section_index(m,".midipix") >= 0)
		framework = PE_FRAMEWORK_MIDIPIX;

	else if (pe_get_named_section_index(m,".freestd") >= 0)
		framework = PE_FRAMEWORK_FREESTD;

	else if (pe_get_named_section_index(m,".cygheap") >= 0)
		framework = PE_FRAMEWORK_CYGONE;

	else if (pe_image_is_psxscl(m))
		framework = PE_FRAMEWORK_PSXSCL;

	else if (pe_image_is_cygwin(m))
		framework = PE_FRAMEWORK_CYGWIN;

	else if (pe_image_is_msys(m))
		framework = PE_FRAMEWORK_MSYS;

	else if (pe_image_is_mingw(m))
		framework = PE_FRAMEWORK_MINGW;

	else if (m->m_opt.oh_img.coh_subsystem == PE_IMAGE_SUBSYSTEM_POSIX_CUI)
		framework = PE_FRAMEWORK_SUACON;

	else if (m->m_opt.oh_img.coh_subsystem == PE_IMAGE_SUBSYSTEM_WINDOWS_CUI)
		framework = PE_FRAMEWORK_WINCON;

	else if (m->m_opt.oh_img.coh_subsystem == PE_IMAGE_SUBSYSTEM_WINDOWS_GUI)
		framework = PE_FRAMEWORK_WIN32;

	else
		framework = PE_FRAMEWORK_UNKNOWN;

	m->m_framework = framework;
}

int pe_get_image_meta(
	const struct pe_driver_ctx *	dctx,
	const struct pe_raw_image *	image,
	struct pe_image_meta ** meta)
{
	int			ret;
	int 			i,s;
	long			l;
	unsigned		j;
	const unsigned char *	mark;
	struct pe_image_meta *	m;
	char *			base;
	uint64_t		vaddr;

	base = image->map_addr;

	if (!(m = calloc(1,sizeof(*m))))
		return PERK_SYSTEM_ERROR(dctx);

	m->r_obj = (struct pe_raw_coff_object_hdr *)base;

	if (pe_read_object_header(m->r_obj,&m->m_coff)) {
		m->r_obj = 0;
		m->r_dos = (struct pe_raw_image_dos_hdr *)base;

		if ((ret = (pe_read_dos_header(m->r_dos,&m->m_dos))))
			return pe_free_image_meta_impl(
				m,PERK_CUSTOM_ERROR(dctx,ret));

		m->r_coff = (struct pe_raw_coff_image_hdr *)(base + m->m_dos.dos_lfanew);

		if ((ret = (pe_read_coff_header(m->r_coff,&m->m_coff))))
			return pe_free_image_meta_impl(
				m,PERK_CUSTOM_ERROR(dctx,ret));
	}

	mark  = (const unsigned char *)base + m->m_coff.cfh_ptr_to_sym_tbl;
	mark += m->m_coff.cfh_num_of_syms * sizeof(struct pe_raw_coff_symbol);

	if (m->m_coff.cfh_ptr_to_sym_tbl) {
		m->m_coff.cfh_ptr_to_str_tbl  = m->m_coff.cfh_ptr_to_sym_tbl;
		m->m_coff.cfh_ptr_to_str_tbl += m->m_coff.cfh_num_of_syms * sizeof(struct pe_raw_coff_symbol);
		m->m_coff.cfh_size_of_str_tbl = pe_read_long(mark);
	}

	if (m->r_dos) {
		mark    = &m->r_coff->cfh_signature[0];
		m->r_opt = (union pe_raw_opt_hdr *)(mark + sizeof(*m->r_coff));

		if ((ret = (pe_read_optional_header(m->r_opt,&m->m_opt))))
			return pe_free_image_meta_impl(
				m,PERK_CUSTOM_ERROR(dctx,ret));

		mark       = &m->r_opt->opt_hdr_32.coh_magic[0];
		m->r_sectbl = (struct pe_raw_sec_hdr *)(mark + m->m_coff.cfh_size_of_opt_hdr);
	} else {
		mark       = &m->r_obj->cfh_machine[0];
		m->r_sectbl = (struct pe_raw_sec_hdr *)(mark + sizeof(*m->r_obj));
	}

	if (!(m->m_sectbl = calloc(m->m_coff.cfh_num_of_sections,sizeof(*(m->m_sectbl)))))
		return pe_free_image_meta_impl(
			m,PERK_SYSTEM_ERROR(dctx));

	for (i=0; i<m->m_coff.cfh_num_of_sections; i++) {
		pe_read_section_header(&m->r_sectbl[i],&m->m_sectbl[i]);

		if (m->m_sectbl[i].sh_name[0] == '/')
			if ((l = strtol(&m->m_sectbl[i].sh_name[1],0,10)) > 0)
				if (l < m->m_coff.cfh_size_of_str_tbl)
					m->m_sectbl[i].sh_long_name = base + m->m_coff.cfh_ptr_to_str_tbl + l;
	}

	/* .edata */
	i = pe_get_named_section_index(m,".edata");
	s = pe_get_block_section_index(m,&m->m_opt.oh_dirs.coh_export_tbl);

	if ((i >= 0) && (i != s))
		return pe_free_image_meta_impl(
			m,PERK_CUSTOM_ERROR(dctx,PERK_ERR_IMAGE_MALFORMED));

	if (s >= 0) {
		m->h_edata = &m->m_sectbl[s];
		m->r_edata = (struct pe_raw_export_hdr *)(base + m->m_sectbl[s].sh_ptr_to_raw_data
				+ m->m_opt.oh_dirs.coh_export_tbl.dh_rva - m->m_sectbl[s].sh_virtual_addr);
		m->m_edata.eh_virtual_addr = m->m_opt.oh_dirs.coh_export_tbl.dh_rva;
	} else if (i >= 0) {
		m->h_edata = &m->m_sectbl[i];
		m->r_edata = (struct pe_raw_export_hdr *)(base + m->m_sectbl[i].sh_ptr_to_raw_data);
		m->m_edata.eh_virtual_addr = m->m_sectbl[i].sh_virtual_addr;
	}

	if (m->r_edata) {
		pe_read_export_header(m->r_edata,&m->m_edata);
		m->m_stats.t_nexpsyms = m->m_edata.eh_num_of_name_ptrs;
	}

	/* .idata */
	struct pe_raw_import_hdr * 	pidata;
	unsigned char *			pitem;
	size_t				psize;

	i = pe_get_named_section_index(m,".idata");
	s = pe_get_block_section_index(m,&m->m_opt.oh_dirs.coh_import_tbl);

	if ((i >= 0) && (i != s))
		return pe_free_image_meta_impl(
			m,PERK_CUSTOM_ERROR(dctx,PERK_ERR_IMAGE_MALFORMED));

	if (s >= 0) {
		m->h_idata = &m->m_sectbl[s];
		m->r_idata = (struct pe_raw_import_hdr *)(base + m->m_sectbl[s].sh_ptr_to_raw_data
				+ m->m_opt.oh_dirs.coh_import_tbl.dh_rva - m->m_sectbl[s].sh_virtual_addr);
		vaddr = m->m_opt.oh_dirs.coh_import_tbl.dh_rva;
	} else if (i >= 0) {
		m->h_idata = &m->m_sectbl[i];
		m->r_idata = (struct pe_raw_import_hdr *)(base + m->m_sectbl[i].sh_ptr_to_raw_data);
		vaddr = m->m_sectbl[i].sh_virtual_addr;
	}

	if (m->r_idata) {
		/* num of implibs */
		for (pidata=m->r_idata; pe_read_long(pidata->ih_name_rva); pidata++)
			m->m_stats.t_nimplibs++;

		/* import headers */
		if (!(m->m_idata = calloc(m->m_stats.t_nimplibs,sizeof(*m->m_idata))))
			return pe_free_image_meta_impl(
				m,PERK_SYSTEM_ERROR(dctx));

		for (i=0; i<m->m_stats.t_nimplibs; i++) {
			m->m_idata[i].ih_virtual_addr = vaddr + (i * sizeof(*m->r_idata));
			pe_read_import_header(&m->r_idata[i],&m->m_idata[i]);

			m->m_idata[i].ih_name = base + m->h_idata->sh_ptr_to_raw_data
						   + m->m_idata[i].ih_name_rva
						   - m->h_idata->sh_virtual_addr;

			if (m->m_idata[i].ih_import_lookup_tbl_rva)
				m->m_idata[i].ih_aitems = (union pe_raw_import_lookup *)(base + m->h_idata->sh_ptr_to_raw_data
							+ m->m_idata[i].ih_import_lookup_tbl_rva
							- m->h_idata->sh_virtual_addr);

			/* items */
			if (m->m_idata[i].ih_import_lookup_tbl_rva) {
				if (m->m_opt.oh_std.coh_magic == PE_MAGIC_PE32) {
					pitem = m->m_idata[i].ih_aitems->ii_import_lookup_entry_32;

					for (; pe_read_long(pitem); m->m_idata[i].ih_count++)
						pitem += sizeof(uint32_t);
				} else if (m->m_opt.oh_std.coh_magic == PE_MAGIC_PE32_PLUS) {
					pitem = m->m_idata[i].ih_aitems->ii_import_lookup_entry_64;

					for (; pe_read_quad(pitem); m->m_idata[i].ih_count++)
						pitem += sizeof(uint64_t);
				} else {
					return pe_free_image_meta_impl(
						m,PERK_CUSTOM_ERROR(
							dctx,PERK_ERR_UNSUPPORTED_ABI));
				}

				if (!(m->m_idata[i].ih_items = calloc(m->m_idata[i].ih_count,sizeof(*(m->m_idata[i].ih_items)))))
					return pe_free_image_meta_impl(
						m,PERK_SYSTEM_ERROR(dctx));
			}

			switch (m->m_opt.oh_std.coh_magic) {
				case PE_MAGIC_PE32:
					pitem = m->m_idata[i].ih_aitems->ii_import_lookup_entry_32;
					psize = sizeof(uint32_t);
					break;

				case PE_MAGIC_PE32_PLUS:
					pitem = m->m_idata[i].ih_aitems->ii_import_lookup_entry_64;
					psize = sizeof(uint64_t);
					break;
			}

			for (j=0; j<m->m_idata[i].ih_count; j++) {
				if ((ret = pe_read_import_lookup(
						pitem + j*psize,
						&(m->m_idata[i].ih_items[j]),
						m->m_opt.oh_std.coh_magic)))
					return pe_free_image_meta_impl(
						m,PERK_CUSTOM_ERROR(dctx,ret));

				if (!m->m_idata[i].ih_items[j].ii_flag) {
					struct pe_raw_hint_name_entry * pentry =
						(struct pe_raw_hint_name_entry *)(base + m->h_idata->sh_ptr_to_raw_data
							+ m->m_idata[i].ih_items[j].ii_hint_name_tbl_rva - m->h_idata->sh_virtual_addr);

					m->m_idata[i].ih_items[j].ii_hint = pe_read_short(pentry->ii_hint);
					m->m_idata[i].ih_items[j].ii_name = (char *)pentry->ii_name;
				}
			}
		}
	}

	/* .dsometa */
	if ((i = pe_get_named_section_index(m,MDSO_META_SECTION)) >= 0) {
		m->h_dsometa = &m->m_sectbl[i];
		m->r_dsometa = base + m->m_sectbl[i].sh_ptr_to_raw_data;

		m->m_stats.t_ndsolibs = (m->m_opt.oh_std.coh_magic == PE_MAGIC_PE32_PLUS)
			? m->h_dsometa->sh_virtual_size / sizeof(struct mdso_raw_meta_record_m64)
			: m->h_dsometa->sh_virtual_size / sizeof(struct mdso_raw_meta_record_m32);
	}

	/* .dsosyms */
	if ((i = pe_get_named_section_index(m,MDSO_SYMS_SECTION)) >= 0) {
		m->h_dsosyms = &m->m_sectbl[i];
		m->r_dsosyms = base + m->m_sectbl[i].sh_ptr_to_raw_data;

		m->m_stats.t_ndsosyms = (m->m_opt.oh_std.coh_magic == PE_MAGIC_PE32_PLUS)
			? m->h_dsosyms->sh_virtual_size / sizeof(struct mdso_raw_sym_entry_m64)
			: m->h_dsosyms->sh_virtual_size / sizeof(struct mdso_raw_sym_entry_m32);
	}

	/* .dsostrs */
	if ((i = pe_get_named_section_index(m,MDSO_STRS_SECTION)) >= 0) {
		m->h_dsostrs = &m->m_sectbl[i];
		m->r_dsostrs = base + m->m_sectbl[i].sh_ptr_to_raw_data;
	}

	/* .dsodata */
	if ((i = pe_get_named_section_index(m,MDSO_DATA_SECTION)) >= 0) {
		m->h_dsodata = &m->m_sectbl[i];
		m->r_dsodata = base + m->m_sectbl[i].sh_ptr_to_raw_data;
	}

	/* image */
	m->r_image.map_addr = image->map_addr;
	m->r_image.map_size = image->map_size;

	/* info */
	pe_detect_image_abi(m);
	pe_detect_image_subtype(m);
	pe_detect_image_framework(m);

	/* mdso abi */
	if (m->h_dsometa || m->h_dsosyms)
		if (m->m_abi == PE_ABI_UNSUPPORTED)
			return pe_free_image_meta_impl(
				m,PERK_CUSTOM_ERROR(
					dctx,PERK_ERR_UNSUPPORTED_ABI));


	/* all done */
	*meta = m;
	return 0;
}
