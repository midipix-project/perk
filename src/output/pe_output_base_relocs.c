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
#include "perk_endian_impl.h"
#include "perk_reader_impl.h"
#include "perk_dprintf_impl.h"
#include "perk_errinfo_impl.h"

static const char * pe_base_reloc_type_desc[0x10] = {
	[PE_IMAGE_REL_BASED_ABSOLUTE]       = "ABSOLUTE",
	[PE_IMAGE_REL_BASED_HIGH]           = "HIGH",
	[PE_IMAGE_REL_BASED_LOW]            = "LOW",
	[PE_IMAGE_REL_BASED_HIGHLOW]        = "HIGHLOW",
	[PE_IMAGE_REL_BASED_HIGHADJ]        = "HIGHADJ",
	[PE_IMAGE_REL_BASED_RESERVED]       = "RESERVED",
	[PE_IMAGE_REL_BASED_DIR64]          = "DIR64",
};

static int pe_output_base_reloc_records(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta,
	int                             fdout)
{
	struct pe_raw_base_reloc_blk *  r;
	int                             i;
	unsigned                        j;
	uint32_t                        rva;
	uint32_t                        size;
	uint32_t                        nents;
	uint16_t                        fixup;
	uint32_t                        offset;
	const char *                    desc;

	for (i=0; i<meta->m_stats.t_nrelblks; i++) {
		r = meta->r_reltbl[i];

		rva  = pe_read_long(r->blk_rva);
		size = pe_read_long(r->blk_size);

		nents  = size - offsetof(struct pe_raw_base_reloc_blk,blk_data);
		nents /= sizeof(uint16_t);

		if (pe_dprintf(fdout,
				"Virtual Address: %08x, "
				"Chunk Size: %u (0x%x), "
				"Number of Fixups: %u\n",
				rva,size,size,nents) < 0)
			return PERK_FILE_ERROR(dctx);

		for (j=0; j<nents; j++) {
			fixup  = pe_read_short(r->blk_data[j]);
			desc   = pe_base_reloc_type_desc[fixup >> 12];
			offset = fixup & 0xfff;

			if (pe_dprintf(fdout,
					"\t"
					"reloc %4d "
					"offset %4x [%x] %s\n",
					j,offset,offset+rva,
					desc) < 0)
				return PERK_FILE_ERROR(dctx);
		}

		if (pe_dprintf(fdout,"\n") < 0)
			return PERK_FILE_ERROR(dctx);
	}

	return 0;
}

static int pe_output_base_relocs_yaml(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta,
	int                             fdout)
{
	struct pe_raw_base_reloc_blk *  r;
	int                             i;
	unsigned                        j;
	uint32_t                        rva;
	uint32_t                        size;
	uint32_t                        nents;
	uint16_t                        fixup;
	uint32_t                        offset;
	const char *                    desc;

	if (pe_dprintf(fdout," - Base relocation blocks:\n") < 0)
		return PERK_FILE_ERROR(dctx);

	for (i=0; i<meta->m_stats.t_nrelblks; i++) {
		r = meta->r_reltbl[i];

		rva  = pe_read_long(r->blk_rva);
		size = pe_read_long(r->blk_size);

		nents  = size - offsetof(struct pe_raw_base_reloc_blk,blk_data);
		nents /= sizeof(uint16_t);

		if (pe_dprintf(fdout,
				"    - base relocation block:\n"
				"      [ page-rva:      0x%08x ]\n"
				"      [ block-size:    0x%08x ]\n"
				"      [ fixup-entries: %u ]\n\n"
				"      - page fixups:\n",
				rva,size,nents) < 0)
			return PERK_FILE_ERROR(dctx);

		for (j=0; j<nents; j++) {
			fixup  = pe_read_short(r->blk_data[j]);
			desc   = pe_base_reloc_type_desc[fixup >> 12];
			offset = fixup & 0xfff;

			if (pe_dprintf(fdout,
					"        [ type   = %s ]\n"
					"        [ offset = 0x%03x ]\n\n",
					desc,offset) < 0)
				return PERK_FILE_ERROR(dctx);
		}
	}

	return 0;
}

int pe_output_base_relocs(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta)
{
	int fdout = pe_driver_fdout(dctx);

	if (dctx->cctx->fmtflags & PERK_PRETTY_YAML) {
		if (pe_output_base_relocs_yaml(dctx,meta,fdout) < 0)
			return PERK_NESTED_ERROR(dctx);

	} else {
		if (pe_output_base_reloc_records(dctx,meta,fdout) < 0)
			return PERK_NESTED_ERROR(dctx);
	}

	return 0;
}
