#ifndef PERK_META_H
#define PERK_META_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

enum pe_abi {
	PE_ABI_UNSUPPORTED,
	PE_ABI_PE32,
	PE_ABI_PE64,
	PE_ABI_CAP
};

enum pe_subtype {
	PE_SUBTYPE_UNRECOGNIZED,
	PE_SUBTYPE_DLL,
	PE_SUBTYPE_EXE,
	PE_SUBTYPE_OBJ,
	PE_SUBTYPE_CAP
};

enum pe_framework {
	PE_FRAMEWORK_UNKNOWN,
	PE_FRAMEWORK_FREESTD,
	PE_FRAMEWORK_PSXSCL,
	PE_FRAMEWORK_MIDIPIX,
	PE_FRAMEWORK_CYGONE,
	PE_FRAMEWORK_CYGWIN,
	PE_FRAMEWORK_MINGW,
	PE_FRAMEWORK_MSYS,
	PE_FRAMEWORK_SUACON,
	PE_FRAMEWORK_WINCON,
	PE_FRAMEWORK_WINCLI,
	PE_FRAMEWORK_WIN32,
	PE_FRAMEWORK_CAP
};

/* generic block info / image directory header */
struct pe_block {
	uint32_t	dh_rva;
	uint32_t	dh_size;
};


/* image dos header */
struct pe_meta_image_dos_hdr {
	unsigned char	dos_magic[2];
	uint16_t	dos_cblp;
	uint16_t	dos_cp;
	uint16_t	dos_crlc;
	uint16_t	dos_cparhdr;
	uint16_t	dos_minalloc;
	uint16_t	dos_maxalloc;
	uint16_t	dos_ss;
	uint16_t	dos_sp;
	uint16_t	dos_csum;
	uint16_t	dos_ip;
	uint16_t	dos_cs;
	uint16_t	dos_lfarlc;
	uint16_t	dos_ovno;
	uint16_t	dos_res[4];
	uint16_t	dos_oemid;
	uint16_t	dos_oeminfo;
	uint16_t	dos_res2[10];
	uint32_t	dos_lfanew;
};


/* pe/coff file header */
struct pe_meta_coff_file_hdr {
	unsigned char	cfh_signature[4];
	uint16_t	cfh_machine;
	uint16_t	cfh_num_of_sections;
	uint32_t	cfh_time_date_stamp;
	uint32_t	cfh_ptr_to_sym_tbl;
	uint32_t	cfh_num_of_syms;
	uint16_t	cfh_size_of_opt_hdr;
	uint16_t	cfh_characteristics;
	uint32_t	cfh_ptr_to_str_tbl;
	uint32_t	cfh_size_of_str_tbl;
};


/* image optional header */
struct pe_meta_opt_hdr_std {
	uint16_t	coh_magic;
	uint8_t		coh_major_linker_ver;
	uint8_t		coh_minor_linker_ver;
	uint32_t	coh_size_of_code;
	uint32_t	coh_size_of_inited_data;
	uint32_t	coh_size_of_uninited_data;
	uint32_t	coh_entry_point;
	uint32_t	coh_base_of_code;
};

struct pe_meta_opt_hdr_align {
	uint32_t	coh_section_align;
	uint32_t	coh_file_align;
};

struct pe_meta_opt_hdr_vers {
	uint16_t	coh_major_os_ver;
	uint16_t	coh_minor_os_ver;
	uint16_t	coh_major_image_ver;
	uint16_t	coh_minor_image_ver;
	uint16_t	coh_major_subsys_ver;
	uint16_t	coh_minor_subsys_ver;
	uint32_t	coh_win32_ver;
};

struct pe_meta_opt_hdr_img {
	uint32_t	coh_size_of_image;
	uint32_t	coh_size_of_headers;
	uint32_t	coh_checksum;
	uint16_t	coh_subsystem;
	uint16_t	coh_dll_characteristics;
};

struct pe_meta_opt_hdr_ldr {
	uint32_t	coh_loader_flags;
	uint32_t	coh_rva_and_sizes;
};

struct pe_meta_opt_hdr_dirs {
	struct pe_block	coh_export_tbl;
	struct pe_block	coh_import_tbl;
	struct pe_block	coh_resource_tbl;
	struct pe_block	coh_exception_tbl;
	struct pe_block	coh_certificate_tbl;
	struct pe_block	coh_base_reloc_tbl;
	struct pe_block	coh_debug;
	struct pe_block	coh_arch;
	struct pe_block	coh_global_ptr;
	struct pe_block	coh_tls_tbl;
	struct pe_block	coh_load_config_tbl;
	struct pe_block	coh_bound_import;
	struct pe_block	coh_iat;
	struct pe_block	coh_delay_import_descriptor;
	struct pe_block	coh_clr_runtime_hdr;
	struct pe_block	coh_reserved;
};

struct pe_meta_opt_hdr_mem_32 {
	uint32_t	coh_base_of_data;
	uint32_t	coh_image_base;
	uint32_t	coh_size_of_stack_reserve;
	uint32_t	coh_size_of_stack_commit;
	uint32_t	coh_size_of_heap_reserve;
	uint32_t	coh_size_of_heap_commit;
};

struct pe_meta_opt_hdr_mem_64 {
	uint64_t	coh_image_base;
	uint64_t	coh_size_of_stack_reserve;
	uint64_t	coh_size_of_stack_commit;
	uint64_t	coh_size_of_heap_reserve;
	uint64_t	coh_size_of_heap_commit;
};

struct pe_meta_opt_hdr_mem {
	uint64_t	coh_base_of_data;
	uint64_t	coh_image_base;
	uint64_t	coh_size_of_stack_reserve;
	uint64_t	coh_size_of_stack_commit;
	uint64_t	coh_size_of_heap_reserve;
	uint64_t	coh_size_of_heap_commit;
};

struct pe_meta_opt_hdr {
	struct pe_meta_opt_hdr_std	oh_std;
	struct pe_meta_opt_hdr_vers	oh_vers;
	struct pe_meta_opt_hdr_align	oh_align;
	struct pe_meta_opt_hdr_img	oh_img;
	struct pe_meta_opt_hdr_ldr	oh_ldr;
	struct pe_meta_opt_hdr_mem	oh_mem;
	struct pe_meta_opt_hdr_dirs	oh_dirs;
};


/* section header */
struct pe_meta_sec_hdr {
	char		sh_name[16];
	char *		sh_long_name;
	uint32_t	sh_virtual_size;
	uint32_t	sh_virtual_addr;
	uint32_t	sh_size_of_raw_data;
	uint32_t	sh_ptr_to_raw_data;
	uint32_t	sh_ptr_to_relocs;
	uint32_t	sh_ptr_to_line_nums;
	uint16_t	sh_num_of_relocs;
	uint16_t	sh_num_of_line_nums;
	uint32_t	sh_characteristics;
};


/* .edata section header*/
struct pe_meta_export_hdr {
	uint32_t	eh_export_flags;
	uint32_t	eh_time_date_stamp;
	uint16_t	eh_major_ver;
	uint16_t	eh_minor_ver;
	uint32_t	eh_name_rva;
	uint32_t	eh_ordinal_base;
	uint32_t	eh_addr_tbl_entries;
	uint32_t	eh_num_of_name_ptrs;
	uint32_t	eh_export_addr_tbl_rva;
	uint32_t	eh_name_ptr_rva;
	uint32_t	eh_ordinal_tbl_rva;
};


/* export rva */
union pe_meta_export_addr_tbl {
		uint32_t	ee_export_rva;
		uint32_t	ee_forwarder_rva;
};


/* image: import directory table entry */
struct pe_meta_import_hdr {
	uint32_t				ih_import_lookup_tbl_rva;
	uint32_t				ih_time_date_stamp;
	uint32_t				ih_forwarder_chain;
	uint32_t				ih_name_rva;
	uint32_t				ih_import_addr_tbl_rva;
	uint32_t				ih_count;
	char *					ih_name;
	struct pe_meta_import_lookup *		ih_items;
	union pe_raw_import_lookup *		ih_aitems;
};


/* image: import table entry lookup item */
struct pe_meta_import_lookup {
	union {
		uint64_t	ii_import_lookup_entry_64;
		uint32_t	ii_import_lookup_entry_32;
		uint32_t	ii_hint_name_tbl_rva;
		uint16_t	ii_ordinal_number;
	} u;

	uint32_t	ii_flags;
	uint16_t	ii_hint;
	char *		ii_name;
};


/* coff: symbol table entry */
struct pe_meta_coff_symbol {
	char		cs_name[24];
	char *		cs_long_name;
	uint32_t	cs_value;
	int16_t		cs_section_number;
	uint16_t	cs_type;
	unsigned char	cs_storage_class;
	unsigned char	cs_num_of_aux_symbols;
};


#ifdef __cplusplus
}
#endif

#endif
