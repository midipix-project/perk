/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2017  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <string.h>

#include <perk/perk.h>
#include <perk/perk_consts.h>
#include <perk/perk_structs.h>
#include "perk_endian_impl.h"
#include "perk_reader_impl.h"

int pe_read_coff_header(const struct pe_raw_coff_image_hdr * p, struct pe_meta_coff_file_hdr * m)
{
	if ((p->cfh_signature[0] != 'P') || (p->cfh_signature[1] != 'E')
			|| p->cfh_signature[2] || p->cfh_signature[3])
		return PERK_ERR_BAD_COFF_HEADER;

	m->cfh_signature[0] = p->cfh_signature[0];
	m->cfh_signature[1] = p->cfh_signature[1];
	m->cfh_signature[2] = p->cfh_signature[2];
	m->cfh_signature[3] = p->cfh_signature[3];

	m->cfh_machine		= pe_read_short(p->cfh_machine);
	m->cfh_num_of_sections	= pe_read_short(p->cfh_num_of_sections);

	m->cfh_time_date_stamp	= pe_read_long(p->cfh_time_date_stamp);
	m->cfh_ptr_to_sym_tbl	= pe_read_long(p->cfh_ptr_to_sym_tbl);
	m->cfh_num_of_syms	= pe_read_long(p->cfh_num_of_syms);

	m->cfh_size_of_opt_hdr	= pe_read_short(p->cfh_size_of_opt_hdr);
	m->cfh_characteristics	= pe_read_short(p->cfh_characteristics);

	return 0;
}

#define   PE_IMAGE_FILE_CHARACTERISTICS (\
          PE_IMAGE_FILE_RELOCS_STRIPPED   \
	| PE_IMAGE_FILE_EXECUTABLE_IMAGE   \
	| PE_IMAGE_FILE_LINE_NUMS_STRIPPED  \
	| PE_IMAGE_FILE_LOCAL_SYMS_STRIPPED  \
	| PE_IMAGE_FILE_AGGRESSIVE_WS_TRIM    \
	| PE_IMAGE_FILE_LARGE_ADDRESS_AWARE    \
	| PE_IMAGE_FILE_RESERVED_CHARACTERISTIC \
	| PE_IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP  \
	| PE_IMAGE_FILE_BYTES_REVERSED_LO        \
	| PE_IMAGE_FILE_32BIT_MACHINE           \
	| PE_IMAGE_FILE_DEBUG_STRIPPED         \
	| PE_IMAGE_FILE_NET_RUN_FROM_SWAP     \
	| PE_IMAGE_FILE_SYSTEM               \
	| PE_IMAGE_FILE_DLL                 \
	| PE_IMAGE_FILE_UP_SYSTEM_ONLY     \
	| PE_IMAGE_FILE_BYTES_REVERSED_HI)

int pe_read_object_header(const struct pe_raw_coff_object_hdr * p, struct pe_meta_coff_file_hdr * m)
{
	if (pe_read_short(p->cfh_size_of_opt_hdr))
		return PERK_ERR_BAD_COFF_HEADER;

	if (pe_read_short(p->cfh_characteristics) & ~PE_IMAGE_FILE_CHARACTERISTICS)
		return PERK_ERR_BAD_COFF_HEADER;

	switch (pe_read_short(p->cfh_machine)) {
		case PE_IMAGE_FILE_MACHINE_I386:
		case PE_IMAGE_FILE_MACHINE_R4000:
		case PE_IMAGE_FILE_MACHINE_WCEMIPSV2:
		case PE_IMAGE_FILE_MACHINE_SH3:
		case PE_IMAGE_FILE_MACHINE_SH3DSP:
		case PE_IMAGE_FILE_MACHINE_SH4:
		case PE_IMAGE_FILE_MACHINE_SH5:
		case PE_IMAGE_FILE_MACHINE_ARM:
		case PE_IMAGE_FILE_MACHINE_THUMB:
		case PE_IMAGE_FILE_MACHINE_ARMV7:
		case PE_IMAGE_FILE_MACHINE_AM33:
		case PE_IMAGE_FILE_MACHINE_POWERPC:
		case PE_IMAGE_FILE_MACHINE_POWERPCFP:
		case PE_IMAGE_FILE_MACHINE_IA64:
		case PE_IMAGE_FILE_MACHINE_MIPS16:
		case PE_IMAGE_FILE_MACHINE_MIPSFPU:
		case PE_IMAGE_FILE_MACHINE_MIPSFPU16:
		case PE_IMAGE_FILE_MACHINE_EBC:
		case PE_IMAGE_FILE_MACHINE_AMD64:
		case PE_IMAGE_FILE_MACHINE_M32R:
			break;

		default:
			return PERK_ERR_BAD_COFF_HEADER;
	}

	m->cfh_signature[0] = 0;
	m->cfh_signature[1] = 0;
	m->cfh_signature[2] = 0;
	m->cfh_signature[3] = 0;

	m->cfh_machine		= pe_read_short(p->cfh_machine);
	m->cfh_num_of_sections	= pe_read_short(p->cfh_num_of_sections);

	m->cfh_time_date_stamp	= pe_read_long(p->cfh_time_date_stamp);
	m->cfh_ptr_to_sym_tbl	= pe_read_long(p->cfh_ptr_to_sym_tbl);
	m->cfh_num_of_syms	= pe_read_long(p->cfh_num_of_syms);

	m->cfh_size_of_opt_hdr	= pe_read_short(p->cfh_size_of_opt_hdr);
	m->cfh_characteristics	= pe_read_short(p->cfh_characteristics);

	return 0;
}
