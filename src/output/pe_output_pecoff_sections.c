/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2025  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdio.h>

#include <perk/perk.h>
#include <perk/perk_output.h>
#include <perk/perk_consts.h>
#include <perk/perk_structs.h>
#include "perk_driver_impl.h"
#include "perk_dprintf_impl.h"
#include "perk_errinfo_impl.h"

static const char * pe_i386_reloc_type_desc[0x16] = {
	[PE_IMAGE_REL_I386_ABSOLUTE]    = "PE_IMAGE_REL_I386_ABSOLUTE",
	[PE_IMAGE_REL_I386_DIR16]       = "PE_IMAGE_REL_I386_DIR16",
	[PE_IMAGE_REL_I386_REL16]       = "PE_IMAGE_REL_I386_REL16",
	[PE_IMAGE_REL_I386_DIR32]       = "PE_IMAGE_REL_I386_DIR32",
	[PE_IMAGE_REL_I386_DIR32NB]     = "PE_IMAGE_REL_I386_DIR32NB",
	[PE_IMAGE_REL_I386_SEG12]       = "PE_IMAGE_REL_I386_SEG12",
	[PE_IMAGE_REL_I386_SECTION]     = "PE_IMAGE_REL_I386_SECTION",
	[PE_IMAGE_REL_I386_SECREL]      = "PE_IMAGE_REL_I386_SECREL",
	[PE_IMAGE_REL_I386_TOKEN]       = "PE_IMAGE_REL_I386_TOKEN",
	[PE_IMAGE_REL_I386_SECREL7]     = "PE_IMAGE_REL_I386_SECREL7",
	[PE_IMAGE_REL_I386_REL32]       = "PE_IMAGE_REL_I386_REL32",
};

static const char * pe_amd64_reloc_type_desc[0x12] = {
	[PE_IMAGE_REL_AMD64_ABSOLUTE]   = "PE_IMAGE_REL_AMD64_ABSOLUTE",
	[PE_IMAGE_REL_AMD64_ADDR64]     = "PE_IMAGE_REL_AMD64_ADDR64",
	[PE_IMAGE_REL_AMD64_ADDR32]     = "PE_IMAGE_REL_AMD64_ADDR32",
	[PE_IMAGE_REL_AMD64_ADDR32NB]   = "PE_IMAGE_REL_AMD64_ADDR32NB",
	[PE_IMAGE_REL_AMD64_REL32]      = "PE_IMAGE_REL_AMD64_REL32",
	[PE_IMAGE_REL_AMD64_REL32_1]    = "PE_IMAGE_REL_AMD64_REL32_1",
	[PE_IMAGE_REL_AMD64_REL32_2]    = "PE_IMAGE_REL_AMD64_REL32_2",
	[PE_IMAGE_REL_AMD64_REL32_3]    = "PE_IMAGE_REL_AMD64_REL32_3",
	[PE_IMAGE_REL_AMD64_REL32_4]    = "PE_IMAGE_REL_AMD64_REL32_4",
	[PE_IMAGE_REL_AMD64_REL32_5]    = "PE_IMAGE_REL_AMD64_REL32_5",
	[PE_IMAGE_REL_AMD64_SECTION]    = "PE_IMAGE_REL_AMD64_SECTION",
	[PE_IMAGE_REL_AMD64_SECREL]     = "PE_IMAGE_REL_AMD64_SECREL",
	[PE_IMAGE_REL_AMD64_SECREL7]    = "PE_IMAGE_REL_AMD64_SECREL7",
	[PE_IMAGE_REL_AMD64_TOKEN]      = "PE_IMAGE_REL_AMD64_TOKEN",
	[PE_IMAGE_REL_AMD64_SREL32]     = "PE_IMAGE_REL_AMD64_SREL32",
	[PE_IMAGE_REL_AMD64_PAIR]       = "PE_IMAGE_REL_AMD64_PAIR",
	[PE_IMAGE_REL_AMD64_SSPAN32]    = "PE_IMAGE_REL_AMD64_SSPAN32",
};

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
	const struct pe_image_meta *    meta,
	const struct pe_meta_sec_hdr *  s,
	const unsigned char *           base)
{
	int                               i;
	const struct pe_raw_coff_reloc *  r;
	struct pe_meta_coff_reloc         m;
	char                              reltypedesc[64];

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

	if (s->sh_num_of_relocs == 0)
		return 0;

	if (pe_dprintf(fdout,"      - relocation-records:\n") < 0)
		return PERK_FILE_ERROR(dctx);

	r = (const struct pe_raw_coff_reloc *)&base[s->sh_ptr_to_relocs];

	for (i=0; i<s->sh_num_of_relocs; i++) {
		pe_read_coff_reloc(&r[i],&m);
		memset(reltypedesc,0,sizeof(reltypedesc));

		switch (meta->m_abi) {
			case PE_ABI_PE32:
				if (m.rel_type <= PE_IMAGE_REL_I386_REL32)
					snprintf(reltypedesc,sizeof(reltypedesc),
						"0x%04x (%s)",
						m.rel_type,
						pe_i386_reloc_type_desc[m.rel_type]);
				break;

			case PE_ABI_PE64:
				if (m.rel_type <= PE_IMAGE_REL_AMD64_SSPAN32)
					snprintf(reltypedesc,sizeof(reltypedesc),
						"0x%04x (%s)",
						m.rel_type,
						pe_amd64_reloc_type_desc[m.rel_type]);
				break;

			default:
				snprintf(reltypedesc,sizeof(reltypedesc),
					"0x%04x",
					m.rel_type);
		}

		if (pe_dprintf(
				fdout,
				"        - reloction-record:\n"
				"          - [ rva:  0x%08x ]\n"
				"          - [ sym:  0x%08x (%s) ]\n"
				"          - [ type: %s ]\n"
				"\n",
				m.rel_rva,
				m.rel_sym,
				meta->m_symvec_symidx[m.rel_sym]->cs_name,
				reltypedesc) < 0)
			return PERK_FILE_ERROR(dctx);
	}

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
		if (pe_output_section_record_yaml(
				fdout,dctx,meta,
				&meta->m_sectbl[i],
				meta->r_image.map_addr) < 0)
			return PERK_NESTED_ERROR(dctx);

	return 0;
}

static int pe_output_pecoff_sections_yaml(
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

int pe_output_pecoff_sections(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta)
{
	int fdout = pe_driver_fdout(dctx);

	if (dctx->cctx->fmtflags & PERK_PRETTY_YAML) {
		if (pe_output_pecoff_sections_yaml(dctx,meta,fdout) < 0)
			return PERK_NESTED_ERROR(dctx);

	} else {
		if (pe_output_section_names(dctx,meta,fdout) < 0)
			return PERK_NESTED_ERROR(dctx);
	}

	return 0;
}
