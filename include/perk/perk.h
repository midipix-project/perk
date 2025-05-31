#ifndef PERK_H
#define PERK_H

#include <stdint.h>
#include <stdio.h>

#include "perk_api.h"
#include "perk_meta.h"
#include "perk_arbits.h"

#ifdef __cplusplus
extern "C" {
#endif

/* pre-alpha */
#ifndef PERK_APP
#ifndef PERK_PRE_ALPHA
#warning libperk: pre-alpha: ABI is not final!
#warning pass -DPERK_PRE_ALPHA to suppress this warning.
#endif
#endif


/* status codes */
#define PERK_OK				0x00
#define PERK_USAGE			0x01
#define PERK_ERROR			0x02

/* driver flags */
#define PERK_DRIVER_VERBOSITY_NONE	0x0000
#define PERK_DRIVER_VERBOSITY_ERRORS	0x0001
#define PERK_DRIVER_VERBOSITY_STATUS	0x0002
#define PERK_DRIVER_VERBOSITY_UTILITY	0x0004
#define PERK_DRIVER_CLONE_VECTOR	0x0008

#define PERK_DRIVER_VERSION		0x0010
#define PERK_DRIVER_DRY_RUN		0x0020

#define PERK_DRIVER_ANNOTATE_ALWAYS	0x1000
#define PERK_DRIVER_ANNOTATE_NEVER	0x2000
#define PERK_DRIVER_ANNOTATE_FULL	0x4000

#define PERK_DRIVER_AR_STRICT_PE        0x10000
#define PERK_DRIVER_AR_STRICT_PE_ARCH   0x20000
#define PERK_DRIVER_AR_OBJECT_VECTOR    0x40000

/* unit action flags */
#define PERK_ACTION_MAP_READWRITE	0x0001

/* error flags */
#define PERK_ERROR_TOP_LEVEL		0x0001
#define PERK_ERROR_NESTED		0x0002
#define PERK_ERROR_CHILD		0x0004
#define PERK_ERROR_CUSTOM		0x0008

enum pe_custom_error {
	PERK_ERR_FLOW_ERROR,
	PERK_ERR_FLEE_ERROR,
	PERK_ERR_NULL_CONTEXT,
	PERK_ERR_NULL_IMAGE,
	PERK_ERR_INVALID_CONTEXT,
	PERK_ERR_INVALID_IMAGE,
	PERK_ERR_IMAGE_SIZE_ZERO,
	PERK_ERR_IMAGE_MALFORMED,
	PERK_ERR_BAD_DOS_HEADER,
	PERK_ERR_BAD_COFF_HEADER,
	PERK_ERR_BAD_IMAGE_TYPE,
	PERK_ERR_UNSUPPORTED_ABI,
	PERK_ERR_AR_FAIL,
	PERK_ERR_AR_EMPTY_FILE,
	PERK_ERR_AR_INVALID_SIGNATURE,
	PERK_ERR_AR_INVALID_HEADER,
	PERK_ERR_AR_TRUNCATED_DATA,
	PERK_ERR_AR_DUPLICATE_LONG_NAMES,
	PERK_ERR_AR_DUPLICATE_ARMAP_MEMBER,
	PERK_ERR_AR_MISPLACED_ARMAP_MEMBER,
	PERK_ERR_AR_NO_ACTION_SPECIFIED,
	PERK_ERR_AR_NO_INPUT_SPECIFIED,
	PERK_ERR_AR_DRIVER_MISMATCH,
	PERK_ERR_AR_ARMAP_MISMATCH,
	PERK_ERR_AR_INVALID_ARMAP_NUMBER_OF_SYMS,
	PERK_ERR_AR_INVALID_ARMAP_SIZE_OF_REFS,
	PERK_ERR_AR_INVALID_ARMAP_SIZE_OF_STRS,
	PERK_ERR_AR_INVALID_ARMAP_STRING_TABLE,
	PERK_ERR_AR_INVALID_ARMAP_MEMBER_OFFSET,
	PERK_ERR_AR_INVALID_ARMAP_NAME_OFFSET,
	PERK_ERR_AR_DLUNIT_NOT_SPECIFIED,
	PERK_ERR_AR_OUTPUT_NOT_SPECIFIED,
	PERK_ERR_AR_OUTPUT_NOT_APPLICABLE,
	PERK_ERR_AR_NON_PE_MEMBERS,
	PERK_ERR_AR_MIXED_PE_MEMBERS,
	PERK_ERR_AR_NESTED_ARCHIVE,
	PERK_ERR_CAP,
};

enum pe_cmd {
	PERK_CMD_DEFAULT,
	PERK_CMD_PERK,
	PERK_CMD_CAP,
};

struct pe_raw_image;
struct pe_raw_image_dos_hdr;
struct pe_raw_coff_image_hdr;
struct pe_raw_coff_object_hdr;
union  pe_raw_opt_hdr;
struct pe_raw_sec_hdr;
struct pe_raw_export_hdr;
struct pe_raw_import_hdr;
struct pe_raw_coff_symbol;

struct pe_raw_image {
	void *		map_addr;
	size_t		map_size;
};

struct pe_raw_archive {
	void *				map_addr;
	size_t				map_size;
};

struct pe_expsym {
	const char *	s_name;
	void *		s_eaddr;
	void *		s_maddr;
	uint32_t	s_roffset;
};

struct pe_meta_stats {
	int32_t		t_nexpsyms;
	int32_t		t_nimplibs;
	int32_t		t_ndsolibs;
	int32_t		t_ndsosyms;
	int32_t		t_nrelocs;
};

struct pe_image_meta {
	struct pe_raw_image		r_image;
	struct pe_raw_image_dos_hdr *	r_dos;
	struct pe_raw_coff_image_hdr *	r_coff;
	struct pe_raw_coff_object_hdr *	r_obj;
	union  pe_raw_opt_hdr *		r_opt;
	struct pe_raw_sec_hdr *		r_sectbl;

	struct pe_raw_export_hdr *	r_edata;
	struct pe_raw_import_hdr *	r_idata;

	void *				r_dsometa;
	void *				r_dsosyms;
	char *				r_dsostrs;
	void *				r_dsodata;

	struct pe_meta_stats		m_stats;
	struct pe_meta_image_dos_hdr	m_dos;
	struct pe_meta_coff_file_hdr	m_coff;
	struct pe_meta_opt_hdr		m_opt;
	struct pe_meta_sec_hdr *	m_sectbl;

	struct pe_meta_export_hdr	m_edata;
	struct pe_meta_import_hdr *	m_idata;

	struct pe_meta_sec_hdr *	h_edata;
	struct pe_meta_sec_hdr *	h_idata;

	struct pe_meta_sec_hdr *	h_dsometa;
	struct pe_meta_sec_hdr *	h_dsosyms;
	struct pe_meta_sec_hdr *	h_dsostrs;
	struct pe_meta_sec_hdr *	h_dsodata;

	enum pe_abi			m_abi;
	enum pe_subtype			m_subtype;
	enum pe_framework		m_framework;
};

struct pe_archive_meta {
	struct pe_raw_archive           r_archive;
	struct ar_raw_signature *       r_signature;

	struct ar_meta_signature *      m_signature;

	struct ar_meta_member_info **   a_memberv;
	struct ar_meta_member_info *    a_arref_longnames;
	struct ar_meta_armap_info       a_armap_primary;
	struct ar_meta_armap_info       a_armap_pecoff;
};

struct pe_source_version {
	int		major;
	int		minor;
	int		revision;
	const char *	commit;
};

struct pe_fd_ctx {
	int		fdin;
	int		fdout;
	int		fderr;
	int		fdlog;
	int		fdcwd;
	int		fddst;
};

struct pe_error_info {
	const struct pe_driver_ctx *	edctx;
	const struct pe_unit_ctx *	euctx;
	const char *			eunit;
	int				esyscode;
	int				elibcode;
	const char *			efunction;
	int				eline;
	unsigned			eflags;
	void *				eany;
};

struct pe_common_ctx {
	uint64_t			drvflags;
	uint64_t			actflags;
	uint64_t			fmtflags;
	uint64_t			hdrdump;
	enum pe_cmd                     cmd;
};

struct pe_driver_ctx {
	const char **			units;
	const char *			program;
	const char *			module;
	const struct pe_common_ctx *	cctx;
	struct pe_error_info **		errv;
	void *				any;
};

struct pe_unit_ctx {
	const char * const *		path;
	const struct pe_image_meta *	meta;
	const struct pe_archive_meta *	armeta;
	const struct pe_image_meta **	objmeta;
	void *				any;
};

struct pe_info_string {
	char				buffer[128];
};

/* driver api */
perk_api int  pe_lib_get_driver_ctx             (char ** argv, char ** envp, uint32_t flags,
                                                 const struct pe_fd_ctx *,
                                                 struct pe_driver_ctx **);

perk_api void pe_lib_free_driver_ctx            (struct pe_driver_ctx *);

perk_api int  pe_lib_get_unit_ctx               (const struct pe_driver_ctx *, const char * path,
                                                 struct pe_unit_ctx **);

perk_api void pe_lib_free_unit_ctx              (struct pe_unit_ctx *);

perk_api int  pe_lib_get_driver_fdctx           (const struct pe_driver_ctx *, struct pe_fd_ctx *);

perk_api int  pe_lib_set_driver_fdctx           (struct pe_driver_ctx *, const struct pe_fd_ctx *);

/* cmd api */
perk_api int  pe_cmd_perk                       (const struct pe_driver_ctx *, const char *);

/* utility api */
perk_api int  pe_main                           (char **, char **, const struct pe_fd_ctx *);

perk_api int  pe_output_image_category          (const struct pe_driver_ctx *, const struct pe_image_meta *);
perk_api int  pe_output_image_sections          (const struct pe_driver_ctx *, const struct pe_image_meta *);
perk_api int  pe_output_image_symbols           (const struct pe_driver_ctx *, const struct pe_image_meta *);
perk_api int  pe_output_image_strings           (const struct pe_driver_ctx *, const struct pe_image_meta *);
perk_api int  pe_output_export_symbols          (const struct pe_driver_ctx *, const struct pe_image_meta *);
perk_api int  pe_output_import_libraries        (const struct pe_driver_ctx *, const struct pe_image_meta *);
perk_api int  pe_output_mdso_libraries          (const struct pe_driver_ctx *, const struct pe_image_meta *);

/* error trace api */
perk_api int  pe_output_error_record            (const struct pe_driver_ctx *, const struct pe_error_info *);
perk_api int  pe_output_error_vector            (const struct pe_driver_ctx *);

/* header dump api */
perk_api int  pe_hexdump_image_dos_hdr          (const struct pe_driver_ctx *, const struct pe_image_meta *);
perk_api int  pe_hexdump_coff_image_hdr         (const struct pe_driver_ctx *, const struct pe_image_meta *);
perk_api int  pe_hexdump_coff_object_hdr        (const struct pe_driver_ctx *, const struct pe_image_meta *);
perk_api int  pe_hexdump_coff_opt_hdr           (const struct pe_driver_ctx *, const struct pe_image_meta *);
perk_api int  pe_hexdump_sec_tbl                (const struct pe_driver_ctx *, const struct pe_image_meta *);
perk_api int  pe_hexdump_export_hdr             (const struct pe_driver_ctx *, const struct pe_image_meta *);
perk_api int  pe_hexdump_import_tbl             (const struct pe_driver_ctx *, const struct pe_image_meta *);

/* raw image api */
perk_api int  pe_raw_map_raw_image              (const struct pe_driver_ctx *,
                                                 int fd, const char * path, int prot,
                                                 struct pe_raw_image *);

perk_api int  pe_raw_unmap_raw_image            (struct pe_raw_image *);

/* image meta api */
perk_api int  pe_meta_get_image_meta            (const struct pe_driver_ctx *,
                                                 const struct pe_raw_image *,
                                                 struct pe_image_meta **);

perk_api void pe_meta_free_image_meta           (struct pe_image_meta *);

perk_api int  pe_meta_get_named_section_index   (const struct pe_image_meta *, const char *);
perk_api int  pe_meta_get_block_section_index   (const struct pe_image_meta *, const struct pe_block *);

perk_api int  pe_meta_get_roffset_from_rva      (const struct pe_image_meta *, uint32_t, uint32_t *);
perk_api int  pe_meta_get_rva_from_roffset      (const struct pe_image_meta *, uint32_t, uint32_t *);

perk_api int  pe_meta_get_expsym_by_name        (const struct pe_image_meta *, const char *, struct pe_expsym *);
perk_api int  pe_meta_get_expsym_by_index       (const struct pe_image_meta *, unsigned,     struct pe_expsym *);

/* info api */
perk_api int  pe_info_get_image_abi             (const struct pe_image_meta *, struct pe_info_string *);
perk_api int  pe_info_get_image_subtype         (const struct pe_image_meta *, struct pe_info_string *);
perk_api int  pe_info_get_image_subsystem       (const struct pe_image_meta *, struct pe_info_string *);
perk_api int  pe_info_get_image_framework       (const struct pe_image_meta *, struct pe_info_string *);

/* low-level reader api */
perk_api int  pe_read_dos_header                (const struct pe_raw_image_dos_hdr *,   struct pe_meta_image_dos_hdr *);
perk_api int  pe_read_coff_header               (const struct pe_raw_coff_image_hdr *,  struct pe_meta_coff_file_hdr *);
perk_api int  pe_read_object_header             (const struct pe_raw_coff_object_hdr *, struct pe_meta_coff_file_hdr *);
perk_api int  pe_read_optional_header           (const union  pe_raw_opt_hdr *,         struct pe_meta_opt_hdr *);
perk_api int  pe_read_section_header            (const struct pe_raw_sec_hdr *,         struct pe_meta_sec_hdr *);
perk_api int  pe_read_export_header             (const struct pe_raw_export_hdr *,      struct pe_meta_export_hdr *);
perk_api int  pe_read_import_header             (const struct pe_raw_import_hdr *,      struct pe_meta_import_hdr *);

perk_api int  pe_read_coff_symbol               (const struct pe_raw_coff_symbol *,     struct pe_meta_coff_symbol *,
                                                 const struct pe_meta_coff_file_hdr *,  void * base);

perk_api int  pe_read_import_lookup             (const unsigned char *,                 struct pe_meta_import_lookup *,
                                                 uint32_t magic);

/* archiver api */
perk_api int  pe_ar_get_archive_meta            (const struct pe_driver_ctx *,
                                                 const struct pe_raw_archive *,
                                                 struct pe_archive_meta **);

perk_api void pe_ar_free_archive_meta           (struct pe_archive_meta *);

/* package info */
perk_api const struct pe_source_version * pe_source_version(void);

#ifdef __cplusplus
}
#endif

#endif
