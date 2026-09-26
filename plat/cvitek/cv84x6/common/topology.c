/*
 * Copyright (c) 2015-2016, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <common/debug.h>
#include <platform_def.h>
#include <cv_private.h>

/* The power domain tree descriptor */
static unsigned char power_domain_tree_desc[] = {
	/* Number of root nodes */
	PLATFORM_CLUSTER_COUNT,
	/* Number of children for the first node */
	PLATFORM_CLUSTER0_CORE_COUNT,
	#ifndef CONFIG_BOARD_fpga
	/* Number of children for the second node */
	PLATFORM_CLUSTER1_CORE_COUNT,
	#endif
};

/*******************************************************************************
 * This function returns the ARM default topology tree information.
 ******************************************************************************/
const unsigned char *plat_get_power_domain_tree_desc(void)
{
	return power_domain_tree_desc;
}

/*******************************************************************************
 * This function implements a part of the critical interface between the psci
 * generic layer and the platform that allows the former to query the platform
 * to convert an MPIDR to a unique linear index. An error code (-1) is returned
 * in case the MPIDR is invalid.
 ******************************************************************************/
int plat_core_pos_by_mpidr(u_register_t mpidr)
{
	unsigned int cluster_id, cpu_id;

	mpidr &= MPIDR_AFFINITY_MASK;
	if (mpidr & ~(MPIDR_CLUSTER_MASK | MPIDR_CPU_MASK))
		return -1;

	/*
	 * CV84X6 MPIDR layout: AFF0=thread ID, AFF1=CPU ID, AFF2=cluster ID.
	 * This differs from the standard ARMv8 layout (AFF0=CPU, AFF1=cluster).
	 */
	cluster_id = (mpidr >> MPIDR_AFF2_SHIFT) & MPIDR_AFFLVL_MASK;
	cpu_id = (mpidr >> MPIDR_AFF1_SHIFT) & MPIDR_AFFLVL_MASK;

	if (cluster_id >= PLATFORM_CLUSTER_COUNT)
		return -1;

	if (cpu_id >= PLATFORM_MAX_CPUS_PER_CLUSTER)
		return -1;

	int core_pos = plat_bm_calc_core_pos(mpidr);
	return core_pos;
}

/*******************************************************************************
 * This function returns the cluster ID for a given MPIDR.
 *
 * CV84X6 MPIDR layout: AFF0=thread ID, AFF1=CPU ID, AFF2=cluster ID.
 ******************************************************************************/
unsigned int plat_cluster_id_by_mpidr(u_register_t mpidr)
{
	return (mpidr >> MPIDR_AFF2_SHIFT) & MPIDR_AFFLVL_MASK;
}
