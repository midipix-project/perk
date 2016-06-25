#ifndef PE_DRIVER_IMPL_H
#define PE_DRIVER_IMPL_H

#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

#include <perk/perk.h>
#include <perk/perk_meta.h>
#include "argv/argv.h"

extern const struct argv_option pe_default_options[];

enum app_tags {
	TAG_HELP,
	TAG_VERSION,
	TAG_OUTPUT,
	TAG_PRETTY,
	TAG_EXPSYMS,
	TAG_IMPLIBS,
	TAG_IMPSYMS,
};

struct pe_driver_ctx_impl {
	struct pe_symbol_ctx	symctx;
	struct pe_output_ctx	outctx;
	struct pe_linker_ctx	lnkctx;
	struct pe_server_ctx	srvctx;
	struct pe_io_ctx	ioctx;
	struct pe_common_ctx	cctx;
	struct pe_driver_ctx	ctx;
};

struct pe_unit_ctx_impl {
	const char *		path;
	struct pe_raw_image	map;
	struct pe_image_meta *	meta;
	struct pe_io_ctx	ioctx;
	struct pe_common_ctx	cctx;
	struct pe_unit_ctx	uctx;
};

#endif
