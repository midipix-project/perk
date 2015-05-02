#ifndef PERK_STRUCTS_H
#define PERK_STRUCTS_H

#ifdef __cplusplus
extern "C" {
#endif

/* pe_image_dos_header... */
#define PE_DOS_MAGIC_BS                             0x02
#define PE_DOS_CBLP_BS                              0x02
#define PE_DOS_CP_BS                                0x02
#define PE_DOS_CRLC_BS                              0x02
#define PE_DOS_CPARHDR_BS                           0x02
#define PE_DOS_MINALLOC_BS                          0x02
#define PE_DOS_MAXALLOC_BS                          0x02
#define PE_DOS_SS_BS                                0x02
#define PE_DOS_SP_BS                                0x02
#define PE_DOS_CSUM_BS                              0x02
#define PE_DOS_IP_BS                                0x02
#define PE_DOS_CS_BS                                0x02
#define PE_DOS_LFARLC_BS                            0x02
#define PE_DOS_OVNO_BS                              0x02
#define PE_DOS_RES_BS                               0x08
#define PE_DOS_OEMID_BS                             0x02
#define PE_DOS_OEMINFO_BS                           0x02
#define PE_DOS_RES2_BS                              0x14
#define PE_DOS_LFANEW_BS                            0x04

struct pe_image_dos_hdr {
	unsigned char	dos_magic                         [PE_DOS_MAGIC_BS];                            /* 0x00 */
	unsigned char	dos_cblp                          [PE_DOS_CBLP_BS];                             /* 0x02 */
	unsigned char	dos_cp                            [PE_DOS_CP_BS];                               /* 0x04 */
	unsigned char	dos_crlc                          [PE_DOS_CRLC_BS];                             /* 0x06 */
	unsigned char	dos_cparhdr                       [PE_DOS_CPARHDR_BS];                          /* 0x08 */
	unsigned char	dos_minalloc                      [PE_DOS_MINALLOC_BS];                         /* 0x0a */
	unsigned char	dos_maxalloc                      [PE_DOS_MAXALLOC_BS];                         /* 0x0c */
	unsigned char	dos_ss                            [PE_DOS_SS_BS];                               /* 0x0e */
	unsigned char	dos_sp                            [PE_DOS_SP_BS];                               /* 0x10 */
	unsigned char	dos_csum                          [PE_DOS_CSUM_BS];                             /* 0x12 */
	unsigned char	dos_ip                            [PE_DOS_IP_BS];                               /* 0x14 */
	unsigned char	dos_cs                            [PE_DOS_CS_BS];                               /* 0x16 */
	unsigned char	dos_lfarlc                        [PE_DOS_LFARLC_BS];                           /* 0x18 */
	unsigned char	dos_ovno                          [PE_DOS_OVNO_BS];                             /* 0x1a */
	unsigned char	dos_res                           [PE_DOS_RES_BS];                              /* 0x1c */
	unsigned char	dos_oemid                         [PE_DOS_OEMID_BS];                            /* 0x24 */
	unsigned char	dos_oeminfo                       [PE_DOS_OEMINFO_BS];                          /* 0x26 */
	unsigned char	dos_res2                          [PE_DOS_RES2_BS];                             /* 0x28 */
	unsigned char	dos_lfanew                        [PE_DOS_LFANEW_BS];                           /* 0x3c */
};

#undef	PE_DOS_MAGIC_BS
#undef	PE_DOS_CBLP_BS
#undef	PE_DOS_CP_BS
#undef	PE_DOS_CRLC_BS
#undef	PE_DOS_CPARHDR_BS
#undef	PE_DOS_MINALLOC_BS
#undef	PE_DOS_MAXALLOC_BS
#undef	PE_DOS_SS_BS
#undef	PE_DOS_SP_BS
#undef	PE_DOS_CSUM_BS
#undef	PE_DOS_IP_BS
#undef	PE_DOS_CS_BS
#undef	PE_DOS_LFARLC_BS
#undef	PE_DOS_OVNO_BS
#undef	PE_DOS_RES_BS
#undef	PE_DOS_OEMID_BS
#undef	PE_DOS_OEMINFO_BS
#undef	PE_DOS_RES2_BS
#undef	PE_DOS_LFANEW_BS


/* pe_coff_file_header... */
#define PE_SIGNATURE_BS                             0x04
#define PE_MACHINE_BS                               0x02
#define PE_NUMBER_OF_SECTIONS_BS                    0x02
#define PE_TIME_DATE_STAMP_BS                       0x04
#define PE_POINTER_TO_SYMBOL_TABLE_BS               0x04
#define PE_NUMBER_OF_SYMBOLS_BS                     0x04
#define PE_SIZE_OF_OPTIONAL_HEADER_BS               0x02
#define PE_CHARACTERISTICS_BS                       0x02

struct pe_coff_file_hdr {
	unsigned char	signature                         [PE_SIGNATURE_BS];                            /* 0x00 */
	unsigned char	machine                           [PE_MACHINE_BS];                              /* 0x04 */
	unsigned char	num_of_sections                   [PE_NUMBER_OF_SECTIONS_BS];                   /* 0x06 */
	unsigned char	time_date_stamp                   [PE_TIME_DATE_STAMP_BS];                      /* 0x08 */
	unsigned char	ptr_to_sym_tbl                    [PE_POINTER_TO_SYMBOL_TABLE_BS];              /* 0x0c */
	unsigned char	num_of_syms                       [PE_NUMBER_OF_SYMBOLS_BS];                    /* 0x10 */
	unsigned char	size_of_opt_hdr                   [PE_SIZE_OF_OPTIONAL_HEADER_BS];              /* 0x14 */
	unsigned char	characteristics                   [PE_CHARACTERISTICS_BS];                      /* 0x16 */
};

#undef	PE_SIGNATURE_BS
#undef	PE_MACHINE_BS
#undef	PE_NUMBER_OF_SECTIONS_BS
#undef	PE_TIME_DATE_STAMP_BS
#undef	PE_POINTER_TO_SYMBOL_TABLE_BS
#undef	PE_NUMBER_OF_SYMBOLS_BS
#undef	PE_SIZE_OF_OPTIONAL_HEADER_BS
#undef	PE_CHARACTERISTICS_BS


/* pe32_optional_header... */
#define PE_MAGIC_BS                                 0x02
#define PE_MAJOR_LINKER_VERSION_BS                  0x01
#define PE_MINOR_LINKER_VERSION_BS                  0x01
#define PE_SIZE_OF_CODE_BS                          0x04
#define PE_SIZE_OF_INITIALIZED_DATA_BS              0x04
#define PE_SIZE_OF_UNINITIALIZED_DATA_BS            0x04
#define PE_ADDRESS_OF_ENTRY_POINT_BS                0x04
#define PE_BASE_OF_CODE_BS                          0x04
#define PE_BASE_OF_DATA_BS                          0x04
#define PE_IMAGE_BASE_BS                            0x04
#define PE_SECTION_ALIGNMENT_BS                     0x04
#define PE_FILE_ALIGNMENT_BS                        0x04
#define PE_MAJOR_OPERATING_SYSTEM_VERSION_BS        0x02
#define PE_MINOR_OPERATING_SYSTEM_VERSION_BS        0x02
#define PE_MAJOR_IMAGE_VERSION_BS                   0x02
#define PE_MINOR_IMAGE_VERSION_BS                   0x02
#define PE_MAJOR_SUBSYSTEM_VERSION_BS               0x02
#define PE_MINOR_SUBSYSTEM_VERSION_BS               0x02
#define PE_WIN32_VERSION_VALUE_BS                   0x04
#define PE_SIZE_OF_IMAGE_BS                         0x04
#define PE_SIZE_OF_HEADERS_BS                       0x04
#define PE_CHECK_SUM_BS                             0x04
#define PE_SUBSYSTEM_BS                             0x02
#define PE_DLL_CHARACTERISTICS_BS                   0x02
#define PE_SIZE_OF_STACK_RESERVE_BS                 0x04
#define PE_SIZE_OF_STACK_COMMIT_BS                  0x04
#define PE_SIZE_OF_HEAP_RESERVE_BS                  0x04
#define PE_SIZE_OF_HEAP_COMMIT_BS                   0x04
#define PE_LOADER_FLAGS_BS                          0x04
#define PE_NUMBER_OF_RVA_AND_SIZES_BS               0x04
#define PE_EXPORT_TABLE_BS                          0x08
#define PE_IMPORT_TABLE_BS                          0x08
#define PE_RESOURCE_TABLE_BS                        0x08
#define PE_EXCEPTION_TABLE_BS                       0x08
#define PE_CERTIFICATE_TABLE_BS                     0x08
#define PE_BASE_RELOCATION_TABLE_BS                 0x08
#define PE_DEBUG_BS                                 0x08
#define PE_ARCHITECTURE_BS                          0x08
#define PE_GLOBAL_PTR_BS                            0x08
#define PE_TLS_TABLE_BS                             0x08
#define PE_LOAD_CONFIG_TABLE_BS                     0x08
#define PE_BOUND_IMPORT_BS                          0x08
#define PE_IAT_BS                                   0x08
#define PE_DELAY_IMPORT_DESCRIPTOR_BS               0x08
#define PE_CLR_RUNTIME_HEADER_BS                    0x08
#define PE_RESERVED__MUST_BE_ZERO_BS                0x08

struct pe_opt_hdr_std {
	unsigned char	magic                             [PE_MAGIC_BS];                                /* 0x00 */
	unsigned char	major_linker_ver                  [PE_MAJOR_LINKER_VERSION_BS];                 /* 0x02 */
	unsigned char	minor_linker_ver                  [PE_MINOR_LINKER_VERSION_BS];                 /* 0x03 */
	unsigned char	size_of_code                      [PE_SIZE_OF_CODE_BS];                         /* 0x04 */
	unsigned char	size_of_initialized_data          [PE_SIZE_OF_INITIALIZED_DATA_BS];             /* 0x08 */
	unsigned char	size_of_uninitialized_data        [PE_SIZE_OF_UNINITIALIZED_DATA_BS];           /* 0x0c */
	unsigned char	entry_point                       [PE_ADDRESS_OF_ENTRY_POINT_BS];               /* 0x10 */
	unsigned char	base_of_code                      [PE_BASE_OF_CODE_BS];                         /* 0x14 */
};

struct pe_opt_hdr_align {
	unsigned char	section_align                     [PE_SECTION_ALIGNMENT_BS];                    /* 0x20 */
	unsigned char	file_align                        [PE_FILE_ALIGNMENT_BS];                       /* 0x24 */
};

struct pe_opt_hdr_vers {
	unsigned char	major_os_ver                      [PE_MAJOR_OPERATING_SYSTEM_VERSION_BS];       /* 0x28 */
	unsigned char	minor_or_ver                      [PE_MINOR_OPERATING_SYSTEM_VERSION_BS];       /* 0x2a */
	unsigned char	major_image_ver                   [PE_MAJOR_IMAGE_VERSION_BS];                  /* 0x2c */
	unsigned char	minor_image_ver                   [PE_MINOR_IMAGE_VERSION_BS];                  /* 0x2e */
	unsigned char	major_subsys_ver                  [PE_MAJOR_SUBSYSTEM_VERSION_BS];              /* 0x30 */
	unsigned char	minor_subsys_ver                  [PE_MINOR_SUBSYSTEM_VERSION_BS];              /* 0x32 */
	unsigned char	win32_ver                         [PE_WIN32_VERSION_VALUE_BS];                  /* 0x34 */
};

struct pe_opt_hdr_img {
	unsigned char	size_of_image                     [PE_SIZE_OF_IMAGE_BS];                        /* 0x38 */
	unsigned char	size_of_headers                   [PE_SIZE_OF_HEADERS_BS];                      /* 0x3c */
	unsigned char	checksum                          [PE_CHECK_SUM_BS];                            /* 0x40 */
	unsigned char	subsystem                         [PE_SUBSYSTEM_BS];                            /* 0x44 */
	unsigned char	dll_characteristics               [PE_DLL_CHARACTERISTICS_BS];                  /* 0x46 */
};

struct pe_opt_hdr_ldr {
	unsigned char	loader_flags                      [PE_LOADER_FLAGS_BS];
	unsigned char	rva_and_sizes                     [PE_NUMBER_OF_RVA_AND_SIZES_BS];
};

struct pe_opt_hdr_dirs {
	unsigned char	export_tbl                        [PE_EXPORT_TABLE_BS];
	unsigned char	import_tbl                        [PE_IMPORT_TABLE_BS];
	unsigned char	resource_tbl                      [PE_RESOURCE_TABLE_BS];
	unsigned char	exception_tbl                     [PE_EXCEPTION_TABLE_BS];
	unsigned char	certificate_tbl                   [PE_CERTIFICATE_TABLE_BS];
	unsigned char	base_reloc_tbl                    [PE_BASE_RELOCATION_TABLE_BS];
	unsigned char	debug                             [PE_DEBUG_BS];
	unsigned char	arch                              [PE_ARCHITECTURE_BS];
	unsigned char	global_ptr                        [PE_GLOBAL_PTR_BS];
	unsigned char	tls_tbl                           [PE_TLS_TABLE_BS];
	unsigned char	load_config_tbl                   [PE_LOAD_CONFIG_TABLE_BS];
	unsigned char	bound_import                      [PE_BOUND_IMPORT_BS];
	unsigned char	iat                               [PE_IAT_BS];
	unsigned char	delay_import_descriptor           [PE_DELAY_IMPORT_DESCRIPTOR_BS];
	unsigned char	clr_runtime_hdr                   [PE_CLR_RUNTIME_HEADER_BS];
	unsigned char	reserved                          [PE_RESERVED__MUST_BE_ZERO_BS];
};

struct pe_opt_hdr_32 {
	unsigned char	magic                             [PE_MAGIC_BS];                                /* 0x00 */
	unsigned char	major_linker_ver                  [PE_MAJOR_LINKER_VERSION_BS];                 /* 0x02 */
	unsigned char	minor_linker_ver                  [PE_MINOR_LINKER_VERSION_BS];                 /* 0x03 */
	unsigned char	size_of_code                      [PE_SIZE_OF_CODE_BS];                         /* 0x04 */
	unsigned char	size_of_initialized_data          [PE_SIZE_OF_INITIALIZED_DATA_BS];             /* 0x08 */
	unsigned char	size_of_uninitialized_data        [PE_SIZE_OF_UNINITIALIZED_DATA_BS];           /* 0x0c */
	unsigned char	entry_point                       [PE_ADDRESS_OF_ENTRY_POINT_BS];               /* 0x10 */
	unsigned char	base_of_code                      [PE_BASE_OF_CODE_BS];                         /* 0x14 */
	unsigned char	base_of_data                      [PE_BASE_OF_DATA_BS];                         /* 0x18 */
	unsigned char	image_base                        [PE_IMAGE_BASE_BS];                           /* 0x1c */
	unsigned char	section_align                     [PE_SECTION_ALIGNMENT_BS];                    /* 0x20 */
	unsigned char	file_align                        [PE_FILE_ALIGNMENT_BS];                       /* 0x24 */
	unsigned char	major_os_ver                      [PE_MAJOR_OPERATING_SYSTEM_VERSION_BS];       /* 0x28 */
	unsigned char	minor_or_ver                      [PE_MINOR_OPERATING_SYSTEM_VERSION_BS];       /* 0x2a */
	unsigned char	major_image_ver                   [PE_MAJOR_IMAGE_VERSION_BS];                  /* 0x2c */
	unsigned char	minor_image_ver                   [PE_MINOR_IMAGE_VERSION_BS];                  /* 0x2e */
	unsigned char	major_subsys_ver                  [PE_MAJOR_SUBSYSTEM_VERSION_BS];              /* 0x30 */
	unsigned char	minor_subsys_ver                  [PE_MINOR_SUBSYSTEM_VERSION_BS];              /* 0x32 */
	unsigned char	win32_ver                         [PE_WIN32_VERSION_VALUE_BS];                  /* 0x34 */
	unsigned char	size_of_image                     [PE_SIZE_OF_IMAGE_BS];                        /* 0x38 */
	unsigned char	size_of_headers                   [PE_SIZE_OF_HEADERS_BS];                      /* 0x3c */
	unsigned char	checksum                          [PE_CHECK_SUM_BS];                            /* 0x40 */
	unsigned char	subsystem                         [PE_SUBSYSTEM_BS];                            /* 0x44 */
	unsigned char	dll_characteristics               [PE_DLL_CHARACTERISTICS_BS];                  /* 0x46 */
	unsigned char	size_of_stack_reserve             [PE_SIZE_OF_STACK_RESERVE_BS];                /* 0x48 */
	unsigned char	size_of_stack_commit              [PE_SIZE_OF_STACK_COMMIT_BS];                 /* 0x4c */
	unsigned char	size_of_heap_reserve              [PE_SIZE_OF_HEAP_RESERVE_BS];                 /* 0x50 */
	unsigned char	size_of_heap_commit               [PE_SIZE_OF_HEAP_COMMIT_BS];                  /* 0x54 */
	unsigned char	loader_flags                      [PE_LOADER_FLAGS_BS];                         /* 0x58 */
	unsigned char	rva_and_sizes                     [PE_NUMBER_OF_RVA_AND_SIZES_BS];              /* 0x5c */
	unsigned char	export_tbl                        [PE_EXPORT_TABLE_BS];                         /* 0x60 */
	unsigned char	import_tbl                        [PE_IMPORT_TABLE_BS];                         /* 0x68 */
	unsigned char	resource_tbl                      [PE_RESOURCE_TABLE_BS];                       /* 0x70 */
	unsigned char	exception_tbl                     [PE_EXCEPTION_TABLE_BS];                      /* 0x78 */
	unsigned char	certificate_tbl                   [PE_CERTIFICATE_TABLE_BS];                    /* 0x80 */
	unsigned char	base_reloc_tbl                    [PE_BASE_RELOCATION_TABLE_BS];                /* 0x88 */
	unsigned char	debug                             [PE_DEBUG_BS];                                /* 0x90 */
	unsigned char	arch                              [PE_ARCHITECTURE_BS];                         /* 0x98 */
	unsigned char	global_ptr                        [PE_GLOBAL_PTR_BS];                           /* 0xa0 */
	unsigned char	tls_tbl                           [PE_TLS_TABLE_BS];                            /* 0xa8 */
	unsigned char	load_config_tbl                   [PE_LOAD_CONFIG_TABLE_BS];                    /* 0xb0 */
	unsigned char	bound_import                      [PE_BOUND_IMPORT_BS];                         /* 0xb8 */
	unsigned char	iat                               [PE_IAT_BS];                                  /* 0xc0 */
	unsigned char	delay_import_descriptor           [PE_DELAY_IMPORT_DESCRIPTOR_BS];              /* 0xc8 */
	unsigned char	clr_runtime_hdr                   [PE_CLR_RUNTIME_HEADER_BS];                   /* 0xd0 */
	unsigned char	reserved                          [PE_RESERVED__MUST_BE_ZERO_BS];               /* 0xd8 */
};

#undef	PE_MAGIC_BS
#undef	PE_MAJOR_LINKER_VERSION_BS
#undef	PE_MINOR_LINKER_VERSION_BS
#undef	PE_SIZE_OF_CODE_BS
#undef	PE_SIZE_OF_INITIALIZED_DATA_BS
#undef	PE_SIZE_OF_UNINITIALIZED_DATA_BS
#undef	PE_ADDRESS_OF_ENTRY_POINT_BS
#undef	PE_BASE_OF_CODE_BS
#undef	PE_BASE_OF_DATA_BS
#undef	PE_IMAGE_BASE_BS
#undef	PE_SECTION_ALIGNMENT_BS
#undef	PE_FILE_ALIGNMENT_BS
#undef	PE_MAJOR_OPERATING_SYSTEM_VERSION_BS
#undef	PE_MINOR_OPERATING_SYSTEM_VERSION_BS
#undef	PE_MAJOR_IMAGE_VERSION_BS
#undef	PE_MINOR_IMAGE_VERSION_BS
#undef	PE_MAJOR_SUBSYSTEM_VERSION_BS
#undef	PE_MINOR_SUBSYSTEM_VERSION_BS
#undef	PE_WIN32_VERSION_VALUE_BS
#undef	PE_SIZE_OF_IMAGE_BS
#undef	PE_SIZE_OF_HEADERS_BS
#undef	PE_CHECK_SUM_BS
#undef	PE_SUBSYSTEM_BS
#undef	PE_DLL_CHARACTERISTICS_BS
#undef	PE_SIZE_OF_STACK_RESERVE_BS
#undef	PE_SIZE_OF_STACK_COMMIT_BS
#undef	PE_SIZE_OF_HEAP_RESERVE_BS
#undef	PE_SIZE_OF_HEAP_COMMIT_BS
#undef	PE_LOADER_FLAGS_BS
#undef	PE_NUMBER_OF_RVA_AND_SIZES_BS
#undef	PE_EXPORT_TABLE_BS
#undef	PE_IMPORT_TABLE_BS
#undef	PE_RESOURCE_TABLE_BS
#undef	PE_EXCEPTION_TABLE_BS
#undef	PE_CERTIFICATE_TABLE_BS
#undef	PE_BASE_RELOCATION_TABLE_BS
#undef	PE_DEBUG_BS
#undef	PE_ARCHITECTURE_BS
#undef	PE_GLOBAL_PTR_BS
#undef	PE_TLS_TABLE_BS
#undef	PE_LOAD_CONFIG_TABLE_BS
#undef	PE_BOUND_IMPORT_BS
#undef	PE_IAT_BS
#undef	PE_DELAY_IMPORT_DESCRIPTOR_BS
#undef	PE_CLR_RUNTIME_HEADER_BS
#undef	PE_RESERVED__MUST_BE_ZERO_BS


/* pe64_optional_header... */
#define PE_MAGIC_BS                                 0x02
#define PE_MAJOR_LINKER_VERSION_BS                  0x01
#define PE_MINOR_LINKER_VERSION_BS                  0x01
#define PE_SIZE_OF_CODE_BS                          0x04
#define PE_SIZE_OF_INITIALIZED_DATA_BS              0x04
#define PE_SIZE_OF_UNINITIALIZED_DATA_BS            0x04
#define PE_ADDRESS_OF_ENTRY_POINT_BS                0x04
#define PE_BASE_OF_CODE_BS                          0x04
#define PE_IMAGE_BASE_BS                            0x08
#define PE_SECTION_ALIGNMENT_BS                     0x04
#define PE_FILE_ALIGNMENT_BS                        0x04
#define PE_MAJOR_OPERATING_SYSTEM_VERSION_BS        0x02
#define PE_MINOR_OPERATING_SYSTEM_VERSION_BS        0x02
#define PE_MAJOR_IMAGE_VERSION_BS                   0x02
#define PE_MINOR_IMAGE_VERSION_BS                   0x02
#define PE_MAJOR_SUBSYSTEM_VERSION_BS               0x02
#define PE_MINOR_SUBSYSTEM_VERSION_BS               0x02
#define PE_WIN32_VERSION_VALUE_BS                   0x04
#define PE_SIZE_OF_IMAGE_BS                         0x04
#define PE_SIZE_OF_HEADERS_BS                       0x04
#define PE_CHECK_SUM_BS                             0x04
#define PE_SUBSYSTEM_BS                             0x02
#define PE_DLL_CHARACTERISTICS_BS                   0x02
#define PE_SIZE_OF_STACK_RESERVE_BS                 0x08
#define PE_SIZE_OF_STACK_COMMIT_BS                  0x08
#define PE_SIZE_OF_HEAP_RESERVE_BS                  0x08
#define PE_SIZE_OF_HEAP_COMMIT_BS                   0x08
#define PE_LOADER_FLAGS_BS                          0x04
#define PE_NUMBER_OF_RVA_AND_SIZES_BS               0x04
#define PE_EXPORT_TABLE_BS                          0x08
#define PE_IMPORT_TABLE_BS                          0x08
#define PE_RESOURCE_TABLE_BS                        0x08
#define PE_EXCEPTION_TABLE_BS                       0x08
#define PE_CERTIFICATE_TABLE_BS                     0x08
#define PE_BASE_RELOCATION_TABLE_BS                 0x08
#define PE_DEBUG_BS                                 0x08
#define PE_ARCHITECTURE_BS                          0x08
#define PE_GLOBAL_PTR_BS                            0x08
#define PE_TLS_TABLE_BS                             0x08
#define PE_LOAD_CONFIG_TABLE_BS                     0x08
#define PE_BOUND_IMPORT_BS                          0x08
#define PE_IAT_BS                                   0x08
#define PE_DELAY_IMPORT_DESCRIPTOR_BS               0x08
#define PE_CLR_RUNTIME_HEADER_BS                    0x08
#define PE_RESERVED__MUST_BE_ZERO_BS                0x08

struct pe_opt_hdr_64 {
	unsigned char	magic                             [PE_MAGIC_BS];                                /* 0x00 */
	unsigned char	major_linker_ver                  [PE_MAJOR_LINKER_VERSION_BS];                 /* 0x02 */
	unsigned char	minor_linker_ver                  [PE_MINOR_LINKER_VERSION_BS];                 /* 0x03 */
	unsigned char	size_of_code                      [PE_SIZE_OF_CODE_BS];                         /* 0x04 */
	unsigned char	size_of_initialized_data          [PE_SIZE_OF_INITIALIZED_DATA_BS];             /* 0x08 */
	unsigned char	size_of_uninitialized_data        [PE_SIZE_OF_UNINITIALIZED_DATA_BS];           /* 0x0c */
	unsigned char	entry_point                       [PE_ADDRESS_OF_ENTRY_POINT_BS];               /* 0x10 */
	unsigned char	base_of_code                      [PE_BASE_OF_CODE_BS];                         /* 0x14 */
	unsigned char	image_base                        [PE_IMAGE_BASE_BS];                           /* 0x18 */
	unsigned char	section_align                     [PE_SECTION_ALIGNMENT_BS];                    /* 0x20 */
	unsigned char	file_align                        [PE_FILE_ALIGNMENT_BS];                       /* 0x24 */
	unsigned char	major_os_ver                      [PE_MAJOR_OPERATING_SYSTEM_VERSION_BS];       /* 0x28 */
	unsigned char	minor_or_ver                      [PE_MINOR_OPERATING_SYSTEM_VERSION_BS];       /* 0x2a */
	unsigned char	major_image_ver                   [PE_MAJOR_IMAGE_VERSION_BS];                  /* 0x2c */
	unsigned char	minor_image_ver                   [PE_MINOR_IMAGE_VERSION_BS];                  /* 0x2e */
	unsigned char	major_subsys_ver                  [PE_MAJOR_SUBSYSTEM_VERSION_BS];              /* 0x30 */
	unsigned char	minor_subsys_ver                  [PE_MINOR_SUBSYSTEM_VERSION_BS];              /* 0x32 */
	unsigned char	win32_ver                         [PE_WIN32_VERSION_VALUE_BS];                  /* 0x34 */
	unsigned char	size_of_image                     [PE_SIZE_OF_IMAGE_BS];                        /* 0x38 */
	unsigned char	size_of_headers                   [PE_SIZE_OF_HEADERS_BS];                      /* 0x3c */
	unsigned char	checksum                          [PE_CHECK_SUM_BS];                            /* 0x40 */
	unsigned char	subsystem                         [PE_SUBSYSTEM_BS];                            /* 0x44 */
	unsigned char	dll_characteristics               [PE_DLL_CHARACTERISTICS_BS];                  /* 0x46 */
	unsigned char	size_of_stack_reserve             [PE_SIZE_OF_STACK_RESERVE_BS];                /* 0x48 */
	unsigned char	size_of_stack_commit              [PE_SIZE_OF_STACK_COMMIT_BS];                 /* 0x50 */
	unsigned char	size_of_heap_reserve              [PE_SIZE_OF_HEAP_RESERVE_BS];                 /* 0x58 */
	unsigned char	size_of_heap_commit               [PE_SIZE_OF_HEAP_COMMIT_BS];                  /* 0x60 */
	unsigned char	loader_flags                      [PE_LOADER_FLAGS_BS];                         /* 0x68 */
	unsigned char	rva_and_sizes                     [PE_NUMBER_OF_RVA_AND_SIZES_BS];              /* 0x6c */
	unsigned char	export_tbl                        [PE_EXPORT_TABLE_BS];                         /* 0x70 */
	unsigned char	import_tbl                        [PE_IMPORT_TABLE_BS];                         /* 0x78 */
	unsigned char	resource_tbl                      [PE_RESOURCE_TABLE_BS];                       /* 0x80 */
	unsigned char	exception_tbl                     [PE_EXCEPTION_TABLE_BS];                      /* 0x88 */
	unsigned char	certificate_tbl                   [PE_CERTIFICATE_TABLE_BS];                    /* 0x90 */
	unsigned char	base_reloc_tbl                    [PE_BASE_RELOCATION_TABLE_BS];                /* 0x98 */
	unsigned char	debug                             [PE_DEBUG_BS];                                /* 0xa0 */
	unsigned char	arch                              [PE_ARCHITECTURE_BS];                         /* 0xa8 */
	unsigned char	global_ptr                        [PE_GLOBAL_PTR_BS];                           /* 0xb0 */
	unsigned char	tls_tbl                           [PE_TLS_TABLE_BS];                            /* 0xb8 */
	unsigned char	load_config_tbl                   [PE_LOAD_CONFIG_TABLE_BS];                    /* 0xc0 */
	unsigned char	bound_import                      [PE_BOUND_IMPORT_BS];                         /* 0xc8 */
	unsigned char	iat                               [PE_IAT_BS];                                  /* 0xd0 */
	unsigned char	delay_import_descriptor           [PE_DELAY_IMPORT_DESCRIPTOR_BS];              /* 0xd8 */
	unsigned char	clr_runtime_hdr                   [PE_CLR_RUNTIME_HEADER_BS];                   /* 0xe0 */
	unsigned char	reserved                          [PE_RESERVED__MUST_BE_ZERO_BS];               /* 0xe8 */
};

#undef	PE_MAGIC_BS
#undef	PE_MAJOR_LINKER_VERSION_BS
#undef	PE_MINOR_LINKER_VERSION_BS
#undef	PE_SIZE_OF_CODE_BS
#undef	PE_SIZE_OF_INITIALIZED_DATA_BS
#undef	PE_SIZE_OF_UNINITIALIZED_DATA_BS
#undef	PE_ADDRESS_OF_ENTRY_POINT_BS
#undef	PE_BASE_OF_CODE_BS
#undef	PE_IMAGE_BASE_BS
#undef	PE_SECTION_ALIGNMENT_BS
#undef	PE_FILE_ALIGNMENT_BS
#undef	PE_MAJOR_OPERATING_SYSTEM_VERSION_BS
#undef	PE_MINOR_OPERATING_SYSTEM_VERSION_BS
#undef	PE_MAJOR_IMAGE_VERSION_BS
#undef	PE_MINOR_IMAGE_VERSION_BS
#undef	PE_MAJOR_SUBSYSTEM_VERSION_BS
#undef	PE_MINOR_SUBSYSTEM_VERSION_BS
#undef	PE_WIN32_VERSION_VALUE_BS
#undef	PE_SIZE_OF_IMAGE_BS
#undef	PE_SIZE_OF_HEADERS_BS
#undef	PE_CHECK_SUM_BS
#undef	PE_SUBSYSTEM_BS
#undef	PE_DLL_CHARACTERISTICS_BS
#undef	PE_SIZE_OF_STACK_RESERVE_BS
#undef	PE_SIZE_OF_STACK_COMMIT_BS
#undef	PE_SIZE_OF_HEAP_RESERVE_BS
#undef	PE_SIZE_OF_HEAP_COMMIT_BS
#undef	PE_LOADER_FLAGS_BS
#undef	PE_NUMBER_OF_RVA_AND_SIZES_BS
#undef	PE_EXPORT_TABLE_BS
#undef	PE_IMPORT_TABLE_BS
#undef	PE_RESOURCE_TABLE_BS
#undef	PE_EXCEPTION_TABLE_BS
#undef	PE_CERTIFICATE_TABLE_BS
#undef	PE_BASE_RELOCATION_TABLE_BS
#undef	PE_DEBUG_BS
#undef	PE_ARCHITECTURE_BS
#undef	PE_GLOBAL_PTR_BS
#undef	PE_TLS_TABLE_BS
#undef	PE_LOAD_CONFIG_TABLE_BS
#undef	PE_BOUND_IMPORT_BS
#undef	PE_IAT_BS
#undef	PE_DELAY_IMPORT_DESCRIPTOR_BS
#undef	PE_CLR_RUNTIME_HEADER_BS
#undef	PE_RESERVED__MUST_BE_ZERO_BS

union pe_opt_hdr {
	struct pe_opt_hdr_32	opt_hdr_32;
	struct pe_opt_hdr_64	opt_hdr_64;
};


/* pe_image_data_directory... */
#define PE_VIRTUAL_ADDRESS_BS                       0x04
#define PE_SIZE_BS                                  0x04

struct pe_image_data_dir {
	unsigned char	rva                               [PE_VIRTUAL_ADDRESS_BS];                      /* 0x00 */
	unsigned char	size                              [PE_SIZE_BS];                                 /* 0x04 */
};

#undef	PE_VIRTUAL_ADDRESS_BS
#undef	PE_SIZE_BS


/* pe_section_table... */
#define PE_NAME_BS                                  0x08
#define PE_VIRTUAL_SIZE_BS                          0x04
#define PE_VIRTUAL_ADDRESS_BS                       0x04
#define PE_SIZE_OF_RAW_DATA_BS                      0x04
#define PE_POINTER_TO_RAW_DATA_BS                   0x04
#define PE_POINTER_TO_RELOCATIONS_BS                0x04
#define PE_POINTER_TO_LINENUMBERS_BS                0x04
#define PE_NUMBER_OF_RELOCATIONS_BS                 0x02
#define PE_NUMBER_OF_LINENUMBERS_BS                 0x02
#define PE_CHARACTERISTICS_BS                       0x04

struct pe_sec_hdr {
	unsigned char	name                              [PE_NAME_BS];                                 /* 0x00 */
	unsigned char	virtual_size                      [PE_VIRTUAL_SIZE_BS];                         /* 0x08 */
	unsigned char	virtual_addr                      [PE_VIRTUAL_ADDRESS_BS];                      /* 0x0c */
	unsigned char	size_of_raw_data                  [PE_SIZE_OF_RAW_DATA_BS];                     /* 0x10 */
	unsigned char	ptr_to_raw_data                   [PE_POINTER_TO_RAW_DATA_BS];                  /* 0x14 */
	unsigned char	ptr_to_relocs                     [PE_POINTER_TO_RELOCATIONS_BS];               /* 0x18 */
	unsigned char	ptr_to_line_nums                  [PE_POINTER_TO_LINENUMBERS_BS];               /* 0x1c */
	unsigned char	num_of_relocs                     [PE_NUMBER_OF_RELOCATIONS_BS];                /* 0x20 */
	unsigned char	num_of_line_nums                  [PE_NUMBER_OF_LINENUMBERS_BS];                /* 0x22 */
	unsigned char	characteristics                   [PE_CHARACTERISTICS_BS];                      /* 0x24 */
};

#undef	PE_NAME_BS
#undef	PE_VIRTUAL_SIZE_BS
#undef	PE_VIRTUAL_ADDRESS_BS
#undef	PE_SIZE_OF_RAW_DATA_BS
#undef	PE_POINTER_TO_RAW_DATA_BS
#undef	PE_POINTER_TO_RELOCATIONS_BS
#undef	PE_POINTER_TO_LINENUMBERS_BS
#undef	PE_NUMBER_OF_RELOCATIONS_BS
#undef	PE_NUMBER_OF_LINENUMBERS_BS
#undef	PE_CHARACTERISTICS_BS


/* pe_export_directory_table... */
#define PE_EXPORT_FLAGS_BS                          0x04
#define PE_TIME_DATE_STAMP_BS                       0x04
#define PE_MAJOR_VERSION_BS                         0x02
#define PE_MINOR_VERSION_BS                         0x02
#define PE_NAME_RVA_BS                              0x04
#define PE_ORDINAL_BASE_BS                          0x04
#define PE_ADDRESS_TABLE_ENTRIES_BS                 0x04
#define PE_NUMBER_OF_NAME_POINTERS_BS               0x04
#define PE_EXPORT_ADDRESS_TABLE_RVA_BS              0x04
#define PE_NAME_POINTER_RVA_BS                      0x04
#define PE_ORDINAL_TABLE_RVA_BS                     0x04

struct pe_export_hdr {
	unsigned char	export_flags                      [PE_EXPORT_FLAGS_BS];                         /* 0x00 */
	unsigned char	time_date_stamp                   [PE_TIME_DATE_STAMP_BS];                      /* 0x04 */
	unsigned char	major_ver                         [PE_MAJOR_VERSION_BS];                        /* 0x08 */
	unsigned char	minor_ver                         [PE_MINOR_VERSION_BS];                        /* 0x0a */
	unsigned char	name_rva                          [PE_NAME_RVA_BS];                             /* 0x0c */
	unsigned char	ordinal_base                      [PE_ORDINAL_BASE_BS];                         /* 0x10 */
	unsigned char	addr_tbl_entries                  [PE_ADDRESS_TABLE_ENTRIES_BS];                /* 0x14 */
	unsigned char	num_of_name_ptrs                  [PE_NUMBER_OF_NAME_POINTERS_BS];              /* 0x18 */
	unsigned char	export_addr_tbl_rva               [PE_EXPORT_ADDRESS_TABLE_RVA_BS];             /* 0x1c */
	unsigned char	name_ptr_rva                      [PE_NAME_POINTER_RVA_BS];                     /* 0x20 */
	unsigned char	ordinal_tbl_rva                   [PE_ORDINAL_TABLE_RVA_BS];                    /* 0x24 */
};

#undef	PE_EXPORT_FLAGS_BS
#undef	PE_TIME_DATE_STAMP_BS
#undef	PE_MAJOR_VERSION_BS
#undef	PE_MINOR_VERSION_BS
#undef	PE_NAME_RVA_BS
#undef	PE_ORDINAL_BASE_BS
#undef	PE_ADDRESS_TABLE_ENTRIES_BS
#undef	PE_NUMBER_OF_NAME_POINTERS_BS
#undef	PE_EXPORT_ADDRESS_TABLE_RVA_BS
#undef	PE_NAME_POINTER_RVA_BS
#undef	PE_ORDINAL_TABLE_RVA_BS


/* pe_export_address_table... */
#define PE_EXPORT_RVA_BS                            0x04
#define PE_FORWARDER_RVA_BS                         0x04

union pe_export_addr_tbl {
	unsigned char	  export_rva                  [PE_EXPORT_RVA_BS];                           /* 0x00 */
	unsigned char	  forwarder_rva               [PE_FORWARDER_RVA_BS];                        /* 0x00 */
};

#undef	PE_EXPORT_RVA_BS
#undef	PE_FORWARDER_RVA_BS


/* image: pe_import_table_entry_lookup_item... */
#define PE_IMPORT_LOOKUP_ENTRY_PE64_BS              0x08
#define PE_IMPORT_LOOKUP_ENTRY_PE32_BS              0x04
#define PE_HINT_NAME_TABLE_RVA_BS                   0x04
#define PE_ORDINAL_NUMBER_BS                        0x02

struct pe_import_lookup_item {
  union {
		unsigned char	  import_lookup_entry_64  [PE_IMPORT_LOOKUP_ENTRY_PE64_BS];             /* 0x00 */
		unsigned char	  import_lookup_entry_32  [PE_IMPORT_LOOKUP_ENTRY_PE32_BS];             /* 0x00 */
		unsigned char	  hint_name_tbl_rva       [PE_HINT_NAME_TABLE_RVA_BS];                  /* 0x00 */
		unsigned char	  ordinal_number          [PE_ORDINAL_NUMBER_BS];                       /* 0x00 */
	} u;
};

#undef	PE_IMPORT_LOOKUP_ENTRY_PE64_BS
#undef	PE_IMPORT_LOOKUP_ENTRY_PE32_BS
#undef	PE_HINT_NAME_TABLE_RVA_BS
#undef	PE_ORDINAL_NUMBER_BS


/* image: pe_import_directory_table_entry... */
#define PE_IMPORT_LOOKUP_TABLE_RVA_BS               0x04
#define PE_TIME_DATE_STAMP_BS                       0x04
#define PE_FORWARDER_CHAIN_BS                       0x04
#define PE_NAME_RVA_BS                              0x04
#define PE_IMPORT_ADDRESS_TABLE_RVA_BS              0x04

struct pe_import_hdr {
	unsigned char	import_lookup_tbl_rva             [PE_IMPORT_LOOKUP_TABLE_RVA_BS];              /* 0x00 */
	unsigned char	time_date_stamp                   [PE_TIME_DATE_STAMP_BS];                      /* 0x04 */
	unsigned char	forwarder_chain                   [PE_FORWARDER_CHAIN_BS];                      /* 0x08 */
	unsigned char	name_rva                          [PE_NAME_RVA_BS];                             /* 0x0c */
	unsigned char	import_addr_tbl_rva               [PE_IMPORT_ADDRESS_TABLE_RVA_BS];             /* 0x10 */
};

#undef	PE_IMPORT_LOOKUP_TABLE_RVA_BS
#undef	PE_TIME_DATE_STAMP_BS
#undef	PE_FORWARDER_CHAIN_BS
#undef	PE_NAME_RVA_BS
#undef	PE_IMPORT_ADDRESS_TABLE_RVA_BS


/* pe_hint_name_table_padded... */
#define PE_HINT_BS                                  0x02
#define PE_NAME_BS                                  0x02

struct pe_hint_name_entry {
	unsigned char	hint                              [PE_HINT_BS];                                 /* 0x00 */
	unsigned char	name                              [PE_NAME_BS];                                 /* 0x02 */
};

#undef	PE_HINT_BS
#undef	PE_NAME_BS

#ifdef __cplusplus
}
#endif

#endif
