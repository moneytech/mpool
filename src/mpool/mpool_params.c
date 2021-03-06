// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2015-2020 Micron Technology, Inc.  All rights reserved.
 */

#include <util/inttypes.h>

#include <mpool/mpool_ioctl.h>

#include <string.h>

void mpool_params_init(struct mpool_params *params)
{
	memset(params, 0, sizeof(*params));

	params->mp_uid      =  MPOOL_UID_INVALID;
	params->mp_gid      =  MPOOL_GID_INVALID;
	params->mp_mode     =  MPOOL_MODE_INVALID;
	params->mp_spare_cap =  MPOOL_SPARES_INVALID;
	params->mp_spare_stg =  MPOOL_SPARES_INVALID;
	params->mp_mclassp  =  MPOOL_MCLASS_INVALID;
	params->mp_ra_pages_max = MPOOL_RA_PAGES_INVALID;
	params->mp_mdc0cap = 0;
	params->mp_mdcncap = 0;
	params->mp_mdcnum  = 0;
	strcpy(params->mp_label, MPOOL_LABEL_INVALID);
}
