/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2017  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <perk/perk.h>
#include "perk_reader_impl.h"
#include "perk_errinfo_impl.h"

static int pe_free_image_meta_impl(struct pe_image_meta * meta, int ret)
{
	int i;

	if (meta) {
		for (i=0; i<meta->summary.nimplibs; i++)
			free(meta->idata[i].ih_items);

		free(meta->idata);
		free(meta->sectbl);
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
	int i; for (i=0; i<m->coff.cfh_num_of_sections; i++)
		if (!(strcmp(name,m->sectbl[i].sh_name)))
			return i;

	return -1;
}

int pe_get_block_section_index(const struct pe_image_meta * m, const struct pe_block * block)
{
	int i;
	uint32_t low,high;

	if (m->aobj)
		return -1;

	for (i=0; i<m->coff.cfh_num_of_sections; i++) {
		low  = m->sectbl[i].sh_virtual_addr;
		high = low + m->sectbl[i].sh_virtual_size;

		if ((block->dh_rva >= low) && (block->dh_rva + block->dh_size <= high))
			return i;
	}

	return -1;
}

int pe_get_roffset_from_rva(const struct pe_image_meta * m, uint32_t rva, uint32_t * roffset)
{
	int i;
	uint32_t low,high;

	for (i=0; i<m->coff.cfh_num_of_sections; i++) {
		low  = m->sectbl[i].sh_virtual_addr;
		high = low + m->sectbl[i].sh_virtual_size;

		if ((rva >= low) && (rva < high)) {
			*roffset = (rva - low) + m->sectbl[i].sh_ptr_to_raw_data;
			return 0;
		}
	}

	return -1;
}

int pe_get_rva_from_roffset(const struct pe_image_meta * m, uint32_t roffset, uint32_t * rva)
{
	int i;
	uint32_t low,high,ref;

	for (i=0, ref=~0; i<m->coff.cfh_num_of_sections; i++) {
		low  = m->sectbl[i].sh_ptr_to_raw_data;
		high = low + m->sectbl[i].sh_virtual_size;

		if ((roffset >= low) && (roffset < high)) {
			*rva = (roffset - low) + m->sectbl[i].sh_virtual_addr;
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

	if (m->aobj)
		return -1;

	offset	= m->hedata->sh_virtual_addr - m->hedata->sh_ptr_to_raw_data;
	symrva	= (uint32_t *)((uintptr_t)m->image.addr + (m->edata.eh_name_ptr_rva - offset));

	for (i=0; i<m->edata.eh_num_of_name_ptrs; i++) {
		sym = (const char *)m->image.addr + symrva[i] - offset;

		if (!(strcmp(sym,name))) {
			if (expsym) {
				expsym->name    = sym;
				expsym->eaddr   = 0;
				expsym->maddr   = 0;
				expsym->roffset = 0;
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

	if (m->aobj)
		return -1;

	if (index >= m->edata.eh_num_of_name_ptrs)
		return -1;

	if (expsym) {
		offset  = m->hedata->sh_virtual_addr - m->hedata->sh_ptr_to_raw_data;
		symrva  = (uint32_t *)((uintptr_t)m->image.addr + (m->edata.eh_name_ptr_rva - offset));
		symaddr = (uintptr_t)m->image.addr + symrva[index] - offset;

		expsym->name    = (const char *)symaddr;
		expsym->eaddr   = 0;
		expsym->maddr   = 0;
		expsym->roffset = 0;
	}

	return 0;
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

	base = image->addr;

	if (!(m = calloc(1,sizeof(*m))))
		return PERK_SYSTEM_ERROR(dctx);

	m->aobj = (struct pe_raw_coff_object_hdr *)base;

	if (pe_read_object_header(m->aobj,&m->coff)) {
		m->aobj = 0;
		m->ados = (struct pe_raw_image_dos_hdr *)base;

		if ((ret = (pe_read_dos_header(m->ados,&m->dos))))
			return pe_free_image_meta_impl(
				m,PERK_CUSTOM_ERROR(dctx,ret));

		m->acoff = (struct pe_raw_coff_image_hdr *)(base + m->dos.dos_lfanew);

		if ((ret = (pe_read_coff_header(m->acoff,&m->coff))))
			return pe_free_image_meta_impl(
				m,PERK_CUSTOM_ERROR(dctx,ret));
	}

	mark  = (const unsigned char *)base + m->coff.cfh_ptr_to_sym_tbl;
	mark += m->coff.cfh_num_of_syms * sizeof(struct pe_raw_coff_symbol);

	if (m->coff.cfh_ptr_to_sym_tbl) {
		m->coff.cfh_ptr_to_str_tbl  = m->coff.cfh_ptr_to_sym_tbl;
		m->coff.cfh_ptr_to_str_tbl += m->coff.cfh_num_of_syms * sizeof(struct pe_raw_coff_symbol);
		m->coff.cfh_size_of_str_tbl = pe_read_long(mark);
	}

	if (m->ados) {
		mark    = &m->acoff->cfh_signature[0];
		m->aopt = (union pe_raw_opt_hdr *)(mark + sizeof(*m->acoff));

		if ((ret = (pe_read_optional_header(m->aopt,&m->opt))))
			return pe_free_image_meta_impl(
				m,PERK_CUSTOM_ERROR(dctx,ret));

		mark       = &m->aopt->opt_hdr_32.coh_magic[0];
		m->asectbl = (struct pe_raw_sec_hdr *)(mark + m->coff.cfh_size_of_opt_hdr);
	} else {
		mark       = &m->aobj->cfh_machine[0];
		m->asectbl = (struct pe_raw_sec_hdr *)(mark + sizeof(*m->aobj));
	}

	if (!(m->sectbl = calloc(m->coff.cfh_num_of_sections,sizeof(*(m->sectbl)))))
		return pe_free_image_meta_impl(
			m,PERK_SYSTEM_ERROR(dctx));

	for (i=0; i<m->coff.cfh_num_of_sections; i++) {
		pe_read_section_header(&m->asectbl[i],&m->sectbl[i]);

		if (m->sectbl[i].sh_name[0] == '/')
			if ((l = strtol(&m->sectbl[i].sh_name[1],0,10)) > 0)
				if (l < m->coff.cfh_size_of_str_tbl)
					m->sectbl[i].sh_long_name = base + m->coff.cfh_ptr_to_str_tbl + l;
	}

	/* .edata */
	i = pe_get_named_section_index(m,".edata");
	s = pe_get_block_section_index(m,&m->opt.oh_dirs.coh_export_tbl);

	if ((i >= 0) && (i != s))
		return pe_free_image_meta_impl(
			m,PERK_CUSTOM_ERROR(dctx,PERK_ERR_IMAGE_MALFORMED));

	if (s >= 0) {
		m->hedata = &m->sectbl[s];
		m->aedata = (struct pe_raw_export_hdr *)(base + m->sectbl[s].sh_ptr_to_raw_data
				+ m->opt.oh_dirs.coh_export_tbl.dh_rva - m->sectbl[s].sh_virtual_addr);
	} else if (i >= 0) {
		m->hedata = &m->sectbl[i];
		m->aedata = (struct pe_raw_export_hdr *)(base + m->sectbl[i].sh_ptr_to_raw_data);
	}

	if (m->aedata) {
		pe_read_export_header(m->aedata,&m->edata);
		m->summary.nexpsyms = m->edata.eh_num_of_name_ptrs;
	}

	/* .idata */
	struct pe_raw_import_hdr * 	pidata;
	union  pe_raw_import_lookup *	pitem;

	i = pe_get_named_section_index(m,".idata");
	s = pe_get_block_section_index(m,&m->opt.oh_dirs.coh_import_tbl);

	if ((i >= 0) && (i != s))
		return pe_free_image_meta_impl(
			m,PERK_CUSTOM_ERROR(dctx,PERK_ERR_IMAGE_MALFORMED));

	if (s >= 0) {
		m->hidata = &m->sectbl[s];
		m->aidata = (struct pe_raw_import_hdr *)(base + m->sectbl[s].sh_ptr_to_raw_data
				+ m->opt.oh_dirs.coh_import_tbl.dh_rva - m->sectbl[s].sh_virtual_addr);
	} else if (i >= 0) {
		m->hidata = &m->sectbl[i];
		m->aidata = (struct pe_raw_import_hdr *)(base + m->sectbl[i].sh_ptr_to_raw_data);
	}

	if (m->aidata) {
		/* num of implibs */
		for (pidata=m->aidata; pe_read_long(pidata->ih_name_rva); pidata++)
			m->summary.nimplibs++;

		/* import headers */
		if (!(m->idata = calloc(m->summary.nimplibs,sizeof(*m->idata))))
			return pe_free_image_meta_impl(
				m,PERK_SYSTEM_ERROR(dctx));

		for (i=0; i<m->summary.nimplibs; i++) {
			pe_read_import_header(&m->aidata[i],&m->idata[i]);

			m->idata[i].ih_name = base + m->hidata->sh_ptr_to_raw_data
						   + m->idata[i].ih_name_rva
						   - m->hidata->sh_virtual_addr;

			if (m->idata[i].ih_import_lookup_tbl_rva)
				m->idata[i].ih_aitems = (union pe_raw_import_lookup *)(base + m->hidata->sh_ptr_to_raw_data
							+ m->idata[i].ih_import_lookup_tbl_rva
							- m->hidata->sh_virtual_addr);

			/* items */
			uint32_t * hint;
			m->idata[i].ih_count = 0;

			if (m->idata[i].ih_import_lookup_tbl_rva) {
				pitem = m->idata[i].ih_aitems;
				hint  = (uint32_t *)pitem->ii_hint_name_tbl_rva;

				for (; *hint; hint=(uint32_t *)((++pitem)->ii_hint_name_tbl_rva))
					m->idata[i].ih_count++;

				if (!(m->idata[i].ih_items = calloc(m->idata[i].ih_count,sizeof(*(m->idata[i].ih_items)))))
					return pe_free_image_meta_impl(
						m,PERK_SYSTEM_ERROR(dctx));
			}

			for (j=0; j<m->idata[i].ih_count; j++) {
				if ((ret = pe_read_import_lookup(
						&(m->idata[i].ih_aitems[j]),
						&(m->idata[i].ih_items[j]),
						m->opt.oh_std.coh_magic)))
					return pe_free_image_meta_impl(
						m,PERK_CUSTOM_ERROR(dctx,ret));

				switch (m->opt.oh_std.coh_magic) {
					case PE_MAGIC_PE32:
						m->idata[i].ih_items[j].ii_flags = m->idata[i].ih_items[j].u.ii_import_lookup_entry_32;
						break;

					case PE_MAGIC_PE32_PLUS:
						m->idata[i].ih_items[j].ii_flags = (m->idata[i].ih_items[j].u.ii_import_lookup_entry_64 >> 32);
						break;
				}

				if (!m->idata[i].ih_items[j].ii_flags) {
					struct pe_raw_hint_name_entry * pentry =
						(struct pe_raw_hint_name_entry *)(base + m->hidata->sh_ptr_to_raw_data
							+ m->idata[i].ih_items[j].u.ii_hint_name_tbl_rva - m->hidata->sh_virtual_addr);

					m->idata[i].ih_items[j].ii_name = (char *)pentry->ii_name;
				}
			}
		}
	}

	/* image */
	m->image.addr = image->addr;
	m->image.size = image->size;

	/* all done */
	*meta = m;
	return 0;
}
