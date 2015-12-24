/****************************************************************************/
/*  argv.h: a thread-safe argument vector parser and usage screen generator */
/*  Copyright (C) 2015  Z. Gilboa                                           */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.                       */
/*  This file is (also) part of sofort: portable software project template. */
/****************************************************************************/

#ifndef ARGV_H
#define ARGV_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define ARGV_VERBOSITY_NONE		0x00
#define ARGV_VERBOSITY_ERRORS		0x01
#define ARGV_VERBOSITY_STATUS		0x02
#define ARGV_CLONE_VECTOR		0x80

#ifndef ARGV_TAB
#define ARGV_TAB			8
#endif

enum argv_optarg {
	ARGV_OPTARG_NONE,
	ARGV_OPTARG_REQUIRED,
	ARGV_OPTARG_OPTIONAL,
};

enum argv_mode {
	ARGV_MODE_SCAN,
	ARGV_MODE_COPY,
};

enum argv_error {
	ARGV_ERROR_OK,
	ARGV_ERROR_INTERNAL,
	ARGV_ERROR_SHORT_OPTION,
	ARGV_ERROR_LONG_OPTION,
	ARGV_ERROR_OPTARG_NONE,
	ARGV_ERROR_OPTARG_REQUIRED,
	ARGV_ERROR_OPTARG_PARADIGM,
};

struct argv_option {
	const char *		long_name;
	const char		short_name;
	int			tag;
	enum argv_optarg	optarg;
	const char *		paradigm;
	const char *		argname;
	const char *		description;
};

struct argv_entry {
	const char *	arg;
	int		tag;
	bool		fopt;
	bool		fval;
	bool		fnoscan;
	enum argv_error errcode;
};

struct argv_meta {
	const char **		argv;
	struct argv_entry *	entries;
};

struct argv_meta_impl {
	const char **		argv;
	char *			strbuf;
	struct argv_meta	meta;
};

struct argv_ctx {
	int				flags;
	int				mode;
	int				nentries;
	enum argv_error 		errcode;
	const char *			errch;
	const struct argv_option *	erropt;
	const char *			program;
};

#ifdef ARGV_DRIVER

static const char * argv_program_name(const char *);

static void argv_usage(
	FILE *,
	const char *	header,
	const struct	argv_option[],
	const char *	mode);

static struct argv_meta * argv_get(
	const char **,
	const struct argv_option[],
	int flags);

static void argv_free(struct argv_meta *);




/*------------------------------------*/
/* implementation of static functions */
/*------------------------------------*/

static const struct argv_option * argv_short_option(
	const char *			ch,
	const struct argv_option	options[],
	struct argv_entry *		entry)
{
	const struct argv_option *	option;

	for (option=options; option->long_name || option->short_name; option++) {
		if (option->short_name == *ch) {
			entry->tag	= option->tag;
			entry->fopt	= true;
			return option;
		}
	}

	return 0;
}

static const struct argv_option * argv_long_option(
	const char *			ch,
	const struct argv_option	options[],
	struct argv_entry *		entry)
{
	const struct argv_option *	option;
	const char *			arg;
	size_t				len;

	for (option=options; option->long_name || option->short_name; option++) {
		len = option->long_name ? strlen(option->long_name) : 0;

		if (len && !(strncmp(option->long_name,ch,len))) {
			arg = ch + len;

			if (!*arg || (*arg == '=')) {
				entry->tag	= option->tag;
				entry->fopt	= true;
				return option;
			}
		}
	}

	return 0;
}

static inline bool is_short_option(const char * arg)
{
	return (arg[0]=='-') && arg[1] && (arg[1]!='-');
}

static inline bool is_long_option(const char * arg)
{
	return (arg[0]=='-') && (arg[1]=='-') && arg[2];
}

static inline bool is_last_option(const char * arg)
{
	return (arg[0]=='-') && (arg[1]=='-') && !arg[2];
}

static inline bool is_arg_in_paradigm(const char * arg, const char * paradigm)
{
	size_t		len;
	const char *	ch;

	for (ch=paradigm,len=strlen(arg); ch; ) {
		if (!strncmp(arg,ch,len)) {
			if (!*(ch += len))
				return true;
			else if (*ch == '|')
				return true;
		}

		if ((ch = strchr(ch,'|')))
			ch++;
	}

	return false;
}

static inline const struct argv_option * option_from_tag(
	const struct argv_option	options[],
	int				tag)
{
	const struct argv_option *	option;

	for (option=options; option->short_name || option->long_name; option++)
		if (option->tag == tag)
			return option;
	return 0;
}

static void argv_scan(
	const char **			argv,
	const struct argv_option	options[],
	struct argv_ctx *		ctx,
	struct argv_meta *		meta)
{
	const char **			parg;
	const char *			ch;
	const char *			val;
	const struct argv_option *	option;
	struct argv_entry		entry;
	struct argv_entry *		mentry;
	enum argv_error			ferror;
	bool				fval;
	bool				fnext;
	bool				fshort;
	bool				fnoscan;

	argv++;
	parg	= argv;
	ch	= *parg;
	ferror	= ARGV_ERROR_OK;
	fshort	= false;
	fnoscan	= false;
	fval	= false;
	mentry	= meta ? meta->entries : 0;

	while (ch && (ferror == ARGV_ERROR_OK)) {
		option = 0;

		if (fnoscan)
			fval = true;

		else if (is_last_option(ch))
			fnoscan = true;

		else if ((fshort || is_short_option(ch))) {
			if (!fshort)
				ch++;

			if ((option = argv_short_option(ch,options,&entry))) {
				if (ch[1]) {
					ch++;
					fnext	= false;
					fshort	= (option->optarg == ARGV_OPTARG_NONE);
				} else {
					parg++;
					ch	= *parg;
					fnext	= true;
					fshort	= false;
				}

				if (option->optarg == ARGV_OPTARG_NONE) {
					if (!fnext && ch && (*ch == '-'))
						ferror = ARGV_ERROR_OPTARG_NONE;
					else
						fval = false;
				} else if (!fnext)
					fval = true;
				else if (option->optarg == ARGV_OPTARG_REQUIRED) {
					if (ch && is_short_option(ch))
						ferror = ARGV_ERROR_OPTARG_REQUIRED;
					else if (ch && is_long_option(ch))
						ferror = ARGV_ERROR_OPTARG_REQUIRED;
					else if (ch && is_last_option(ch))
						ferror = ARGV_ERROR_OPTARG_REQUIRED;
					else if (ch)
						fval = true;
					else
						ferror = ARGV_ERROR_OPTARG_REQUIRED;
				} else {
					/* ARGV_OPTARG_OPTIONAL */
					if (ch && is_short_option(ch))
						fval = false;
					else if (ch && is_long_option(ch))
						fval = false;
					else if (ch && is_last_option(ch))
						fval = false;
					else
						fval = ch;
				}
			} else
				ferror = ARGV_ERROR_SHORT_OPTION;

		} else if ((is_long_option(ch))) {
			if ((option = argv_long_option(ch+=2,options,&entry))) {
				val = ch + strlen(option->long_name);

				/* val[0] is either '=' or '\0' */
				if (!val[0]) {
					parg++;
					ch = *parg;
				}

				if (option->optarg == ARGV_OPTARG_NONE) {
					if (val[0]) {
						ferror = ARGV_ERROR_OPTARG_NONE;
						ctx->errch = val + 1;
					} else
						fval = false;
				} else if (val[0] && !val[1])
					ferror = ARGV_ERROR_OPTARG_REQUIRED;
				else if (val[0] && val[1]) {
					fval = true;
					ch   = ++val;
				} else if (option->optarg == ARGV_OPTARG_REQUIRED) {
					if (!val[0] && !*parg)
						ferror = ARGV_ERROR_OPTARG_REQUIRED;
					else if (*parg && is_short_option(*parg))
						ferror = ARGV_ERROR_OPTARG_REQUIRED;
					else if (*parg && is_long_option(*parg))
						ferror = ARGV_ERROR_OPTARG_REQUIRED;
					else if (*parg && is_last_option(*parg))
						ferror = ARGV_ERROR_OPTARG_REQUIRED;
					else
						fval = true;
				} else {
					/* ARGV_OPTARG_OPTIONAL */
					if (!val[0] && !*parg)
						fval = false;
					if (*parg && is_short_option(*parg))
						fval = false;
					else if (*parg && is_long_option(*parg))
						fval = false;
					else if (*parg && is_last_option(*parg))
						fval = false;
					else
						fval = *parg;
				}
			} else
				ferror = ARGV_ERROR_LONG_OPTION;
		}

		if (ferror == ARGV_ERROR_OK)
			if (option && fval && option->paradigm)
				if (!is_arg_in_paradigm(ch,option->paradigm))
					ferror = ARGV_ERROR_OPTARG_PARADIGM;

		if (ferror != ARGV_ERROR_OK) {
			ctx->errcode = ferror;
			ctx->errch   = ctx->errch ? ctx->errch : ch;
			ctx->erropt  = option;
			return;
		} else if (ctx->mode == ARGV_MODE_SCAN) {
			if (!fnoscan)
				ctx->nentries++;
			else if (fval)
				ctx->nentries++;

			if (fval || !option) {
				parg++;
				ch = *parg;
			}
		} else if (ctx->mode == ARGV_MODE_COPY) {
			if (fnoscan) {
				if (fval) {
					mentry->arg	= ch;
					mentry->fnoscan = true;
					mentry++;
				}

				parg++;
				ch = *parg;
			} else if (option) {
				mentry->arg	= fval ? ch : 0;
				mentry->tag	= option->tag;
				mentry->fopt	= true;
				mentry->fval	= fval;
				mentry++;

				if (fval) {
					parg++;
					ch = *parg;
				}
			} else {
				mentry->arg = ch;
				mentry++;
				parg++;
				ch = *parg;
			}
		}
	}
}

static const char * argv_program_name(const char * program_path)
{
	const char * ch;

	if (program_path) {
		if ((ch = strrchr(program_path,'/')))
			return *(++ch) ? ch : 0;

		if ((ch = strrchr(program_path,'\\')))
			return *(++ch) ? ch : 0;
	}

	return program_path;
}

static void argv_show_error(struct argv_ctx * ctx)
{
	fprintf(stderr,"%s: error: ",ctx->program);

	switch (ctx->errcode) {
		case ARGV_ERROR_SHORT_OPTION:
			fprintf(stderr,"'%c' is not a valid short option\n",*ctx->errch);
			break;

		case ARGV_ERROR_LONG_OPTION:
			fprintf(stderr,"'--%s' is not a valid long option\n",ctx->errch);
			break;

		case ARGV_ERROR_OPTARG_NONE:
			fprintf(stderr,"'%s' is not a valid option value for [%s%c%s%s%s] (option values may not be specified)\n",
				ctx->errch,
				ctx->erropt->short_name ? "-" : "",
				ctx->erropt->short_name,
				ctx->erropt->short_name ? "," : "",
				ctx->erropt->long_name ? "--" : "",
				ctx->erropt->long_name);
			break;

		case ARGV_ERROR_OPTARG_REQUIRED:
			fprintf(stderr,"option [%s%c%s%s%s] requires %s %s%s%s\n",
				ctx->erropt->short_name ? "-" : "",
				ctx->erropt->short_name,
				ctx->erropt->short_name ? "," : "",
				ctx->erropt->long_name ? "--" : "",
				ctx->erropt->long_name,
				ctx->erropt->paradigm ? "one of the following values:" : "a value",
				ctx->erropt->paradigm ? "{" : "",
				ctx->erropt->paradigm ? ctx->erropt->paradigm : "",
				ctx->erropt->paradigm ? "}" : "");
			break;

		case ARGV_ERROR_OPTARG_PARADIGM:
			fprintf(stderr,"'%s' is not a valid option value for [%s%c%s%s%s]={%s}\n",
				ctx->errch,
				ctx->erropt->short_name ? "-" : "",
				ctx->erropt->short_name,
				ctx->erropt->short_name ? "," : "",
				ctx->erropt->long_name ? "--" : "",
				ctx->erropt->long_name,
				ctx->erropt->paradigm);
			break;

		case ARGV_ERROR_INTERNAL:
			fputs("internal error",stderr);
			break;

		default:
			break;
	}
}

static void argv_show_status(
	const struct argv_option	options[],
	struct argv_ctx *		ctx,
	struct argv_meta *		meta)
{
	int				argc;
	const char **			argv;
	struct argv_entry *		entry;
	const struct argv_option *	option;
	char				short_name[2] = {0};
	const char *			space = "";

	fputs("\n\nconcatenated command line:\n",stderr);
	for (argv=meta->argv; *argv; argv++) {
		fprintf(stderr,"%s%s",space,*argv);
		space = " ";
	}

	fputs("\n\nargument vector:\n",stderr);
	for (argc=0,argv=meta->argv; *argv; argc++,argv++)
		fprintf(stderr,"argv[%d]: %s\n",argc,*argv);

	fputs("\n\nparsed entries:\n",stderr);
	for (entry=meta->entries; entry->arg || entry->fopt; entry++)
		if (entry->fopt) {
			option = option_from_tag(options,entry->tag);
			short_name[0] = option->short_name;

			if (entry->fval)
				fprintf(stderr,"[-%s,--%s] := %s\n",
					short_name,option->long_name,entry->arg);
			else
				fprintf(stderr,"[-%s,--%s]\n",
					short_name,option->long_name);
		} else
			fprintf(stderr,"<program arg> := %s\n",entry->arg);

	fputs("\n\n",stderr);
}

static struct argv_meta * argv_free_impl(struct argv_meta_impl * imeta)
{
	if (imeta->argv)
		free(imeta->argv);

	if (imeta->strbuf)
		free(imeta->strbuf);

	if (imeta->meta.entries)
		free(imeta->meta.entries);

	free(imeta);
	return 0;
}

static struct argv_meta * argv_alloc(const char ** argv, struct argv_ctx * ctx)
{
	struct argv_meta_impl * imeta;
	const char **		vector;
	char *			dst;
	size_t			size;
	int			argc;
	int			i;

	if (!(imeta = calloc(sizeof(*imeta),1)))
		return 0;

	if (ctx->flags & ARGV_CLONE_VECTOR) {
		for (vector=argv,argc=0,size=0; *vector; vector++) {
			size += strlen(*vector) + 1;
			argc++;
		}

		if (!(imeta->argv = calloc(sizeof(char *),argc+1)))
			return argv_free_impl(imeta);
		else if (!(imeta->strbuf = calloc(size+1,1)))
			return argv_free_impl(imeta);

		for (i=0,dst=imeta->strbuf; i<argc; i++) {
			strcpy(dst,argv[i]);
			imeta->argv[i] = dst;
			dst += strlen(dst)+1;
		}

		imeta->meta.argv = imeta->argv;
	} else
		imeta->meta.argv = argv;

	if (!(imeta->meta.entries = calloc(sizeof(struct argv_entry),ctx->nentries+1)))
		return argv_free_impl(imeta);
	else
		return &imeta->meta;
}

static struct argv_meta * argv_get(
	const char *			argv[],
	const struct argv_option	options[],
	int				flags)
{
	struct argv_meta *	meta;
	struct argv_ctx		ctx = {flags,ARGV_MODE_SCAN,0,0,0,0};

	argv_scan(argv,options,&ctx,0);

	if (ctx.errcode != ARGV_ERROR_OK) {
		if (!ctx.program)
			ctx.program = argv_program_name(argv[0]);

		if (ctx.flags & ARGV_VERBOSITY_ERRORS)
			argv_show_error(&ctx);

		return 0;
	}

	if (!(meta = argv_alloc(argv,&ctx)))
		return 0;

	ctx.mode = ARGV_MODE_COPY;
	argv_scan(meta->argv,options,&ctx,meta);

	if (ctx.errcode != ARGV_ERROR_OK) {
		if (!ctx.program)
			ctx.program = argv[0];

		ctx.errcode = ARGV_ERROR_INTERNAL;
		argv_show_error(&ctx);
		argv_free(meta);

		return 0;
	}

	if (ctx.flags & ARGV_VERBOSITY_STATUS)
		argv_show_status(options,&ctx,meta);

	return meta;
}

static void argv_free(struct argv_meta * xmeta)
{
	struct argv_meta_impl * imeta;
	uintptr_t		addr;

	if (xmeta) {
		addr  = (uintptr_t)xmeta - offsetof(struct argv_meta_impl,meta);
		imeta = (struct argv_meta_impl *)addr;
		argv_free_impl(imeta);
	}
}

static void argv_usage(
	FILE *				file,
	const char *    		header,
	const struct argv_option	options[],
	const char *			mode)
{
	const struct argv_option *	option;
	bool				fshort,flong;
	bool				fnewline;
	size_t				len,optlen;
	size_t				paralen,rparalen,mparalen;
	size_t				desclen,rdesclen;

	char *				para;
	char *				next_para;
	char *				desc;
	char *				next_desc;
	char *				paradigm;
	char *				buf;
	size_t				buflen;
	const char *			sdescription;
	const char *			sargname;

	const char			indent[] = "  ";
	const int			rblen  = sizeof("}") - sizeof(char);
	const int			rbblen = sizeof("{]") - sizeof(char);
	const int			brcklen= sizeof("[]") - sizeof(char);
	const int			solen  = sizeof("-") - sizeof(char);
	const int			lolen  = sizeof("--") - sizeof(char);
	const int			slolen = sizeof("-X,--") - sizeof(char);

	fshort = mode ? !strcmp(mode,"short") : 0;
	flong  = fshort ? 0 : mode && !strcmp(mode,"long");

	if (header)
		fprintf(stdout,"%s",header);

	for (option=options,optlen=0,paralen=0; option->short_name || option->long_name; option++) {
		if (fshort)
			len = option->short_name ? sizeof(char) + solen : 0;
		else if (flong)
			len = option->long_name ? strlen(option->long_name) + lolen : 0;
		else
			len = option->long_name ? strlen(option->long_name) + slolen : 0;

		if (len) {
			if (len > optlen)
				optlen = len;

			if (option->paradigm)
				len = strlen(option->paradigm) + strlen("{}");
			else if (option->argname)
				len = strlen(option->argname);
			else if (option->optarg != ARGV_OPTARG_NONE)
				len = strlen("<val>");

			if (option->optarg == ARGV_OPTARG_OPTIONAL)
				len += strlen("[]");

			if (len > paralen)
				paralen = len;
		}
	}

	optlen += ARGV_TAB;
	optlen &= (~(ARGV_TAB-1));

	if (paralen) {
		paralen += (ARGV_TAB);
		paralen &= (~(ARGV_TAB-1));
		mparalen = paralen + 2*rbblen;

		if (optlen + paralen > 64)
			paralen = 32;
	}

	/* account for '  ','\t', try to fit in 80 or 96 columns */
	if (optlen+paralen+2+ARGV_TAB < 80-32)
		desclen = 80 - (optlen+paralen+2+ARGV_TAB);
	else if (optlen+paralen+2+ARGV_TAB < 96-32)
		desclen = 96 - (optlen+paralen+2+ARGV_TAB);
	else
		desclen = 32;

	paradigm = next_para = buf = 0;
	fnewline = false;
	rparalen = 0;
	mparalen = 0;

	for (option=options,buflen=0,rdesclen=1; option->short_name || option->long_name; option++) {
		if (option->paradigm) {
			if (option->optarg == ARGV_OPTARG_OPTIONAL)
				rparalen = strlen(option->paradigm) - 2*rbblen;
			else
				rparalen = strlen(option->paradigm) - 2*rblen;
		}

		sdescription 	= option->description ? option->description : "";
		sargname	= option->argname ? option->argname : "";

		if (option->paradigm)
			rdesclen = snprintf(buf,buflen,sdescription,option->paradigm);
		else
			rdesclen = snprintf(buf,buflen,sdescription,sargname);

		if (fnewline)
			(void)0;

		if ((rparalen > paralen) || (rdesclen > desclen)) {
			if (!fnewline) {
				(void)0;
				fnewline = true;
			}
		} else
			fnewline = false;

		if (fshort)
			fprintf(file,"%s-%-*c",indent,(int)(optlen-solen),option->short_name);
		else if (flong)
			fprintf(file,"%s--%-*s",indent,(int)(optlen-lolen),option->long_name);
		else {
			if (option->short_name && option->long_name)
				fprintf(file,"%s-%c,--%-*s",indent,option->short_name,(int)(optlen-slolen),option->long_name);
			else if (option->short_name)
				 fprintf(file,"%s-%-*c",indent,(int)(optlen-solen),option->short_name);
			else
				fprintf(file,"%s%3s--%-*s",indent,"",(int)(optlen-slolen),option->long_name);
		}

		if (rdesclen > buflen) {
			if (buf) {
				free(buf);
				buf = 0;
			}

			len =  rdesclen + 512;
			len &= (~511);

			if ((buf = calloc(len,1))) {
				buflen = len;

				if (option->paradigm)
					rdesclen = snprintf(buf,buflen,option->description,option->paradigm);
				else
					rdesclen = snprintf(buf,buflen,option->description,option->argname);
			} else {
				buflen = 0;
				continue;
			}
		}

		if (option->paradigm && (rparalen <= paralen)) {
			if (option->optarg == ARGV_OPTARG_OPTIONAL)
				fprintf(file,"[{%s}]%-*c",option->paradigm,(int)(paralen-strlen(option->paradigm)-2*rbblen),' ');
			else
				fprintf(file,"{%s}%-*c",option->paradigm,(int)(paralen-strlen(option->paradigm)-rbblen),' ');
			para = (char *)0;
		} else if (option->paradigm) {
			if (!paradigm && !(paradigm = calloc(mparalen,1))) {
				fputc('\n',file);
				continue;
			} else
				para = strcpy(paradigm,option->paradigm);

			if (option->optarg == ARGV_OPTARG_OPTIONAL) {
				fputs("[{",file);
				rparalen = paralen - rbblen;
			} else {
				fputc('{',file);
				rparalen = paralen - rblen;
			}
		} else if (option->argname) {
			if (option->optarg == ARGV_OPTARG_OPTIONAL)
				fprintf(file,"[%s]%-*c",option->argname,(int)(paralen-strlen(option->argname)-brcklen),' ');
			else
				fprintf(file,"%s%-*c",option->argname,(int)(paralen-strlen(option->argname)),' ');
			para = (char *)0;
		} else {
			fprintf(file,"%-*c",(int)paralen,' ');
			para = (char *)0;
		}


		if (!para && option->description && rdesclen <= desclen) {
			fputc('\t',file);
			fputs(buf,file);
			desc = (char *)0;
		} else if (option->description)
			desc = buf;
		else
			desc = (char *)0;

		while (para || desc) {
			if (para) {
				for (next_para=para+rparalen-1; (next_para>para) && (*next_para!='|'); )
					next_para--;

				if (para > paradigm) {
					if (option->optarg == ARGV_OPTARG_OPTIONAL)
						fputs("  ",file);
					else
						fputc(' ',file);
				}

				if (*next_para != '|') {
					fprintf(file,"%s",para);
					para = (char *)0;
				} else if (next_para > para) {
					*next_para = '\0';
					fprintf(file,"%-*s",(int)rparalen,para);
					*next_para = '|';
					para = next_para;
					rparalen = strlen(para);

					/* 2*rbblen,2*rblen, etc.: account for indentation */
					if (option->optarg == ARGV_OPTARG_OPTIONAL)
						rparalen = (rparalen+2*rbblen > paralen) ? paralen-rbblen : rparalen;
					else
						rparalen = (rparalen+2*rblen > paralen) ? paralen-rblen : rparalen;
				} else {
					if (option->optarg == ARGV_OPTARG_OPTIONAL)
						fprintf(file,"%s}]%-*c",para,(int)(paralen-strlen(para)-rbblen),' ');
					else
						fprintf(file,"%s}%-*c",para,(int)(paralen-strlen(para)-rblen),' ');
					para = (char *)0;
				}
			} else if (desc > buf)
				fprintf(file,"%-*c",(int)paralen,' ');

			if (desc) {
				if (desc > buf)
					fputs("\t ",file);
				else
					fputc('\t',file);

				if ((rdesclen = strlen(desc)+(desc>buf)) <= desclen) {
					fputs(desc,file);
					desc = (char *)0;
				} else {
					for (next_desc=desc+desclen-1; (next_desc>desc) && (*next_desc!=' ') && (*next_desc!='\n'); )
						next_desc--;

					if ((*next_desc != ' ') && (*next_desc!='\n')) {
						fputs(desc,file);
						desc = (char *)0;
					} else if (next_desc > desc) {
						*next_desc = '\0';
						fputs(desc,file);
						desc = ++next_desc;
					} else {
						fputs(desc,file);
						desc = (char *)0;
					}
				}
			}

			if (para || desc)
				fprintf(file,"\n%s%-*c",indent,(int)optlen,' ');
		}

		fputc('\n',file);
	}

	if (paradigm)
		free(paradigm);

	if (buf)
		free(buf);
}

#endif

#endif
