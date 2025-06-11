/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2025  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdio.h>

#include <perk/perk.h>
#include <perk/perk_output.h>
#include "perk_driver_impl.h"
#include "perk_dprintf_impl.h"
#include "perk_errinfo_impl.h"

static int pe_output_section_names(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta,
	int                             fdout)
{
	int i;

	for (i=0; i<meta->m_coff.cfh_num_of_sections; i++)
		if (pe_dprintf(fdout,"%s\n",meta->m_sectbl[i].sh_name) < 0)
			return PERK_FILE_ERROR(dctx);

	return 0;
}

static int pe_output_section_names_yaml(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta,
	int                             fdout)
{
	int i;

	if (pe_dprintf(fdout,"  - Sections:\n") < 0)
		return PERK_FILE_ERROR(dctx);

	for (i=0; i<meta->m_coff.cfh_num_of_sections; i++)
		if (pe_dprintf(fdout,"    - section: %s\n",meta->m_sectbl[i].sh_name) < 0)
			return PERK_FILE_ERROR(dctx);

	return 0;
}

static int pe_output_section_record_yaml(
	int                             fdout,
	const struct pe_driver_ctx *	dctx,
	const struct pe_meta_sec_hdr *  s)
{
	if (pe_dprintf(fdout,
			"    - section:\n"
			"      - [ name:              %s ]\n"
			"      - [ virtual-size:      0x%08x ]\n"
			"      - [ virtual-addr:      0x%08x ]\n"
			"      - [ size-of-raw-data:  0x%08x ]\n"
			"      - [ ptr-to-raw-data:   0x%08x ]\n"
			"      - [ ptr-to-relocs:     0x%08x ]\n"
			"      - [ ptr-to-line-nums:  0x%08x ]\n"
			"      - [ num-of-relocs:     %u ]\n"
			"      - [ num-of-line-nums:  %u ]\n"
			"      - [ characteristics:   0x%08x ]\n"
			"\n",
			s->sh_name,
			s->sh_virtual_size,
			s->sh_virtual_addr,
			s->sh_size_of_raw_data,
			s->sh_ptr_to_raw_data,
			s->sh_ptr_to_relocs,
			s->sh_ptr_to_line_nums,
			s->sh_num_of_relocs,
			s->sh_num_of_line_nums,
			s->sh_characteristics) < 0)
		return PERK_FILE_ERROR(dctx);

	return 0;
}

static int pe_output_section_records_yaml(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta,
	int                             fdout)
{
	int i;

	if (pe_dprintf(fdout,"  - Sections:\n") < 0)
		return PERK_FILE_ERROR(dctx);

	for (i=0; i<meta->m_coff.cfh_num_of_sections; i++)
		if (pe_output_section_record_yaml(fdout,dctx,&meta->m_sectbl[i]) < 0)
			return PERK_NESTED_ERROR(dctx);

	return 0;
}

static int pe_output_image_sections_yaml(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta,
	int                             fdout)
{
	if (dctx->cctx->fmtflags & PERK_PRETTY_VERBOSE) {
		if (pe_output_section_records_yaml(dctx,meta,fdout) < 0)
			return PERK_NESTED_ERROR(dctx);
	} else {
		if (pe_output_section_names_yaml(dctx,meta,fdout) < 0)
			return PERK_NESTED_ERROR(dctx);
	}

	return 0;
}

int pe_output_image_sections(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta)
{
	int fdout = pe_driver_fdout(dctx);

	if (dctx->cctx->fmtflags & PERK_PRETTY_YAML) {
		if (pe_output_image_sections_yaml(dctx,meta,fdout) < 0)
			return PERK_NESTED_ERROR(dctx);

	} else {
		if (pe_output_section_names(dctx,meta,fdout) < 0)
			return PERK_NESTED_ERROR(dctx);
	}

	return 0;
}
