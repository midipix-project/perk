#ifndef PERK_DPRINTF_IMPL_H
#define PERK_DPRINTF_IMPL_H

#ifdef  ARGV_DRIVER
#define argv_dprintf pe_dprintf
#endif

int pe_dprintf(int fd, const char * fmt, ...);

#endif
