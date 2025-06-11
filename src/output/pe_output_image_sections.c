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

static int pe_output_section_records_yaml(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta,
	int                             fdout)
{
	(void)dctx;
	(void)meta;
	(void)fdout;
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
