API_SRCS = \
	src/arbits/pe_archive_meta.c \
	src/arbits/pe_armap_bsd_32.c \
	src/arbits/pe_armap_bsd_64.c \
	src/arbits/pe_armap_sysv_32.c \
	src/arbits/pe_armap_sysv_64.c \
	src/cmds/pe_cmd_perk.c \
	src/driver/pe_amain.c \
	src/driver/pe_driver_ctx.c \
	src/driver/pe_unit_ctx.c \
	src/hexdump/pe_hexdump_export_hdr.c \
	src/hexdump/pe_hexdump_image_dos_hdr.c \
	src/hexdump/pe_hexdump_import_tbl.c \
	src/hexdump/pe_hexdump_coff_image_hdr.c \
	src/hexdump/pe_hexdump_coff_object_hdr.c \
	src/hexdump/pe_hexdump_coff_opt_hdr.c \
	src/hexdump/pe_hexdump_sec_tbl.c \
	src/info/pe_get_image_abi.c \
	src/info/pe_get_image_framework.c \
	src/info/pe_get_image_subsystem.c \
	src/info/pe_get_image_subtype.c \
	src/logic/pe_get_image_meta.c \
	src/logic/pe_map_raw_image.c \
	src/output/pe_output_error.c \
	src/output/pe_output_export_symbols.c \
	src/output/pe_output_image_category.c \
	src/output/pe_output_image_sections.c \
	src/output/pe_output_image_strings.c \
	src/output/pe_output_image_symbols.c \
	src/output/pe_output_idata_libraries.c \
	src/output/pe_output_mdso_libraries.c \
	src/reader/pe_read_coff_header.c \
	src/reader/pe_read_coff_symbol.c \
	src/reader/pe_read_dos_header.c \
	src/reader/pe_read_export_header.c \
	src/reader/pe_read_import_header.c \
	src/reader/pe_read_optional_header.c \
	src/reader/pe_read_section_header.c \
	src/skin/pe_skin_default.c \
	src/skin/pe_skin_perk.c \

INTERNAL_SRCS = \
	src/internal/$(PACKAGE)_dprintf_impl.c \
	src/internal/$(PACKAGE)_errinfo_impl.c \
	src/internal/$(PACKAGE)_hexdump_impl.c \

APP_SRCS = \
	src/perk.c

COMMON_SRCS = $(API_SRCS) $(INTERNAL_SRCS)
