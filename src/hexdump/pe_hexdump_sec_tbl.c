/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2021  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdio.h>

#include <perk/perk.h>
#include <perk/perk_consts.h>
#include <perk/perk_structs.h>
#include <perk/perk_output.h>
#include "perk_driver_impl.h"
#include "perk_hexdump_impl.h"
#include "perk_dprintf_impl.h"
#include "perk_errinfo_impl.h"

#define PE_ADDR       &meta->r_sectbl[idx]
#define PE_RAW_STRUCT sec_hdr
#define PE_OUTPUT(x)  PE_OUTPUT_TABLE(x)

static int pe_hexdump_sec_hdr_impl(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta,
	uint32_t			idx)
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

	sprintf(
		hdr,
		"struct pe_raw_sec_hdr [%d]",
		idx);

	faddr = (char *)(PE_ADDR) - (char *)meta->r_image.map_addr;
	vaddr = meta->m_opt.oh_mem.coh_image_base + faddr;

	ch  = buf;
	ch += pe_output_hex_header(
		ch,hdr,
		faddr,vaddr,bits);

	ch += PE_OUTPUT(sh_name);
	ch += PE_OUTPUT(sh_virtual_size);
	ch += PE_OUTPUT(sh_virtual_addr);
	ch += PE_OUTPUT(sh_size_of_raw_data);
	ch += PE_OUTPUT(sh_ptr_to_raw_data);
	ch += PE_OUTPUT(sh_ptr_to_relocs);
	ch += PE_OUTPUT(sh_ptr_to_line_nums);
	ch += PE_OUTPUT(sh_num_of_relocs);
	ch += PE_OUTPUT(sh_num_of_line_nums);
	ch += PE_OUTPUT(sh_characteristics);

	ch += pe_output_hex_footer(ch);

	*ch = 0;

	if (pe_dprintf(pe_driver_fdout(dctx),"%s",buf) < 0)
		return PERK_FILE_ERROR(dctx);

	return 0;
}

int pe_hexdump_sec_tbl(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta)
{
	uint32_t idx;

	for (idx=0; idx<meta->m_coff.cfh_num_of_sections; idx++)
		if (pe_hexdump_sec_hdr_impl(dctx,meta,idx) < 0)
			return PERK_NESTED_ERROR(dctx);

	return 0;
}
