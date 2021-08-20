/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2021  SysDeer Technologies, LLC        */
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
#include "perk_driver_impl.h"
#include "perk_errinfo_impl.h"

int pe_map_raw_image(
	const struct pe_driver_ctx *	dctx,
	int				fd,
	const char *			path,
	int				prot,
	struct pe_raw_image *		map)
{
	int		ret;
	struct stat	st;
	bool		fnew;
	int		fdcwd;

	fdcwd = pe_driver_fdcwd(dctx);

	if ((fnew = (fd < 0)))
		fd  = openat(fdcwd,path,O_RDONLY | O_CLOEXEC);

	if (fd < 0)
		return PERK_SYSTEM_ERROR(dctx);

	if ((ret = fstat(fd,&st) < 0) && fnew)
		close(fd);

	else if ((st.st_size == 0) && fnew)
		close(fd);

	if (ret < 0)
		return PERK_SYSTEM_ERROR(dctx);

	else if (st.st_size == 0)
		return PERK_CUSTOM_ERROR(
			dctx,PERK_ERR_IMAGE_SIZE_ZERO);

	map->map_size = st.st_size;
	map->map_addr = mmap(0,map->map_size,prot,MAP_PRIVATE,fd,0);

	if (fnew)
		close(fd);

	return (map->map_addr == MAP_FAILED)
		? PERK_SYSTEM_ERROR(dctx)
		: 0;
}

int pe_unmap_raw_image(struct pe_raw_image * map)
{
	return munmap(map->map_addr,map->map_size);
}
