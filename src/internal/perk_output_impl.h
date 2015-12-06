#include <unistd.h>
#include <perk/perk.h>

static inline FILE * pe_output_prolog(
	const struct pe_common_ctx *	cctx,
	FILE *				fout,
	FILE **				ftmp)
{
	int fdout;

	*ftmp = (FILE *)0;

	if (fout)
		return fout;
	else if (!cctx)
		return 0;
	else if (cctx->ioctx->fdout < 0)
		return stdout;

	if ((fdout = dup(cctx->ioctx->fdout)) < 0)
		return 0;

	if ((*ftmp = fdopen(fdout,"a")))
		return *ftmp;

	close(fdout);
	return 0;
}

static inline int pe_output_epilog(int ret, FILE * f)
{
	if (f)
		fclose(f);
	return ret;
}
