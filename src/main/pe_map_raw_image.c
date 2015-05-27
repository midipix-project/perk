#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <perk/perk.h>

int pe_map_raw_image (int fd, const char * fname, int prot, struct pe_raw_image * map)
{
	struct stat stat;
	int nfd, ret;

	if ((nfd = !fd))
		fd  = open(fname,O_RDONLY | O_CLOEXEC);

	if ((fd < 0) || (fstat(fd,&stat) < 0))
		return errno;

	map->size = stat.st_size;
	map->addr = mmap(0,map->size,prot,MAP_PRIVATE,fd,0);

	if (map->addr == MAP_FAILED) {
		map->addr = 0;
		ret = PERK_MAP_ERROR;
	} else
		ret = 0;

	if (nfd) close(fd);

	return ret;
}

int pe_unmap_raw_image (struct pe_raw_image * map)
{
	return munmap(map->addr, map->size);
}
