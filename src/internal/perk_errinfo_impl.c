/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2016  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <perk/perk.h>
#include "perk_driver_impl.h"
#include "perk_errinfo_impl.h"

int pe_record_error(
	const struct pe_driver_ctx *	dctx,
	int				esyscode,
	int				elibcode,
	const char *			efunction,
	int				eline,
	unsigned			eflags,
	void *				eany)
{
	struct pe_driver_ctx_impl *	ictx;
	struct pe_error_info *		erri;

	ictx = pe_get_driver_ictx(dctx);

	if (ictx->errinfp == ictx->erricap)
		return -1;

	*ictx->errinfp = &ictx->erribuf[ictx->errinfp - ictx->erriptr];
	erri = *ictx->errinfp;

	erri->edctx     = dctx;
	erri->esyscode  = esyscode;
	erri->elibcode  = elibcode;
	erri->efunction = efunction;
	erri->eline     = eline;
	erri->eflags    = eflags;
	erri->eany      = eany;

	ictx->errinfp++;

	return -1;
}
