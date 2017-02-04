#ifndef PERK_H
#define PERK_H

#include <stdint.h>
#include <stdio.h>

#include "perk_api.h"
#include "perk_consts.h"
#include "perk_structs.h"
#include "perk_meta.h"

#ifdef __cplusplus
extern "C" {
#endif

/* pre-alpha */
#ifndef PERK_APP
#ifndef PERK_PRE_ALPHA
#error  libperk: pre-alpha: ABI is not final!
#error  to use the library, please pass -DPERK_PRE_ALPHA to the compiler.
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
#define PERK_DRIVER_VERBOSITY_USAGE	0x0004
#define PERK_DRIVER_CLONE_VECTOR	0x0008

#define PERK_DRIVER_VERSION		0x0010
#define PERK_DRIVER_DRY_RUN		0x0020

#define PERK_DRIVER_ANNOTATE_ALWAYS	0x1000
#define PERK_DRIVER_ANNOTATE_NEVER	0x2000
#define PERK_DRIVER_ANNOTATE_FULL	0x4000

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
	PERK_ERR_CAP,
};

struct pe_source_version {
	int		major;
	int		minor;
	int		revision;
	const char *	commit;
};

struct pe_raw_image {
	void *	addr;
	size_t	size;
};

struct pe_expsym {
	const char *	name;
	void *		eaddr;
	void *		maddr;
	uint32_t	roffset;
};

struct pe_meta_summary {
	int32_t		nexpsyms;
	int32_t		nimplibs;
	int32_t		nrelocs;
};

struct pe_image_meta {
	struct pe_meta_summary		summary;
	struct pe_meta_image_dos_hdr	dos;
	struct pe_meta_coff_file_hdr	coff;
	struct pe_meta_opt_hdr		opt;
	struct pe_meta_sec_hdr *	sectbl;

	struct pe_meta_export_hdr	edata;
	struct pe_meta_sec_hdr *	hedata;

	struct pe_meta_import_hdr *	idata;
	struct pe_meta_sec_hdr *	hidata;

	struct pe_raw_image		image;
	struct pe_raw_image_dos_hdr *	ados;
	struct pe_raw_coff_image_hdr *	acoff;
	struct pe_raw_coff_object_hdr *	aobj;
	union  pe_raw_opt_hdr *		aopt;
	struct pe_raw_sec_hdr *		asectbl;

	struct pe_raw_export_hdr *	aedata;
	struct pe_raw_import_hdr *	aidata;
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
	const struct pe_raw_image *	map;
	const struct pe_image_meta *	meta;
	void *				any;
};

struct pe_info_string {
	char				buffer[128];
};

/* package info */
perk_api				const struct pe_source_version * pe_source_version(void);

/* driver api */
perk_api int  pe_get_driver_ctx		(char ** argv, char ** envp, uint32_t flags, struct pe_driver_ctx **);
perk_api void pe_free_driver_ctx	(struct pe_driver_ctx *);

perk_api int  pe_get_unit_ctx		(const struct pe_driver_ctx *, const char * path, struct pe_unit_ctx **);
perk_api void pe_free_unit_ctx		(struct pe_unit_ctx *);

/* utility api */
perk_api int  pe_main			(int, char **, char **);
perk_api int  pe_output_image_category	(const struct pe_driver_ctx *, const struct pe_image_meta *, FILE *);
perk_api int  pe_output_image_sections	(const struct pe_driver_ctx *, const struct pe_image_meta *, FILE *);
perk_api int  pe_output_image_symbols	(const struct pe_driver_ctx *, const struct pe_image_meta *, FILE *);
perk_api int  pe_output_image_strings	(const struct pe_driver_ctx *, const struct pe_image_meta *, FILE *);
perk_api int  pe_output_export_symbols	(const struct pe_driver_ctx *, const struct pe_image_meta *, FILE *);
perk_api int  pe_output_import_libraries(const struct pe_driver_ctx *, const struct pe_image_meta *, FILE *);

/* error trace api */
perk_api int  pe_output_error_record	(const struct pe_driver_ctx *, const struct pe_error_info *);
perk_api int  pe_output_error_vector	(const struct pe_driver_ctx *);

/* high-level api */
perk_api int  pe_map_raw_image		(const struct pe_driver_ctx *, int fd, const char * path, int prot, struct pe_raw_image *);
perk_api int  pe_unmap_raw_image	(struct pe_raw_image *);

perk_api int  pe_get_image_meta		(const struct pe_driver_ctx *, const struct pe_raw_image *, struct pe_image_meta **);
perk_api void pe_free_image_meta	(struct pe_image_meta *);

perk_api int  pe_get_named_section_index(const struct pe_image_meta *, const char * name);
perk_api int  pe_get_block_section_index(const struct pe_image_meta *, const struct pe_block *);

perk_api int  pe_get_roffset_from_rva	(const struct pe_image_meta *, uint32_t rva, uint32_t * roffset);
perk_api int  pe_get_rva_from_roffset	(const struct pe_image_meta *, uint32_t roffset, uint32_t * rva);

perk_api int  pe_get_expsym_by_name	(const struct pe_image_meta *, const char * name, struct pe_expsym * optional);
perk_api int  pe_get_expsym_by_index	(const struct pe_image_meta *, unsigned index, struct pe_expsym * optional);

/* info api */
perk_api int  pe_get_image_abi		(const struct pe_image_meta *, struct pe_info_string * optional);
perk_api int  pe_get_image_subtype	(const struct pe_image_meta *, struct pe_info_string * optional);
perk_api int  pe_get_image_subsystem	(const struct pe_image_meta *, struct pe_info_string * optional);
perk_api int  pe_get_image_framework	(const struct pe_image_meta *, struct pe_info_string * optional);

/* low-level api */
perk_api int  pe_read_dos_header	(const struct pe_raw_image_dos_hdr *,	struct pe_meta_image_dos_hdr *);
perk_api int  pe_read_coff_header	(const struct pe_raw_coff_image_hdr *,	struct pe_meta_coff_file_hdr *);
perk_api int  pe_read_object_header	(const struct pe_raw_coff_object_hdr *,	struct pe_meta_coff_file_hdr *);
perk_api int  pe_read_optional_header	(const union  pe_raw_opt_hdr *,		struct pe_meta_opt_hdr *);
perk_api int  pe_read_section_header	(const struct pe_raw_sec_hdr *,		struct pe_meta_sec_hdr *);
perk_api int  pe_read_export_header	(const struct pe_raw_export_hdr *,	struct pe_meta_export_hdr *);
perk_api int  pe_read_import_header	(const struct pe_raw_import_hdr *,	struct pe_meta_import_hdr *);

perk_api int  pe_read_coff_symbol	(const struct pe_raw_coff_symbol *,	struct pe_meta_coff_symbol *,
					 const struct pe_meta_coff_file_hdr *,	void * base);

perk_api int  pe_read_import_lookup	(const union  pe_raw_import_lookup *,	struct pe_meta_import_lookup *,
					 uint32_t magic);

#ifdef __cplusplus
}
#endif

#endif
