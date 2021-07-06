/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2021  Z. Gilboa                        */
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

#define PE_ADDR       meta->r_obj
#define PE_RAW_STRUCT coff_object_hdr
#define PE_OUTPUT(x)  PE_OUTPUT_TABLE(x)

int pe_hdrdump_coff_object_hdr(
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
	vaddr = meta->m_opt.oh_mem.coh_image_base + faddr;

	ch  = buf;
	ch += pe_output_hex_header(
		ch,
		"struct pe_raw_coff_object_hdr",
		faddr,vaddr,bits);

	ch += PE_OUTPUT(cfh_machine);
	ch += PE_OUTPUT(cfh_num_of_sections);
	ch += PE_OUTPUT(cfh_time_date_stamp);
	ch += PE_OUTPUT(cfh_ptr_to_sym_tbl);
	ch += PE_OUTPUT(cfh_num_of_syms);
	ch += PE_OUTPUT(cfh_size_of_opt_hdr);
	ch += PE_OUTPUT(cfh_characteristics);

	ch += pe_output_hex_footer(ch);

	*ch = 0;

	if (pe_dprintf(pe_driver_fdout(dctx),"%s",buf) < 0)
		return PERK_FILE_ERROR(dctx);

	return 0;
}
