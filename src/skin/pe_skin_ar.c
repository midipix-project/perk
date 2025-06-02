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

	{0,0,0,0,0,0,0,0}
};
