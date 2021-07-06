/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2021  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stddef.h>
#include <string.h>

#include <perk/perk.h>
#include <perk/perk_consts.h>
#include <perk/perk_structs.h>
#include "perk_hdrdump_impl.h"

#define PE_TABWIDTH	8
#define PE_HDRSPACE	40

static const char pe_hex_header_hex[] =
	"  0  1  2  3  4  5  6  7    8  9  a  b  c  d  e  f          01234567 89abcdef\n";

static const char pe_hex_header_dot[] =
	" .-------------------------------------------------.       .-----------------.\n";

static const char pe_hex_footer[] =
	"|---------------------------------------------------|      |-----------------|\n\n";

size_t pe_output_hex_header(
	char *		buf,
	const char *	sname,
	uint64_t	faddr,
	uint64_t	vaddr,
	int		bits)
{
	char *		ch;
	size_t		hlen;
	size_t		dlen;
	size_t		slen;
	size_t		nlen;
	uint32_t	high;
	uint32_t	low;

	ch   = buf;
	hlen = strlen(pe_hex_header_hex);
	dlen = strlen(pe_hex_header_dot);
	slen = strlen(sname);

	switch (bits) {
		case 32:
			low  = faddr;
			nlen = sprintf(ch,"faddr: 0x%08x\n",low);
			break;

		case 64:
			low  = faddr;
			high = faddr >> 32;
			nlen = sprintf(ch,"faddr: 0x%08x%08x\n",high,low);
			break;

		default:
			return -1;
	}

	ch += nlen;

	switch (bits) {
		case 32:
			low  = vaddr;
			nlen = sprintf(ch,"vaddr: 0x%08x",low);
			break;

		default:
			low  = vaddr;
			high = vaddr >> 32;
			nlen = sprintf(ch,"vaddr: 0x%08x%08x",high,low);
			break;
	}

	ch += nlen;
	memset(ch,' ',PE_HDRSPACE - nlen);
	ch += PE_HDRSPACE - nlen;

	memcpy(ch,pe_hex_header_hex,hlen);
	ch += hlen;

	memcpy(ch,sname,slen);

	ch += slen;
	memset(ch,' ',PE_HDRSPACE - slen);
	ch += PE_HDRSPACE - slen;

	memcpy(ch,pe_hex_header_dot,dlen);
	ch += dlen;

	return ch - buf;
}

size_t pe_output_hex_footer(char * buf)
{
	size_t	nlen;

	memset(buf,' ',PE_HDRSPACE);
	buf += PE_HDRSPACE;

	nlen = strlen(pe_hex_footer);
	memcpy(buf,pe_hex_footer,nlen);

	return PE_HDRSPACE + nlen;
}

size_t pe_output_raw_element(
	char *		ch,
	const void *	rdata,
	const char *	mname,
	uint32_t	moffset,
	uint32_t	msize)
{
	size_t			nlen;
	char *			base;
	uintptr_t		addr;
	const unsigned char *	src;
	const unsigned char *	cap;
	const unsigned char *	mark;
	size_t			pos;
	size_t			xpos;
	const char		xch[] = "0123456789abcdef";

	/* init */
	base = ch;

	/* pos */
	addr  = (uintptr_t)rdata;
	addr += moffset;
	pos   = addr % 0x10;
	xpos  = pos;

	/* struct member name */
	memset(ch,' ',PE_TABWIDTH);
	ch += PE_TABWIDTH;

	*ch++ = '.';
	nlen = strlen(mname);
	memcpy(ch,mname,nlen);

	ch += nlen;
	*ch++ = ':';

	/* hex dump column */
	nlen  = PE_HDRSPACE - PE_TABWIDTH - nlen - 2;
	memset(ch,' ',nlen);
	ch += nlen;

	*ch++ = '|';
	*ch++ = ' ';

	nlen = 3 * pos;
	memset(ch,' ',nlen);
	ch += nlen;

	if (pos >= 8) {
		*ch++ = ' ';
		*ch++ = ' ';
	}

	/* hex dump */
	src  = rdata;
	src += moffset;

	mark = src;
	cap  = src + msize;

	for (; src<cap; ) {
		*ch++ = xch[*src / 0x10];
		*ch++ = xch[*src % 0x10];
		*ch++ = ' ';

		/* pretty columns */
		if (pos == 7) {
			*ch++ = ' ';
			*ch++ = ' ';
		}

		/* binary dump needed? */
		if ((++src == cap) || (pos == 0xf)) {
			/* binary dump column */
			nlen  = 3 * (0xf - pos);
			nlen += (pos < 7) ? 2 : 0;

			memset(ch,' ',nlen);
			ch += nlen;

			*ch++ = '|';
			*ch++ = ' '; *ch++ = ' '; *ch++ = ' ';
			*ch++ = ' '; *ch++ = ' '; *ch++ = ' ';
			*ch++ = '|';

			nlen = xpos + (xpos >= 7);
			memset(ch,' ',nlen);
			ch += nlen;

			/* binary dump */
			for (; mark<src; ) {
				*ch++ = (*mark >= 0x20) && (*mark < 0x7f)
					? *mark : '.';

				/* pretty columns */
				if ((++mark < src) && (++xpos == 8)) {
					*ch++ = ' ';
					xpos  = 0;
				}
			}

			nlen  = 0xf - pos;
			nlen += (pos <= 7);

			memset(ch,' ',nlen);
			ch += nlen;

			*ch++ = '|';
			*ch++ = '\n';
		}

		/* does member hex dump span across more than one line? */
		if ((src < cap) && (++pos == 0x10)) {
			memset(ch,' ',PE_HDRSPACE);
			ch += PE_HDRSPACE;
			*ch++ = '|';
			*ch++ = ' ';

			mark = src;
			pos  = 0;
			xpos = 0;
		}
	}

	return ch - base;
}
