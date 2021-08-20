/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2021  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#ifndef PERK_READER_IMPL_H
#define PERK_READER_IMPL_H

#include <stdint.h>
#include "perk_endian_impl.h"

static inline uint16_t pe_read_short(const unsigned char * raw)
{
	if (PERK_LITTLE_ENDIAN) {
		return *(uint16_t *)raw;
	} else {
		uint16_t x = *(uint16_t *)raw;
		return x<<8 | x>>8;
	}
}

static inline uint32_t pe_swap_long(uint32_t x)
{
	return x<<24 | ((x<<8) & 0xff0000) | ((x>>8) & 0xff00) | x>>24;
}

static inline uint32_t pe_read_long(const unsigned char * raw)
{
	if (PERK_LITTLE_ENDIAN)
		return *(uint32_t *)raw;
	else
		return pe_swap_long(*(uint32_t *)raw);
}

static inline uint64_t pe_read_quad(const unsigned char * raw)
{
	if (PERK_LITTLE_ENDIAN) {
		return *(uint64_t *)raw;
	} else {
		uint64_t x = *(uint64_t *)raw;
		return ((uint64_t)pe_swap_long(x)<<32) | pe_swap_long(x>>32);
	}
}

#endif
