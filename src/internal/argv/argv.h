/****************************************************************************/
/*  argv.h: a thread-safe argument vector parser and usage screen generator */
/*  Copyright (C) 2015--2016  Z. Gilboa                                     */
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
#include <unistd.h>

#define ARGV_VERBOSITY_NONE		0x00
#define ARGV_VERBOSITY_ERRORS		0x01
#define ARGV_VERBOSITY_STATUS		0x02
#define ARGV_CLONE_VECTOR		0x80

#ifndef ARGV_TAB_WIDTH
#define ARGV_TAB_WIDTH			8
#endif

/*******************************************/
/*                                         */
/* support of hybrid options               */
/* -------------------------               */
/* hybrid options are very similar to      */
/* long options, yet are prefixed by       */
/* a single dash rather than two           */
/* (i.e. -std, -isystem).                  */
/* hybrid options are supported by this    */
/* driver for compatibility with legacy    */
/* tools; note, however, that the use      */
/* of hybrid options should be strongly    */
/* discouraged due to the limitations      */
/* they impose on short options (for       */
/* example, a driver implementing -std     */
/* may not provide -s as a short option    */
/* that takes an arbitrary value).         */
/*                                         */
/* SPACE: -hybrid VALUE (i.e. -MF file)    */
/* EQUAL: -hybrid=VALUE (i.e. -std=c99)    */
/* COMMA: -hybrid,VALUE (i.e. -Wl,<arg>)   */
/* ONLY:  -opt accepted, --opt rejected    */
/* JOINED: -optVALUE                       */
/*                                         */
/*******************************************/


#define ARGV_OPTION_HYBRID_NONE		0x00
#define ARGV_OPTION_HYBRID_ONLY		0x01
#define ARGV_OPTION_HYBRID_SPACE	0x02
#define ARGV_OPTION_HYBRID_EQUAL	0x04
#define ARGV_OPTION_HYBRID_COMMA	0x08
#define ARGV_OPTION_HYBRID_JOINED	0x10
#define ARGV_OPTION_HYBRID_CIRCUS	(ARGV_OPTION_HYBRID_SPACE \
					| ARGV_OPTION_HYBRID_JOINED)
#define ARGV_OPTION_HYBRID_DUAL		(ARGV_OPTION_HYBRID_SPACE \
					| ARGV_OPTION_HYBRID_EQUAL)
#define ARGV_OPTION_HYBRID_SWITCH	(ARGV_OPTION_HYBRID_ONLY \
					| ARGV_OPTION_HYBRID_SPACE \
					| ARGV_OPTION_HYBRID_EQUAL \
					| ARGV_OPTION_HYBRID_COMMA \
					| ARGV_OPTION_HYBRID_JOINED)

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
	ARGV_ERROR_HYBRID_NONE,
	ARGV_ERROR_HYBRID_ONLY,
	ARGV_ERROR_HYBRID_SPACE,
	ARGV_ERROR_HYBRID_EQUAL,
	ARGV_ERROR_HYBRID_COMMA,
};

struct argv_option {
	const char *		long_name;
	const char		short_name;
	int			tag;
	enum argv_optarg	optarg;
	int			flags;
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
	char **			argv;
	struct argv_entry *	entries;
};

struct argv_meta_impl {
	char **			argv;
	char *			strbuf;
	struct argv_meta	meta;
};

struct argv_ctx {
	int				flags;
	int				mode;
	int				nentries;
	int				unitidx;
	int				erridx;
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
	char **,
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

			if (!*arg
				|| (*arg == '=')
				|| (option->flags & ARGV_OPTION_HYBRID_JOINED)
				|| ((option->flags & ARGV_OPTION_HYBRID_COMMA)
					&& (*arg == ','))) {
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

static inline bool is_hybrid_option(
	const char *			arg,
	const struct argv_option	options[])
{
	const struct argv_option *	option;
	struct argv_entry		entry;

	if (!is_short_option(arg))
		return false;

	if (!(option = argv_long_option(++arg,options,&entry)))
		return false;

	if (!(option->flags & ARGV_OPTION_HYBRID_SWITCH))
		if (argv_short_option(arg,options,&entry))
			return false;

	return true;
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
	char **				argv,
	const struct argv_option	options[],
	struct argv_ctx *		ctx,
	struct argv_meta *		meta)
{
	char **				parg;
	const char *			ch;
	const char *			val;
	const struct argv_option *	option;
	struct argv_entry		entry;
	struct argv_entry *		mentry;
	enum argv_error			ferr;
	bool				fval;
	bool				fnext;
	bool				fshort;
	bool				fhybrid;
	bool				fnoscan;

	parg	= &argv[1];
	ch	= *parg;
	ferr	= ARGV_ERROR_OK;
	fshort	= false;
	fnoscan	= false;
	fval	= false;
	mentry	= meta ? meta->entries : 0;

	ctx->unitidx = 0;
	ctx->erridx  = 0;

	while (ch && (ferr == ARGV_ERROR_OK)) {
		option  = 0;
		fhybrid = false;

		if (fnoscan)
			fval = true;

		else if (is_last_option(ch))
			fnoscan = true;

		else if (!fshort && is_hybrid_option(ch,options))
			fhybrid = true;

		if (!fnoscan && !fhybrid && (fshort || is_short_option(ch))) {
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
						ferr = ARGV_ERROR_OPTARG_NONE;
					else
						fval = false;
				} else if (!fnext)
					fval = true;
				else if (option->optarg == ARGV_OPTARG_REQUIRED) {
					if (ch && is_short_option(ch))
						ferr = ARGV_ERROR_OPTARG_REQUIRED;
					else if (ch && is_long_option(ch))
						ferr = ARGV_ERROR_OPTARG_REQUIRED;
					else if (ch && is_last_option(ch))
						ferr = ARGV_ERROR_OPTARG_REQUIRED;
					else if (ch)
						fval = true;
					else
						ferr = ARGV_ERROR_OPTARG_REQUIRED;
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
				ferr = ARGV_ERROR_SHORT_OPTION;

		} else if (!fnoscan && (fhybrid || is_long_option(ch))) {
			ch += (fhybrid ? 1 : 2);

			if ((option = argv_long_option(ch,options,&entry))) {
				val = ch + strlen(option->long_name);

				/* val[0] is either '=' (or ',') or '\0' */
				if (!val[0]) {
					parg++;
					ch = *parg;
				}

				if (fhybrid && !(option->flags & ARGV_OPTION_HYBRID_SWITCH))
					ferr = ARGV_ERROR_HYBRID_NONE;
				else if (!fhybrid && (option->flags & ARGV_OPTION_HYBRID_ONLY))
					ferr = ARGV_ERROR_HYBRID_ONLY;
				else if (option->optarg == ARGV_OPTARG_NONE) {
					if (val[0]) {
						ferr = ARGV_ERROR_OPTARG_NONE;
						ctx->errch = val + 1;
					} else
						fval = false;
				} else if (val[0] && (option->flags & ARGV_OPTION_HYBRID_JOINED)) {
					fval = true;
					ch   = val;
				} else if (fhybrid && !val[0] && !(option->flags & ARGV_OPTION_HYBRID_SPACE))
					ferr = ARGV_ERROR_HYBRID_SPACE;
				else if (fhybrid && (val[0]=='=') && !(option->flags & ARGV_OPTION_HYBRID_EQUAL))
					ferr = ARGV_ERROR_HYBRID_EQUAL;
				else if (fhybrid && (val[0]==',') && !(option->flags & ARGV_OPTION_HYBRID_COMMA))
					ferr = ARGV_ERROR_HYBRID_COMMA;
				else if (!fhybrid && (val[0]==','))
					ferr = ARGV_ERROR_HYBRID_COMMA;
				else if (val[0] && !val[1])
					ferr = ARGV_ERROR_OPTARG_REQUIRED;
				else if (val[0] && val[1]) {
					fval = true;
					ch   = ++val;
				} else if (option->optarg == ARGV_OPTARG_REQUIRED) {
					if (!val[0] && !*parg)
						ferr = ARGV_ERROR_OPTARG_REQUIRED;
					else if (*parg && is_short_option(*parg))
						ferr = ARGV_ERROR_OPTARG_REQUIRED;
					else if (*parg && is_long_option(*parg))
						ferr = ARGV_ERROR_OPTARG_REQUIRED;
					else if (*parg && is_last_option(*parg))
						ferr = ARGV_ERROR_OPTARG_REQUIRED;
					else
						fval = true;
				} else {
					/* ARGV_OPTARG_OPTIONAL */
					fval = val[0];
				}
			} else
				ferr = ARGV_ERROR_LONG_OPTION;
		}

		if (ferr == ARGV_ERROR_OK)
			if (option && fval && option->paradigm)
				if (!is_arg_in_paradigm(ch,option->paradigm))
					ferr = ARGV_ERROR_OPTARG_PARADIGM;

		if (ferr == ARGV_ERROR_OK)
			if (!option && !ctx->unitidx)
				ctx->unitidx = parg - argv;

		if (ferr != ARGV_ERROR_OK) {
			ctx->errcode = ferr;
			ctx->errch   = ctx->errch ? ctx->errch : ch;
			ctx->erropt  = option;
			ctx->erridx  = parg - argv;
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
			fprintf(stderr,"'-%c' is not a valid short option\n",*ctx->errch);
			break;

		case ARGV_ERROR_LONG_OPTION:
			fprintf(stderr,"'--%s' is not a valid long option\n",ctx->errch);
			break;

		case ARGV_ERROR_OPTARG_NONE:
			fprintf(stderr,"'%s' is not a valid option value for [%s%c%s%s%s] "
					"(option values may not be specified)\n",
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

		case ARGV_ERROR_HYBRID_NONE:
			fprintf(stderr,"-%s is not a synonym for --%s\n",
				ctx->erropt->long_name,
				ctx->erropt->long_name);
			break;

		case ARGV_ERROR_HYBRID_ONLY:
			fprintf(stderr,"--%s is not a synonym for -%s\n",
				ctx->erropt->long_name,
				ctx->erropt->long_name);
			break;

		case ARGV_ERROR_HYBRID_SPACE:
		case ARGV_ERROR_HYBRID_EQUAL:
		case ARGV_ERROR_HYBRID_COMMA:
			fprintf(stderr,"-%s: illegal value assignment; valid syntax is "
					"-%s%sVAL\n",
				ctx->erropt->long_name,
				ctx->erropt->long_name,
				(ctx->erropt->flags & ARGV_OPTION_HYBRID_SPACE)
					? " " : (ctx->erropt->flags & ARGV_OPTION_HYBRID_EQUAL)
					? "=" : (ctx->erropt->flags & ARGV_OPTION_HYBRID_COMMA)
					? "," : "");

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
	char **				argv;
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

static struct argv_meta * argv_alloc(char ** argv, struct argv_ctx * ctx)
{
	struct argv_meta_impl * imeta;
	char **			vector;
	char *			dst;
	size_t			size;
	int			argc;
	int			i;

	if (!(imeta = calloc(1,sizeof(*imeta))))
		return 0;

	if (ctx->flags & ARGV_CLONE_VECTOR) {
		for (vector=argv,argc=0,size=0; *vector; vector++) {
			size += strlen(*vector) + 1;
			argc++;
		}

		if (!(imeta->argv = calloc(argc+1,sizeof(char *))))
			return argv_free_impl(imeta);
		else if (!(imeta->strbuf = calloc(1,size+1)))
			return argv_free_impl(imeta);

		for (i=0,dst=imeta->strbuf; i<argc; i++) {
			strcpy(dst,argv[i]);
			imeta->argv[i] = dst;
			dst += strlen(dst)+1;
		}

		imeta->meta.argv = imeta->argv;
	} else
		imeta->meta.argv = argv;

	if (!(imeta->meta.entries = calloc(
				ctx->nentries+1,
				sizeof(struct argv_entry))))
		return argv_free_impl(imeta);
	else
		return &imeta->meta;
}

static struct argv_meta * argv_get(
	char *				argv[],
	const struct argv_option	options[],
	int				flags)
{
	struct argv_meta *	meta;
	struct argv_ctx		ctx = {flags,ARGV_MODE_SCAN,0,0,0,0,0,0,0};

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
	bool				fshort,flong,fboth;
	size_t				len,optlen,desclen;
	char				cache;
	char *				prefix;
	char *				desc;
	char *				mark;
	char *				cap;
	char				description[4096];
	char				optstr     [72];
	const size_t			optcap    = 64;
	const size_t			width     = 80;
	const char			indent[]  = "  ";
	const char			creset[]  = "\x1b[0m";
	const char			cbold []  = "\x1b[1m";
	const char			cgreen[]  = "\x1b[32m";
	const char			cblue []  = "\x1b[34m";
	const char			ccyan []  = "\x1b[36m";
	const char *			color     = ccyan;
	bool				fcolor;

	fshort = mode ? !strcmp(mode,"short") : 0;
	flong  = fshort ? 0 : mode && !strcmp(mode,"long");
	fboth  = !fshort && !flong;
	fcolor = isatty(STDOUT_FILENO);

	if (fcolor)
		fprintf(stdout,"%s%s",cbold,cgreen);

	if (header)
		fprintf(stdout,"%s",header);

	option = options;
	optlen = 0;

	for (; option->short_name || option->long_name; option++) {
		/* indent + comma */
		len = fboth ? sizeof(indent) + 1 : sizeof(indent);

		/* -o */
		if (fshort || fboth)
			len += option->short_name
				? 2 : 0;

		/* --option */
		if (flong || fboth)
			len += option->long_name
				? 2 + strlen(option->long_name) : 0;

		/* optlen */
		if (len > optlen)
			optlen = len;
	}

	if (optlen >= optcap) {
		fprintf(stderr,
			"Option strings exceed %zu characters, "
			"please generate the usage screen manually.\n",
			optcap);
		return;
	}

	optlen += ARGV_TAB_WIDTH;
	optlen &= (~(ARGV_TAB_WIDTH-1));
	desclen = (optlen < width / 2) ? width - optlen : optlen;

	for (option=options; option->short_name || option->long_name; option++) {
		/* color */
		if (fcolor) {
			color = (color == ccyan) ? cblue : ccyan;
			fputs(color,stdout);
		}

		/* description, using either paradigm or argname if applicable */
		snprintf(description,sizeof(description),option->description,
			option->paradigm
				? option->paradigm
				: option->argname ? option->argname : "");
		description[sizeof(description)-1] = 0;

		/* long/hybrid option prefix (-/--) */
		prefix = option->flags & ARGV_OPTION_HYBRID_ONLY
				? " -" : "--";

		/* option string */
		if (fboth && option->short_name && option->long_name)
			sprintf(optstr,"%s-%c,%s%s",
				indent,option->short_name,prefix,option->long_name);

		else if ((fshort || fboth) && option->short_name)
			sprintf(optstr,"%s-%d",indent,option->short_name);

		else if (flong && option->long_name)
			sprintf(optstr,"%s%s%s",
				indent,prefix,option->long_name);

		else if (fboth && option->long_name)
			sprintf(optstr,"%s   %s%s",
				indent,prefix,option->long_name);

		else
			optstr[0] = 0;

		/* right-indented option buffer */
		if (description[0]) {
			len = strlen(optstr);
			sprintf(&optstr[len],"%-*c",(int)(optlen-len),' ');
		}

		/* single line? */
		if (optlen + strlen(description) < width) {
			fprintf(stdout,"%s%s\n",optstr,description);

		} else {
			desc = description;
			cap  = desc + strlen(description);

			while (desc < cap) {
				mark = (desc + desclen >= cap)
					? cap : desc + desclen;

				while (*mark && (mark > desc)
						&& (*mark != ' ')
						&& (*mark != '\t')
						&& (*mark != '\n'))
					mark--;

				if (mark == desc) {
					mark = (desc + desclen >= cap)
						? cap : desc + desclen;
					cache = *mark;
					*mark = 0;
				} else {
					cache = 0;
					*mark = 0;
				}

				/* first line? */
				if (desc == description)
					fprintf(stdout,"%s%s\n",optstr,desc);
				else
					fprintf(stdout,"%-*c %s\n",(int)optlen,' ',desc);

				if (cache)
					*mark = cache;
				else
					mark++;

				desc = mark;
			}
		}
	}

	if (fcolor)
		fputs(creset,stdout);
}

#endif

#endif
