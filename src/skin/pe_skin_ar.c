#include "perk_driver_impl.h"
#include "perk_visibility_impl.h"
#include "argv/argv.h"

const perk_hidden struct argv_option pe_ar_options[] = {
	{"Wversion",	'V',TAG_VERSION,ARGV_OPTARG_NONE,
			ARGV_OPTION_HYBRID_ONLY,0,0,
			"show version information"},

	{"Whelp",	'h',TAG_HELP,ARGV_OPTARG_NONE,
			ARGV_OPTION_HYBRID_ONLY,0,0,
			"show usage information"},

	{"Wverbose",	'v',TAG_VERBOSE,ARGV_OPTARG_NONE,
			ARGV_OPTION_HYBRID_ONLY,0,0,
			"provide verbose output"},

	{"Wlist",	't',TAG_AR_LIST_MEMBERS,ARGV_OPTARG_NONE,
			ARGV_OPTION_HYBRID_ONLY,0,0,
			"list archive members; names only be default, "
			"or otherwise a detailed table of contents in "
			"verbose mode"},

	{"Wpretty",	0,TAG_PRETTY,ARGV_OPTARG_REQUIRED,
			ARGV_OPTION_HYBRID_ONLY|ARGV_OPTION_HYBRID_EQUAL,
			"yaml|posix|hexdata",0,
			"list archive members or print content "
			"using the %s format specification"},

	{0,0,0,0,0,0,0,0}
};
