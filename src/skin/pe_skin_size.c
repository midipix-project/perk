#include "perk_driver_impl.h"
#include "perk_visibility_impl.h"
#include "argv/argv.h"

const perk_hidden struct argv_option pe_size_options[] = {
	{"version",	'V',TAG_VERSION,ARGV_OPTARG_NONE,0,0,0,
			"show version information"},

	{"help",	'h',TAG_HELP,ARGV_OPTARG_OPTIONAL,0,"short|long",0,
			"show usage information [listing %s options only]"},

	{0,0,0,0,0,0,0,0}
};
