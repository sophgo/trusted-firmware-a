/*
 * Copyright (c) 2013-2023, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <console.h>
#include <assert.h>
#include <debug.h>

#pragma weak putchar
int putchar(int c)
{
	int res;
	/*
	#if defined(IMAGE_BL1)
		// Skip UART message
		if (plat_bm_gpio_read(BIT_MASK_GPIO_DIS_UART_LOG))
			return c;
	#endif
	*/
		if (console_putc((unsigned char)c) >= 0)
			res = c;
		else
			res = EOF;
	
		return res;
}
