#ifndef PERK_H
#define PERK_H

#include <stdint.h>
#include <stdio.h>

#include "perk_api.h"
#include "perk_consts.h"
#include "perk_structs.h"
#include "perk_meta.h"
#include "perk_output.h"

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

/* exec flags */
#define PERK_HELP		0x01

/* errors */
#define PERK_OK			0x00
#define PERK_BAD_OPT		0xA1
#define PERK_BAD_OPT_VAL	0xA2
#define PERK_IO_ERROR		0xA3
#define PERK_MAP_ERROR		0xA4
#define PERK_BAD_DOS_HEADER	0xA5
#define PERK_BAD_COFF_HEADER	0xA6
#define PERK_BAD_IMAGE_TYPE	0xA7
#define PERK_MALFORMED_IMAGE	0xA8

struct pe_raw_image {
	char *	addr;
	size_t	size;
};

struct pe_image_summary {
	uint32_t	num_of_export_syms;
	uint32_t	num_of_implibs;
	uint32_t	num_of_relocs;
};

struct pe_image_meta {
	struct pe_raw_image		image;
	struct pe_image_summary		summary;

	struct pe_meta_image_dos_hdr	dos;
	struct pe_meta_coff_file_hdr	coff;
	struct pe_meta_opt_hdr		opt;
	struct pe_meta_sec_hdr *	sectbl;

	struct pe_image_dos_hdr *	ados;
	struct pe_coff_file_hdr *	acoff;
	union  pe_opt_hdr *		aopt;
	struct pe_sec_hdr *		asectbl;

	struct pe_meta_export_hdr	edata;
	struct pe_export_hdr *		aedata;
	struct pe_meta_sec_hdr *	hedata;

	struct pe_meta_import_hdr *	idata;
	struct pe_import_hdr *		aidata;
	struct pe_meta_sec_hdr *	hidata;
};

/* utility api */
perk_api int pe_output_export_symbols	(const struct pe_image_meta *, uint32_t flags, FILE *);

/* high-level api */
perk_api int pe_map_raw_image		(int fd, const char * name, int prot, struct pe_raw_image *);
perk_api int pe_unmap_raw_image		(struct pe_raw_image *);

perk_api int pe_get_image_meta		(const struct pe_raw_image *, struct pe_image_meta **);
perk_api int pe_free_image_meta		(struct pe_image_meta *);

perk_api int pe_get_named_section_index	(const struct pe_image_meta *, const char * name);
perk_api int pe_get_block_section_index	(const struct pe_image_meta *, const struct pe_block *);

/* low-level api */
perk_api int pe_read_dos_header		(const struct pe_image_dos_hdr *,	struct pe_meta_image_dos_hdr *);
perk_api int pe_read_coff_header	(const struct pe_coff_file_hdr *,	struct pe_meta_coff_file_hdr *);
perk_api int pe_read_optional_header	(const union  pe_opt_hdr *,		struct pe_meta_opt_hdr *);
perk_api int pe_read_section_header	(const struct pe_sec_hdr *,		struct pe_meta_sec_hdr *);
perk_api int pe_read_export_header	(const struct pe_export_hdr *,		struct pe_meta_export_hdr *);
perk_api int pe_read_import_header	(const struct pe_import_hdr *,		struct pe_meta_import_hdr *);
perk_api int pe_read_import_lookup_item	(const struct pe_import_lookup_item *,	struct pe_meta_import_lookup_item *,uint32_t magic);

#ifdef __cplusplus
}
#endif

#endif
