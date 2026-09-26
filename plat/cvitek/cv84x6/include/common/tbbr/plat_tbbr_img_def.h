/*
 * Copyright (c) 2015-2020, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PLAT_TBBR_IMG_DEF_H
#define PLAT_TBBR_IMG_DEF_H

/* Standard TF-A image IDs (defined here because tbbr_img_def_exp.h may not
 * resolve during SDK builds via export subdirectory include path) */
#define BL31_IMAGE_ID			U(3)
#define BL32_IMAGE_ID			U(4)
#define BL33_IMAGE_ID			U(5)

/* CV private image IDs (match 84x6_release/fsbl values) */
#define BLD_IMAGE_ID			U(2)
#define CV_BLPARAM_IMAGE_ID		U(22)
#define CV_DDRC_IMAGE_ID		U(23)
#define CV_TRUSTED_KEY_CERT_ID		U(30)
#define CV_NON_TRUSTED_KEY_CERT_ID	U(31)
#define CV_LICENSE_FILE_IMAGE_ID	U(40)

#endif /* PLAT_TBBR_IMG_DEF_H */