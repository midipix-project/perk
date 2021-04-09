#include "perk_driver_impl.h"
#include "argv/argv.h"

const struct argv_option pe_default_options[] = {
	{"version",	'v',TAG_VERSION,ARGV_OPTARG_NONE,0,0,0,
			"show version information"},

	{"help",	'h',TAG_HELP,ARGV_OPTARG_OPTIONAL,0,"short|long",0,
			"show usage information [listing %s options only]"},

	{"pretty",	'p',TAG_PRETTY,ARGV_OPTARG_REQUIRED,0,"yaml|dlltool",0,
			"format output for parsing by %s"},

	{"category",	'y',TAG_CATEGORY,ARGV_OPTARG_NONE,0,0,0,
			"print image category"},

	{"sections",	'c',TAG_SECTIONS,ARGV_OPTARG_NONE,0,0,0,
			"list image sections"},

	{"symbols",	'm',TAG_SYMBOLS,ARGV_OPTARG_NONE,0,0,0,
			"list symbols found in the coff symbol table"},

	{"strings",	's',TAG_STRINGS,ARGV_OPTARG_NONE,0,0,0,
			"list strings found in the coff string table"},

	{"expsyms",	'e',TAG_EXPSYMS,ARGV_OPTARG_NONE,0,0,0,
			"list exported symbols" },

	{"implibs",	'i',TAG_IMPLIBS,ARGV_OPTARG_NONE,0,0,0,
			"list .idata dependency libraries"},

	{"impsyms",	'I',TAG_IMPSYMS,ARGV_OPTARG_NONE,0,0,0,
			"list .idata dependency libraries "
			"along with imported symbols"},

	{"dsolibs",	'd',TAG_DSOLIBS,ARGV_OPTARG_NONE,0,0,0,
			"list .dsometa (mdso) dependency libraries"},

	{"dsosyms",	'D',TAG_DSOSYMS,ARGV_OPTARG_NONE,0,0,0,
			"list .dsometa (mdso) dependency libraries "
			"along with referenced symbols"},

	{"hdrdump",	'X',TAG_HDRDUMP,ARGV_OPTARG_OPTIONAL,0,
			"dos|image.dos|"
			"coff|coff.image|coff.object|coff.optional|"
			"sectbl|section.table|"
			"exphdr|export.header|"
			"imptbl|import.table",0,
			"output struct-based information for "
			"either all headers (the default) "
			"or a specific header"},

	{0,0,0,0,0,0,0,0}
};
