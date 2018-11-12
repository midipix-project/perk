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

#define PE_ADDR       meta->r_dos
#define PE_RAW_STRUCT image_dos_hdr
#define PE_OUTPUT(x)  PE_OUTPUT_TABLE(x)

int pe_hdrdump_image_dos_hdr(
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

	faddr = 0;
	vaddr = meta->m_opt.oh_mem.coh_image_base;

	ch  = buf;
	ch += pe_output_hex_header(
		ch,
		"struct pe_raw_image_dos_hdr",
		faddr,vaddr,bits);

	ch += PE_OUTPUT(dos_magic);
	ch += PE_OUTPUT(dos_cblp);
	ch += PE_OUTPUT(dos_cp);
	ch += PE_OUTPUT(dos_crlc);
	ch += PE_OUTPUT(dos_cparhdr);
	ch += PE_OUTPUT(dos_minalloc);
	ch += PE_OUTPUT(dos_maxalloc);
	ch += PE_OUTPUT(dos_ss);
	ch += PE_OUTPUT(dos_sp);
	ch += PE_OUTPUT(dos_csum);
	ch += PE_OUTPUT(dos_ip);
	ch += PE_OUTPUT(dos_cs);
	ch += PE_OUTPUT(dos_lfarlc);
	ch += PE_OUTPUT(dos_ovno);
	ch += PE_OUTPUT(dos_res);
	ch += PE_OUTPUT(dos_oemid);
	ch += PE_OUTPUT(dos_oeminfo);
	ch += PE_OUTPUT(dos_res2);
	ch += PE_OUTPUT(dos_lfanew);

	ch += pe_output_hex_footer(ch);

	*ch = 0;

	if (pe_dprintf(pe_driver_fdout(dctx),"%s",buf) < 0)
		return PERK_FILE_ERROR(dctx);

	return 0;
}
