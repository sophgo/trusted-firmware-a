/*
 *Copyright (c) 2013-2017, ARM Limited and Contributors. All rights reserved.
 *
 *SPDX-License-Identifier: BSD-3-Clause
 */

#include <gicv3.h>
#include <plat_arm.h>
#include <platform.h>
#include <platform_def.h>
#include <cv_private.h>
#include <debug.h>
#include <console.h>

/******************************************************************************
 * The following functions are defined as weak to allow a platform to override
 * the way the GICv3 driver is initialised and used.
 *****************************************************************************/
#pragma weak plat_arm_gic_driver_init
#pragma weak plat_arm_gic_init
#pragma weak plat_arm_gic_cpuif_enable
#pragma weak plat_arm_gic_cpuif_disable
#pragma weak plat_arm_gic_pcpu_init
#pragma weak plat_arm_gic_redistif_on
#pragma weak plat_arm_gic_redistif_off

/******************************************************************************
 * Platform-specific wrapper for plat_arm_calc_core_pos
 * This maps to the platform's plat_bm_calc_core_pos function
 *****************************************************************************/
unsigned int plat_arm_calc_core_pos(u_register_t mpidr)
{
	return plat_bm_calc_core_pos(mpidr);
}

/* The GICv3 driver only needs to be initialized in EL3 */
static uintptr_t rdistif_base_addrs[PLATFORM_CORE_COUNT];

/* Interrupt properties for GICv3 driver */
static const interrupt_prop_t arm_interrupt_props[] = {
	PLAT_ARM_G1S_IRQ_PROPS(INTR_GROUP1S),
	PLAT_ARM_G0_IRQ_PROPS(INTR_GROUP0)
};

const gicv3_driver_data_t arm_gic_data = {
	.gicd_base = PLAT_ARM_GICD_BASE,
	.gicr_base = PLAT_ARM_GICR_BASE,
	.interrupt_props = arm_interrupt_props,
	.interrupt_props_num = ARRAY_SIZE(arm_interrupt_props),
	.rdistif_num = PLATFORM_CORE_COUNT,
	.rdistif_base_addrs = rdistif_base_addrs,
	.mpidr_to_core_pos = plat_arm_calc_core_pos
};

void plat_arm_gic_driver_init(void)
{
	/*
	 * The GICv3 driver is initialized in EL3 and does not need
	 * to be initialized again in SEL1. This is because the S-EL1
	 * can use GIC system registers to manage interrupts and does
	 * not need GIC interface base addresses to be configured.
	 */
#if (defined(AARCH32) && defined(IMAGE_BL32)) || \
	(defined(IMAGE_BL31) && !defined(AARCH32))
	gicv3_driver_init(&arm_gic_data);
#endif
}

/******************************************************************************
 * ARM common helper to initialize the GIC. Only invoked by BL31
 *****************************************************************************/
void plat_arm_gic_init(void)
{
	gicv3_distif_init();
	gicv3_rdistif_init(plat_my_core_pos());
	gicv3_cpuif_enable(plat_my_core_pos());

}

/******************************************************************************
 * ARM common helper to enable the GIC CPU interface
 *****************************************************************************/
void plat_arm_gic_cpuif_enable(void)
{
	gicv3_cpuif_enable(plat_my_core_pos());
}

/******************************************************************************
 * ARM common helper to disable the GIC CPU interface
 *****************************************************************************/
void plat_arm_gic_cpuif_disable(void)
{
	gicv3_cpuif_disable(plat_my_core_pos());
}

/******************************************************************************
 * ARM common helper to initialize the per-cpu redistributor interface in GICv3
 *****************************************************************************/
void plat_arm_gic_pcpu_init(void)
{
	gicv3_rdistif_init(plat_my_core_pos());
}

/******************************************************************************
 * ARM common helpers to power GIC redistributor interface
 *****************************************************************************/
void plat_arm_gic_redistif_on(void)
{
	gicv3_rdistif_on(plat_my_core_pos());
}

void plat_arm_gic_redistif_off(void)
{
	gicv3_rdistif_off(plat_my_core_pos());
}