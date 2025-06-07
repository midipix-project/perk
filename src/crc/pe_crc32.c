/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2025  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdint.h>
#include <unistd.h>

#include <perk/perk.h>
#include <perk/perk_crc32.h>

static const uint32_t crc32_table[256] = PERK_CRC32_TABLE;

uint32_t pe_hash_mbstr_crc32(const unsigned char * str, size_t * symlen)
{
	const unsigned char *	ch;
	uint32_t		crc32;

	crc32	= 0 ^ 0xFFFFFFFF;
	ch	= str;

	while (*ch) {
		crc32 = (crc32 >> 8) ^ crc32_table[(crc32 ^ *ch) & 0xFF];
		ch++;
	}

	if (symlen)
		*symlen = ch - str;

	return (crc32 ^ 0xFFFFFFFF);
}
