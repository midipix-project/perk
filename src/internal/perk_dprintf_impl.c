#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int pe_dprintf(int fd, const char * fmt, ...)
{
	int	ret;
	int	cnt;
	int	size;
	va_list	ap;
	char *	ch;
	char *	buf;
	char	chbuf[2048];

	va_start(ap,fmt);

	size = sizeof(chbuf);
	buf  = ((cnt = vsnprintf(chbuf,size,fmt,ap)) < size)
		? chbuf : malloc(cnt + 1);

	va_end(ap);

	if (buf == chbuf) {
		(void)0;

	} else if (buf) {
		va_start(ap,fmt);
		vsprintf(buf,fmt,ap);
		va_end(ap);

	} else {
		return -1;
	}

	ret = 0;
	ch  = buf;

	for (; cnt && ret>=0; ) {
		ret = write(fd,ch,cnt);

		while ((ret < 0) && (errno == EINTR))
			ret = write(fd,ch,cnt);

		ch  += ret;
		cnt -= ret;
	}

	ret = (ret < 0) ? -1 : ch - buf;

	if (buf != chbuf)
		free(buf);

	return ret;
}
