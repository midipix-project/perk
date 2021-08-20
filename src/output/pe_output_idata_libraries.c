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

#include <perk/perk.h>
#include <perk/perk_output.h>
#include "perk_driver_impl.h"
#include "perk_dprintf_impl.h"
#include "perk_errinfo_impl.h"

static int pretty_header(
	int				fdout,
	const struct pe_common_ctx *	cctx)
{
	return (cctx->fmtflags & PERK_PRETTY_YAML)
		? pe_dprintf(fdout,"imports:\n")
		: 0;
}

static int pretty_implib_header(
	int				fdout,
	const struct pe_common_ctx *	cctx,
	const char *			name)
{
	if ((cctx->fmtflags & PERK_PRETTY_YAML) && (cctx->fmtflags & PERK_OUTPUT_IMPORT_SYMS))
		return pe_dprintf(fdout,"  %s:\n",name);
	else if (cctx->fmtflags & PERK_PRETTY_YAML)
		return pe_dprintf(fdout,"- %s:\n",name);
	else if (cctx->fmtflags & PERK_OUTPUT_IMPORT_SYMS)
		return pe_dprintf(fdout,"%s:\n",name);
	else
		return pe_dprintf(fdout,"%s\n",name);
}

static int pretty_implib_item(
	int				fdout,
	const struct pe_common_ctx *	cctx,
	const char *			name)
{
	if (cctx->fmtflags & PERK_PRETTY_YAML)
		return pe_dprintf(fdout,"  - %s\n",name);
	else
		return pe_dprintf(fdout,"%s\n",name);
}

int pe_output_import_libraries(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	m)
{
	int				fdout;
	int				i;
	unsigned			j;
	const struct pe_common_ctx *	cctx = dctx->cctx;

	fdout = pe_driver_fdout(dctx);

	if (!m->m_stats.t_nimplibs)
		return 0;

	if ((pretty_header(fdout,cctx)) < 0)
		return PERK_FILE_ERROR(dctx);

	for (i=0; i<m->m_stats.t_nimplibs; i++) {
		if ((pretty_implib_header(
				fdout,cctx,
				m->m_idata[i].ih_name)) < 0)
			return PERK_FILE_ERROR(dctx);

		if (cctx->fmtflags & PERK_OUTPUT_IMPORT_SYMS)
			for (j=0; j<m->m_idata[i].ih_count; j++)
				if (m->m_idata[i].ih_items[j].ii_name)
					if ((pretty_implib_item(
							fdout,cctx,
							m->m_idata[i].ih_items[j].ii_name)) < 0)
						return PERK_FILE_ERROR(dctx);
	}

	return 0;
}
