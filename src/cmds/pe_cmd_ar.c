/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2025  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <perk/perk.h>
#include "perk_driver_impl.h"

int pe_cmd_ar(
	const struct pe_driver_ctx * dctx,
	uint64_t                     flags,
	const char *                 posname,
	const char *                 arname,
	const char **                units)
{
	(void)dctx;
	(void)flags;
	(void)posname;
	(void)arname;
	(void)units;
	return 0;
}
