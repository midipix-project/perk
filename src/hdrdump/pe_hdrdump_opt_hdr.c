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

#define PE_ADDR       &meta->r_opt->opt_hdr_32
#define PE_RAW_STRUCT opt_hdr_32
#define PE_OUTPUT(x)  PE_OUTPUT_TABLE(x)

static int pe_hdrdump_opt_hdr_32(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta)
{
	uint64_t	faddr;
	uint64_t	vaddr;
	char *		ch;
	char		buf[8192];

	if (!PE_ADDR)
		return 0;

	faddr = (char *)(PE_ADDR) - (char *)meta->r_image.map_addr;
	vaddr = meta->m_opt.oh_mem.coh_image_base + faddr;

	ch  = buf;
	ch += pe_output_hex_header(
		ch,
		"struct pe_raw_opt_hdr_32",
		faddr,vaddr,32);

	ch += PE_OUTPUT(coh_magic);
	ch += PE_OUTPUT(coh_major_linker_ver);
	ch += PE_OUTPUT(coh_minor_linker_ver);
	ch += PE_OUTPUT(coh_size_of_code);
	ch += PE_OUTPUT(coh_size_of_inited_data);
	ch += PE_OUTPUT(coh_size_of_uninited_data);
	ch += PE_OUTPUT(coh_entry_point);
	ch += PE_OUTPUT(coh_base_of_code);
	ch += PE_OUTPUT(coh_base_of_data);
	ch += PE_OUTPUT(coh_image_base);
	ch += PE_OUTPUT(coh_section_align);
	ch += PE_OUTPUT(coh_file_align);
	ch += PE_OUTPUT(coh_major_os_ver);
	ch += PE_OUTPUT(coh_minor_os_ver);
	ch += PE_OUTPUT(coh_major_image_ver);
	ch += PE_OUTPUT(coh_minor_image_ver);
	ch += PE_OUTPUT(coh_major_subsys_ver);
	ch += PE_OUTPUT(coh_minor_subsys_ver);
	ch += PE_OUTPUT(coh_win32_ver);
	ch += PE_OUTPUT(coh_size_of_image);
	ch += PE_OUTPUT(coh_size_of_headers);
	ch += PE_OUTPUT(coh_checksum);
	ch += PE_OUTPUT(coh_subsystem);
	ch += PE_OUTPUT(coh_dll_characteristics);
	ch += PE_OUTPUT(coh_size_of_stack_reserve);
	ch += PE_OUTPUT(coh_size_of_stack_commit);
	ch += PE_OUTPUT(coh_size_of_heap_reserve);
	ch += PE_OUTPUT(coh_size_of_heap_commit);
	ch += PE_OUTPUT(coh_loader_flags);
	ch += PE_OUTPUT(coh_rva_and_sizes);
	ch += PE_OUTPUT(coh_export_tbl);
	ch += PE_OUTPUT(coh_import_tbl);
	ch += PE_OUTPUT(coh_resource_tbl);
	ch += PE_OUTPUT(coh_exception_tbl);
	ch += PE_OUTPUT(coh_certificate_tbl);
	ch += PE_OUTPUT(coh_base_reloc_tbl);
	ch += PE_OUTPUT(coh_debug);
	ch += PE_OUTPUT(coh_arch);
	ch += PE_OUTPUT(coh_global_ptr);
	ch += PE_OUTPUT(coh_tls_tbl);
	ch += PE_OUTPUT(coh_load_config_tbl);
	ch += PE_OUTPUT(coh_bound_import);
	ch += PE_OUTPUT(coh_iat);
	ch += PE_OUTPUT(coh_delay_import_descriptor);
	ch += PE_OUTPUT(coh_clr_runtime_hdr);
	ch += PE_OUTPUT(coh_reserved);

	ch += pe_output_hex_footer(ch);

	*ch = 0;

	if (pe_dprintf(pe_driver_fdout(dctx),"%s",buf) < 0)
		return PERK_FILE_ERROR(dctx);

	return 0;
}

#undef  PE_ADDR
#undef  PE_RAW_STRUCT
#undef  PE_OUTPUT

#define PE_ADDR       &meta->r_opt->opt_hdr_64
#define PE_RAW_STRUCT opt_hdr_64
#define PE_OUTPUT(x)  PE_OUTPUT_TABLE(x)

static int pe_hdrdump_opt_hdr_64(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta)
{
	uint64_t	faddr;
	uint64_t	vaddr;
	char *		ch;
	char		buf[8192];

	if (!PE_ADDR)
		return 0;

	faddr = (char *)(PE_ADDR) - (char *)meta->r_image.map_addr;
	vaddr = meta->m_opt.oh_mem.coh_image_base + faddr;

	ch  = buf;
	ch += pe_output_hex_header(
		ch,
		"struct pe_raw_opt_hdr_64",
		faddr,vaddr,64);

	ch += PE_OUTPUT(coh_magic);
	ch += PE_OUTPUT(coh_major_linker_ver);
	ch += PE_OUTPUT(coh_minor_linker_ver);
	ch += PE_OUTPUT(coh_size_of_code);
	ch += PE_OUTPUT(coh_size_of_inited_data);
	ch += PE_OUTPUT(coh_size_of_uninited_data);
	ch += PE_OUTPUT(coh_entry_point);
	ch += PE_OUTPUT(coh_base_of_code);
	ch += PE_OUTPUT(coh_image_base);
	ch += PE_OUTPUT(coh_section_align);
	ch += PE_OUTPUT(coh_file_align);
	ch += PE_OUTPUT(coh_major_os_ver);
	ch += PE_OUTPUT(coh_minor_os_ver);
	ch += PE_OUTPUT(coh_major_image_ver);
	ch += PE_OUTPUT(coh_minor_image_ver);
	ch += PE_OUTPUT(coh_major_subsys_ver);
	ch += PE_OUTPUT(coh_minor_subsys_ver);
	ch += PE_OUTPUT(coh_win32_ver);
	ch += PE_OUTPUT(coh_size_of_image);
	ch += PE_OUTPUT(coh_size_of_headers);
	ch += PE_OUTPUT(coh_checksum);
	ch += PE_OUTPUT(coh_subsystem);
	ch += PE_OUTPUT(coh_dll_characteristics);
	ch += PE_OUTPUT(coh_size_of_stack_reserve);
	ch += PE_OUTPUT(coh_size_of_stack_commit);
	ch += PE_OUTPUT(coh_size_of_heap_reserve);
	ch += PE_OUTPUT(coh_size_of_heap_commit);
	ch += PE_OUTPUT(coh_loader_flags);
	ch += PE_OUTPUT(coh_rva_and_sizes);
	ch += PE_OUTPUT(coh_export_tbl);
	ch += PE_OUTPUT(coh_import_tbl);
	ch += PE_OUTPUT(coh_resource_tbl);
	ch += PE_OUTPUT(coh_exception_tbl);
	ch += PE_OUTPUT(coh_certificate_tbl);
	ch += PE_OUTPUT(coh_base_reloc_tbl);
	ch += PE_OUTPUT(coh_debug);
	ch += PE_OUTPUT(coh_arch);
	ch += PE_OUTPUT(coh_global_ptr);
	ch += PE_OUTPUT(coh_tls_tbl);
	ch += PE_OUTPUT(coh_load_config_tbl);
	ch += PE_OUTPUT(coh_bound_import);
	ch += PE_OUTPUT(coh_iat);
	ch += PE_OUTPUT(coh_delay_import_descriptor);
	ch += PE_OUTPUT(coh_clr_runtime_hdr);
	ch += PE_OUTPUT(coh_reserved);

	ch += pe_output_hex_footer(ch);

	*ch = 0;

	if (pe_dprintf(pe_driver_fdout(dctx),"%s",buf) < 0)
		return PERK_FILE_ERROR(dctx);

	return 0;
}

int pe_hdrdump_opt_hdr(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta)
{
	switch (pe_image_bits(meta)) {
		case 32:
			return pe_hdrdump_opt_hdr_32(dctx,meta);

		case 64:
			return pe_hdrdump_opt_hdr_64(dctx,meta);

		default:
			return PERK_CUSTOM_ERROR(
				dctx,PERK_ERR_UNSUPPORTED_ABI);
	}
}
