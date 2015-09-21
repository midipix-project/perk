COMMON_SRCS = \
	src/main/pe_get_image_meta.c \
	src/main/pe_map_raw_image.c \
	src/reader/pe_read_import_header.c \
	src/reader/pe_read_export_header.c \
	src/reader/pe_read_dos_header.c \
	src/reader/pe_read_coff_header.c \
	src/reader/pe_read_section_header.c \
	src/reader/pe_read_optional_header.c \
	src/output/pe_output_export_symbols.c

APP_SRCS = \
	src/main/perk.c
