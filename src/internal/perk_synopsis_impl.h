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



#define PERK_NM_CMD_SYNOPSIS                                             \
	"%s — PE/COFF Name Mangler\n\n"                                   \
	"Synopsis:\n"                                                      \
	"       %s [-V] [-APv] [-efox] [-g|-u] [-t format] <file> ...\n\n"  \
	"Options:\n"



#define PERK_SIZE_CMD_SYNOPSIS                                        \
	"%s — PE/COFF Section Size Information\n\n"                    \
	"Synopsis:\n"                                                   \
	"       %s [-V] [-A|-B|-G] [-d|-o|-x] [-t] [-f] <file> ...\n\n"  \
	"Options:\n"



#define PERK_STRIP_CMD_SYNOPSIS                        \
	"%s — PE/COFF Fluff Removal Utility\n\n"        \
	"Synopsis:\n"                                    \
	"       %s [-V] [-v] [-p] [-o output] <file> \n"  \
	"       %s [-X] [-x]             <file> ... \n"    \
	"       %s [-s] [-g|-d|-S]       <file> ... \n"     \
	"       %s [-w] [-R secname] ... <file> ... \n"      \
	"       %s [-w] [-K symname] ... <file> ... \n"       \
	"       %s [-w] [-N symname] ... <file> ... \n\n"      \
	"Options:\n"



#define PERK_RANLIB_CMD_SYNOPSIS                   \
	"%s — PE/COFF Archive Indexer\n\n"          \
	"Synopsis:\n"                                \
	"       %s [-V] [-D|-U] [-t] <file> ...\n\n"  \
	"Options:\n"



#define PERK_STRINGS_CMD_SYNOPSIS                                  \
	"%s — PE/COFF String Finder\n\n"                            \
	"Synopsis:\n"                                                \
	"       %s [-V] [-a] [-t format] [-n number] <file> ...\n\n"  \
	"Options:\n"



#define PERK_AR_CMD_SYNOPSIS                                                 \
	"%s — the PE/COFF Archive Manipulator\n\n"                            \
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
