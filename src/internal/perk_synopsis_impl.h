/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2025  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#ifndef SLIBTOOL_SYNOPSIS_IMPL_H
#define SLIBTOOL_SYNOPSIS_IMPL_H

#define PERK_DEFAULT_CMD_SYNOPSIS                          \
	"%s — PE/COFF Resource Kit\n\n"                     \
	"Synopsis:\n"                                        \
	"       %s                 [option] ...\n"            \
	"       %s --cmd=<command> [option] ...\n"             \
	"       %s --cmd=<command> [option] ... <file> ...\n\n" \
	"Options:\n"

#define PERK_PERK_CMD_SYNOPSIS              \
	"%s — PE/COFF Resource Kit\n\n"      \
	"Synopsis:\n"                         \
	"       %s [option] ...\n"             \
	"       %s [option] ... <file> ...\n\n" \
	"Options:\n"

#define PERK_AR_CMD_SYNOPSIS                                                 \
	"%s — the PE/COFF Resource Kit Archiver\n\n"                          \
	"Synopsis:\n"                                                          \
	"       %s -d [-v]                          <archive> <file> ...\n"     \
	"       %s -p [-v] [-s]                     <archive> <file> ...\n"     \
	"       %s -q [-v] [-c]                     <archive> <file> ...\n"     \
	"       %s -r [-v] [-c] [-u]                <archive> <file> ...\n"     \
	"       %s -t [-v] [-s]                     <archive> [<file> ...]\n"   \
	"       %s -x [-v] [-s] [-C] [-T]           <archive> [<file> ...]\n\n" \
	                                                                        \
	"       %s -m    [-v]                       <archive> <file> ...\n"     \
	"       %s -m -a [-v]             <posname> <archive> <file> ...\n"     \
	"       %s -m -b [-v]             <posname> <archive> <file> ...\n"     \
	"       %s -m -i [-v]             <posname> <archive> <file> ...\n\n"   \
	                                                                        \
	"       %s -r    [-v] [-c] [-u]             <archive> <file> ...\n"     \
	"       %s -r -a [-v] [-c] [-u]   <posname> <archive> <file> ...\n"     \
	"       %s -r -b [-v] [-c] [-u]   <posname> <archive> <file> ...\n"     \
	"       %s -r -i [-v] [-c] [-u]   <posname> <archive> <file> ...\n\n"   \
	"Options:\n"

#endif
