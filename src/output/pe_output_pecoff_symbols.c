/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2025  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include <perk/perk.h>
#include <perk/perk_consts.h>
#include <perk/perk_structs.h>
#include <perk/perk_output.h>
#include "perk_reader_impl.h"
#include "perk_driver_impl.h"
#include "perk_dprintf_impl.h"
#include "perk_errinfo_impl.h"

#define PPRIX64 "%"PRIx64

static const char * pe_sym_type_desc_msb[0x10] = {
	[PE_IMAGE_SYM_DTYPE_NULL]     = "scalar variable, ",
	[PE_IMAGE_SYM_DTYPE_POINTER]  = "pointer to ",
	[PE_IMAGE_SYM_DTYPE_FUNCTION] = "function returning ",
	[PE_IMAGE_SYM_DTYPE_ARRAY]    = "array of ",
};

static const char * pe_sym_type_desc_lsb[0x10] = {
	[PE_IMAGE_SYM_TYPE_NULL]      = "unknown type",
	[PE_IMAGE_SYM_TYPE_VOID]      = "void",
	[PE_IMAGE_SYM_TYPE_CHAR]      = "char",
	[PE_IMAGE_SYM_TYPE_SHORT]     = "short",
	[PE_IMAGE_SYM_TYPE_INT]       = "int",
	[PE_IMAGE_SYM_TYPE_LONG]      = "long",
	[PE_IMAGE_SYM_TYPE_FLOAT]     = "float",
	[PE_IMAGE_SYM_TYPE_DOUBLE]    = "double",
	[PE_IMAGE_SYM_TYPE_STRUCT]    = "struct",
	[PE_IMAGE_SYM_TYPE_UNION]     = "union",
	[PE_IMAGE_SYM_TYPE_ENUM]      = "enum",
	[PE_IMAGE_SYM_TYPE_MOE]       = "enum member",
	[PE_IMAGE_SYM_TYPE_BYTE]      = "byte",
	[PE_IMAGE_SYM_TYPE_WORD]      = "word",
	[PE_IMAGE_SYM_TYPE_UINT]      = "uint",
	[PE_IMAGE_SYM_TYPE_DWORD]     = "dword",
};

static const char * pe_sym_storage_class_desc[256] = {
	[PE_IMAGE_SYM_CLASS_NULL]             = "storage class not assigned",
	[PE_IMAGE_SYM_CLASS_AUTOMATIC]        = "automatic (stack) variable",
	[PE_IMAGE_SYM_CLASS_EXTERNAL]         = "external symbol",
	[PE_IMAGE_SYM_CLASS_STATIC]           = "static variable",
	[PE_IMAGE_SYM_CLASS_REGISTER]         = "register variable",
	[PE_IMAGE_SYM_CLASS_EXTERNAL_DEF]     = "externally defined variable",
	[PE_IMAGE_SYM_CLASS_LABEL]            = "label",
	[PE_IMAGE_SYM_CLASS_UNDEFINED_LABEL]  = "undefined label",
	[PE_IMAGE_SYM_CLASS_MEMBER_OF_STRUCT] = "struct member",
	[PE_IMAGE_SYM_CLASS_ARGUMENT]         = "formal function parameter",
	[PE_IMAGE_SYM_CLASS_STRUCT_TAG]       = "struct tag",
	[PE_IMAGE_SYM_CLASS_MEMBER_OF_UNION]  = "union member",
	[PE_IMAGE_SYM_CLASS_UNION_TAG]        = "union tag",
	[PE_IMAGE_SYM_CLASS_TYPE_DEFINITION]  = "type definition",
	[PE_IMAGE_SYM_CLASS_UNDEFINED_STATIC] = "undefined (.bss) static data",
	[PE_IMAGE_SYM_CLASS_ENUM_TAG]         = "enum tag",
	[PE_IMAGE_SYM_CLASS_MEMBER_OF_ENUM]   = "enum member",
	[PE_IMAGE_SYM_CLASS_REGISTER_PARAM]   = "register parameter",
	[PE_IMAGE_SYM_CLASS_BIT_FIELD]        = "bit field",

	[PE_IMAGE_SYM_CLASS_BLOCK]            = "a beginning-of-block or end-of-block record",
	[PE_IMAGE_SYM_CLASS_FUNCTION]         = "function record",
	[PE_IMAGE_SYM_CLASS_END_OF_STRUCT]    = "end-of-struct",
	[PE_IMAGE_SYM_CLASS_FILE]             = "file",
	[PE_IMAGE_SYM_CLASS_SECTION]          = "section definition",
	[PE_IMAGE_SYM_CLASS_WEAK_EXTERN]      = "weak external",

	[PE_IMAGE_SYM_CLASS_CLR_TOKEN]        = "CLR token",

	[PE_IMAGE_SYM_CLASS_END_OF_FUNC]      = "end-of-function"
};

static const char * pe_weak_extern_switches[4] = {
	[PE_IMAGE_WEAK_EXTERN_SEARCH_NOLIBRARY]  = "do not perform a library search",
	[PE_IMAGE_WEAK_EXTERN_SEARCH_LIBRARY]    = "do perform a library search",
	[PE_IMAGE_WEAK_EXTERN_SEARCH_ALIAS]      = "apply weak alias semantics",
};

static const char * pe_comdat_select_desc[0x7] = {
	[PE_IMAGE_COMDAT_SELECT_NODUPLICATES]    = "PE_IMAGE_COMDAT_SELECT_NODUPLICATES",
	[PE_IMAGE_COMDAT_SELECT_ANY]             = "PE_IMAGE_COMDAT_SELECT_ANY",
	[PE_IMAGE_COMDAT_SELECT_SAME_SIZE]       = "PE_IMAGE_COMDAT_SELECT_SAME_SIZE",
	[PE_IMAGE_COMDAT_SELECT_EXACT_MATCH]     = "PE_IMAGE_COMDAT_SELECT_EXACT_MATCH",
	[PE_IMAGE_COMDAT_SELECT_ASSOCIATIVE]     = "PE_IMAGE_COMDAT_SELECT_ASSOCIATIVE",
	[PE_IMAGE_COMDAT_SELECT_LARGEST]         = "PE_IMAGE_COMDAT_SELECT_LARGEST",
};

static int pe_output_symbol_names(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta,
	int                             fdout)
{
	struct pe_meta_coff_symbol *	symrec;

	for (symrec=meta->m_symtbl; symrec->cs_name; symrec++)
		if (pe_dprintf(fdout,"%s\n",symrec->cs_name) < 0)
			return PERK_FILE_ERROR(dctx);

	return 0;
}

static int pe_output_symbol_names_yaml(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta,
	int                             fdout)
{
	struct pe_meta_coff_symbol *	symrec;

	if (pe_dprintf(fdout,"  - Symbols:\n") < 0)
		return PERK_FILE_ERROR(dctx);

	for (symrec=meta->m_symtbl; symrec->cs_name; symrec++)
		if (pe_dprintf(fdout,"    - [ symbol: %s ]\n",symrec->cs_name) < 0)
			return PERK_FILE_ERROR(dctx);

	return 0;
}

static int pe_output_symbol_records_yaml(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta,
	int                             fdout)
{
	struct pe_meta_coff_symbol *	symrec;
	struct pe_meta_sec_hdr *        sechdr;
	const char *                    secname;
	const char *                    classdesc;
	const char *                    typedesc[2];

	if (pe_dprintf(fdout,"  - Symbols:\n") < 0)
		return PERK_FILE_ERROR(dctx);

	for (symrec=meta->m_symtbl; symrec->cs_name; symrec++) {
		switch (symrec->cs_section_number) {
			case PE_IMAGE_SYM_UNDEFINED:
				secname = "NULL (section is not yet defined)";
				break;

			case PE_IMAGE_SYM_ABSOLUTE:
				secname = "N/A (symbol is an absolute value)";
				break;

			case PE_IMAGE_SYM_DEBUG:
				secname = "N/A (debug symbol only)";
				break;

			default:
				sechdr  = &meta->m_sectbl[symrec->cs_section_number - 1];
				secname = sechdr->sh_name;
		}

		typedesc[0] = pe_sym_type_desc_msb[(symrec->cs_type >> 4) & 0x03];
		typedesc[1] = pe_sym_type_desc_lsb[(symrec->cs_type >> 0) & 0x0f];

		if (!(classdesc = pe_sym_storage_class_desc[symrec->cs_storage_class]))
			classdesc = "UNRECOGNIZED DATA";

		if (pe_dprintf(fdout,
				"    - symbol:\n"
				"      - [ name:       %s ]\n"
				"      - [ value:      0x%08x ]\n"
				"      - [ crc32:      0x%08x ]\n"
				"      - [ crc64:      0x"PPRIX64" ]\n"
				"      - [ secnum:     %d (one-based) ]\n"
				"      - [ secname:    %s ]\n"
				"      - [ type:       0x%02X ]\n"
				"      - [ typedesc:   %s%s ]\n"
				"      - [ class:      %d ]\n"
				"      - [ classname:  %s ]\n"
				"      - [ aux-recs:   %d ]\n"
				"\n",
				symrec->cs_name,
				symrec->cs_value,
				symrec->cs_crc32,
				symrec->cs_crc64,
				symrec->cs_section_number,
				secname,
				symrec->cs_type,
				typedesc[0],typedesc[1],
				symrec->cs_storage_class,
				classdesc,
				symrec->cs_num_of_aux_recs) < 0)
			return PERK_FILE_ERROR(dctx);

		if (symrec->cs_storage_class == PE_IMAGE_SYM_CLASS_WEAK_EXTERN) {
			const struct pe_raw_coff_symbol * coffsym;
			struct pe_meta_aux_rec_weaksym    auxrec;
			int                               idx;

			coffsym = (const struct pe_raw_coff_symbol *)symrec->cs_aux_recs;
			coffsym--;

			if (pe_dprintf(fdout,"      - aux-rec:\n") < 0)
				return PERK_SYSTEM_ERROR(dctx);

			for (idx=0; idx<symrec->cs_num_of_aux_recs; idx++) {
				pe_read_aux_rec_weaksym(coffsym,&auxrec,idx);

				if (pe_dprintf(fdout,
						"        - [ tag-index:             %d ]\n"
						"        - [ tag-characteristics:   0x%01X (%s) ]\n\n",
						auxrec.aux_tag_index,
						auxrec.aux_characteristics,
						pe_weak_extern_switches[auxrec.aux_characteristics & 0x03]) < 0)
					return PERK_SYSTEM_ERROR(dctx);
			}

		} else if (!strcmp(symrec->cs_name,secname)) {
			const struct pe_raw_coff_symbol * coffsym;
			struct pe_meta_aux_rec_section    auxrec;
			int                               idx;
			char                              selection[64];
			const int                         seldesclo = PE_IMAGE_COMDAT_SELECT_NODUPLICATES;
			const int                         seldeschi = PE_IMAGE_COMDAT_SELECT_LARGEST;

			coffsym = (const struct pe_raw_coff_symbol *)symrec->cs_aux_recs;
			coffsym--;

			if (pe_dprintf(fdout,"      - aux-rec:\n") < 0)
				return PERK_SYSTEM_ERROR(dctx);

			for (idx=0; idx<symrec->cs_num_of_aux_recs; idx++) {
				pe_read_aux_rec_section(coffsym,&auxrec,idx);

				if ((auxrec.aux_selection >= seldesclo) && (auxrec.aux_selection <= seldeschi)) {
					snprintf(selection,sizeof(selection),"%u (%s)",
						auxrec.aux_selection,
						pe_comdat_select_desc[auxrec.aux_selection]);
				} else {
					snprintf(selection,sizeof(selection),"%u",
						auxrec.aux_selection);
				}

				if (pe_dprintf(fdout,
						"        - [ size:              0x%08x ]\n"
						"        - [ num-of-relocs:     0x%08X ]\n"
						"        - [ num-of-line-nums:  0x%08X ]\n"
						"        - [ check-sum:         0x%08X ]\n"
						"        - [ number:            %u ]\n"
						"        - [ selection:         %s ]\n"
						"\n",
						auxrec.aux_size,
						auxrec.aux_num_of_relocs,
						auxrec.aux_num_of_line_nums,
						auxrec.aux_check_sum,
						auxrec.aux_number,
						selection) < 0)
					return PERK_SYSTEM_ERROR(dctx);
			}
		}
	}

	return 0;
}

static int pe_output_pecoff_symbols_yaml(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta,
	int                             fdout)
{
	if (dctx->cctx->fmtflags & PERK_PRETTY_VERBOSE) {
		if (pe_output_symbol_records_yaml(dctx,meta,fdout) < 0)
			return PERK_NESTED_ERROR(dctx);
	} else {
		if (pe_output_symbol_names_yaml(dctx,meta,fdout) < 0)
			return PERK_NESTED_ERROR(dctx);
	}

	return 0;
}

int pe_output_pecoff_symbols(
	const struct pe_driver_ctx *	dctx,
	const struct pe_image_meta *	meta)
{
	int fdout = pe_driver_fdout(dctx);

	if (!meta->m_symtbl)
		return 0;

	if (dctx->cctx->fmtflags & PERK_PRETTY_YAML) {
		if (pe_output_pecoff_symbols_yaml(dctx,meta,fdout) < 0)
			return PERK_NESTED_ERROR(dctx);

	} else {
		if (pe_output_symbol_names(dctx,meta,fdout) < 0)
			return PERK_NESTED_ERROR(dctx);
	}

	return 0;
}
