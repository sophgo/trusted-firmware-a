/*
 * Copyright (c) 2013-2017, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __ARCH_HELPERS_H__
#define __ARCH_HELPERS_H__

/* Include TF-A core arch_helpers.h for all standard definitions */
#include <include/arch/aarch64/arch_helpers.h>

/* CVITEK platform-specific additions below */

#ifndef __ARCH_HELPERS_CVITEK_H__
#define __ARCH_HELPERS_CVITEK_H__

#include <cpu.h>	/* for additional register definitions */
#include <cdefs.h>	/* For __dead2 */
#include <stdint.h>
#include <sys/types.h>

/*******************************************************************************
 * Misc. accessor prototypes
 ******************************************************************************/

void __dead2 eret(uint64_t x0, uint64_t x1, uint64_t x2, uint64_t x3,
		  uint64_t x4, uint64_t x5, uint64_t x6, uint64_t x7);

uint32_t get_afflvl_shift(uint32_t);
uint32_t mpidr_mask_lower_afflvls(uint64_t, uint32_t);

#endif /* __ARCH_HELPERS_CVITEK_H__ */
#endif /* __ARCH_HELPERS_H__ */
