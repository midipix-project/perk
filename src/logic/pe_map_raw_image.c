/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2016  Z. Gilboa                        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <perk/perk.h>
#include "perk_errinfo_impl.h"

int pe_map_raw_image(
	const struct pe_driver_ctx *	dctx,
	int				fd,
	const char *			path,
	int				prot,
	struct pe_raw_image *		map)
{
	struct stat	st;
	bool		fnew;
	int		ret;

	if ((fnew = (fd < 0)))
		fd  = open(path,O_RDONLY | O_CLOEXEC);

	if (fd < 0)
		return PERK_SYSTEM_ERROR(dctx);

	if ((ret = fstat(fd,&st) < 0) && fnew)
		close(fd);

	if (ret < 0)
		return PERK_SYSTEM_ERROR(dctx);

	map->size = st.st_size;
	map->addr = mmap(0,map->size,prot,MAP_PRIVATE,fd,0);

	if (fnew)
		close(fd);

	return (map->addr == MAP_FAILED)
		? PERK_SYSTEM_ERROR(dctx)
		: 0;
}

int pe_unmap_raw_image(struct pe_raw_image * map)
{
	return munmap(map->addr,map->size);
}
