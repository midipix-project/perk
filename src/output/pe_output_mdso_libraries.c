/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2021  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <mdso/mdso_specs.h>
#include <mdso/mdso_structs.h>

#include <perk/perk.h>
#include <perk/perk_consts.h>
#include <perk/perk_output.h>
#include "perk_reader_impl.h"
#include "perk_driver_impl.h"
#include "perk_dprintf_impl.h"
#include "perk_errinfo_impl.h"

static int pretty_header(
	int				fdout,
	const struct pe_common_ctx *	cctx)
{
	return (cctx->fmtflags & PERK_PRETTY_YAML)
		? pe_dprintf(fdout,"dsolibs:\n")
		: 0;
}

static int pretty_dsolib_header(
	int				fdout,
	const struct pe_common_ctx *	cctx,
	const char *			name)
{
	if ((cctx->fmtflags & PERK_PRETTY_YAML) && (cctx->fmtflags & PERK_OUTPUT_MDSO_SYMS))
		return pe_dprintf(fdout,"  %s:\n",name);
	else if (cctx->fmtflags & PERK_PRETTY_YAML)
		return pe_dprintf(fdout,"- %s:\n",name);
	else if (cctx->fmtflags & PERK_OUTPUT_MDSO_SYMS)
		return pe_dprintf(fdout,"%s:\n",name);
	else
		return pe_dprintf(fdout,"%s\n",name);
}

static int pretty_dsolib_item(
	int				fdout,
	const struct pe_common_ctx *	cctx,
	const char *			name)
{
	if (cctx->fmtflags & PERK_PRETTY_YAML)
		return pe_dprintf(fdout,"  - %s\n",name);
	else
		return pe_dprintf(fdout,"%s\n",name);
}

static unsigned char * dsolib_meta(const struct pe_image_meta * m, int i)
{
	struct mdso_raw_meta_record_m32 *	mdso32;
	struct mdso_raw_meta_record_m64 *	mdso64;

	if (m->m_opt.oh_std.coh_magic == PE_MAGIC_PE32_PLUS) {
		mdso64 = (struct mdso_raw_meta_record_m64 *)m->r_dsometa + i;
		return mdso64->mrec_base;
	} else {
		mdso32 = (struct mdso_raw_meta_record_m32 *)m->r_dsometa + i;
		return mdso32->mrec_base;
	}
}

static char * dsolib_name(const struct pe_image_meta * m, int i)
{
	uintptr_t				va;
	uint64_t				rva;
	char *					addr;
	uint32_t				roffset;
	struct mdso_raw_meta_record_m32 *	mdso32;
	struct mdso_raw_meta_record_m64 *	mdso64;

	if (m->m_opt.oh_std.coh_magic == PE_MAGIC_PE32_PLUS) {
		mdso64  = (struct mdso_raw_meta_record_m64 *)m->r_dsometa + i;
		va      = pe_read_quad(mdso64->mrec_name);
		rva     = va - m->m_opt.oh_mem.coh_image_base;

		if (rva >> 32)
			return 0;
	} else {
		mdso32  = (struct mdso_raw_meta_record_m32 *)m->r_dsometa + i;
		va      = pe_read_long(mdso32->mrec_name);
		rva     = va - m->m_opt.oh_mem.coh_image_base;
	}

	if (pe_get_roffset_from_rva(m,(uint32_t)rva,&roffset) < 0)
		return 0;

	return (addr = m->r_image.map_addr) + roffset;
}

static unsigned char * dsosym_meta(const struct pe_image_meta * m, int j)
{
	uint64_t			va;
	uint32_t			rva;
	uint32_t			roffset;
	struct mdso_raw_sym_entry_m32 *	sym32;
	struct mdso_raw_sym_entry_m64 *	sym64;
	int				idx;

	if (m->m_opt.oh_std.coh_magic == PE_MAGIC_PE32_PLUS) {
		sym64 = (struct mdso_raw_sym_entry_m64 *)m->r_dsosyms + j;
		va    = pe_read_quad(sym64->msym_meta);
		rva   = pe_read_long(sym64->msym_meta);
	} else {
		sym32 = (struct mdso_raw_sym_entry_m32 *)m->r_dsosyms + j;
		va    = pe_read_long(sym32->msym_meta);
		rva   = va;
	}

	if ((idx = pe_get_named_section_index(m,MDSO_META_SECTION)) >= 0)
		if (idx != pe_get_block_section_index(m,&(struct pe_block){rva,0}))
			return (unsigned char *)(-1);

	if (va > rva)
		return 0;

	if (pe_get_roffset_from_rva(m,rva,&roffset) < 0)
		return 0;

	return (unsigned char *)m->r_image.map_addr + roffset;
}

static char * dsosym_string(const struct pe_image_meta * m, int j)
{
	uint64_t			va;
	uint32_t			rva;
	char *				addr;
	uint32_t			roffset;
	struct mdso_raw_sym_entry_m32 *	sym32;
	struct mdso_raw_sym_entry_m64 *	sym64;
	int				idx;

	if (m->m_opt.oh_std.coh_magic == PE_MAGIC_PE32_PLUS) {
		sym64 = (struct mdso_raw_sym_entry_m64 *)m->r_dsosyms + j;
		va    = pe_read_quad(sym64->msym_string);
		rva   = pe_read_long(sym64->msym_string);
	} else {
		sym32 = (struct mdso_raw_sym_entry_m32 *)m->r_dsosyms + j;
		va    = pe_read_long(sym32->msym_string);
		rva   = va;
	}

	if ((idx = pe_get_named_section_index(m,MDSO_STRS_SECTION)) >= 0)
		if (idx != pe_get_block_section_index(m,&(struct pe_block){rva,0}))
			return (char *)(-2);

	if (va > rva)
		return 0;

	if (pe_get_roffset_from_rva(m,rva,&roffset) < 0)
		return 0;

	return (addr = m->r_image.map_addr) + roffset;
}

int pe_output_mdso_libraries(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	m)
{
	int				i,j;
	int				fdout;
	char *				name;
	unsigned char *			symmeta;
	unsigned char *			dsometa;
	char *				symstr;
	const struct pe_common_ctx *	cctx = dctx->cctx;

	fdout = pe_driver_fdout(dctx);

	if (!m->m_stats.t_ndsolibs)
		return 0;

	if (pe_get_image_abi(m,0) == PE_ABI_UNSUPPORTED)
		return PERK_CUSTOM_ERROR(
			dctx,PERK_ERR_UNSUPPORTED_ABI);

	if ((pretty_header(fdout,cctx)) < 0)
		return PERK_FILE_ERROR(dctx);

	for (i=0; i<m->m_stats.t_ndsolibs; i++) {
		dsometa = dsolib_meta(m,i);

		if (!(name = dsolib_name(m,i)))
			return PERK_CUSTOM_ERROR(
				dctx,PERK_ERR_IMAGE_MALFORMED);

		if ((pretty_dsolib_header(fdout,cctx,name)) < 0)
			return PERK_FILE_ERROR(dctx);

		if (cctx->fmtflags & PERK_OUTPUT_MDSO_SYMS) {
			for (j=0; j<m->m_stats.t_ndsosyms; j++) {
				if (!(symmeta = dsosym_meta(m,j)))
					return PERK_CUSTOM_ERROR(
						dctx,PERK_ERR_IMAGE_MALFORMED);

				if (!(symstr = dsosym_string(m,j)))
					return PERK_CUSTOM_ERROR(
						dctx,PERK_ERR_IMAGE_MALFORMED);

				if (symmeta == dsometa)
					if ((pretty_dsolib_item(fdout,cctx,symstr)) < 0)
						return PERK_FILE_ERROR(dctx);
			}
		}
	}

	return 0;
}
