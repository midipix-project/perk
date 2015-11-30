#ifndef PERK_STRUCTS_H
#define PERK_STRUCTS_H

#ifdef __cplusplus
extern "C" {
#endif

struct pe_image_dos_hdr {
	unsigned char	dos_magic			[0x02];		/* 0x00 */
	unsigned char	dos_cblp			[0x02];		/* 0x02 */
	unsigned char	dos_cp				[0x02];		/* 0x04 */
	unsigned char	dos_crlc			[0x02];		/* 0x06 */
	unsigned char	dos_cparhdr			[0x02];		/* 0x08 */
	unsigned char	dos_minalloc			[0x02];		/* 0x0a */
	unsigned char	dos_maxalloc			[0x02];		/* 0x0c */
	unsigned char	dos_ss				[0x02];		/* 0x0e */
	unsigned char	dos_sp				[0x02];		/* 0x10 */
	unsigned char	dos_csum			[0x02];		/* 0x12 */
	unsigned char	dos_ip				[0x02];		/* 0x14 */
	unsigned char	dos_cs				[0x02];		/* 0x16 */
	unsigned char	dos_lfarlc			[0x02];		/* 0x18 */
	unsigned char	dos_ovno			[0x02];		/* 0x1a */
	unsigned char	dos_res				[0x08];		/* 0x1c */
	unsigned char	dos_oemid			[0x02];		/* 0x24 */
	unsigned char	dos_oeminfo			[0x02];		/* 0x26 */
	unsigned char	dos_res2			[0x14];		/* 0x28 */
	unsigned char	dos_lfanew			[0x04];		/* 0x3c */
};


struct pe_coff_file_hdr {
	unsigned char	signature			[0x04];		/* 0x00 */
	unsigned char	machine				[0x02];		/* 0x04 */
	unsigned char	num_of_sections			[0x02];		/* 0x06 */
	unsigned char	time_date_stamp			[0x04];		/* 0x08 */
	unsigned char	ptr_to_sym_tbl			[0x04];		/* 0x0c */
	unsigned char	num_of_syms			[0x04];		/* 0x10 */
	unsigned char	size_of_opt_hdr			[0x02];		/* 0x14 */
	unsigned char	characteristics			[0x02];		/* 0x16 */
};


struct pe_opt_hdr_std {
	unsigned char	magic				[0x02];		/* 0x00 */
	unsigned char	major_linker_ver		[0x01];		/* 0x02 */
	unsigned char	minor_linker_ver		[0x01];		/* 0x03 */
	unsigned char	size_of_code			[0x04];		/* 0x04 */
	unsigned char	size_of_initialized_data	[0x04];		/* 0x08 */
	unsigned char	size_of_uninitialized_data	[0x04];		/* 0x0c */
	unsigned char	entry_point			[0x04];		/* 0x10 */
	unsigned char	base_of_code			[0x04];		/* 0x14 */
};

struct pe_opt_hdr_align {
	unsigned char	section_align			[0x04];		/* 0x20 */
	unsigned char	file_align			[0x04];         /* 0x24 */
};

struct pe_opt_hdr_vers {
	unsigned char	major_os_ver			[0x02];		/* 0x28 */
	unsigned char	minor_os_ver			[0x02];		/* 0x2a */
	unsigned char	major_image_ver			[0x02];		/* 0x2c */
	unsigned char	minor_image_ver			[0x02];		/* 0x2e */
	unsigned char	major_subsys_ver		[0x02];		/* 0x30 */
	unsigned char	minor_subsys_ver		[0x02];		/* 0x32 */
	unsigned char	win32_ver			[0x04];		/* 0x34 */
};

struct pe_opt_hdr_img {
	unsigned char	size_of_image			[0x04];		/* 0x38 */
	unsigned char	size_of_headers			[0x04];		/* 0x3c */
	unsigned char	checksum			[0x04];		/* 0x40 */
	unsigned char	subsystem			[0x02];		/* 0x44 */
	unsigned char	dll_characteristics		[0x02];		/* 0x46 */
};

struct pe_opt_hdr_ldr {
	unsigned char	loader_flags			[0x04];
	unsigned char	rva_and_sizes			[0x04];
};

struct pe_opt_hdr_dirs {
	unsigned char	export_tbl			[0x08];
	unsigned char	import_tbl			[0x08];
	unsigned char	resource_tbl			[0x08];
	unsigned char	exception_tbl			[0x08];
	unsigned char	certificate_tbl			[0x08];
	unsigned char	base_reloc_tbl			[0x08];
	unsigned char	debug				[0x08];
	unsigned char	arch				[0x08];
	unsigned char	global_ptr			[0x08];
	unsigned char	tls_tbl				[0x08];
	unsigned char	load_config_tbl			[0x08];
	unsigned char	bound_import			[0x08];
	unsigned char	iat				[0x08];
	unsigned char	delay_import_descriptor		[0x08];
	unsigned char	clr_runtime_hdr			[0x08];
	unsigned char	reserved			[0x08];
};

struct pe_data_dirs {
	unsigned char	rva_and_sizes			[0x04];
	unsigned char	export_tbl			[0x08];
	unsigned char	import_tbl			[0x08];
	unsigned char	resource_tbl			[0x08];
	unsigned char	exception_tbl			[0x08];
	unsigned char	certificate_tbl			[0x08];
	unsigned char	base_reloc_tbl			[0x08];
	unsigned char	debug				[0x08];
	unsigned char	arch				[0x08];
	unsigned char	global_ptr			[0x08];
	unsigned char	tls_tbl				[0x08];
	unsigned char	load_config_tbl			[0x08];
	unsigned char	bound_import			[0x08];
	unsigned char	iat				[0x08];
	unsigned char	delay_import_descriptor		[0x08];
	unsigned char	clr_runtime_hdr			[0x08];
	unsigned char	reserved			[0x08];
};

struct pe_opt_hdr_32 {
	unsigned char	magic				[0x02];		/* 0x00 */
	unsigned char	major_linker_ver		[0x01];		/* 0x02 */
	unsigned char	minor_linker_ver		[0x01];		/* 0x03 */
	unsigned char	size_of_code			[0x04];		/* 0x04 */
	unsigned char	size_of_initialized_data	[0x04];		/* 0x08 */
	unsigned char	size_of_uninitialized_data	[0x04];		/* 0x0c */
	unsigned char	entry_point			[0x04];		/* 0x10 */
	unsigned char	base_of_code			[0x04];		/* 0x14 */
	unsigned char	base_of_data			[0x04];		/* 0x18 */
	unsigned char	image_base			[0x04];		/* 0x1c */
	unsigned char	section_align			[0x04];		/* 0x20 */
	unsigned char	file_align			[0x04];         /* 0x24 */
	unsigned char	major_os_ver			[0x02];		/* 0x28 */
	unsigned char	minor_os_ver			[0x02];		/* 0x2a */
	unsigned char	major_image_ver			[0x02];		/* 0x2c */
	unsigned char	minor_image_ver			[0x02];		/* 0x2e */
	unsigned char	major_subsys_ver		[0x02];		/* 0x30 */
	unsigned char	minor_subsys_ver		[0x02];		/* 0x32 */
	unsigned char	win32_ver			[0x04];		/* 0x34 */
	unsigned char	size_of_image			[0x04];		/* 0x38 */
	unsigned char	size_of_headers			[0x04];		/* 0x3c */
	unsigned char	checksum			[0x04];		/* 0x40 */
	unsigned char	subsystem			[0x02];		/* 0x44 */
	unsigned char	dll_characteristics		[0x02];		/* 0x46 */
	unsigned char	size_of_stack_reserve		[0x04];		/* 0x48 */
	unsigned char	size_of_stack_commit		[0x04];		/* 0x4c */
	unsigned char	size_of_heap_reserve		[0x04];		/* 0x50 */
	unsigned char	size_of_heap_commit		[0x04];		/* 0x54 */
	unsigned char	loader_flags			[0x04];		/* 0x58 */
	unsigned char	rva_and_sizes			[0x04];		/* 0x5c */
	unsigned char	export_tbl			[0x08];		/* 0x60 */
	unsigned char	import_tbl			[0x08];		/* 0x68 */
	unsigned char	resource_tbl			[0x08];		/* 0x70 */
	unsigned char	exception_tbl			[0x08];		/* 0x78 */
	unsigned char	certificate_tbl			[0x08];		/* 0x80 */
	unsigned char	base_reloc_tbl			[0x08];		/* 0x88 */
	unsigned char	debug				[0x08];		/* 0x90 */
	unsigned char	arch				[0x08];		/* 0x98 */
	unsigned char	global_ptr			[0x08];		/* 0xa0 */
	unsigned char	tls_tbl				[0x08];		/* 0xa8 */
	unsigned char	load_config_tbl			[0x08];		/* 0xb0 */
	unsigned char	bound_import			[0x08];		/* 0xb8 */
	unsigned char	iat				[0x08];		/* 0xc0 */
	unsigned char	delay_import_descriptor		[0x08];		/* 0xc8 */
	unsigned char	clr_runtime_hdr			[0x08];		/* 0xd0 */
	unsigned char	reserved			[0x08];		/* 0xd8 */
};


struct pe_opt_hdr_64 {
	unsigned char	magic				[0x02];		/* 0x00 */
	unsigned char	major_linker_ver		[0x01];		/* 0x02 */
	unsigned char	minor_linker_ver		[0x01];		/* 0x03 */
	unsigned char	size_of_code			[0x04];		/* 0x04 */
	unsigned char	size_of_initialized_data	[0x04];		/* 0x08 */
	unsigned char	size_of_uninitialized_data	[0x04];		/* 0x0c */
	unsigned char	entry_point			[0x04];		/* 0x10 */
	unsigned char	base_of_code			[0x04];		/* 0x14 */
	unsigned char	image_base			[0x08];		/* 0x18 */
	unsigned char	section_align			[0x04];		/* 0x20 */
	unsigned char	file_align			[0x04];         /* 0x24 */
	unsigned char	major_os_ver			[0x02];		/* 0x28 */
	unsigned char	minor_os_ver			[0x02];		/* 0x2a */
	unsigned char	major_image_ver			[0x02];		/* 0x2c */
	unsigned char	minor_image_ver			[0x02];		/* 0x2e */
	unsigned char	major_subsys_ver		[0x02];		/* 0x30 */
	unsigned char	minor_subsys_ver		[0x02];		/* 0x32 */
	unsigned char	win32_ver			[0x04];		/* 0x34 */
	unsigned char	size_of_image			[0x04];		/* 0x38 */
	unsigned char	size_of_headers			[0x04];		/* 0x3c */
	unsigned char	checksum			[0x04];		/* 0x40 */
	unsigned char	subsystem			[0x02];		/* 0x44 */
	unsigned char	dll_characteristics		[0x02];		/* 0x46 */
	unsigned char	size_of_stack_reserve		[0x08];		/* 0x48 */
	unsigned char	size_of_stack_commit		[0x08];		/* 0x50 */
	unsigned char	size_of_heap_reserve		[0x08];		/* 0x58 */
	unsigned char	size_of_heap_commit		[0x08];		/* 0x60 */
	unsigned char	loader_flags			[0x04];		/* 0x68 */
	unsigned char	rva_and_sizes			[0x04];		/* 0x6c */
	unsigned char	export_tbl			[0x08];		/* 0x70 */
	unsigned char	import_tbl			[0x08];		/* 0x78 */
	unsigned char	resource_tbl			[0x08];		/* 0x80 */
	unsigned char	exception_tbl			[0x08];		/* 0x88 */
	unsigned char	certificate_tbl			[0x08];		/* 0x90 */
	unsigned char	base_reloc_tbl			[0x08];		/* 0x98 */
	unsigned char	debug				[0x08];		/* 0xa0 */
	unsigned char	arch				[0x08];		/* 0xa8 */
	unsigned char	global_ptr			[0x08];		/* 0xb0 */
	unsigned char	tls_tbl				[0x08];		/* 0xb8 */
	unsigned char	load_config_tbl			[0x08];		/* 0xc0 */
	unsigned char	bound_import			[0x08];		/* 0xc8 */
	unsigned char	iat				[0x08];		/* 0xd0 */
	unsigned char	delay_import_descriptor		[0x08];		/* 0xd8 */
	unsigned char	clr_runtime_hdr			[0x08];		/* 0xe0 */
	unsigned char	reserved			[0x08];		/* 0xe8 */
};


union pe_opt_hdr {
	struct pe_opt_hdr_32	opt_hdr_32;
	struct pe_opt_hdr_64	opt_hdr_64;
};


struct pe_image_data_dir {
	unsigned char	rva				[0x04];		/* 0x00 */
	unsigned char	size				[0x04];		/* 0x04 */
};


struct pe_sec_hdr {
	unsigned char	name				[0x08];		/* 0x00 */
	unsigned char	virtual_size			[0x04];		/* 0x08 */
	unsigned char	virtual_addr			[0x04];		/* 0x0c */
	unsigned char	size_of_raw_data		[0x04];		/* 0x10 */
	unsigned char	ptr_to_raw_data			[0x04];		/* 0x14 */
	unsigned char	ptr_to_relocs			[0x04];		/* 0x18 */
	unsigned char	ptr_to_line_nums		[0x04];		/* 0x1c */
	unsigned char	num_of_relocs			[0x02];		/* 0x20 */
	unsigned char	num_of_line_nums		[0x02];		/* 0x22 */
	unsigned char	characteristics			[0x04];		/* 0x24 */
};


struct pe_export_hdr {
	unsigned char	export_flags			[0x04];		/* 0x00 */
	unsigned char	time_date_stamp			[0x04];		/* 0x04 */
	unsigned char	major_ver			[0x02];		/* 0x08 */
	unsigned char	minor_ver			[0x02];		/* 0x0a */
	unsigned char	name_rva			[0x04];		/* 0x0c */
	unsigned char	ordinal_base			[0x04];		/* 0x10 */
	unsigned char	addr_tbl_entries		[0x04];		/* 0x14 */
	unsigned char	num_of_name_ptrs		[0x04];		/* 0x18 */
	unsigned char	export_addr_tbl_rva		[0x04];		/* 0x1c */
	unsigned char	name_ptr_rva			[0x04];		/* 0x20 */
	unsigned char	ordinal_tbl_rva			[0x04];		/* 0x24 */
};


union pe_export_addr_tbl {
	unsigned char	  export_rva			[0x04];		/* 0x00 */
	unsigned char	  forwarder_rva			[0x04];		/* 0x00 */
};


union pe_import_lookup_item {
	unsigned char	import_lookup_entry_64		[0x08];		/* 0x00 */
	unsigned char	import_lookup_entry_32		[0x04];		/* 0x00 */
	unsigned char	hint_name_tbl_rva		[0x04];		/* 0x00 */
	unsigned char	ordinal_number			[0x02];		/* 0x00 */
};


struct pe_import_hdr {
	unsigned char	import_lookup_tbl_rva		[0x04];		/* 0x00 */
	unsigned char	time_date_stamp			[0x04];		/* 0x04 */
	unsigned char	forwarder_chain			[0x04];		/* 0x08 */
	unsigned char	name_rva			[0x04];		/* 0x0c */
	unsigned char	import_addr_tbl_rva		[0x04];		/* 0x10 */
};


struct pe_hint_name_entry {
	unsigned char	hint				[0x02];		/* 0x00 */
	unsigned char	name				[0x02];		/* 0x02 */
};


#ifdef __cplusplus
}
#endif

#endif
