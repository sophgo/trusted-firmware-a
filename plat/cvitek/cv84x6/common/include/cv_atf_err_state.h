/*
 * Legacy ATF state/error codes used by cv84x6 BL2/BL31 platform C code.
 * Kept separate from upstream <debug.h> to avoid include shadowing.
 */
#ifndef CV_ATF_ERR_STATE_H
#define CV_ATF_ERR_STATE_H

#define ATF_ERR_NONE			0xBEFFFFFFU
#define ATF_ERR_BL1_RETURN		0xBE000001U
#define ATF_ERR_PLAT_PANIC		0xBE00E002U
#define ATF_ERR_PLAT_SYSTEM_RESET	0xBE00E003U
#define ATF_ERR_PLAT_SYSTEM_PWR_CYC	0xBE00E004U

#define ATF_STATE_RESET_WAIT		0xBE003001U
#define ATF_STATE_RESET_RTC_WAIT	0xBE003002U

#endif /* CV_ATF_ERR_STATE_H */