#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <perk/perk.h>

int pe_map_raw_image(
	int			fd,
	const char *		path,
	int			prot,
	struct pe_raw_image *	map)
{
	struct stat	stat;
	bool		fnew;
	int		ret;

	if ((fnew = (fd < 0)))
		fd  = open(path,O_RDONLY | O_CLOEXEC);

	if (fd < 0)
		return -1;

	if ((ret = fstat(fd,&stat) < 0) && fnew)
		close(fd);

	if (ret < 0)
		return -1;

	map->size = stat.st_size;
	map->addr = mmap(0,map->size,prot,MAP_PRIVATE,fd,0);

	if (fnew)
		close(fd);

	return (map->addr == MAP_FAILED) ? -1 : 0;
}

int pe_unmap_raw_image(struct pe_raw_image * map)
{
	return munmap(map->addr,map->size);
}
