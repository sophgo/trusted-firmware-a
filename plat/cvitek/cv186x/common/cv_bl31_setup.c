/*
 * Copyright (c) 2015-2016, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <common/bl_common.h>
#include <console.h>
#include <gicv2.h>
#include <platform_def.h>
#include <cv_private.h>
#include <plat_arm.h>
#include <delay_timer.h>
#include <generic_delay_timer.h>
#include <mmio.h>
#include <platform_common_def.h>
#include <common_def.h>

/*
 * The next 3 constants identify the extents of the code, RO data region and the
 * limit of the BL3-1 image.  These addresses are used by the MMU setup code and
 * therefore they must be page-aligned.  It is the responsibility of the linker
 * script to ensure that __RO_START__, __RO_END__ & __BL31_END__ linker symbols
 * refer to page-aligned addresses.
 */
#define BL31_RO_BASE (unsigned long)(&__RO_START__)
#define BL31_RO_LIMIT (unsigned long)(&__RO_END__)
/*
 * [xun] use BL31_LIMIT to map all memory to MMU including code,data in linkscript
 * and extra memory assignment for runtime
 */
#define BL31_END (unsigned long)(BL31_LIMIT) //(&__BL31_END__)
/*
 * Placeholder variables for copying the arguments that have been passed to
 * BL3-1 from BL2.
 */
static entry_point_info_t bl32_image_ep_info;
static entry_point_info_t bl33_image_ep_info;
enum {
	BM_CORE_DOWN = 0,
	BM_CORE_UP = 1,
};
/*******************************************************************************
 * Perform any BL3-1 early platform setup.  Here is an opportunity to copy
 * parameters passed by the calling EL (S-EL1 in BL2 & S-EL3 in BL1) before
 * they are lost (potentially). This needs to be done before the MMU is
 * initialized so that the memory layout can be used while creating page
 * tables. BL2 has flushed this information to memory, so we are guaranteed
 * to pick up good data.
 ******************************************************************************/
void bl31_early_platform_setup(bl31_params_t *from_bl2,
				void *plat_params_from_bl2)
{
	mmio_write_32(ATF_STATE_REG, 0xB31E0001U);

	console_init(PLAT_BOOT_UART_BASE, PLAT_BM_BOOT_UART_CLK_IN_HZ,
		     PLAT_BM_CONSOLE_BAUDRATE);
	if (!from_bl2 && !plat_params_from_bl2)
		return;
	bm_ncore_init(NCORE_BASE);
	bm_ncore_enable_cluster(BM_CORE_UP);
	if (from_bl2 == NULL) {
		ERROR("from_bl2 is NULL\n");
		return;
	}
	if (from_bl2->h.type != PARAM_BL31 || from_bl2->h.version < VERSION_1) {
		ERROR("invalid bl31 params header\n");
		return;
	}
#if DEBUG
	assert(((unsigned long long)plat_params_from_bl2) ==
		BM_BL31_PLAT_PARAM_VAL);
#endif

	/*
	 * Copy BL3-2 (if populated by BL2) and BL3-3 entry point information.
	 * They are stored in Secure RAM, in BL2's address space.
	 */

	if (from_bl2->bl32_ep_info)
		bl32_image_ep_info = *from_bl2->bl32_ep_info;
	if (from_bl2->bl33_ep_info == NULL) {
		ERROR("bl33_ep_info is NULL\n");
		return;
	}
	bl33_image_ep_info = *from_bl2->bl33_ep_info;
}

void bl31_plat_arch_setup(void)
{
	unsigned long coh_start = 0UL;
	unsigned long coh_end = 0UL;

#if defined(BL_COHERENT_RAM_BASE) && defined(BL_COHERENT_RAM_END)
	coh_start = BL_COHERENT_RAM_BASE;
	coh_end = BL_COHERENT_RAM_END;
#endif
	bm_configure_mmu_el3(BL31_RO_BASE, (BL31_END - BL31_RO_BASE),
				BL31_RO_BASE, BL31_RO_LIMIT,
				coh_start, coh_end);
}

void bl31_platform_setup(void)
{
	/* Initialize the gic cpu and distributor interfaces */
	plat_arm_gic_driver_init();
	plat_arm_gic_init();
	timer_init();
}

unsigned int plat_get_syscnt_freq2(void)
{
	return SYS_COUNTER_FREQ_IN_TICKS;
}

/*******************************************************************************
 * Return a pointer to the 'entry_point_info' structure of the next image
 * for the security state specified. BL3-3 corresponds to the non-secure
 * image type while BL3-2 corresponds to the secure image type. A NULL
 * pointer is returned if the image does not exist.
 ******************************************************************************/
entry_point_info_t *bl31_plat_get_next_image_ep_info(uint32_t type)
{
	entry_point_info_t *next_image_info;

	assert(sec_state_is_valid(type));
	next_image_info = (type == NON_SECURE)
			? &bl33_image_ep_info : &bl32_image_ep_info;
	/*
	 * None of the images on the ARM development platforms can have 0x0
	 * as the entrypoint
	 */
	if (next_image_info->pc)
		return next_image_info;
	else
		return NULL;
}

