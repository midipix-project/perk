/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2025  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <mdso/mdso_specs.h>
#include <mdso/mdso_structs.h>

#include <perk/perk.h>
#include <perk/perk_consts.h>
#include <perk/perk_structs.h>
#include "perk_endian_impl.h"
#include "perk_reader_impl.h"
#include "perk_errinfo_impl.h"

static int pe_free_image_meta_impl(struct pe_image_meta * meta, int ret)
{
	int i;

	if (meta) {
		for (i=0; i<meta->m_stats.t_nimplibs; i++)
			free(meta->m_idata[i].ih_items);

		free(meta->m_symvec_crc32);
		free(meta->m_symvec_crc64);
		free(meta->m_symvec_symidx);

		free(meta->m_idata);
		free(meta->m_symtbl);
		free(meta->m_sectbl);

		free(meta->r_reltbl);

		free(meta);
	}

	return ret;
}

void pe_meta_free_image_meta(struct pe_image_meta * meta)
{
	pe_free_image_meta_impl(meta,0);
}

static int pe_symrec_crc32_compare(const void * a, const void * b)
{
	struct pe_meta_coff_symbol * syma = *(struct pe_meta_coff_symbol **)a;
	struct pe_meta_coff_symbol * symb = *(struct pe_meta_coff_symbol **)b;
	int                          eqor = !!(syma->cs_crc32 - symb->cs_crc32);

	return eqor * (syma->cs_crc32 > symb->cs_crc32 ? (1) : (-1));
}

static int pe_symrec_crc64_compare(const void * a, const void * b)
{
	struct pe_meta_coff_symbol * syma = *(struct pe_meta_coff_symbol **)a;
	struct pe_meta_coff_symbol * symb = *(struct pe_meta_coff_symbol **)b;
	int                          eqor = !!(syma->cs_crc64 - symb->cs_crc64);

	return eqor * (syma->cs_crc64 > symb->cs_crc64 ? (1) : (-1));
}

static int pe_get_named_section_index(
	const struct pe_image_meta *  m,
	const char *                  name)
{
	int i; for (i=0; i<m->m_coff.cfh_num_of_sections; i++)
		if (!(strcmp(name,m->m_sectbl[i].sh_name)))
			return i;

	return -1;
}

int pe_meta_get_named_section_index(
	const struct pe_image_meta *  m,
	const char *                  name)
{
	return pe_get_named_section_index(m,name);
}

static int pe_get_block_section_index(
	const struct pe_image_meta *  m,
	const struct pe_block *       block)
{
	int i;
	uint32_t low,high;

	if (m->r_obj)
		return -1;

	for (i=0; i<m->m_coff.cfh_num_of_sections; i++) {
		low  = m->m_sectbl[i].sh_virtual_addr;
		high = low + m->m_sectbl[i].sh_virtual_size;

		if (block->dh_rva >= low)
			if (block->dh_rva + block->dh_size <= high)
				return i;
	}

	return -1;
}

int pe_meta_get_block_section_index(
	const struct pe_image_meta *  m,
	const struct pe_block *       block)
{
	return pe_get_block_section_index(m,block);
}

int pe_meta_get_roffset_from_rva(
	const struct pe_image_meta *  m,
	uint32_t                      rva,
	uint32_t *                    roffset)
{
	int      i;
	uint32_t offset;
	uint32_t low,high;

	for (i=0; i<m->m_coff.cfh_num_of_sections; i++) {
		low  = m->m_sectbl[i].sh_virtual_addr;
		high = low + m->m_sectbl[i].sh_virtual_size;

		if ((rva >= low) && (rva < high)) {
			offset   = m->m_sectbl[i].sh_ptr_to_raw_data;
			offset  += rva - low;

			*roffset = offset;

			return 0;
		}
	}

	return -1;
}

int pe_meta_get_rva_from_roffset(
	const struct pe_image_meta *  m,
	uint32_t                      roffset,
	uint32_t *                    rva)
{
	int      i;
	uint32_t low,high,ref;

	for (i=0, ref=~0; i<m->m_coff.cfh_num_of_sections; i++) {
		low  = m->m_sectbl[i].sh_ptr_to_raw_data;
		high = low + m->m_sectbl[i].sh_virtual_size;

		if ((roffset >= low) && (roffset < high)) {
			roffset -= low;
			roffset += m->m_sectbl[i].sh_virtual_addr;

			*rva = roffset;

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

static int pe_get_expsym_by_name(
	const struct pe_image_meta *	m,
	const char *			name,
	struct pe_expsym *		expsym)
{
	uint32_t              offset;
	const unsigned char * ptrtbl;
	const char *	      sym;
	unsigned              i;

	if (m->r_obj || !m->h_edata)
		return -1;

	offset	= m->h_edata->sh_virtual_addr;
	offset -= m->h_edata->sh_ptr_to_raw_data;

	ptrtbl  = m->r_image.map_addr;
	ptrtbl += m->m_edata.eh_name_ptr_rva;
	ptrtbl -= offset;

	for (i=0; i<m->m_edata.eh_num_of_name_ptrs; i++) {
		sym  = m->r_image.map_addr;
		sym += pe_read_long(ptrtbl) - offset;

		if (!(strcmp(sym,name))) {
			if (expsym) {
				expsym->s_name    = sym;
				expsym->s_eaddr   = 0;
				expsym->s_maddr   = 0;
				expsym->s_roffset = 0;
			}

			return 0;
		}

		ptrtbl += sizeof(uint32_t);
	}

	return -1;
}

int pe_meta_get_expsym_by_name(
	const struct pe_image_meta *	m,
	const char *			name,
	struct pe_expsym *		expsym)
{
	return pe_get_expsym_by_name(m,name,expsym);
}

static int pe_get_expsym_by_index(
	const struct pe_image_meta *	m,
	unsigned			index,
	struct pe_expsym *		expsym)
{
	uint32_t              offset;
	const unsigned char * symptr;
	const char *          sym;

	if (m->r_obj)
		return -1;

	if (index >= m->m_edata.eh_num_of_name_ptrs)
		return -1;

	if (expsym) {
		offset  = m->h_edata->sh_virtual_addr;
		offset -= m->h_edata->sh_ptr_to_raw_data;

		symptr  = m->r_image.map_addr;
		symptr += m->m_edata.eh_name_ptr_rva - offset;
		symptr += index * sizeof(uint32_t);

		sym  = m->r_image.map_addr;
		sym += pe_read_long(symptr) - offset;

		expsym->s_name    = sym;
		expsym->s_eaddr   = 0;
		expsym->s_maddr   = 0;
		expsym->s_roffset = 0;
	}

	return 0;
}


int pe_meta_get_expsym_by_index(
	const struct pe_image_meta *	m,
	unsigned			index,
	struct pe_expsym *		expsym)
{
	return pe_get_expsym_by_index(m,index,expsym);
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

int pe_meta_get_image_meta(
	const struct pe_driver_ctx *    dctx,
	const struct pe_raw_image *     image,
	struct pe_image_meta **         meta)
{
	int                             ret;
	int                             i,s;
	long                            l;
	unsigned                        j;

	void *                          addr;
	char *                          sptr;
	unsigned char *                 base;
	const unsigned char *           rtbl;
	const unsigned char *           mark;
	const unsigned char *           cap;
	uint64_t                        vaddr;
	uint32_t                        strtbl;
	uint32_t                        symtbl;
	uint32_t                        arroff;

	uint32_t                        tbllen;
	uint32_t                        reclen;

	struct pe_image_meta *          m;
	struct pe_meta_coff_symbol *    symrec;
	union pe_raw_import_lookup *    imptbl;
	int                             nrecs;
	int                             nsyms;


	/* mapped pe/coff or archive member data */
	base = image->map_addr;
	sptr = image->map_addr;


	/* context allocation */
	if (!(m = calloc(1,sizeof(*m))))
		return PERK_SYSTEM_ERROR(dctx);


	/* image dos header, coff object header */
	m->r_obj = (struct pe_raw_coff_object_hdr *)base;

	if (pe_read_object_header(m->r_obj,&m->m_coff)) {
		m->r_obj = 0;
		m->r_dos = (struct pe_raw_image_dos_hdr *)base;

		if ((ret = (pe_read_dos_header(m->r_dos,&m->m_dos))))
			return pe_free_image_meta_impl(
				m,PERK_CUSTOM_ERROR(dctx,ret));

		mark      = &base[m->m_dos.dos_lfanew];
		m->r_coff = (struct pe_raw_coff_image_hdr *)mark;

		if ((ret = (pe_read_coff_header(m->r_coff,&m->m_coff))))
			return pe_free_image_meta_impl(
				m,PERK_CUSTOM_ERROR(dctx,ret));
	}


	/* symbol table & string table */
	symtbl = m->m_coff.cfh_ptr_to_sym_tbl;
	tbllen = m->m_coff.cfh_size_of_sym_tbl;
	reclen = sizeof(struct pe_raw_coff_symbol);
	strtbl = symtbl + tbllen;


	if (symtbl) {
		mark        = &base[symtbl];
		m->r_symtbl = (struct pe_raw_coff_symbol *)mark;

		m->m_coff.cfh_ptr_to_str_tbl  = strtbl;
		m->m_coff.cfh_size_of_str_tbl = pe_read_long(&base[strtbl]);
	}


	if ((nrecs = tbllen/reclen)) {
		if (!(m->m_symtbl = calloc(
				nrecs+1,
				sizeof(struct pe_meta_coff_symbol))))
			return PERK_SYSTEM_ERROR(dctx);

		if (!(m->m_symvec_symidx = calloc(
				nrecs,
				sizeof(struct pe_meta_coff_symbol *))))
			return PERK_SYSTEM_ERROR(dctx);
	}


	for (i=0,symrec=m->m_symtbl; i<nrecs; i++,symrec++) {
		m->m_symvec_symidx[i] = symrec;

		pe_read_coff_symbol(
			&m->r_symtbl[i],symrec,
			&m->m_coff,base);

		addr = symrec->cs_name;
		mark = addr;

		symrec->cs_crc32 = pe_hash_mbstr_crc32(mark);
		symrec->cs_crc64 = pe_hash_mbstr_crc64(mark);

		i += m->r_symtbl[i].cs_num_of_aux_recs[0];
	}

	m->m_stats.t_nsymbols = symrec - m->m_symtbl;

	if ((nsyms = m->m_stats.t_nsymbols) && true) {
		if (!(m->m_symvec_crc32 = calloc(
				nsyms,
				sizeof(m->m_symvec_crc32[0]))))
			return PERK_SYSTEM_ERROR(dctx);

		for (i=0; i<nsyms; i++)
			m->m_symvec_crc32[i] = &m->m_symtbl[i];

		qsort(
			m->m_symvec_crc32,nsyms,
			sizeof(m->m_symvec_crc32[0]),
			pe_symrec_crc32_compare);
	}

	if (nsyms && true) {
		if (!(m->m_symvec_crc64 = calloc(
				nsyms,
				sizeof(m->m_symvec_crc64[0]))))
			return PERK_SYSTEM_ERROR(dctx);

		for (i=0; i<nsyms; i++)
			m->m_symvec_crc64[i] = &m->m_symtbl[i];

		qsort(
			m->m_symvec_crc64,nsyms,
			sizeof(m->m_symvec_crc64[0]),
			pe_symrec_crc64_compare);
	}


	/* optional header & section table */
	if (m->r_dos) {
		mark     = &m->r_coff->cfh_signature[0];
		mark    += sizeof(m->r_coff[0]);
		m->r_opt = (union pe_raw_opt_hdr *)mark;

		if ((ret = (pe_read_optional_header(m->r_opt,&m->m_opt))))
			return pe_free_image_meta_impl(
				m,PERK_CUSTOM_ERROR(dctx,ret));

		mark         = &m->r_opt->opt_hdr_32.coh_magic[0];
		mark        += m->m_coff.cfh_size_of_opt_hdr;
		m->r_sectbl  = (struct pe_raw_sec_hdr *)mark;
	} else {
		mark         = &m->r_obj->cfh_machine[0];
		mark        += sizeof(m->r_obj[0]);
		m->r_sectbl  = (struct pe_raw_sec_hdr *)mark;
	}

	if (!(m->m_sectbl = calloc(
			m->m_coff.cfh_num_of_sections,
			sizeof(m->m_sectbl[0]))))
		return pe_free_image_meta_impl(
			m,PERK_SYSTEM_ERROR(dctx));

	for (i=0; i<m->m_coff.cfh_num_of_sections; i++) {
		pe_read_section_header(&m->r_sectbl[i],&m->m_sectbl[i]);

		if (m->m_sectbl[i].sh_name_buf[0] == '/')
			if ((l = strtol(&m->m_sectbl[i].sh_name_buf[1],0,10)) > 0)
				if (l < m->m_coff.cfh_size_of_str_tbl)
					m->m_sectbl[i].sh_name = &sptr[strtbl+l];
	}


	/* .relocs */
	struct pe_raw_base_reloc_blk *   r;
	struct pe_block                  b;

	i = pe_get_named_section_index(m,".reloc");
	s = pe_get_block_section_index(m,&m->m_opt.oh_dirs.coh_base_reloc_tbl);

	if ((i >= 0) && (i != s))
		return pe_free_image_meta_impl(
			m,PERK_CUSTOM_ERROR(
				dctx,
				PERK_ERR_IMAGE_MALFORMED));


	if (s >= 0) {
		rtbl  = image->map_addr;
		rtbl += m->m_sectbl[s].sh_ptr_to_raw_data;
		rtbl += m->m_opt.oh_dirs.coh_base_reloc_tbl.dh_rva;
		rtbl -= m->m_sectbl[s].sh_virtual_addr;

		mark  = rtbl;
		cap   = &mark[m->m_sectbl[s].sh_virtual_size];

	} else if (i >= 0) {
		rtbl  = image->map_addr;
		rtbl += m->m_sectbl[i].sh_ptr_to_raw_data;

		mark  = rtbl;
		cap   = &mark[m->m_sectbl[s].sh_virtual_size];

	} else {
		rtbl = 0;
		mark = 0;
		cap  = 0;
	}



	for (; mark < cap; ) {
		r = (struct pe_raw_base_reloc_blk *)mark;

		b.dh_rva  = pe_read_long(r->blk_rva);
		b.dh_size = pe_read_long(r->blk_size);

		if ((b.dh_rva == 0) && (b.dh_size == 0)) {
			mark = cap;

		} else {
			mark      += b.dh_size;
			b.dh_size -= offsetof(struct pe_raw_base_reloc_blk,blk_data);

			m->m_stats.t_nrelocs += b.dh_size / sizeof(uint16_t);
			m->m_stats.t_nrelblks++;
		}
	}


	if (m->m_stats.t_nrelblks)
		if (!(m->r_reltbl = calloc(
				m->m_stats.t_nrelblks + 1,
				sizeof(m->r_reltbl[0]))))
			return PERK_SYSTEM_ERROR(dctx);

	for (i=0,mark=rtbl; i<m->m_stats.t_nrelblks; i++) {
		m->r_reltbl[i] = (struct pe_raw_base_reloc_blk *)mark;
		mark += pe_read_long(m->r_reltbl[i]->blk_size);
	}


	/* .edata */
	i = pe_get_named_section_index(m,".edata");
	s = pe_get_block_section_index(m,&m->m_opt.oh_dirs.coh_export_tbl);

	if ((i >= 0) && (i != s))
		return pe_free_image_meta_impl(
			m,PERK_CUSTOM_ERROR(
				dctx,
				PERK_ERR_IMAGE_MALFORMED));

	if (s >= 0) {
		mark  = base;
		mark += m->m_sectbl[s].sh_ptr_to_raw_data;
		mark += m->m_opt.oh_dirs.coh_export_tbl.dh_rva;
		mark -= m->m_sectbl[s].sh_virtual_addr;

		m->h_edata = &m->m_sectbl[s];
		m->r_edata = (struct pe_raw_export_hdr *)mark;

		m->m_edata.eh_virtual_addr = m->m_opt.oh_dirs.coh_export_tbl.dh_rva;

	} else if (i >= 0) {
		mark  = base;
		mark += m->m_sectbl[i].sh_ptr_to_raw_data;

		m->h_edata = &m->m_sectbl[i];
		m->r_edata = (struct pe_raw_export_hdr *)mark;

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
			m,PERK_CUSTOM_ERROR(
				dctx,
				PERK_ERR_IMAGE_MALFORMED));


	if (s >= 0) {
		mark  = base;
		mark += m->m_sectbl[s].sh_ptr_to_raw_data;
		mark += m->m_opt.oh_dirs.coh_import_tbl.dh_rva;
		mark -= m->m_sectbl[s].sh_virtual_addr;

		m->h_idata = &m->m_sectbl[s];
		m->r_idata = (struct pe_raw_import_hdr *)mark;
		vaddr      = m->m_opt.oh_dirs.coh_import_tbl.dh_rva;

	} else if (i >= 0) {
		mark  = base;
		mark += m->m_sectbl[i].sh_ptr_to_raw_data;

		m->h_idata = &m->m_sectbl[i];
		m->r_idata = (struct pe_raw_import_hdr *)mark;
		vaddr      = m->m_sectbl[i].sh_virtual_addr;
	}


	if ((pidata = m->r_idata)) {
		for (; pe_read_long(pidata->ih_name_rva); ) {
			m->m_stats.t_nimplibs++;
			pidata++;
		}


		if (!(m->m_idata = calloc(
				m->m_stats.t_nimplibs,
				sizeof(m->m_idata[0]))))
			return pe_free_image_meta_impl(
				m,PERK_SYSTEM_ERROR(dctx));


		for (i=0; i<m->m_stats.t_nimplibs; i++) {
			arroff                        = i * sizeof(m->r_idata[0]);
			m->m_idata[i].ih_virtual_addr = vaddr + arroff;

			pe_read_import_header(&m->r_idata[i],&m->m_idata[i]);

			arroff  = m->h_idata->sh_ptr_to_raw_data;
			arroff += m->m_idata[i].ih_name_rva;
			arroff -= m->h_idata->sh_virtual_addr;

			m->m_idata[i].ih_name = &sptr[arroff];

			if (m->m_idata[i].ih_import_lookup_tbl_rva) {
				mark   = base;
				mark  += m->h_idata->sh_ptr_to_raw_data;
				mark  += m->m_idata[i].ih_import_lookup_tbl_rva;
				mark  -= m->h_idata->sh_virtual_addr;
				imptbl = (union pe_raw_import_lookup *)mark;

				m->m_idata[i].ih_aitems = imptbl;


				if (m->m_opt.oh_std.coh_magic == PE_MAGIC_PE32) {
					pitem = imptbl->ii_import_lookup_entry_32;

					for (; pe_read_long(pitem); ) {
						pitem += sizeof(uint32_t);
						m->m_idata[i].ih_count++;
					}

				} else if (m->m_opt.oh_std.coh_magic == PE_MAGIC_PE32_PLUS) {
					pitem = imptbl->ii_import_lookup_entry_64;

					for (; pe_read_quad(pitem); ) {
						pitem += sizeof(uint64_t);
						m->m_idata[i].ih_count++;
					}

				} else {
					return pe_free_image_meta_impl(
						m,PERK_CUSTOM_ERROR(
							dctx,
							PERK_ERR_UNSUPPORTED_ABI));
				}

				if (!(m->m_idata[i].ih_items = calloc(
						m->m_idata[i].ih_count,
						sizeof(m->m_idata[i].ih_items[0]))))
					return pe_free_image_meta_impl(
						m,PERK_SYSTEM_ERROR(dctx));
			}


			switch (m->m_opt.oh_std.coh_magic) {
				case PE_MAGIC_PE32:
					pitem = imptbl->ii_import_lookup_entry_32;
					psize = sizeof(uint32_t);
					break;

				case PE_MAGIC_PE32_PLUS:
					pitem = imptbl->ii_import_lookup_entry_64;
					psize = sizeof(uint64_t);
					break;
			}


			for (j=0; j<m->m_idata[i].ih_count; j++) {
				struct pe_raw_hint_name_entry * pentry;
				struct pe_meta_import_lookup *  ihitem;

				ihitem = &m->m_idata[i].ih_items[j];

				if ((ret = pe_read_import_lookup(
						&pitem[j*psize],ihitem,
						m->m_opt.oh_std.coh_magic)))
					return pe_free_image_meta_impl(
						m,PERK_CUSTOM_ERROR(
							dctx,ret));

				if (!ihitem->ii_flag) {
					mark  = base;
					mark += m->h_idata->sh_ptr_to_raw_data;
					mark += ihitem->ii_hint_name_tbl_rva;
					mark -= m->h_idata->sh_virtual_addr;

					pentry = (struct pe_raw_hint_name_entry *)mark;

					ihitem->ii_hint = pe_read_short(pentry->ii_hint);
					ihitem->ii_name = &sptr[pentry->ii_name - base];
				}
			}
		}
	}

	/* .dsometa */
	if ((i = pe_get_named_section_index(m,MDSO_META_SECTION)) >= 0) {
		m->h_dsometa = &m->m_sectbl[i];
		m->r_dsometa = base + m->m_sectbl[i].sh_ptr_to_raw_data;

		psize = (m->m_opt.oh_std.coh_magic == PE_MAGIC_PE32_PLUS)
			? sizeof(struct mdso_raw_meta_record_m64)
			: sizeof(struct mdso_raw_meta_record_m32);

		m->m_stats.t_ndsolibs = m->h_dsometa->sh_virtual_size / psize;
	}

	/* .dsosyms */
	if ((i = pe_get_named_section_index(m,MDSO_SYMS_SECTION)) >= 0) {
		m->h_dsosyms = &m->m_sectbl[i];
		m->r_dsosyms = base + m->m_sectbl[i].sh_ptr_to_raw_data;

		psize = (m->m_opt.oh_std.coh_magic == PE_MAGIC_PE32_PLUS)
			? sizeof(struct mdso_raw_sym_entry_m64)
			: sizeof(struct mdso_raw_sym_entry_m32);

		m->m_stats.t_ndsosyms = m->h_dsosyms->sh_virtual_size / psize;
	}

	/* .dsostrs */
	if ((i = pe_get_named_section_index(m,MDSO_STRS_SECTION)) >= 0) {
		m->h_dsostrs = &m->m_sectbl[i];
		m->r_dsostrs = sptr + m->m_sectbl[i].sh_ptr_to_raw_data;
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
					dctx,
					PERK_ERR_UNSUPPORTED_ABI));


	/* all done */
	*meta = m;
	return 0;
}
