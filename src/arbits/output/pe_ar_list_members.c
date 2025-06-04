/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2025  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <time.h>
#include <locale.h>
#include <inttypes.h>
#include <perk/perk.h>
#include <perk/perk_output.h>
#include "perk_driver_impl.h"
#include "perk_dprintf_impl.h"
#include "perk_errinfo_impl.h"
#include "perk_ar_impl.h"

#define PERK_PRETTY_FLAGS       (PERK_PRETTY_YAML      \
	                         | PERK_PRETTY_POSIX    \
	                         | PERK_PRETTY_HEXDATA)

#define PPRIU64 "%"PRIu64

const char pe_ar_perm_strs[8][4] = {
	{'-','-','-','\0'},
	{'-','-','x','\0'},
	{'-','w','-','\0'},
	{'-','w','x','\0'},
	{'r','-','-','\0'},
	{'r','-','x','\0'},
	{'r','w','-','\0'},
	{'r','w','x','\0'}
};

static unsigned pe_ar_output_decimal_len_from_val(size_t val, unsigned min)
{
	unsigned ret;

	for (ret=0; val; ret++)
		val /= 10;

	return (ret > min) ? ret : min;
}

static void pe_ar_member_not_found(
	const struct pe_archive_meta *  meta,
	const char *                    name)
{
	const struct pe_driver_ctx *  dctx;
	int                           fderr;

	dctx = (pe_archive_meta_ictx(meta))->dctx;

	if ((fderr = pe_driver_fderr(dctx)) >= 0)
		pe_dprintf(
			fderr,
			"%s: error: member '%s' was not found in the current archive\n",
			dctx->program,name);
}

static struct ar_meta_member_info * pe_ar_get_member_info(
	const struct pe_archive_meta *  meta,
	const char *                    name)
{
	struct ar_meta_member_info **   memberp;
	const char *                    slash;

	if ((slash = strrchr(name,'/')))
		name = ++slash;

	for (memberp=meta->a_memberv; *memberp; memberp++)
		if (!strcmp(memberp[0]->ar_file_header.ar_member_name,name))
			return *memberp;

	pe_ar_member_not_found(meta,name);

	return 0;
}

static int pe_ar_member_in_members(
	struct ar_meta_member_info *    minfo,
	const char **                   members)
{
	const char ** pname;

	for (pname=members; *pname; pname++)
		if (!strcmp(*pname,minfo->ar_file_header.ar_member_name))
			return 1;

	return 0;
}

static int pe_ar_list_one_member_posix(
	int                             fdout,
	struct ar_meta_member_info *    minfo)
{
	return pe_dprintf(
		fdout,"%s\n",
		minfo->ar_file_header.ar_member_name);
}

static int pe_ar_list_one_member_posix_verbose(
	int                             fdout,
	struct ar_meta_member_info *    minfo,
	const char *                    fmtstr,
	locale_t                        arlocale)
{
	unsigned    ownerbits;
	unsigned    groupbits;
	unsigned    worldbits;
	time_t      artimeval;
	struct tm   artimeloc;
	char        artimestr[64] = {0};

	ownerbits = (minfo->ar_file_header.ar_file_mode & 0700) >> 6;
	groupbits = (minfo->ar_file_header.ar_file_mode & 0070) >> 3;
	worldbits = (minfo->ar_file_header.ar_file_mode & 0007);
	artimeval = minfo->ar_file_header.ar_time_date_stamp;

	if (localtime_r(&artimeval,&artimeloc))
		strftime_l(
			artimestr,sizeof(artimestr),
			"%b %e %H:%M %Y",&artimeloc,
			arlocale);

	return pe_dprintf(
		fdout,fmtstr,
		pe_ar_perm_strs[ownerbits],
		pe_ar_perm_strs[groupbits],
		pe_ar_perm_strs[worldbits],
		minfo->ar_file_header.ar_uid,
		minfo->ar_file_header.ar_gid,
		minfo->ar_object_size,
		artimestr,
		minfo->ar_file_header.ar_member_name);
}

static int pe_ar_list_members_posix(
	const struct pe_driver_ctx *    dctx,
	const struct pe_archive_meta *  meta,
	const struct pe_fd_ctx *        fdctx,
	const char **                   members)
{
	struct ar_meta_member_info **   memberp;
	struct ar_meta_member_info *    minfo;
	const char **                   pname;
	int                             fdout;
	size_t                          testval;
	size_t                          sizelen;
	size_t                          uidlen;
	size_t                          gidlen;
	locale_t                        arloc;
	char                            fmtstr[64];

	fdout = fdctx->fdout;
	arloc = 0;

	if (dctx->cctx->fmtflags & PERK_PRETTY_VERBOSE) {
		for (sizelen=0,memberp=meta->a_memberv; *memberp; memberp++)
			if ((testval = memberp[0]->ar_object_size) > sizelen)
				if (!members || pe_ar_member_in_members(*memberp,members))
					sizelen = testval;

		for (uidlen=0,memberp=meta->a_memberv; *memberp; memberp++)
			if ((testval = memberp[0]->ar_file_header.ar_uid) > uidlen)
				if (!members || pe_ar_member_in_members(*memberp,members))
					uidlen = testval;

		for (gidlen=0,memberp=meta->a_memberv; *memberp; memberp++)
			if ((testval = memberp[0]->ar_file_header.ar_gid) > gidlen)
				if (!members || pe_ar_member_in_members(*memberp,members))
					gidlen = testval;

		sizelen = pe_ar_output_decimal_len_from_val(sizelen,6);
		uidlen  = pe_ar_output_decimal_len_from_val(uidlen,1);
		gidlen  = pe_ar_output_decimal_len_from_val(gidlen,1);
		arloc   = newlocale(LC_ALL,setlocale(LC_ALL,0),0);

		sprintf(
			fmtstr,
			"%%s%%s%%s "
			"%%"   PPRIU64 "u"
			"/%%-" PPRIU64 "u "
			"%%"   PPRIU64 "u "
			"%%s "
			"%%s\n",
			uidlen,
			gidlen,
			sizelen);
	}

	for (pname=members; pname && *pname; pname++) {
		if ((minfo = pe_ar_get_member_info(meta,*pname))) {
			switch (minfo->ar_member_attr) {
				case AR_MEMBER_ATTR_ARMAP:
				case AR_MEMBER_ATTR_LINKINFO:
				case AR_MEMBER_ATTR_NAMESTRS:
					break;

				default:
					if (arloc) {
						if (pe_ar_list_one_member_posix_verbose(
								fdout,minfo,fmtstr,arloc) < 0)
							return PERK_SYSTEM_ERROR(dctx);
					} else {
						if (pe_ar_list_one_member_posix(
								fdout,minfo) < 0)
							return PERK_SYSTEM_ERROR(dctx);
					}
			}
		}
	}

	for (memberp=meta->a_memberv; !members && *memberp; memberp++) {
		switch ((*memberp)->ar_member_attr) {
			case AR_MEMBER_ATTR_ARMAP:
			case AR_MEMBER_ATTR_LINKINFO:
			case AR_MEMBER_ATTR_NAMESTRS:
				break;

			default:
				if (arloc) {
					if (pe_ar_list_one_member_posix_verbose(
							fdout,*memberp,fmtstr,arloc) < 0)
						return PERK_SYSTEM_ERROR(dctx);
				} else {
					if (pe_ar_list_one_member_posix(
							fdout,*memberp) < 0)
						return PERK_SYSTEM_ERROR(dctx);
				}
		}
	}

	if (arloc)
		freelocale(arloc);

	return 0;
}

static int pe_ar_list_one_member_yaml(
	int                             fdout,
	struct ar_meta_member_info *    minfo)
{
	return pe_dprintf(
		fdout,
		"    - [ member: %s ]\n",
		minfo->ar_file_header.ar_member_name);
}

static int pe_ar_list_one_member_yaml_verbose(
	int                             fdout,
	struct ar_meta_member_info *    minfo,
	locale_t                        arlocale)
{
	time_t      artimeval;
	struct tm   artimeloc;
	char        artimestr[64] = {0};

	artimeval = minfo->ar_file_header.ar_time_date_stamp;

	if (localtime_r(&artimeval,&artimeloc))
		strftime_l(
			artimestr,sizeof(artimestr),
			"%Y/%m/%d @ %H:%M",&artimeloc,
			arlocale);

	return pe_dprintf(
		fdout,
		"    - Member:\n"
		"      - [ name:       " "%s"    " ]\n"
		"      - [ timestamp:  " "%s"    " ]\n"
		"      - [ filesize:   " PPRIU64 " ]\n"
		"      - [ uid:        " "%d"    " ]\n"
		"      - [ gid:        " "%d"    " ]\n"
		"      - [ mode:       " "%d"    " ]\n\n",
		minfo->ar_file_header.ar_member_name,
		artimestr,
		minfo->ar_object_size,
		minfo->ar_file_header.ar_uid,
		minfo->ar_file_header.ar_gid,
		minfo->ar_file_header.ar_file_mode);
}

static int pe_ar_list_members_yaml(
	const struct pe_driver_ctx *    dctx,
	const struct pe_archive_meta *  meta,
	const struct pe_fd_ctx *        fdctx,
	const char **                   members)
{
	struct ar_meta_member_info **   memberp;
	struct ar_meta_member_info *    minfo;
	const char **                   pname;
	int                             fdout;
	locale_t                        arloc;

	fdout = fdctx->fdout;
	arloc = 0;

	if (dctx->cctx->fmtflags & PERK_PRETTY_VERBOSE) {
		arloc   = newlocale(LC_ALL,setlocale(LC_ALL,0),0);
	}

	if (pe_dprintf(fdctx->fdout,"  - Members:\n") < 0)
		return PERK_SYSTEM_ERROR(dctx);

	for (pname=members; pname && *pname; pname++) {
		if ((minfo = pe_ar_get_member_info(meta,*pname))) {
			switch (minfo->ar_member_attr) {
				case AR_MEMBER_ATTR_ARMAP:
				case AR_MEMBER_ATTR_LINKINFO:
				case AR_MEMBER_ATTR_NAMESTRS:
					break;

				default:
					if (arloc) {
						if (pe_ar_list_one_member_yaml_verbose(
								fdout,minfo,arloc) < 0)
							return PERK_SYSTEM_ERROR(dctx);
					} else {
						if (pe_ar_list_one_member_yaml(
								fdout,minfo) < 0)
							return PERK_SYSTEM_ERROR(dctx);
					}
			}
		}
	}

	for (memberp=meta->a_memberv; !members && *memberp; memberp++) {
		switch ((*memberp)->ar_member_attr) {
			case AR_MEMBER_ATTR_ARMAP:
			case AR_MEMBER_ATTR_LINKINFO:
			case AR_MEMBER_ATTR_NAMESTRS:
				break;

			default:
				if (arloc) {
					if (pe_ar_list_one_member_yaml_verbose(
							fdout,*memberp,arloc) < 0)
						return PERK_SYSTEM_ERROR(dctx);
				} else {
					if (pe_ar_list_one_member_yaml(
							fdout,*memberp) < 0)
						return PERK_SYSTEM_ERROR(dctx);
				}
		}
	}

	if (arloc)
		freelocale(arloc);

	return 0;
}

int pe_ar_list_members(const struct pe_archive_meta * meta, const char ** members)
{
	const struct pe_driver_ctx *  dctx;
	struct pe_fd_ctx              fdctx;

	const char ** pmember;

	dctx = (pe_archive_meta_ictx(meta))->dctx;

	if (pe_lib_get_driver_fdctx(dctx,&fdctx) < 0)
		return PERK_NESTED_ERROR(dctx);

	if (members && !members[0])
		members = 0;

	if (!meta->a_memberv) {
		for (pmember=members; pmember && *pmember; pmember++)
			pe_ar_member_not_found(meta,*pmember);

		return members ? 1 : 0;
	}

	switch (dctx->cctx->fmtflags & PERK_PRETTY_FLAGS) {
		case PERK_PRETTY_YAML:
			return pe_ar_list_members_yaml(
				dctx,meta,&fdctx,members);

		case PERK_PRETTY_POSIX:
			return pe_ar_list_members_posix(
				dctx,meta,&fdctx,members);

		default:
			return pe_ar_list_members_posix(
				dctx,meta,&fdctx,members);
	}
}
