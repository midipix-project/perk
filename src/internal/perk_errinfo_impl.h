/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2025  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <errno.h>
#include <perk/perk.h>

int pe_record_error(
	const struct pe_driver_ctx *,
	int		esyscode,
	int		elibcode,
	const char *	efunction,
	int		eline,
	unsigned	eflags,
	void *		ectx);

#define PERK_SYSTEM_ERROR(dctx)           \
	pe_record_error(                  \
		dctx,                     \
		errno,                    \
		0,                        \
		__func__,                 \
		__LINE__,                 \
		PERK_ERROR_TOP_LEVEL,     \
		0)

#define PERK_BUFFER_ERROR(dctx)           \
	pe_record_error(                  \
		dctx,                     \
		ENOBUFS,                  \
		0,                        \
		__func__,                 \
		__LINE__,                 \
		PERK_ERROR_TOP_LEVEL,     \
		0)

#define PERK_SPAWN_ERROR(dctx)            \
	pe_record_error(                  \
		dctx,                     \
		errno,                    \
		0,                        \
		__func__,                 \
		__LINE__,                 \
		PERK_ERROR_TOP_LEVEL      \
		| (errno ? 0              \
		   : PERK_ERROR_CHILD),   \
		0)

#define PERK_FILE_ERROR(dctx)             \
	pe_record_error(                  \
		dctx,                     \
		EIO,                      \
		0,                        \
		__func__,                 \
		__LINE__,                 \
		PERK_ERROR_TOP_LEVEL,     \
		0)

#define PERK_CUSTOM_ERROR(dctx,elibcode)  \
	pe_record_error(                  \
		dctx,                     \
		0,                        \
		elibcode,                 \
		__func__,                 \
		__LINE__,                 \
		PERK_ERROR_TOP_LEVEL      \
		| PERK_ERROR_CUSTOM,      \
		0)

#define PERK_NESTED_ERROR(dctx)           \
	pe_record_error(                  \
		dctx,                     \
		0,                        \
		0,                        \
		__func__,                 \
		__LINE__,                 \
		PERK_ERROR_NESTED,        \
		0)
