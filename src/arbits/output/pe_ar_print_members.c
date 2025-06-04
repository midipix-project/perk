/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2025  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <perk/perk.h>
#include <perk/perk_output.h>
#include "perk_driver_impl.h"
#include "perk_dprintf_impl.h"
#include "perk_errinfo_impl.h"
#include "perk_ar_impl.h"

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

static int pe_ar_print_one_member_posix(
	int                             fdout,
	const struct pe_driver_ctx *    dctx,
	struct ar_meta_member_info *    minfo)
{
	ssize_t      ret;
	size_t       cnt;
	const char * ch;

	ret = 0;
	cnt = minfo->ar_file_header.ar_file_size;
	ch  = minfo->ar_object_data;

	for (; cnt && ret>=0; ) {
		ch  += ret;
		cnt -= ret;

		ret = write(fdout,ch,cnt);

		while ((ret < 0) && (errno == EINTR))
			ret = write(fdout,ch,cnt);
	}

	if (ret < 0)
		return PERK_SYSTEM_ERROR(dctx);

	return 0;
}

static int pe_ar_print_one_member_posix_verbose(
	int                             fdout,
	const struct pe_driver_ctx *    dctx,
	struct ar_meta_member_info *    minfo,
	const char *                    aname)
{
	const char * name = aname ? aname : minfo->ar_file_header.ar_member_name;

	if (pe_dprintf(fdout,"\n<%s>\n\n",name) < 0)
		return PERK_SYSTEM_ERROR(dctx);

	if (pe_ar_print_one_member_posix(fdout,dctx,minfo) < 0)
		return PERK_NESTED_ERROR(dctx);

	return 0;
}

static int pe_ar_print_members_posix(
	const struct pe_driver_ctx *    dctx,
	const struct pe_archive_meta *  meta,
	const struct pe_fd_ctx *        fdctx,
	const char **                   members)
{
	struct ar_meta_member_info **   memberp;
	struct ar_meta_member_info *    minfo;
	const char **                   pname;
	int                             fdout;
	bool                            fname;

	fdout = fdctx->fdout;
	fname = dctx->cctx->fmtflags & PERK_PRETTY_VERBOSE;

	for (pname=members; pname && *pname; pname++) {
		if ((minfo = pe_ar_get_member_info(meta,*pname))) {
			switch (minfo->ar_member_attr) {
				case AR_MEMBER_ATTR_ARMAP:
				case AR_MEMBER_ATTR_LINKINFO:
				case AR_MEMBER_ATTR_NAMESTRS:
					break;

				default:
					if (fname) {
						if (pe_ar_print_one_member_posix_verbose(
								fdout,dctx,minfo,*pname) < 0)
							return PERK_NESTED_ERROR(dctx);
					} else {
						if (pe_ar_print_one_member_posix(
								fdout,dctx,minfo) < 0)
							return PERK_NESTED_ERROR(dctx);
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
				if (fname) {
					if (pe_ar_print_one_member_posix_verbose(
							fdout,dctx,*memberp,0) < 0)
						return PERK_NESTED_ERROR(dctx);
				} else {
					if (pe_ar_print_one_member_posix(
							fdout,dctx,*memberp) < 0)
						return PERK_NESTED_ERROR(dctx);
				}
		}
	}

	return 0;
}

int pe_ar_print_members(const struct pe_archive_meta * meta, const char ** members)
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

	if (pe_ar_print_members_posix(dctx,meta,&fdctx,members) < 0)
		return PERK_NESTED_ERROR(dctx);

	return 0;
}
