/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2017  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <perk/perk.h>

static const char aclr_reset[]   = "\x1b[0m";
static const char aclr_bold[]    = "\x1b[1m";

static const char aclr_red[]     = "\x1b[31m";
static const char aclr_green[]   = "\x1b[32m";
static const char aclr_blue[]    = "\x1b[34m";
static const char aclr_magenta[] = "\x1b[35m";

static const char * const pe_error_strings[PERK_ERR_CAP] = {
	[PERK_ERR_FLOW_ERROR]      = "flow error: unexpected condition or other",
	[PERK_ERR_FLEE_ERROR]      = "flees and bugs and cats and mice",
	[PERK_ERR_NULL_CONTEXT]    = "null driver or unit context",
	[PERK_ERR_NULL_IMAGE]      = "null image base pointer",
	[PERK_ERR_INVALID_CONTEXT] = "invalid driver or unit context",
	[PERK_ERR_INVALID_IMAGE]   = "invalid PE image",
	[PERK_ERR_IMAGE_SIZE_ZERO] = "PE image size cannot be zero",
	[PERK_ERR_IMAGE_MALFORMED] = "malformed PE image detected",
	[PERK_ERR_BAD_DOS_HEADER]  = "bad DOS header",
	[PERK_ERR_BAD_COFF_HEADER] = "bad COFF header",
	[PERK_ERR_BAD_IMAGE_TYPE]  = "bad PE image type",
};

static const char * pe_output_error_header(const struct pe_error_info * erri)
{
	if (erri->eflags & PERK_ERROR_CHILD)
		return "exec error upon";

	else if (erri->eflags & PERK_ERROR_TOP_LEVEL)
		return "error logged in";

	else if (erri->eflags & PERK_ERROR_NESTED)
		return "< returned to >";

	else
		return "distorted state";
}

static const char * pe_output_unit_header(const struct pe_error_info * erri)
{
	if (!(erri->eflags & PERK_ERROR_CUSTOM))
		return "while opening";

	else if (erri->elibcode == PERK_ERR_IMAGE_SIZE_ZERO)
		return "while mapping";

	else
		return "while parsing";
}

static const char * pe_output_strerror(const struct pe_error_info * erri)
{
	if (erri->eflags & PERK_ERROR_CUSTOM)
		return ((erri->elibcode < 0) || (erri->elibcode >= PERK_ERR_CAP))
			? "internal error: please report to the maintainer"
			: pe_error_strings[erri->elibcode];

	else if (erri->eflags & PERK_ERROR_NESTED)
		return "";

	else if (erri->eflags & PERK_ERROR_CHILD)
		return "(see child process error messages)";

	else if (erri->esyscode == ENOBUFS)
		return "input error: string length exceeds buffer size.";

	else
		return strerror(erri->esyscode);
}

static int pe_output_error_record_plain(
	const struct pe_driver_ctx *	dctx,
	const struct pe_error_info *	erri)
{
	const char * epath;
	const char * errdesc = pe_output_strerror(erri);

	epath = erri->euctx
		? *erri->euctx->path
		: erri->eunit;

	if (epath && !(erri->eflags & PERK_ERROR_NESTED))
		if (fprintf(stderr,"%s: [%s] '%s':\n",
				dctx->program,
				pe_output_unit_header(erri),
				epath) < 0)
			return -1;

	if (fprintf(stderr,"%s: %s %s(), line %d%s%s.\n",
			dctx->program,
			pe_output_error_header(erri),
			erri->efunction,
			erri->eline,
			strlen(errdesc) ? ": " : "",
			errdesc) < 0)
		return -1;

	return fflush(stderr);
}

static int pe_output_error_record_annotated(
	const struct pe_driver_ctx *	dctx,
	const struct pe_error_info *	erri)
{
	const char * epath;
	const char * errdesc = pe_output_strerror(erri);

	epath = erri->euctx
		? *erri->euctx->path
		: erri->eunit;

	if (epath && !(erri->eflags & PERK_ERROR_NESTED))
		if (fprintf(
				stderr,
				"%s%s%s:%s %s[%s]%s %s%s'%s'%s:\n",

				aclr_bold,aclr_magenta,
				dctx->program,
				aclr_reset,

				aclr_bold,
				pe_output_unit_header(erri),
				aclr_reset,

				aclr_bold,aclr_red,
				epath,
				aclr_reset) < 0)
			return -1;

	if (fprintf(
			stderr,
			"%s%s%s:%s %s%s%s %s%s%s()%s, %s%sline %d%s%s%s%s%s.\n",

			aclr_bold,aclr_magenta,
			dctx->program,
			aclr_reset,

			aclr_bold,
			pe_output_error_header(erri),
			aclr_reset,

			aclr_bold,aclr_blue,
			erri->efunction,
			aclr_reset,

			aclr_bold,aclr_green,
			erri->eline,
			aclr_reset,
			strlen(errdesc) ? ": " : "",

			aclr_bold,
			pe_output_strerror(erri),
			aclr_reset) < 0)
		return -1;

	return fflush(stderr);
}

int pe_output_error_record(
	const struct pe_driver_ctx *	dctx,
	const struct pe_error_info *	erri)
{
	if (dctx->cctx->drvflags & PERK_DRIVER_ANNOTATE_NEVER)
		return pe_output_error_record_plain(dctx,erri);

	else if (dctx->cctx->drvflags & PERK_DRIVER_ANNOTATE_ALWAYS)
		return pe_output_error_record_annotated(dctx,erri);

	else if (isatty(STDERR_FILENO))
		return pe_output_error_record_annotated(dctx,erri);

	else
		return pe_output_error_record_plain(dctx,erri);
}

static int pe_output_error_vector_plain(const struct pe_driver_ctx * dctx)
{
	struct pe_error_info ** perr;

	for (perr=dctx->errv; *perr; perr++)
		if (pe_output_error_record_plain(dctx,*perr))
			return -1;

	return 0;
}

static int pe_output_error_vector_annotated(const struct pe_driver_ctx * dctx)
{
	struct pe_error_info ** perr;

	for (perr=dctx->errv; *perr; perr++)
		if (pe_output_error_record_annotated(dctx,*perr))
			return -1;

	return 0;
}

int pe_output_error_vector(const struct pe_driver_ctx * dctx)
{
	if (dctx->cctx->drvflags & PERK_DRIVER_ANNOTATE_NEVER)
		return pe_output_error_vector_plain(dctx);

	else if (dctx->cctx->drvflags & PERK_DRIVER_ANNOTATE_ALWAYS)
		return pe_output_error_vector_annotated(dctx);

	else if (isatty(STDERR_FILENO))
		return pe_output_error_vector_annotated(dctx);

	else
		return pe_output_error_vector_plain(dctx);
}
