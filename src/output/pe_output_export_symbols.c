/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2021  Z. Gilboa                        */
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
	if (cctx->fmtflags & PERK_PRETTY_YAML)
		return pe_dprintf(fdout,"exports:\n");
	else if (cctx->fmtflags & PERK_PRETTY_DLLTOOL)
		return pe_dprintf(fdout,"EXPORTS\n");
	return 0;
}

static int pretty_export_item(
	int				fdout,
	const struct pe_common_ctx *	cctx,
	const char *			name)
{
	if (cctx->fmtflags & PERK_PRETTY_YAML)
		return pe_dprintf(fdout,"- %s\n",name);
	else
		return pe_dprintf(fdout,"%s\n",name);
}

int pe_output_export_symbols(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	m)
{
	int				fdout;
	char *				mark;
	uint32_t 			offset;
	uint32_t *			symrva;
	unsigned			i;
	const struct pe_common_ctx *	cctx = dctx->cctx;

	fdout = pe_driver_fdout(dctx);

	if (!m->h_edata)
		return 0;

	if ((pretty_header(fdout,cctx)) < 0)
		return PERK_FILE_ERROR(dctx);

	mark	= m->r_image.map_addr;
	offset	= m->h_edata->sh_virtual_addr - m->h_edata->sh_ptr_to_raw_data;
	symrva	= (uint32_t *)(mark + m->m_edata.eh_name_ptr_rva - offset);

	for (i=0; i<m->m_edata.eh_num_of_name_ptrs; i++)
		if ((pretty_export_item(
				fdout,cctx,
				&mark[symrva[i] - offset])) < 0)
			return PERK_FILE_ERROR(dctx);

	return 0;
}
