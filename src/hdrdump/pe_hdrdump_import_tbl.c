/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2017  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdio.h>

#include <perk/perk.h>
#include <perk/perk_output.h>
#include "perk_driver_impl.h"
#include "perk_hdrdump_impl.h"
#include "perk_dprintf_impl.h"
#include "perk_errinfo_impl.h"

#define PE_ADDR       &meta->r_idata[idx]
#define PE_RAW_STRUCT import_hdr
#define PE_OUTPUT(x)  PE_OUTPUT_TABLE(x)

static int pe_hdrdump_import_hdr_impl(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta,
	int32_t				idx)
{
	int		bits;
	uint64_t	faddr;
	uint64_t	vaddr;
	char *		ch;
	char		hdr[64];
	char		buf[8192];

	if ((bits = pe_image_bits(meta)) < 0)
		return PERK_CUSTOM_ERROR(
			dctx,PERK_ERR_UNSUPPORTED_ABI);

	/* import header */
	sprintf(
		hdr,
		"struct pe_raw_import_hdr [%d]",
		idx);

	faddr = (char *)(PE_ADDR) - (char *)meta->r_image.map_addr;
	vaddr = meta->m_opt.oh_mem.coh_image_base + meta->m_idata[idx].ih_virtual_addr;

	ch  = buf;
	ch += pe_output_hex_header(
		ch,hdr,
		faddr,vaddr,bits);

	ch += PE_OUTPUT(ih_import_lookup_tbl_rva);
	ch += PE_OUTPUT(ih_time_date_stamp);
	ch += PE_OUTPUT(ih_forwarder_chain);
	ch += PE_OUTPUT(ih_name_rva);
	ch += PE_OUTPUT(ih_import_addr_tbl_rva);

	ch += pe_output_hex_footer(ch);

	/* import name for all but the null header */
	if (idx == meta->m_stats.t_nimplibs) {
		(void)0;

	} else if (meta->m_idata[idx].ih_name_rva) {
		sprintf(
			hdr,
			"struct pe_raw_import_name [%d]",
			idx);

		faddr = meta->m_idata[idx].ih_name - (char *)meta->r_image.map_addr;
		vaddr = meta->m_idata[idx].ih_name_rva;

		ch += pe_output_hex_header(
			ch,hdr,
			faddr,vaddr,bits);

		ch += pe_output_raw_element(
			ch,
			meta->m_idata[idx].ih_name,
			"ih_name",0,
			strlen(meta->m_idata[idx].ih_name) + 1);

		ch += pe_output_hex_footer(ch);
	}

	*ch = 0;

	/* fdout */
	if (pe_dprintf(pe_driver_fdout(dctx),"%s",buf) < 0)
		return PERK_FILE_ERROR(dctx);

	return 0;
}

int pe_hdrdump_import_tbl(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta)
{
	int idx;

	for (idx=0; idx<=meta->m_stats.t_nimplibs; idx++)
		if (pe_hdrdump_import_hdr_impl(dctx,meta,idx) < 0)
			return PERK_NESTED_ERROR(dctx);

	return 0;
}
