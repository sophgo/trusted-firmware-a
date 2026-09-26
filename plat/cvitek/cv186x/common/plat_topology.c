/*
 * Copyright (c) 2013-2016, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <platform_def.h>

#include <arch.h>
#include <lib/psci/psci.h>

#include <cv_private.h>

static unsigned char power_domain_tree_desc[] = {
	/* Number of root nodes */
	PLATFORM_CLUSTER_COUNT,
	/* Number of children for the first node */
	PLATFORM_CLUSTER0_CORE_COUNT,
#if defined(PLATFORM_CLUSTER1_CORE_COUNT) && (PLATFORM_CLUSTER1_CORE_COUNT > 0)
	/* Number of children for the second node */
	PLATFORM_CLUSTER1_CORE_COUNT,
#endif
};
/*******************************************************************************
 * This function returns the RockChip default topology tree information.
 ******************************************************************************/
const unsigned char *plat_get_power_domain_tree_desc(void)
{
	return power_domain_tree_desc;
}

int plat_core_pos_by_mpidr(u_register_t mpidr)
{
	unsigned int cluster_id, cpu_id;

	mpidr &= MPIDR_AFFINITY_MASK;
	if (mpidr & ~(MPIDR_CLUSTER_MASK | MPIDR_CPU_MASK))
		return -1;

	cluster_id = (mpidr >> MPIDR_AFF1_SHIFT) & MPIDR_AFFLVL_MASK;
	cpu_id = (mpidr >> MPIDR_AFF0_SHIFT) & MPIDR_AFFLVL_MASK;

	if (cluster_id >= PLATFORM_CLUSTER_COUNT)
		return -1;

	if (cpu_id >= PLATFORM_MAX_CPUS_PER_CLUSTER)
		return -1;

	return (cluster_id * PLATFORM_MAX_CPUS_PER_CLUSTER) + cpu_id;
}
