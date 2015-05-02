#ifndef PERK_IMPL_H
#define PERK_IMPL_H

#include <stdint.h>
#include <endian.h>

/* internal structures */
struct perk_ctx {
	int			argc;
	char **			argv;
	char **			envp;
	const char *		fname;
	int			status;
	int			flags;
	int			fd;
	FILE *			fout;
	FILE *			tmp;
	struct pe_raw_image	map;
};

#if (BYTE_ORDER == LITTLE_ENDIAN)

static inline uint16_t pe_read_short(const unsigned char * raw)
{
	return *(uint16_t *)raw;
}

static inline uint32_t pe_read_long(const unsigned char * raw)
{
	return *(uint32_t *)raw;
}

static inline uint64_t pe_read_quad(const unsigned char * raw)
{
	return *(uint64_t *)raw;
}

#else

static inline uint16_t pe_read_short(unsigned char * raw)
{
	uint16_t x = *(uint16_t *)raw;
	return x<<8 | x>>8;
}

static inline uint32_t pe_swap_long(uint32_t x)
{
	return x<<24 | (x<<8) & 0xff0000 | (x>>8) & 0xff00 | x>>24;
}

static inline uint32_t pe_read_long(unsigned char * raw)
{
	return pe_swap_long(*(uint32_t *)raw);
}

static inline uint64_t pe_read_quad(unsigned char * raw)
{
	uint64_t x = *(uint64_t *)raw;
	return ((uint64_t)pe_swap_long(x)<<32) | pe_swap_long(x>>32);
}

#endif

#endif
