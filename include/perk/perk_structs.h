#ifndef PERK_STRUCTS_H
#define PERK_STRUCTS_H

#ifdef __cplusplus
extern "C" {
#endif

struct pe_raw_image_dos_hdr {
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


struct pe_raw_coff_image_hdr {
	unsigned char	cfh_signature			[0x04];		/* 0x00 */
	unsigned char	cfh_machine			[0x02];		/* 0x04 */
	unsigned char	cfh_num_of_sections		[0x02];		/* 0x06 */
	unsigned char	cfh_time_date_stamp		[0x04];		/* 0x08 */
	unsigned char	cfh_ptr_to_sym_tbl		[0x04];		/* 0x0c */
	unsigned char	cfh_num_of_syms			[0x04];		/* 0x10 */
	unsigned char	cfh_size_of_opt_hdr		[0x02];		/* 0x14 */
	unsigned char	cfh_characteristics		[0x02];		/* 0x16 */
};


struct pe_raw_coff_object_hdr {
	unsigned char	cfh_machine			[0x02];		/* 0x00 */
	unsigned char	cfh_num_of_sections		[0x02];		/* 0x02 */
	unsigned char	cfh_time_date_stamp		[0x04];		/* 0x04 */
	unsigned char	cfh_ptr_to_sym_tbl		[0x04];		/* 0x08 */
	unsigned char	cfh_num_of_syms			[0x04];		/* 0x0c */
	unsigned char	cfh_size_of_opt_hdr		[0x02];		/* 0x10 */
	unsigned char	cfh_characteristics		[0x02];		/* 0x12 */
};


struct pe_raw_opt_hdr_std {
	unsigned char	coh_magic			[0x02];		/* 0x00 */
	unsigned char	coh_major_linker_ver		[0x01];		/* 0x02 */
	unsigned char	coh_minor_linker_ver		[0x01];		/* 0x03 */
	unsigned char	coh_size_of_code		[0x04];		/* 0x04 */
	unsigned char	coh_size_of_inited_data		[0x04];		/* 0x08 */
	unsigned char	coh_size_of_uninited_data	[0x04];		/* 0x0c */
	unsigned char	coh_entry_point			[0x04];		/* 0x10 */
	unsigned char	coh_base_of_code		[0x04];		/* 0x14 */
};


struct pe_raw_opt_hdr_align {
	unsigned char	coh_section_align		[0x04];		/* 0x20 */
	unsigned char	coh_file_align			[0x04];         /* 0x24 */
};


struct pe_raw_opt_hdr_vers {
	unsigned char	coh_major_os_ver		[0x02];		/* 0x28 */
	unsigned char	coh_minor_os_ver		[0x02];		/* 0x2a */
	unsigned char	coh_major_image_ver		[0x02];		/* 0x2c */
	unsigned char	coh_minor_image_ver		[0x02];		/* 0x2e */
	unsigned char	coh_major_subsys_ver		[0x02];		/* 0x30 */
	unsigned char	coh_minor_subsys_ver		[0x02];		/* 0x32 */
	unsigned char	coh_win32_ver			[0x04];		/* 0x34 */
};


struct pe_raw_opt_hdr_img {
	unsigned char	coh_size_of_image		[0x04];		/* 0x38 */
	unsigned char	coh_size_of_headers		[0x04];		/* 0x3c */
	unsigned char	coh_checksum			[0x04];		/* 0x40 */
	unsigned char	coh_subsystem			[0x02];		/* 0x44 */
	unsigned char	coh_dll_characteristics		[0x02];		/* 0x46 */
};


struct pe_raw_opt_hdr_ldr {
	unsigned char	coh_loader_flags		[0x04];
	unsigned char	coh_rva_and_sizes		[0x04];
};


struct pe_raw_opt_hdr_dirs {
	unsigned char	coh_export_tbl			[0x08];
	unsigned char	coh_import_tbl			[0x08];
	unsigned char	coh_resource_tbl		[0x08];
	unsigned char	coh_exception_tbl		[0x08];
	unsigned char	coh_certificate_tbl		[0x08];
	unsigned char	coh_base_reloc_tbl		[0x08];
	unsigned char	coh_debug			[0x08];
	unsigned char	coh_arch			[0x08];
	unsigned char	coh_global_ptr			[0x08];
	unsigned char	coh_tls_tbl			[0x08];
	unsigned char	coh_load_config_tbl		[0x08];
	unsigned char	coh_bound_import		[0x08];
	unsigned char	coh_iat				[0x08];
	unsigned char	coh_delay_import_descriptor	[0x08];
	unsigned char	coh_clr_runtime_hdr		[0x08];
	unsigned char	coh_reserved			[0x08];
};


struct pe_raw_data_dirs {
	unsigned char	coh_rva_and_sizes		[0x04];
	unsigned char	coh_export_tbl			[0x08];
	unsigned char	coh_import_tbl			[0x08];
	unsigned char	coh_resource_tbl		[0x08];
	unsigned char	coh_exception_tbl		[0x08];
	unsigned char	coh_certificate_tbl		[0x08];
	unsigned char	coh_base_reloc_tbl		[0x08];
	unsigned char	coh_debug			[0x08];
	unsigned char	coh_arch			[0x08];
	unsigned char	coh_global_ptr			[0x08];
	unsigned char	coh_tls_tbl			[0x08];
	unsigned char	coh_load_config_tbl		[0x08];
	unsigned char	coh_bound_import		[0x08];
	unsigned char	coh_iat				[0x08];
	unsigned char	coh_delay_import_descriptor	[0x08];
	unsigned char	coh_clr_runtime_hdr		[0x08];
	unsigned char	coh_reserved			[0x08];
};


struct pe_raw_opt_hdr_32 {
	unsigned char	coh_magic			[0x02];		/* 0x00 */
	unsigned char	coh_major_linker_ver		[0x01];		/* 0x02 */
	unsigned char	coh_minor_linker_ver		[0x01];		/* 0x03 */
	unsigned char	coh_size_of_code		[0x04];		/* 0x04 */
	unsigned char	coh_size_of_inited_data		[0x04];		/* 0x08 */
	unsigned char	coh_size_of_uninited_data	[0x04];		/* 0x0c */
	unsigned char	coh_entry_point			[0x04];		/* 0x10 */
	unsigned char	coh_base_of_code		[0x04];		/* 0x14 */
	unsigned char	coh_base_of_data		[0x04];		/* 0x18 */
	unsigned char	coh_image_base			[0x04];		/* 0x1c */
	unsigned char	coh_section_align		[0x04];		/* 0x20 */
	unsigned char	coh_file_align			[0x04];         /* 0x24 */
	unsigned char	coh_major_os_ver		[0x02];		/* 0x28 */
	unsigned char	coh_minor_os_ver		[0x02];		/* 0x2a */
	unsigned char	coh_major_image_ver		[0x02];		/* 0x2c */
	unsigned char	coh_minor_image_ver		[0x02];		/* 0x2e */
	unsigned char	coh_major_subsys_ver		[0x02];		/* 0x30 */
	unsigned char	coh_minor_subsys_ver		[0x02];		/* 0x32 */
	unsigned char	coh_win32_ver			[0x04];		/* 0x34 */
	unsigned char	coh_size_of_image		[0x04];		/* 0x38 */
	unsigned char	coh_size_of_headers		[0x04];		/* 0x3c */
	unsigned char	coh_checksum			[0x04];		/* 0x40 */
	unsigned char	coh_subsystem			[0x02];		/* 0x44 */
	unsigned char	coh_dll_characteristics		[0x02];		/* 0x46 */
	unsigned char	coh_size_of_stack_reserve	[0x04];		/* 0x48 */
	unsigned char	coh_size_of_stack_commit	[0x04];		/* 0x4c */
	unsigned char	coh_size_of_heap_reserve	[0x04];		/* 0x50 */
	unsigned char	coh_size_of_heap_commit		[0x04];		/* 0x54 */
	unsigned char	coh_loader_flags		[0x04];		/* 0x58 */
	unsigned char	coh_rva_and_sizes		[0x04];		/* 0x5c */
	unsigned char	coh_export_tbl			[0x08];		/* 0x60 */
	unsigned char	coh_import_tbl			[0x08];		/* 0x68 */
	unsigned char	coh_resource_tbl		[0x08];		/* 0x70 */
	unsigned char	coh_exception_tbl		[0x08];		/* 0x78 */
	unsigned char	coh_certificate_tbl		[0x08];		/* 0x80 */
	unsigned char	coh_base_reloc_tbl		[0x08];		/* 0x88 */
	unsigned char	coh_debug			[0x08];		/* 0x90 */
	unsigned char	coh_arch			[0x08];		/* 0x98 */
	unsigned char	coh_global_ptr			[0x08];		/* 0xa0 */
	unsigned char	coh_tls_tbl			[0x08];		/* 0xa8 */
	unsigned char	coh_load_config_tbl		[0x08];		/* 0xb0 */
	unsigned char	coh_bound_import		[0x08];		/* 0xb8 */
	unsigned char	coh_iat				[0x08];		/* 0xc0 */
	unsigned char	coh_delay_import_descriptor	[0x08];		/* 0xc8 */
	unsigned char	coh_clr_runtime_hdr		[0x08];		/* 0xd0 */
	unsigned char	coh_reserved			[0x08];		/* 0xd8 */
};


struct pe_raw_opt_hdr_64 {
	unsigned char	coh_magic			[0x02];		/* 0x00 */
	unsigned char	coh_major_linker_ver		[0x01];		/* 0x02 */
	unsigned char	coh_minor_linker_ver		[0x01];		/* 0x03 */
	unsigned char	coh_size_of_code		[0x04];		/* 0x04 */
	unsigned char	coh_size_of_inited_data		[0x04];		/* 0x08 */
	unsigned char	coh_size_of_uninited_data	[0x04];		/* 0x0c */
	unsigned char	coh_entry_point			[0x04];		/* 0x10 */
	unsigned char	coh_base_of_code		[0x04];		/* 0x14 */
	unsigned char	coh_image_base			[0x08];		/* 0x18 */
	unsigned char	coh_section_align		[0x04];		/* 0x20 */
	unsigned char	coh_file_align			[0x04];         /* 0x24 */
	unsigned char	coh_major_os_ver		[0x02];		/* 0x28 */
	unsigned char	coh_minor_os_ver		[0x02];		/* 0x2a */
	unsigned char	coh_major_image_ver		[0x02];		/* 0x2c */
	unsigned char	coh_minor_image_ver		[0x02];		/* 0x2e */
	unsigned char	coh_major_subsys_ver		[0x02];		/* 0x30 */
	unsigned char	coh_minor_subsys_ver		[0x02];		/* 0x32 */
	unsigned char	coh_win32_ver			[0x04];		/* 0x34 */
	unsigned char	coh_size_of_image		[0x04];		/* 0x38 */
	unsigned char	coh_size_of_headers		[0x04];		/* 0x3c */
	unsigned char	coh_checksum			[0x04];		/* 0x40 */
	unsigned char	coh_subsystem			[0x02];		/* 0x44 */
	unsigned char	coh_dll_characteristics		[0x02];		/* 0x46 */
	unsigned char	coh_size_of_stack_reserve	[0x08];		/* 0x48 */
	unsigned char	coh_size_of_stack_commit	[0x08];		/* 0x50 */
	unsigned char	coh_size_of_heap_reserve	[0x08];		/* 0x58 */
	unsigned char	coh_size_of_heap_commit		[0x08];		/* 0x60 */
	unsigned char	coh_loader_flags		[0x04];		/* 0x68 */
	unsigned char	coh_rva_and_sizes		[0x04];		/* 0x6c */
	unsigned char	coh_export_tbl			[0x08];		/* 0x70 */
	unsigned char	coh_import_tbl			[0x08];		/* 0x78 */
	unsigned char	coh_resource_tbl		[0x08];		/* 0x80 */
	unsigned char	coh_exception_tbl		[0x08];		/* 0x88 */
	unsigned char	coh_certificate_tbl		[0x08];		/* 0x90 */
	unsigned char	coh_base_reloc_tbl		[0x08];		/* 0x98 */
	unsigned char	coh_debug			[0x08];		/* 0xa0 */
	unsigned char	coh_arch			[0x08];		/* 0xa8 */
	unsigned char	coh_global_ptr			[0x08];		/* 0xb0 */
	unsigned char	coh_tls_tbl			[0x08];		/* 0xb8 */
	unsigned char	coh_load_config_tbl		[0x08];		/* 0xc0 */
	unsigned char	coh_bound_import		[0x08];		/* 0xc8 */
	unsigned char	coh_iat				[0x08];		/* 0xd0 */
	unsigned char	coh_delay_import_descriptor	[0x08];		/* 0xd8 */
	unsigned char	coh_clr_runtime_hdr		[0x08];		/* 0xe0 */
	unsigned char	coh_reserved			[0x08];		/* 0xe8 */
};


union pe_raw_opt_hdr {
	struct pe_raw_opt_hdr_32    opt_hdr_32;
	struct pe_raw_opt_hdr_64    opt_hdr_64;
};


struct pe_raw_image_data_dir {
	unsigned char	dh_rva				[0x04];		/* 0x00 */
	unsigned char	dh_size				[0x04];		/* 0x04 */
};


struct pe_raw_sec_hdr {
	unsigned char	sh_name				[0x08];		/* 0x00 */
	unsigned char	sh_virtual_size			[0x04];		/* 0x08 */
	unsigned char	sh_virtual_addr			[0x04];		/* 0x0c */
	unsigned char	sh_size_of_raw_data		[0x04];		/* 0x10 */
	unsigned char	sh_ptr_to_raw_data		[0x04];		/* 0x14 */
	unsigned char	sh_ptr_to_relocs		[0x04];		/* 0x18 */
	unsigned char	sh_ptr_to_line_nums		[0x04];		/* 0x1c */
	unsigned char	sh_num_of_relocs		[0x02];		/* 0x20 */
	unsigned char	sh_num_of_line_nums		[0x02];		/* 0x22 */
	unsigned char	sh_characteristics		[0x04];		/* 0x24 */
};


struct pe_raw_export_hdr {
	unsigned char	eh_export_flags			[0x04];		/* 0x00 */
	unsigned char	eh_time_date_stamp		[0x04];		/* 0x04 */
	unsigned char	eh_major_ver			[0x02];		/* 0x08 */
	unsigned char	eh_minor_ver			[0x02];		/* 0x0a */
	unsigned char	eh_name_rva			[0x04];		/* 0x0c */
	unsigned char	eh_ordinal_base			[0x04];		/* 0x10 */
	unsigned char	eh_addr_tbl_entries		[0x04];		/* 0x14 */
	unsigned char	eh_num_of_name_ptrs		[0x04];		/* 0x18 */
	unsigned char	eh_export_addr_tbl_rva		[0x04];		/* 0x1c */
	unsigned char	eh_name_ptr_rva			[0x04];		/* 0x20 */
	unsigned char	eh_ordinal_tbl_rva		[0x04];		/* 0x24 */
};


union pe_raw_export_addr_tbl {
	unsigned char	  ee_export_rva			[0x04];		/* 0x00 */
	unsigned char	  ee_forwarder_rva		[0x04];		/* 0x00 */
};


struct pe_raw_import_hdr {
	unsigned char	ih_import_lookup_tbl_rva	[0x04];		/* 0x00 */
	unsigned char	ih_time_date_stamp		[0x04];		/* 0x04 */
	unsigned char	ih_forwarder_chain		[0x04];		/* 0x08 */
	unsigned char	ih_name_rva			[0x04];		/* 0x0c */
	unsigned char	ih_import_addr_tbl_rva		[0x04];		/* 0x10 */
};


union pe_raw_import_lookup {
	unsigned char	ii_import_lookup_entry_64	[0x08];		/* 0x00 */
	unsigned char	ii_import_lookup_entry_32	[0x04];		/* 0x00 */
	unsigned char	ii_hint_name_tbl_rva		[0x04];		/* 0x00 */
	unsigned char	ii_ordinal_number		[0x02];		/* 0x00 */
};


struct pe_raw_hint_name_entry {
	unsigned char	ii_hint				[0x02];		/* 0x00 */
	unsigned char	ii_name				[0x02];		/* 0x02 */
};


struct pe_raw_coff_symbol {
	unsigned char	cs_name				[0x08];		/* 0x00 */
	unsigned char	cs_value			[0x04];		/* 0x08 */
	unsigned char	cs_section_number		[0x02];		/* 0x0c */
	unsigned char	cs_type				[0x02];		/* 0x0e */
	unsigned char	cs_storage_class		[0x01];		/* 0x10 */
	unsigned char	cs_num_of_aux_symbols		[0x01];		/* 0x11 */
};


struct pe_raw_coff_symbol_name {
	union {
		unsigned char		cs_short_name	[0x08];		/* 0x00 */

		struct {
			unsigned char	cs_zeroes	[0x04];		/* 0x00 */
			unsigned char	cs_offset	[0x04];		/* 0x04 */
		} long_name;
	};
};


#ifdef __cplusplus
}
#endif

#endif
