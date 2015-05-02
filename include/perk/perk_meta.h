#ifndef PERK_META_H
#define PERK_META_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* generic block info / image directory header */
struct pe_block {
	uint32_t	rva;
	uint32_t	size;
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
	unsigned char	signature[4];
	uint16_t	machine;
	uint16_t	num_of_sections;
	uint32_t	time_date_stamp;
	uint32_t	ptr_to_sym_tbl;
	uint32_t	num_of_syms;
	uint16_t	size_of_opt_hdr;
	uint16_t	characteristics;
};


/* image optional header */
struct pe_meta_opt_hdr_std {
	uint16_t	magic;
	uint8_t		major_linker_ver;
	uint8_t		minor_linker_ver;
	uint32_t	size_of_code;
	uint32_t	size_of_initialized_data;
	uint32_t	size_of_uninitialized_data;
	uint32_t	entry_point;
	uint32_t	base_of_code;
};

struct pe_meta_opt_hdr_vers {
	uint16_t	major_os_ver;
	uint16_t	minor_or_ver;
	uint16_t	major_image_ver;
	uint16_t	minor_image_ver;
	uint16_t	major_subsys_ver;
	uint16_t	minor_subsys_ver;
	uint32_t	win32_ver;
};

struct pe_meta_opt_hdr_align {
	uint32_t	section_align;
	uint32_t	file_align;
};

struct pe_meta_opt_hdr_img {
	uint32_t	size_of_image;
	uint32_t	size_of_headers;
	uint32_t	checksum;
	uint16_t	subsystem;
	uint16_t	dll_characteristics;
};

struct pe_meta_opt_hdr_ldr {
	uint32_t	loader_flags;
	uint32_t	rva_and_sizes;
};

struct pe_meta_opt_hdr_dirs {
	struct pe_block	export_tbl;
	struct pe_block	import_tbl;
	struct pe_block	resource_tbl;
	struct pe_block	exception_tbl;
	struct pe_block	certificate_tbl;
	struct pe_block	base_reloc_tbl;
	struct pe_block	debug;
	struct pe_block	arch;
	struct pe_block	global_ptr;
	struct pe_block	tls_tbl;
	struct pe_block	load_config_tbl;
	struct pe_block	bound_import;
	struct pe_block	iat;
	struct pe_block	delay_import_descriptor;
	struct pe_block	clr_runtime_hdr;
	struct pe_block	reserved;
};

struct pe_meta_opt_hdr_mem_32 {
	uint32_t	base_of_data;
	uint32_t	image_base;
	uint32_t	size_of_stack_reserve;
	uint32_t	size_of_stack_commit;
	uint32_t	size_of_heap_reserve;
	uint32_t	size_of_heap_commit;
};

struct pe_meta_opt_hdr_mem_64 {
	uint64_t	image_base;
	uint64_t	size_of_stack_reserve;
	uint64_t	size_of_stack_commit;
	uint64_t	size_of_heap_reserve;
	uint64_t	size_of_heap_commit;
};

struct pe_meta_opt_hdr_mem {
	uint64_t	base_of_data;
	uint64_t	image_base;
	uint64_t	size_of_stack_reserve;
	uint64_t	size_of_stack_commit;
	uint64_t	size_of_heap_reserve;
	uint64_t	size_of_heap_commit;
};

struct pe_meta_opt_hdr {
	struct pe_meta_opt_hdr_std	std;
	struct pe_meta_opt_hdr_vers	vers;
	struct pe_meta_opt_hdr_align	align;
	struct pe_meta_opt_hdr_img	img;
	struct pe_meta_opt_hdr_ldr	ldr;
	struct pe_meta_opt_hdr_mem	mem;
	struct pe_meta_opt_hdr_dirs	dirs;
};


/* section header */
struct pe_meta_sec_hdr {
	char		name[16];
	char *		long_name;
	uint32_t	virtual_size;
	uint32_t	virtual_addr;
	uint32_t	size_of_raw_data;
	uint32_t	ptr_to_raw_data;
	uint32_t	ptr_to_relocs;
	uint32_t	ptr_to_line_nums;
	uint16_t	num_of_relocs;
	uint16_t	num_of_line_nums;
	uint32_t	characteristics;
};


/* .edata section header*/
struct pe_meta_export_hdr {
	uint32_t	export_flags;
	uint32_t	time_date_stamp;
	uint16_t	major_ver;
	uint16_t	minor_ver;
	uint32_t	name_rva;
	uint32_t	ordinal_base;
	uint32_t	addr_tbl_entries;
	uint32_t	num_of_name_ptrs;
	uint32_t	export_addr_tbl_rva;
	uint32_t	name_ptr_rva;
	uint32_t	ordinal_tbl_rva;
};


/* export rva */
union pe_meta_export_addr_tbl {
		uint32_t	  export_rva;
		uint32_t	  forwarder_rva;
};


/* image: import table entry lookup item */
struct pe_meta_import_lookup_item {
	union {
		uint64_t	import_lookup_entry_64;
		uint32_t	import_lookup_entry_32;
		uint32_t	hint_name_tbl_rva;
		uint16_t	ordinal_number;
	} u;

	uint32_t	flags;
	uint16_t	hint;
	char *		name;
};


/* image: import directory table entry */
struct pe_meta_import_hdr {
	uint32_t				import_lookup_tbl_rva;
	uint32_t				time_date_stamp;
	uint32_t				forwarder_chain;
	uint32_t				name_rva;
	uint32_t				import_addr_tbl_rva;
	uint32_t				count;
	char *					name;
	struct pe_meta_import_lookup_item *	items;
	struct pe_import_lookup_item *		aitems;
};


#ifdef __cplusplus
}
#endif

#endif
