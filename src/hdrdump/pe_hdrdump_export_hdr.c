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
#include "perk_hdrdump_impl.h"
#include "perk_dprintf_impl.h"
#include "perk_errinfo_impl.h"

#define PE_ADDR       meta->r_edata
#define PE_RAW_STRUCT export_hdr
#define PE_OUTPUT(x)  PE_OUTPUT_TABLE(x)

int pe_hdrdump_export_hdr(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta)
{
	int		bits;
	uint64_t	faddr;
	uint64_t	vaddr;
	char *		ch;
	char		buf[8192];

	if (!PE_ADDR)
		return 0;

	if ((bits = pe_image_bits(meta)) < 0)
		return PERK_CUSTOM_ERROR(
			dctx,PERK_ERR_UNSUPPORTED_ABI);

	faddr = (char *)(PE_ADDR) - (char *)meta->r_image.map_addr;
	vaddr = meta->m_opt.oh_mem.coh_image_base + meta->m_edata.eh_virtual_addr;

	ch  = buf;
	ch += pe_output_hex_header(
		ch,
		"struct pe_raw_export_hdr",
		faddr,vaddr,bits);

	ch += PE_OUTPUT(eh_export_flags);
	ch += PE_OUTPUT(eh_time_date_stamp);
	ch += PE_OUTPUT(eh_major_ver);
	ch += PE_OUTPUT(eh_minor_ver);
	ch += PE_OUTPUT(eh_name_rva);
	ch += PE_OUTPUT(eh_ordinal_base);
	ch += PE_OUTPUT(eh_addr_tbl_entries);
	ch += PE_OUTPUT(eh_num_of_name_ptrs);
	ch += PE_OUTPUT(eh_export_addr_tbl_rva);
	ch += PE_OUTPUT(eh_name_ptr_rva);
	ch += PE_OUTPUT(eh_ordinal_tbl_rva);

	ch += pe_output_hex_footer(ch);

	*ch = 0;

	if (pe_dprintf(pe_driver_fdout(dctx),"%s",buf) < 0)
		return PERK_FILE_ERROR(dctx);

	return 0;
}
