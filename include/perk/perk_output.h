#ifndef PERK_OUTPUT_H
#define PERK_OUTPUT_H

#include <stdint.h>

#define PERK_PRETTY(x)			((uint64_t)x << 32)

/* output actions */
#define PERK_OUTPUT_EXPORT_SYMS		0x00000001


/* pretty-printer flags */
#define PERK_PRETTY_LIST		PERK_PRETTY(0x00000001)
#define PERK_PRETTY_TABLE		PERK_PRETTY(0x00000002)
#define PERK_PRETTY_READOBJ		PERK_PRETTY(0x00000004)
#define PERK_PRETTY_OBJDUMP		PERK_PRETTY(0x00000008)

#endif
