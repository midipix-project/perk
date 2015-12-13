#include "perk_driver_impl.h"
#include "argv/argv.h"

const struct argv_option pe_default_options[] = {
	{"version",	'v',TAG_VERSION,ARGV_OPTARG_NONE,0,0,
			"show version information"},

	{"help",	'h',TAG_HELP,ARGV_OPTARG_OPTIONAL,"short|long",0,
			"show usage information [listing %s options only]"},

	{"output",	'o',TAG_OUTPUT,ARGV_OPTARG_REQUIRED,0,"<file>",
			"write output to %s"},

	{"pretty",	'p',TAG_PRETTY,ARGV_OPTARG_REQUIRED,"yaml",0,
			"format output for parsing by %s"},

	{"expsyms",	'e',TAG_EXPSYMS,ARGV_OPTARG_NONE,0,0,
			"print exported symbols" },

	{"implibs",	'i',TAG_IMPLIBS,ARGV_OPTARG_NONE,0,0,
			"list direct dependency libraries"},

	{"impsyms",	'I',TAG_IMPSYMS,ARGV_OPTARG_NONE,0,0,
			"list direct dependency libraries "
			"along with required symbols"},
	{0}
};
