/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2021  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#ifndef PERK_HDRDUMP_IMPL_H
#define PERK_HDRDUMP_IMPL_H

#include <stddef.h>

#define PE_QUOTE(s)	#s
#define PE_STRING(s)	PE_QUOTE(s)
#define PE_TYPE(t)	struct pe_raw_ ## t
#define PE_OFFSET(t,x)	offsetof(PE_TYPE(t),x)
#define PE_SIZE(p,x)	sizeof((p)->x)

#define PE_OUTPUT_RAW_ELEMENT_TABLE(ch,p,t,x)   \
	pe_output_raw_element(                  \
		ch,p,                           \
		PE_STRING(x),                   \
		PE_OFFSET(t,x),                 \
		PE_SIZE(p,x))

#define PE_OUTPUT_TABLE(x)              \
	PE_OUTPUT_RAW_ELEMENT_TABLE(    \
		ch,                     \
		PE_ADDR,                \
		PE_RAW_STRUCT,          \
		x)

size_t pe_output_hex_header(
	char *		buf,
	const char * 	sname,
	uint64_t	faddr,
	uint64_t	vaddr,
	int		bits);

size_t pe_output_hex_footer(
	char *		buf);

size_t pe_output_raw_element(
	char *		ch,
	const void *	rdata,
	const char *	mname,
	uint32_t	moffset,
	uint32_t	msize);

static inline int pe_image_bits(const struct pe_image_meta * m)
{
	switch (m->m_abi) {
		case PE_ABI_PE32:
			return 32;

		case PE_ABI_PE64:
			return 64;

		default:
			return -1;
	}
}

#endif
