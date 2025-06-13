/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2025  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <perk/perk.h>
#include <perk/perk_output.h>
#include "perk_driver_impl.h"
#include "perk_dprintf_impl.h"

static void pe_perform_hdrdump_actions(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta)
{
	if (dctx->cctx->hdrdump & PERK_HDRDUMP_IMAGE_DOS_HEADER)
		pe_hexdump_image_dos_hdr(dctx,meta);

	if (dctx->cctx->hdrdump & PERK_HDRDUMP_COFF_IMAGE_HEADER)
		pe_hexdump_coff_image_hdr(dctx,meta);

	if (dctx->cctx->hdrdump & PERK_HDRDUMP_COFF_OBJECT_HEADER)
		pe_hexdump_coff_object_hdr(dctx,meta);

	if (dctx->cctx->hdrdump & PERK_HDRDUMP_COFF_OPT_HEADER)
		pe_hexdump_coff_opt_hdr(dctx,meta);

	if (dctx->cctx->hdrdump & PERK_HDRDUMP_SECTION_TABLE)
		pe_hexdump_sec_tbl(dctx,meta);

	if (dctx->cctx->hdrdump & PERK_HDRDUMP_EXPORT_HEADER)
		pe_hexdump_export_hdr(dctx,meta);

	if (dctx->cctx->hdrdump & PERK_HDRDUMP_IMPORT_TABLE)
		pe_hexdump_import_tbl(dctx,meta);
}

static void pe_perform_unit_actions_impl(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta)
{
	uint64_t flags = dctx->cctx->fmtflags;

	if (flags & PERK_OUTPUT_IMAGE_CATEGORY)
		pe_output_pecoff_category(dctx,meta);

	if (flags & PERK_OUTPUT_IMAGE_SECTIONS)
		pe_output_pecoff_sections(dctx,meta);

	if (flags & PERK_OUTPUT_IMAGE_SYMBOLS)
		pe_output_image_symbols(dctx,meta);

	if (flags & PERK_OUTPUT_IMAGE_STRINGS)
		pe_output_pecoff_strings(dctx,meta);

	if (flags & PERK_OUTPUT_EXPORT_SYMS)
		pe_output_export_symbols(dctx,meta);

	if ((flags & PERK_OUTPUT_IMPORT_LIBS) || (flags & PERK_OUTPUT_IMPORT_SYMS))
		pe_output_import_libraries(dctx,meta);

	if ((flags & PERK_OUTPUT_MDSO_LIBS) || (flags & PERK_OUTPUT_MDSO_SYMS))
		pe_output_mdso_libraries(dctx,meta);

	if (dctx->cctx->hdrdump)
		pe_perform_hdrdump_actions(dctx,meta);
}

static void pe_perform_unit_actions(
	const struct pe_driver_ctx *	dctx,
	const struct pe_unit_ctx *	uctx)
{
	const struct pe_image_meta **   objmeta;

	if (uctx->meta)
		pe_perform_unit_actions_impl(dctx,uctx->meta);

	if ((objmeta = uctx->objmeta))
		for (; *objmeta; )
			pe_perform_unit_actions_impl(dctx,*objmeta++);
}

int pe_cmd_perk(const struct pe_driver_ctx * dctx, const char * path)
{
	int                     ret;
	struct pe_unit_ctx *	uctx;

	if ((ret = pe_lib_get_unit_ctx(dctx,path,&uctx)) < 0)
		return ret;

	pe_perform_unit_actions(dctx,uctx);
	pe_lib_free_unit_ctx(uctx);

	return 0;
}
