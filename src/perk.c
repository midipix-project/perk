/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2021  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <perk/perk.h>

#define PE_UNUSED_PARAMETER(p) (void)p

int main(int argc, char ** argv, char ** envp)
{
	PE_UNUSED_PARAMETER(argc);
	return pe_main(argv,envp,0);
}
