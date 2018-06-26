API_SRCS = \
	src/driver/pe_amain.c \
	src/driver/pe_driver_ctx.c \
	src/driver/pe_unit_ctx.c \
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

INTERNAL_SRCS = \
	src/internal/$(PACKAGE)_dprintf_impl.c \
	src/internal/$(PACKAGE)_errinfo_impl.c \

APP_SRCS = \
	src/perk.c

COMMON_SRCS = $(API_SRCS) $(INTERNAL_SRCS)
