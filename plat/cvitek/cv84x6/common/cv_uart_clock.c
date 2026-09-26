/*
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <cv_private.h>
#include <platform_def.h>

uint32_t plat_bm_get_uart_clock(void)
{
	return PLAT_BM_BOOT_UART_CLK_IN_HZ;
}