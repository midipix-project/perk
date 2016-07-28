#define ARGV_DRIVER
#include "argv/argv.h"

int foo(char ** argv, const struct argv_option * options,unsigned int flags, char * header, char * arg)
{
	argv_usage(0,header,options,arg);
	return argv_get(argv,options,flags) ? 0 : -1;
}
