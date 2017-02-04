/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2017  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#ifndef PERK_ENDIAN_IMPL_H
#define PERK_ENDIAN_IMPL_H

#include <stdbool.h>

#define PERK_LITTLE_ENDIAN	pe_little_endian_host()

static inline bool pe_little_endian_host(void)
{
	const long test = 1;
	return *((char *)&test);
}

#endif
