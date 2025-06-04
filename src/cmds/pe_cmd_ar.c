/***************************************************************/
/*  perk: PE Resource Kit                                      */
/*  Copyright (C) 2015--2025  SysDeer Technologies, LLC        */
/*  Released under GPLv2 and GPLv3; see COPYING.PERK.          */
/***************************************************************/

#include <perk/perk.h>
#include "perk_driver_impl.h"
#include "perk_errinfo_impl.h"
#include "perk_ar_impl.h"

int pe_ar_list_members(const struct pe_archive_meta *, const char **);

static int pe_cmd_ar_perform_unit_actions(
	const struct pe_driver_ctx * dctx,
	const char *                 arname,
	const char **                members,
	uint64_t                     action)
{
	struct pe_unit_ctx * arctx = 0;
	int  (*pe_ar_fn)(const struct pe_archive_meta *,const char **);

	if (action == PERK_DRIVER_AR_LIST_MEMBERS) {
		pe_ar_fn = pe_ar_list_members;

	} else if (action == PERK_DRIVER_AR_PRINT_MEMBERS) {
		pe_ar_fn = pe_ar_print_members;

	} else {
		return 0;
	}

	if (pe_lib_get_unit_ctx(dctx,arname,&arctx) < 0)
		return PERK_NESTED_ERROR(dctx);

	if (arctx->armeta == 0)
		return PERK_CUSTOM_ERROR(dctx,
			PERK_ERR_AR_NON_ARCHIVE_IMAGE);

	if (pe_ar_fn(arctx->armeta,members) < 0) {
		pe_lib_free_unit_ctx(arctx);
		return PERK_NESTED_ERROR(dctx);
	}

	pe_lib_free_unit_ctx(arctx);

	return 0;
}

static int pe_cmd_ar_verify_cmdline(
	const struct pe_driver_ctx * dctx,
	uint64_t                     flags,
	const char *                 posname,
	const char *                 arname,
	const char **                members)
{
	uint64_t action;
	uint64_t poscmd;
	uint64_t vercmd;

	action = (flags & AR_ACTION_MASK);
	poscmd = (flags & AR_POSNAME_MASK);
	vercmd = (flags & PERK_DRIVER_VERSION);

	if (vercmd && !posname && !arname && !members)
		return 0;

	switch (action) {
		case 0:
			return PERK_CUSTOM_ERROR(dctx,
				PERK_ERR_AR_MISSING_ACTION);

		case PERK_DRIVER_AR_LIST_MEMBERS:
		case PERK_DRIVER_AR_DELETE_MEMBERS:
		case PERK_DRIVER_AR_APPEND_MEMBERS:
		case PERK_DRIVER_AR_EXTRACT_MEMBERS:
		case PERK_DRIVER_AR_PRINT_MEMBERS:
			if (poscmd || posname)
				return PERK_CUSTOM_ERROR(dctx,
					PERK_ERR_AR_INVALID_ANCHORS);

			break;

		case AR_UPDATE_MASK:
		case PERK_DRIVER_AR_REPLACE_MEMBERS:
		case PERK_DRIVER_AR_MOVE_MEMBERS:
			switch (poscmd) {
				case 0:
					if (posname)
						return PERK_CUSTOM_ERROR(dctx,
							PERK_ERR_AR_MISSING_ANCHOR);
					break;

				case PERK_DRIVER_AR_POSITION_AFTER:
				case PERK_DRIVER_AR_POSITION_BEFORE:
					if (!posname)
						return PERK_CUSTOM_ERROR(dctx,
							PERK_ERR_AR_NULL_POSNAME);
					break;

				default:
					return PERK_CUSTOM_ERROR(dctx,
						PERK_ERR_AR_MULTIPLE_ANCHORS);
			}

		default:
			return PERK_CUSTOM_ERROR(dctx,
				PERK_ERR_AR_MULTIPLE_ACTIONS);
	}

	if (!arname)
		return PERK_CUSTOM_ERROR(dctx,
			PERK_ERR_AR_NULL_ARNAME);

	return 0;
}

int pe_cmd_ar(
	const struct pe_driver_ctx * dctx,
	uint64_t                     flags,
	const char *                 posname,
	const char *                 arname,
	const char **                members)
{
	uint64_t action = (flags & AR_ACTION_MASK);

	if (pe_cmd_ar_verify_cmdline(dctx,flags,posname,arname,members) < 0)
		return PERK_NESTED_ERROR(dctx);

	if (pe_cmd_ar_perform_unit_actions(dctx,arname,members,action) < 0)
		return PERK_NESTED_ERROR(dctx);

	return 0;
}
