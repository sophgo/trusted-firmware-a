/*
 * Copyright (c) 2013-2017, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ARCH_H
#define ARCH_H

#include <utils_def.h>

/*******************************************************************************
 * MIDR bit definitions
 ******************************************************************************/
#ifndef MIDR_IMPL_MASK
#ifndef MIDR_IMPL_MASK
#define MIDR_IMPL_MASK		U(0xff)
#endif /* MIDR_IMPL_MASK */
#endif /* MIDR_IMPL_MASK */
#ifndef MIDR_IMPL_SHIFT
#ifndef MIDR_IMPL_SHIFT
#define MIDR_IMPL_SHIFT		U(0x18)
#endif /* MIDR_IMPL_SHIFT */
#endif /* MIDR_IMPL_SHIFT */
#ifndef MIDR_VAR_SHIFT
#ifndef MIDR_VAR_SHIFT
#define MIDR_VAR_SHIFT		U(20)
#endif /* MIDR_VAR_SHIFT */
#endif /* MIDR_VAR_SHIFT */
#ifndef MIDR_VAR_BITS
#ifndef MIDR_VAR_BITS
#define MIDR_VAR_BITS		U(4)
#endif /* MIDR_VAR_BITS */
#endif /* MIDR_VAR_BITS */
#ifndef MIDR_VAR_MASK
#ifndef MIDR_VAR_MASK
#define MIDR_VAR_MASK		U(0xf)
#endif /* MIDR_VAR_MASK */
#endif /* MIDR_VAR_MASK */
#ifndef MIDR_REV_SHIFT
#ifndef MIDR_REV_SHIFT
#define MIDR_REV_SHIFT		U(0)
#endif /* MIDR_REV_SHIFT */
#endif /* MIDR_REV_SHIFT */
#ifndef MIDR_REV_BITS
#ifndef MIDR_REV_BITS
#define MIDR_REV_BITS		U(4)
#endif /* MIDR_REV_BITS */
#endif /* MIDR_REV_BITS */
#ifndef MIDR_REV_MASK
#ifndef MIDR_REV_MASK
#define MIDR_REV_MASK		U(0xf)
#endif /* MIDR_REV_MASK */
#endif /* MIDR_REV_MASK */
#ifndef MIDR_PN_MASK
#ifndef MIDR_PN_MASK
#define MIDR_PN_MASK		U(0xfff)
#endif /* MIDR_PN_MASK */
#endif /* MIDR_PN_MASK */
#ifndef MIDR_PN_SHIFT
#ifndef MIDR_PN_SHIFT
#define MIDR_PN_SHIFT		U(0x4)
#endif /* MIDR_PN_SHIFT */
#endif /* MIDR_PN_SHIFT */

/*******************************************************************************
 * MPIDR macros
 ******************************************************************************/
#ifndef MPIDR_MT_MASK
#ifndef MPIDR_MT_MASK
#define MPIDR_MT_MASK		(U(1) << 24)
#endif /* MPIDR_MT_MASK */
#endif /* MPIDR_MT_MASK */
#ifndef MPIDR_CPU_MASK
#ifndef MPIDR_CPU_MASK
#define MPIDR_CPU_MASK		MPIDR_AFFLVL_MASK
#endif /* MPIDR_CPU_MASK */
#endif /* MPIDR_CPU_MASK */
#ifndef MPIDR_CLUSTER_MASK
#ifndef MPIDR_CLUSTER_MASK
#define MPIDR_CLUSTER_MASK	(MPIDR_AFFLVL_MASK << MPIDR_AFFINITY_BITS)
#endif /* MPIDR_CLUSTER_MASK */
#endif /* MPIDR_CLUSTER_MASK */
#ifndef MPIDR_AFFINITY_BITS
#ifndef MPIDR_AFFINITY_BITS
#define MPIDR_AFFINITY_BITS	U(8)
#endif /* MPIDR_AFFINITY_BITS */
#endif /* MPIDR_AFFINITY_BITS */
#ifndef MPIDR_AFFLVL_MASK
#ifndef MPIDR_AFFLVL_MASK
#define MPIDR_AFFLVL_MASK	U(0xff)
#endif /* MPIDR_AFFLVL_MASK */
#endif /* MPIDR_AFFLVL_MASK */
#ifndef MPIDR_AFF0_SHIFT
#ifndef MPIDR_AFF0_SHIFT
#define MPIDR_AFF0_SHIFT	U(0)
#endif /* MPIDR_AFF0_SHIFT */
#endif /* MPIDR_AFF0_SHIFT */
#ifndef MPIDR_AFF1_SHIFT
#ifndef MPIDR_AFF1_SHIFT
#define MPIDR_AFF1_SHIFT	U(8)
#endif /* MPIDR_AFF1_SHIFT */
#endif /* MPIDR_AFF1_SHIFT */
#ifndef MPIDR_AFF2_SHIFT
#ifndef MPIDR_AFF2_SHIFT
#define MPIDR_AFF2_SHIFT	U(16)
#endif /* MPIDR_AFF2_SHIFT */
#endif /* MPIDR_AFF2_SHIFT */
#ifndef MPIDR_AFF3_SHIFT
#ifndef MPIDR_AFF3_SHIFT
#define MPIDR_AFF3_SHIFT	U(32)
#endif /* MPIDR_AFF3_SHIFT */
#endif /* MPIDR_AFF3_SHIFT */
#ifndef MPIDR_AFFINITY_MASK
#ifndef MPIDR_AFFINITY_MASK
#define MPIDR_AFFINITY_MASK	U(0xff00ffffff)
#endif /* MPIDR_AFFINITY_MASK */
#endif /* MPIDR_AFFINITY_MASK */
#ifndef MPIDR_AFFLVL_SHIFT
#ifndef MPIDR_AFFLVL_SHIFT
#define MPIDR_AFFLVL_SHIFT	U(3)
#endif /* MPIDR_AFFLVL_SHIFT */
#endif /* MPIDR_AFFLVL_SHIFT */
#ifndef MPIDR_AFFLVL0
#ifndef MPIDR_AFFLVL0
#define MPIDR_AFFLVL0		U(0)
#endif /* MPIDR_AFFLVL0 */
#endif /* MPIDR_AFFLVL0 */
#ifndef MPIDR_AFFLVL1
#ifndef MPIDR_AFFLVL1
#define MPIDR_AFFLVL1		U(1)
#endif /* MPIDR_AFFLVL1 */
#endif /* MPIDR_AFFLVL1 */
#ifndef MPIDR_AFFLVL2
#ifndef MPIDR_AFFLVL2
#define MPIDR_AFFLVL2		U(2)
#endif /* MPIDR_AFFLVL2 */
#endif /* MPIDR_AFFLVL2 */
#ifndef MPIDR_AFFLVL3
#ifndef MPIDR_AFFLVL3
#define MPIDR_AFFLVL3		U(3)
#endif /* MPIDR_AFFLVL3 */
#endif /* MPIDR_AFFLVL3 */
#ifndef MPIDR_AFFLVL0_VAL
#define MPIDR_AFFLVL0_VAL(mpidr) \
		((mpidr >> MPIDR_AFF0_SHIFT) & MPIDR_AFFLVL_MASK)
#endif /* MPIDR_AFFLVL0_VAL */
#ifndef MPIDR_AFFLVL1_VAL
#define MPIDR_AFFLVL1_VAL(mpidr) \
		((mpidr >> MPIDR_AFF1_SHIFT) & MPIDR_AFFLVL_MASK)
#endif /* MPIDR_AFFLVL1_VAL */
#ifndef MPIDR_AFFLVL2_VAL
#define MPIDR_AFFLVL2_VAL(mpidr) \
		((mpidr >> MPIDR_AFF2_SHIFT) & MPIDR_AFFLVL_MASK)
#endif /* MPIDR_AFFLVL2_VAL */
#ifndef MPIDR_AFFLVL3_VAL
#define MPIDR_AFFLVL3_VAL(mpidr) \
		((mpidr >> MPIDR_AFF3_SHIFT) & MPIDR_AFFLVL_MASK)
#endif /* MPIDR_AFFLVL3_VAL */
/*
 * The MPIDR_MAX_AFFLVL count starts from 0. Take care to
 * add one while using this macro to define array sizes.
 * TODO: Support only the first 3 affinity levels for now.
 */
#ifndef MPIDR_MAX_AFFLVL
#ifndef MPIDR_MAX_AFFLVL
#define MPIDR_MAX_AFFLVL	U(2)
#endif /* MPIDR_MAX_AFFLVL */
#endif /* MPIDR_MAX_AFFLVL */

/* Constant to highlight the assumption that MPIDR allocation starts from 0 */
#ifndef FIRST_MPIDR
#ifndef FIRST_MPIDR
#define FIRST_MPIDR		U(0)
#endif /* FIRST_MPIDR */
#endif /* FIRST_MPIDR */

/*******************************************************************************
 * Definitions for CPU system register interface to GICv3
 ******************************************************************************/
#ifndef ICC_SRE_EL1
#ifndef ICC_SRE_EL1
#define ICC_SRE_EL1     S3_0_C12_C12_5
#endif /* ICC_SRE_EL1 */
#endif /* ICC_SRE_EL1 */
#ifndef ICC_SRE_EL2
#ifndef ICC_SRE_EL2
#define ICC_SRE_EL2     S3_4_C12_C9_5
#endif /* ICC_SRE_EL2 */
#endif /* ICC_SRE_EL2 */
#ifndef ICC_SRE_EL3
#ifndef ICC_SRE_EL3
#define ICC_SRE_EL3     S3_6_C12_C12_5
#endif /* ICC_SRE_EL3 */
#endif /* ICC_SRE_EL3 */
#ifndef ICC_CTLR_EL1
#ifndef ICC_CTLR_EL1
#define ICC_CTLR_EL1    S3_0_C12_C12_4
#endif /* ICC_CTLR_EL1 */
#endif /* ICC_CTLR_EL1 */
#ifndef ICC_CTLR_EL3
#ifndef ICC_CTLR_EL3
#define ICC_CTLR_EL3    S3_6_C12_C12_4
#endif /* ICC_CTLR_EL3 */
#endif /* ICC_CTLR_EL3 */
#ifndef ICC_PMR_EL1
#ifndef ICC_PMR_EL1
#define ICC_PMR_EL1     S3_0_C4_C6_0
#endif /* ICC_PMR_EL1 */
#endif /* ICC_PMR_EL1 */
#ifndef ICC_IGRPEN1_EL3
#ifndef ICC_IGRPEN1_EL3
#define ICC_IGRPEN1_EL3 S3_6_c12_c12_7
#endif /* ICC_IGRPEN1_EL3 */
#endif /* ICC_IGRPEN1_EL3 */
#ifndef ICC_IGRPEN0_EL1
#ifndef ICC_IGRPEN0_EL1
#define ICC_IGRPEN0_EL1 S3_0_c12_c12_6
#endif /* ICC_IGRPEN0_EL1 */
#endif /* ICC_IGRPEN0_EL1 */
#ifndef ICC_HPPIR0_EL1
#ifndef ICC_HPPIR0_EL1
#define ICC_HPPIR0_EL1  S3_0_c12_c8_2
#endif /* ICC_HPPIR0_EL1 */
#endif /* ICC_HPPIR0_EL1 */
#ifndef ICC_HPPIR1_EL1
#ifndef ICC_HPPIR1_EL1
#define ICC_HPPIR1_EL1  S3_0_c12_c12_2
#endif /* ICC_HPPIR1_EL1 */
#endif /* ICC_HPPIR1_EL1 */
#ifndef ICC_IAR0_EL1
#ifndef ICC_IAR0_EL1
#define ICC_IAR0_EL1    S3_0_c12_c8_0
#endif /* ICC_IAR0_EL1 */
#endif /* ICC_IAR0_EL1 */
#ifndef ICC_IAR1_EL1
#ifndef ICC_IAR1_EL1
#define ICC_IAR1_EL1    S3_0_c12_c12_0
#endif /* ICC_IAR1_EL1 */
#endif /* ICC_IAR1_EL1 */
#ifndef ICC_EOIR0_EL1
#ifndef ICC_EOIR0_EL1
#define ICC_EOIR0_EL1   S3_0_c12_c8_1
#endif /* ICC_EOIR0_EL1 */
#endif /* ICC_EOIR0_EL1 */
#ifndef ICC_EOIR1_EL1
#ifndef ICC_EOIR1_EL1
#define ICC_EOIR1_EL1   S3_0_c12_c12_1
#endif /* ICC_EOIR1_EL1 */
#endif /* ICC_EOIR1_EL1 */

/*******************************************************************************
 * Generic timer memory mapped registers & offsets
 ******************************************************************************/
#ifndef CNTCR_OFF
#ifndef CNTCR_OFF
#define CNTCR_OFF			U(0x000)
#endif /* CNTCR_OFF */
#endif /* CNTCR_OFF */
#ifndef CNTFID_OFF
#ifndef CNTFID_OFF
#define CNTFID_OFF			U(0x020)
#endif /* CNTFID_OFF */
#endif /* CNTFID_OFF */

#ifndef CNTCR_EN
#ifndef CNTCR_EN
#define CNTCR_EN			(U(1) << 0)
#endif /* CNTCR_EN */
#endif /* CNTCR_EN */
#ifndef CNTCR_HDBG
#ifndef CNTCR_HDBG
#define CNTCR_HDBG			(U(1) << 1)
#endif /* CNTCR_HDBG */
#endif /* CNTCR_HDBG */
#ifndef CNTCR_FCREQ
#define CNTCR_FCREQ(x)			((x) << 8)
#endif /* CNTCR_FCREQ */

/*******************************************************************************
 * System register bit definitions
 ******************************************************************************/
/* CLIDR definitions */
#ifndef LOUIS_SHIFT
#ifndef LOUIS_SHIFT
#define LOUIS_SHIFT		U(21)
#endif /* LOUIS_SHIFT */
#endif /* LOUIS_SHIFT */
#ifndef LOC_SHIFT
#ifndef LOC_SHIFT
#define LOC_SHIFT		U(24)
#endif /* LOC_SHIFT */
#endif /* LOC_SHIFT */
#ifndef CLIDR_FIELD_WIDTH
#ifndef CLIDR_FIELD_WIDTH
#define CLIDR_FIELD_WIDTH	U(3)
#endif /* CLIDR_FIELD_WIDTH */
#endif /* CLIDR_FIELD_WIDTH */

/* CSSELR definitions */
#ifndef LEVEL_SHIFT
#ifndef LEVEL_SHIFT
#define LEVEL_SHIFT		U(1)
#endif /* LEVEL_SHIFT */
#endif /* LEVEL_SHIFT */

/* D$ set/way op type defines */
#ifndef DCISW
#ifndef DCISW
#define DCISW			U(0x0)
#endif /* DCISW */
#endif /* DCISW */
#ifndef DCCISW
#ifndef DCCISW
#define DCCISW			U(0x1)
#endif /* DCCISW */
#endif /* DCCISW */
#ifndef DCCSW
#ifndef DCCSW
#define DCCSW			U(0x2)
#endif /* DCCSW */
#endif /* DCCSW */

/* ID_AA64PFR0_EL1 definitions */
#ifndef ID_AA64PFR0_EL0_SHIFT
#ifndef ID_AA64PFR0_EL0_SHIFT
#define ID_AA64PFR0_EL0_SHIFT	U(0)
#endif /* ID_AA64PFR0_EL0_SHIFT */
#endif /* ID_AA64PFR0_EL0_SHIFT */
#ifndef ID_AA64PFR0_EL1_SHIFT
#ifndef ID_AA64PFR0_EL1_SHIFT
#define ID_AA64PFR0_EL1_SHIFT	U(4)
#endif /* ID_AA64PFR0_EL1_SHIFT */
#endif /* ID_AA64PFR0_EL1_SHIFT */
#ifndef ID_AA64PFR0_EL2_SHIFT
#ifndef ID_AA64PFR0_EL2_SHIFT
#define ID_AA64PFR0_EL2_SHIFT	U(8)
#endif /* ID_AA64PFR0_EL2_SHIFT */
#endif /* ID_AA64PFR0_EL2_SHIFT */
#ifndef ID_AA64PFR0_EL3_SHIFT
#ifndef ID_AA64PFR0_EL3_SHIFT
#define ID_AA64PFR0_EL3_SHIFT	U(12)
#endif /* ID_AA64PFR0_EL3_SHIFT */
#endif /* ID_AA64PFR0_EL3_SHIFT */
#ifndef ID_AA64PFR0_ELX_MASK
#ifndef ID_AA64PFR0_ELX_MASK
#define ID_AA64PFR0_ELX_MASK	U(0xf)
#endif /* ID_AA64PFR0_ELX_MASK */
#endif /* ID_AA64PFR0_ELX_MASK */

/* ID_AA64DFR0_EL1.PMS definitions (for ARMv8.2+) */
#ifndef ID_AA64DFR0_PMS_SHIFT
#ifndef ID_AA64DFR0_PMS_SHIFT
#define ID_AA64DFR0_PMS_SHIFT	U(32)
#endif /* ID_AA64DFR0_PMS_SHIFT */
#endif /* ID_AA64DFR0_PMS_SHIFT */
#ifndef ID_AA64DFR0_PMS_LENGTH
#ifndef ID_AA64DFR0_PMS_LENGTH
#define ID_AA64DFR0_PMS_LENGTH	U(4)
#endif /* ID_AA64DFR0_PMS_LENGTH */
#endif /* ID_AA64DFR0_PMS_LENGTH */
#ifndef ID_AA64DFR0_PMS_MASK
#ifndef ID_AA64DFR0_PMS_MASK
#define ID_AA64DFR0_PMS_MASK	U(0xf)
#endif /* ID_AA64DFR0_PMS_MASK */
#endif /* ID_AA64DFR0_PMS_MASK */

/* ID_AA64DFR1_EL1 definitions */
#ifndef ID_AA64DFR1_BRP_SHIFT
#ifndef ID_AA64DFR1_BRP_SHIFT
#define ID_AA64DFR1_BRP_SHIFT		U(8)
#endif /* ID_AA64DFR1_BRP_SHIFT */
#endif /* ID_AA64DFR1_BRP_SHIFT */
#ifndef ID_AA64DFR1_BRP_WIDTH
#ifndef ID_AA64DFR1_BRP_WIDTH
#define ID_AA64DFR1_BRP_WIDTH		U(8)
#endif /* ID_AA64DFR1_BRP_WIDTH */
#endif /* ID_AA64DFR1_BRP_WIDTH */

#ifndef EL_IMPL_NONE
#ifndef EL_IMPL_NONE
#define EL_IMPL_NONE		U(0)
#endif /* EL_IMPL_NONE */
#endif /* EL_IMPL_NONE */
#ifndef EL_IMPL_A64ONLY
#ifndef EL_IMPL_A64ONLY
#define EL_IMPL_A64ONLY		U(1)
#endif /* EL_IMPL_A64ONLY */
#endif /* EL_IMPL_A64ONLY */
#ifndef EL_IMPL_A64_A32
#ifndef EL_IMPL_A64_A32
#define EL_IMPL_A64_A32		U(2)
#endif /* EL_IMPL_A64_A32 */
#endif /* EL_IMPL_A64_A32 */

#ifndef ID_AA64PFR0_GIC_SHIFT
#ifndef ID_AA64PFR0_GIC_SHIFT
#define ID_AA64PFR0_GIC_SHIFT	U(24)
#endif /* ID_AA64PFR0_GIC_SHIFT */
#endif /* ID_AA64PFR0_GIC_SHIFT */
#ifndef ID_AA64PFR0_GIC_WIDTH
#ifndef ID_AA64PFR0_GIC_WIDTH
#define ID_AA64PFR0_GIC_WIDTH	U(4)
#endif /* ID_AA64PFR0_GIC_WIDTH */
#endif /* ID_AA64PFR0_GIC_WIDTH */
#ifndef ID_AA64PFR0_GIC_MASK
#ifndef ID_AA64PFR0_GIC_MASK
#define ID_AA64PFR0_GIC_MASK	((U(1) << ID_AA64PFR0_GIC_WIDTH) - 1)
#endif /* ID_AA64PFR0_GIC_MASK */
#endif /* ID_AA64PFR0_GIC_MASK */

/* ID_AA64MMFR0_EL1 definitions */
#ifndef ID_AA64MMFR0_EL1_PARANGE_MASK
#ifndef ID_AA64MMFR0_EL1_PARANGE_MASK
#define ID_AA64MMFR0_EL1_PARANGE_MASK	U(0xf)
#endif /* ID_AA64MMFR0_EL1_PARANGE_MASK */
#endif /* ID_AA64MMFR0_EL1_PARANGE_MASK */

#ifndef PARANGE_0000
#ifndef PARANGE_0000
#define PARANGE_0000	U(32)
#endif /* PARANGE_0000 */
#endif /* PARANGE_0000 */
#ifndef PARANGE_0001
#ifndef PARANGE_0001
#define PARANGE_0001	U(36)
#endif /* PARANGE_0001 */
#endif /* PARANGE_0001 */
#ifndef PARANGE_0010
#ifndef PARANGE_0010
#define PARANGE_0010	U(40)
#endif /* PARANGE_0010 */
#endif /* PARANGE_0010 */
#ifndef PARANGE_0011
#ifndef PARANGE_0011
#define PARANGE_0011	U(42)
#endif /* PARANGE_0011 */
#endif /* PARANGE_0011 */
#ifndef PARANGE_0100
#ifndef PARANGE_0100
#define PARANGE_0100	U(44)
#endif /* PARANGE_0100 */
#endif /* PARANGE_0100 */
#ifndef PARANGE_0101
#ifndef PARANGE_0101
#define PARANGE_0101	U(48)
#endif /* PARANGE_0101 */
#endif /* PARANGE_0101 */

/* ID_PFR1_EL1 definitions */
#ifndef ID_PFR1_VIRTEXT_SHIFT
#ifndef ID_PFR1_VIRTEXT_SHIFT
#define ID_PFR1_VIRTEXT_SHIFT	U(12)
#endif /* ID_PFR1_VIRTEXT_SHIFT */
#endif /* ID_PFR1_VIRTEXT_SHIFT */
#ifndef ID_PFR1_VIRTEXT_MASK
#ifndef ID_PFR1_VIRTEXT_MASK
#define ID_PFR1_VIRTEXT_MASK	U(0xf)
#endif /* ID_PFR1_VIRTEXT_MASK */
#endif /* ID_PFR1_VIRTEXT_MASK */
#ifndef GET_VIRT_EXT
#define GET_VIRT_EXT(id)	((id >> ID_PFR1_VIRTEXT_SHIFT) \
				 & ID_PFR1_VIRTEXT_MASK)
#endif /* GET_VIRT_EXT */

/* SCTLR definitions */
#ifndef SCTLR_EL2_RES1
#ifndef SCTLR_EL2_RES1
#define SCTLR_EL2_RES1	((U(1) << 29) | (U(1) << 28) | (U(1) << 23) | \
			 (U(1) << 22) | (U(1) << 18) | (U(1) << 16) | \
			 (U(1) << 11) | (U(1) << 5) | (U(1) << 4))
#endif /* SCTLR_EL2_RES1 */
#endif /* SCTLR_EL2_RES1 */

#ifndef SCTLR_EL1_RES1
#ifndef SCTLR_EL1_RES1
#define SCTLR_EL1_RES1	((U(1) << 29) | (U(1) << 28) | (U(1) << 23) | \
			 (U(1) << 22) | (U(1) << 20) | (U(1) << 11))
#endif /* SCTLR_EL1_RES1 */
#endif /* SCTLR_EL1_RES1 */
#ifndef SCTLR_AARCH32_EL1_RES1
#ifndef SCTLR_AARCH32_EL1_RES1
#define SCTLR_AARCH32_EL1_RES1 \
			((U(1) << 23) | (U(1) << 22) | (U(1) << 11) | \
			 (U(1) << 4) | (U(1) << 3))
#endif /* SCTLR_AARCH32_EL1_RES1 */
#endif /* SCTLR_AARCH32_EL1_RES1 */

#ifndef SCTLR_EL3_RES1
#ifndef SCTLR_EL3_RES1
#define SCTLR_EL3_RES1	((U(1) << 29) | (U(1) << 28) | (U(1) << 23) | \
			(U(1) << 22) | (U(1) << 18) | (U(1) << 16) | \
			(U(1) << 11) | (U(1) << 5) | (U(1) << 4))
#endif /* SCTLR_EL3_RES1 */
#endif /* SCTLR_EL3_RES1 */

#ifndef SCTLR_M_BIT
#ifndef SCTLR_M_BIT
#define SCTLR_M_BIT		(U(1) << 0)
#endif /* SCTLR_M_BIT */
#endif /* SCTLR_M_BIT */
#ifndef SCTLR_A_BIT
#ifndef SCTLR_A_BIT
#define SCTLR_A_BIT		(U(1) << 1)
#endif /* SCTLR_A_BIT */
#endif /* SCTLR_A_BIT */
#ifndef SCTLR_C_BIT
#ifndef SCTLR_C_BIT
#define SCTLR_C_BIT		(U(1) << 2)
#endif /* SCTLR_C_BIT */
#endif /* SCTLR_C_BIT */
#ifndef SCTLR_SA_BIT
#ifndef SCTLR_SA_BIT
#define SCTLR_SA_BIT		(U(1) << 3)
#endif /* SCTLR_SA_BIT */
#endif /* SCTLR_SA_BIT */
#ifndef SCTLR_CP15BEN_BIT
#ifndef SCTLR_CP15BEN_BIT
#define SCTLR_CP15BEN_BIT	(U(1) << 5)
#endif /* SCTLR_CP15BEN_BIT */
#endif /* SCTLR_CP15BEN_BIT */
#ifndef SCTLR_I_BIT
#ifndef SCTLR_I_BIT
#define SCTLR_I_BIT		(U(1) << 12)
#endif /* SCTLR_I_BIT */
#endif /* SCTLR_I_BIT */
#ifndef SCTLR_NTWI_BIT
#ifndef SCTLR_NTWI_BIT
#define SCTLR_NTWI_BIT		(U(1) << 16)
#endif /* SCTLR_NTWI_BIT */
#endif /* SCTLR_NTWI_BIT */
#ifndef SCTLR_NTWE_BIT
#ifndef SCTLR_NTWE_BIT
#define SCTLR_NTWE_BIT		(U(1) << 18)
#endif /* SCTLR_NTWE_BIT */
#endif /* SCTLR_NTWE_BIT */
#ifndef SCTLR_WXN_BIT
#ifndef SCTLR_WXN_BIT
#define SCTLR_WXN_BIT		(U(1) << 19)
#endif /* SCTLR_WXN_BIT */
#endif /* SCTLR_WXN_BIT */
#ifndef SCTLR_EE_BIT
#ifndef SCTLR_EE_BIT
#define SCTLR_EE_BIT		(U(1) << 25)
#endif /* SCTLR_EE_BIT */
#endif /* SCTLR_EE_BIT */
#ifndef SCTLR_RESET_VAL
#ifndef SCTLR_RESET_VAL
#define SCTLR_RESET_VAL		SCTLR_EL3_RES1
#endif /* SCTLR_RESET_VAL */
#endif /* SCTLR_RESET_VAL */

/* CPACR_El1 definitions */
#ifndef CPACR_EL1_FPEN
#define CPACR_EL1_FPEN(x)	((x) << 20)
#endif /* CPACR_EL1_FPEN */
#ifndef CPACR_EL1_FP_TRAP_EL0
#ifndef CPACR_EL1_FP_TRAP_EL0
#define CPACR_EL1_FP_TRAP_EL0	U(0x1)
#endif /* CPACR_EL1_FP_TRAP_EL0 */
#endif /* CPACR_EL1_FP_TRAP_EL0 */
#ifndef CPACR_EL1_FP_TRAP_ALL
#ifndef CPACR_EL1_FP_TRAP_ALL
#define CPACR_EL1_FP_TRAP_ALL	U(0x2)
#endif /* CPACR_EL1_FP_TRAP_ALL */
#endif /* CPACR_EL1_FP_TRAP_ALL */
#ifndef CPACR_EL1_FP_TRAP_NONE
#ifndef CPACR_EL1_FP_TRAP_NONE
#define CPACR_EL1_FP_TRAP_NONE	U(0x3)
#endif /* CPACR_EL1_FP_TRAP_NONE */
#endif /* CPACR_EL1_FP_TRAP_NONE */

/* SCR definitions */
#ifndef SCR_RES1_BITS
#ifndef SCR_RES1_BITS
#define SCR_RES1_BITS		((U(1) << 4) | (U(1) << 5))
#endif /* SCR_RES1_BITS */
#endif /* SCR_RES1_BITS */
#ifndef SCR_TWE_BIT
#ifndef SCR_TWE_BIT
#define SCR_TWE_BIT		(U(1) << 13)
#endif /* SCR_TWE_BIT */
#endif /* SCR_TWE_BIT */
#ifndef SCR_TWI_BIT
#ifndef SCR_TWI_BIT
#define SCR_TWI_BIT		(U(1) << 12)
#endif /* SCR_TWI_BIT */
#endif /* SCR_TWI_BIT */
#ifndef SCR_ST_BIT
#ifndef SCR_ST_BIT
#define SCR_ST_BIT		(U(1) << 11)
#endif /* SCR_ST_BIT */
#endif /* SCR_ST_BIT */
#ifndef SCR_RW_BIT
#ifndef SCR_RW_BIT
#define SCR_RW_BIT		(U(1) << 10)
#endif /* SCR_RW_BIT */
#endif /* SCR_RW_BIT */
#ifndef SCR_SIF_BIT
#ifndef SCR_SIF_BIT
#define SCR_SIF_BIT		(U(1) << 9)
#endif /* SCR_SIF_BIT */
#endif /* SCR_SIF_BIT */
#ifndef SCR_HCE_BIT
#ifndef SCR_HCE_BIT
#define SCR_HCE_BIT		(U(1) << 8)
#endif /* SCR_HCE_BIT */
#endif /* SCR_HCE_BIT */
#ifndef SCR_SMD_BIT
#ifndef SCR_SMD_BIT
#define SCR_SMD_BIT		(U(1) << 7)
#endif /* SCR_SMD_BIT */
#endif /* SCR_SMD_BIT */
#ifndef SCR_EA_BIT
#ifndef SCR_EA_BIT
#define SCR_EA_BIT		(U(1) << 3)
#endif /* SCR_EA_BIT */
#endif /* SCR_EA_BIT */
#ifndef SCR_FIQ_BIT
#ifndef SCR_FIQ_BIT
#define SCR_FIQ_BIT		(U(1) << 2)
#endif /* SCR_FIQ_BIT */
#endif /* SCR_FIQ_BIT */
#ifndef SCR_IRQ_BIT
#ifndef SCR_IRQ_BIT
#define SCR_IRQ_BIT		(U(1) << 1)
#endif /* SCR_IRQ_BIT */
#endif /* SCR_IRQ_BIT */
#ifndef SCR_NS_BIT
#ifndef SCR_NS_BIT
#define SCR_NS_BIT		(U(1) << 0)
#endif /* SCR_NS_BIT */
#endif /* SCR_NS_BIT */
#ifndef SCR_VALID_BIT_MASK
#ifndef SCR_VALID_BIT_MASK
#define SCR_VALID_BIT_MASK	U(0x2f8f)
#endif /* SCR_VALID_BIT_MASK */
#endif /* SCR_VALID_BIT_MASK */
#ifndef SCR_RESET_VAL
#ifndef SCR_RESET_VAL
#define SCR_RESET_VAL		SCR_RES1_BITS
#endif /* SCR_RESET_VAL */
#endif /* SCR_RESET_VAL */

/* MDCR_EL3 definitions */
#ifndef MDCR_SPD32
#define MDCR_SPD32(x)		((x) << 14)
#endif /* MDCR_SPD32 */
#ifndef MDCR_SPD32_LEGACY
#ifndef MDCR_SPD32_LEGACY
#define MDCR_SPD32_LEGACY	U(0x0)
#endif /* MDCR_SPD32_LEGACY */
#endif /* MDCR_SPD32_LEGACY */
#ifndef MDCR_SPD32_DISABLE
#ifndef MDCR_SPD32_DISABLE
#define MDCR_SPD32_DISABLE	U(0x2)
#endif /* MDCR_SPD32_DISABLE */
#endif /* MDCR_SPD32_DISABLE */
#ifndef MDCR_SPD32_ENABLE
#ifndef MDCR_SPD32_ENABLE
#define MDCR_SPD32_ENABLE	U(0x3)
#endif /* MDCR_SPD32_ENABLE */
#endif /* MDCR_SPD32_ENABLE */
#ifndef MDCR_SDD_BIT
#ifndef MDCR_SDD_BIT
#define MDCR_SDD_BIT		(U(1) << 16)
#endif /* MDCR_SDD_BIT */
#endif /* MDCR_SDD_BIT */
#ifndef MDCR_NSPB
#define MDCR_NSPB(x)		((x) << 12)
#endif /* MDCR_NSPB */
#ifndef MDCR_NSPB_EL1
#ifndef MDCR_NSPB_EL1
#define MDCR_NSPB_EL1		U(0x3)
#endif /* MDCR_NSPB_EL1 */
#endif /* MDCR_NSPB_EL1 */
#ifndef MDCR_TDOSA_BIT
#ifndef MDCR_TDOSA_BIT
#define MDCR_TDOSA_BIT		(U(1) << 10)
#endif /* MDCR_TDOSA_BIT */
#endif /* MDCR_TDOSA_BIT */
#ifndef MDCR_TDA_BIT
#ifndef MDCR_TDA_BIT
#define MDCR_TDA_BIT		(U(1) << 9)
#endif /* MDCR_TDA_BIT */
#endif /* MDCR_TDA_BIT */
#ifndef MDCR_TPM_BIT
#ifndef MDCR_TPM_BIT
#define MDCR_TPM_BIT		(U(1) << 6)
#endif /* MDCR_TPM_BIT */
#endif /* MDCR_TPM_BIT */
#ifndef MDCR_EL3_RESET_VAL
#ifndef MDCR_EL3_RESET_VAL
#define MDCR_EL3_RESET_VAL	U(0x0)
#endif /* MDCR_EL3_RESET_VAL */
#endif /* MDCR_EL3_RESET_VAL */

/* MDCR_EL2 definitions */
#ifndef MDCR_EL2_TPMS
#ifndef MDCR_EL2_TPMS
#define MDCR_EL2_TPMS		(U(1) << 14)
#endif /* MDCR_EL2_TPMS */
#endif /* MDCR_EL2_TPMS */
#ifndef MDCR_EL2_E2PB
#define MDCR_EL2_E2PB(x)	((x) << 12)
#endif /* MDCR_EL2_E2PB */
#ifndef MDCR_EL2_E2PB_EL1
#ifndef MDCR_EL2_E2PB_EL1
#define MDCR_EL2_E2PB_EL1	U(0x3)
#endif /* MDCR_EL2_E2PB_EL1 */
#endif /* MDCR_EL2_E2PB_EL1 */
#ifndef MDCR_EL2_TDRA_BIT
#ifndef MDCR_EL2_TDRA_BIT
#define MDCR_EL2_TDRA_BIT	(U(1) << 11)
#endif /* MDCR_EL2_TDRA_BIT */
#endif /* MDCR_EL2_TDRA_BIT */
#ifndef MDCR_EL2_TDOSA_BIT
#ifndef MDCR_EL2_TDOSA_BIT
#define MDCR_EL2_TDOSA_BIT	(U(1) << 10)
#endif /* MDCR_EL2_TDOSA_BIT */
#endif /* MDCR_EL2_TDOSA_BIT */
#ifndef MDCR_EL2_TDA_BIT
#ifndef MDCR_EL2_TDA_BIT
#define MDCR_EL2_TDA_BIT	(U(1) << 9)
#endif /* MDCR_EL2_TDA_BIT */
#endif /* MDCR_EL2_TDA_BIT */
#ifndef MDCR_EL2_TDE_BIT
#ifndef MDCR_EL2_TDE_BIT
#define MDCR_EL2_TDE_BIT	(U(1) << 8)
#endif /* MDCR_EL2_TDE_BIT */
#endif /* MDCR_EL2_TDE_BIT */
#ifndef MDCR_EL2_HPME_BIT
#ifndef MDCR_EL2_HPME_BIT
#define MDCR_EL2_HPME_BIT	(U(1) << 7)
#endif /* MDCR_EL2_HPME_BIT */
#endif /* MDCR_EL2_HPME_BIT */
#ifndef MDCR_EL2_TPM_BIT
#ifndef MDCR_EL2_TPM_BIT
#define MDCR_EL2_TPM_BIT	(U(1) << 6)
#endif /* MDCR_EL2_TPM_BIT */
#endif /* MDCR_EL2_TPM_BIT */
#ifndef MDCR_EL2_TPMCR_BIT
#ifndef MDCR_EL2_TPMCR_BIT
#define MDCR_EL2_TPMCR_BIT	(U(1) << 5)
#endif /* MDCR_EL2_TPMCR_BIT */
#endif /* MDCR_EL2_TPMCR_BIT */
#ifndef MDCR_EL2_RESET_VAL
#ifndef MDCR_EL2_RESET_VAL
#define MDCR_EL2_RESET_VAL	U(0x0)
#endif /* MDCR_EL2_RESET_VAL */
#endif /* MDCR_EL2_RESET_VAL */

/* HSTR_EL2 definitions */
#ifndef HSTR_EL2_RESET_VAL
#ifndef HSTR_EL2_RESET_VAL
#define HSTR_EL2_RESET_VAL	U(0x0)
#endif /* HSTR_EL2_RESET_VAL */
#endif /* HSTR_EL2_RESET_VAL */
#ifndef HSTR_EL2_T_MASK
#ifndef HSTR_EL2_T_MASK
#define HSTR_EL2_T_MASK		U(0xff)
#endif /* HSTR_EL2_T_MASK */
#endif /* HSTR_EL2_T_MASK */

/* CNTHP_CTL_EL2 definitions */
#ifndef CNTHP_CTL_ENABLE_BIT
#ifndef CNTHP_CTL_ENABLE_BIT
#define CNTHP_CTL_ENABLE_BIT	(U(1) << 0)
#endif /* CNTHP_CTL_ENABLE_BIT */
#endif /* CNTHP_CTL_ENABLE_BIT */
#ifndef CNTHP_CTL_RESET_VAL
#ifndef CNTHP_CTL_RESET_VAL
#define CNTHP_CTL_RESET_VAL	U(0x0)
#endif /* CNTHP_CTL_RESET_VAL */
#endif /* CNTHP_CTL_RESET_VAL */

/* VTTBR_EL2 definitions */
#ifndef VTTBR_RESET_VAL
#ifndef VTTBR_RESET_VAL
#define VTTBR_RESET_VAL		ULL(0x0)
#endif /* VTTBR_RESET_VAL */
#endif /* VTTBR_RESET_VAL */
#ifndef VTTBR_VMID_MASK
#ifndef VTTBR_VMID_MASK
#define VTTBR_VMID_MASK		ULL(0xff)
#endif /* VTTBR_VMID_MASK */
#endif /* VTTBR_VMID_MASK */
#ifndef VTTBR_VMID_SHIFT
#ifndef VTTBR_VMID_SHIFT
#define VTTBR_VMID_SHIFT	U(48)
#endif /* VTTBR_VMID_SHIFT */
#endif /* VTTBR_VMID_SHIFT */
#ifndef VTTBR_BADDR_MASK
#ifndef VTTBR_BADDR_MASK
#define VTTBR_BADDR_MASK	ULL(0xffffffffffff)
#endif /* VTTBR_BADDR_MASK */
#endif /* VTTBR_BADDR_MASK */
#ifndef VTTBR_BADDR_SHIFT
#ifndef VTTBR_BADDR_SHIFT
#define VTTBR_BADDR_SHIFT	U(0)
#endif /* VTTBR_BADDR_SHIFT */
#endif /* VTTBR_BADDR_SHIFT */

/* HCR definitions */
#ifndef HCR_RW_SHIFT
#ifndef HCR_RW_SHIFT
#define HCR_RW_SHIFT		U(31)
#endif /* HCR_RW_SHIFT */
#endif /* HCR_RW_SHIFT */
#ifndef HCR_RW_BIT
#ifndef HCR_RW_BIT
#define HCR_RW_BIT		(ULL(1) << HCR_RW_SHIFT)
#endif /* HCR_RW_BIT */
#endif /* HCR_RW_BIT */
#ifndef HCR_AMO_BIT
#ifndef HCR_AMO_BIT
#define HCR_AMO_BIT		(U(1) << 5)
#endif /* HCR_AMO_BIT */
#endif /* HCR_AMO_BIT */
#ifndef HCR_IMO_BIT
#ifndef HCR_IMO_BIT
#define HCR_IMO_BIT		(U(1) << 4)
#endif /* HCR_IMO_BIT */
#endif /* HCR_IMO_BIT */
#ifndef HCR_FMO_BIT
#ifndef HCR_FMO_BIT
#define HCR_FMO_BIT		(U(1) << 3)
#endif /* HCR_FMO_BIT */
#endif /* HCR_FMO_BIT */

/* ISR definitions */
#ifndef ISR_A_SHIFT
#ifndef ISR_A_SHIFT
#define ISR_A_SHIFT		U(8)
#endif /* ISR_A_SHIFT */
#endif /* ISR_A_SHIFT */
#ifndef ISR_I_SHIFT
#ifndef ISR_I_SHIFT
#define ISR_I_SHIFT		U(7)
#endif /* ISR_I_SHIFT */
#endif /* ISR_I_SHIFT */
#ifndef ISR_F_SHIFT
#ifndef ISR_F_SHIFT
#define ISR_F_SHIFT		U(6)
#endif /* ISR_F_SHIFT */
#endif /* ISR_F_SHIFT */

/* CNTHCTL_EL2 definitions */
#ifndef CNTHCTL_RESET_VAL
#ifndef CNTHCTL_RESET_VAL
#define CNTHCTL_RESET_VAL	U(0x0)
#endif /* CNTHCTL_RESET_VAL */
#endif /* CNTHCTL_RESET_VAL */
#ifndef EVNTEN_BIT
#ifndef EVNTEN_BIT
#ifndef EVNTEN_BIT
#define EVNTEN_BIT		(U(1) << 2)
#endif /* EVNTEN_BIT */
#endif /* EVNTEN_BIT */
#endif /* EVNTEN_BIT */
#ifndef EL1PCEN_BIT
#ifndef EL1PCEN_BIT
#define EL1PCEN_BIT		(U(1) << 1)
#endif /* EL1PCEN_BIT */
#endif /* EL1PCEN_BIT */
#ifndef EL1PCTEN_BIT
#ifndef EL1PCTEN_BIT
#define EL1PCTEN_BIT		(U(1) << 0)
#endif /* EL1PCTEN_BIT */
#endif /* EL1PCTEN_BIT */

/* CNTKCTL_EL1 definitions */
#ifndef EL0PTEN_BIT
#ifndef EL0PTEN_BIT
#define EL0PTEN_BIT		(U(1) << 9)
#endif /* EL0PTEN_BIT */
#endif /* EL0PTEN_BIT */
#ifndef EL0VTEN_BIT
#ifndef EL0VTEN_BIT
#define EL0VTEN_BIT		(U(1) << 8)
#endif /* EL0VTEN_BIT */
#endif /* EL0VTEN_BIT */
#ifndef EL0PCTEN_BIT
#ifndef EL0PCTEN_BIT
#define EL0PCTEN_BIT		(U(1) << 0)
#endif /* EL0PCTEN_BIT */
#endif /* EL0PCTEN_BIT */
#ifndef EL0VCTEN_BIT
#ifndef EL0VCTEN_BIT
#define EL0VCTEN_BIT		(U(1) << 1)
#endif /* EL0VCTEN_BIT */
#endif /* EL0VCTEN_BIT */
#ifndef EVNTEN_BIT
#define EVNTEN_BIT		(U(1) << 2)
#endif /* EVNTEN_BIT */
#ifndef EVNTDIR_BIT
#ifndef EVNTDIR_BIT
#define EVNTDIR_BIT		(U(1) << 3)
#endif /* EVNTDIR_BIT */
#endif /* EVNTDIR_BIT */
#ifndef EVNTI_SHIFT
#ifndef EVNTI_SHIFT
#define EVNTI_SHIFT		U(4)
#endif /* EVNTI_SHIFT */
#endif /* EVNTI_SHIFT */
#ifndef EVNTI_MASK
#ifndef EVNTI_MASK
#define EVNTI_MASK		U(0xf)
#endif /* EVNTI_MASK */
#endif /* EVNTI_MASK */

/* CPTR_EL3 definitions */
#ifndef TCPAC_BIT
#ifndef TCPAC_BIT
#define TCPAC_BIT		(U(1) << 31)
#endif /* TCPAC_BIT */
#endif /* TCPAC_BIT */
#ifndef TTA_BIT
#ifndef TTA_BIT
#define TTA_BIT			(U(1) << 20)
#endif /* TTA_BIT */
#endif /* TTA_BIT */
#ifndef TFP_BIT
#ifndef TFP_BIT
#define TFP_BIT			(U(1) << 10)
#endif /* TFP_BIT */
#endif /* TFP_BIT */
#ifndef CPTR_EL3_RESET_VAL
#ifndef CPTR_EL3_RESET_VAL
#define CPTR_EL3_RESET_VAL	U(0x0)
#endif /* CPTR_EL3_RESET_VAL */
#endif /* CPTR_EL3_RESET_VAL */

/* CPTR_EL2 definitions */
#ifndef CPTR_EL2_RES1
#ifndef CPTR_EL2_RES1
#define CPTR_EL2_RES1		((U(1) << 13) | (U(1) << 12) | (U(0x3ff)))
#endif /* CPTR_EL2_RES1 */
#endif /* CPTR_EL2_RES1 */
#ifndef CPTR_EL2_TCPAC_BIT
#ifndef CPTR_EL2_TCPAC_BIT
#define CPTR_EL2_TCPAC_BIT	(U(1) << 31)
#endif /* CPTR_EL2_TCPAC_BIT */
#endif /* CPTR_EL2_TCPAC_BIT */
#ifndef CPTR_EL2_TTA_BIT
#ifndef CPTR_EL2_TTA_BIT
#define CPTR_EL2_TTA_BIT	(U(1) << 20)
#endif /* CPTR_EL2_TTA_BIT */
#endif /* CPTR_EL2_TTA_BIT */
#ifndef CPTR_EL2_TFP_BIT
#ifndef CPTR_EL2_TFP_BIT
#define CPTR_EL2_TFP_BIT	(U(1) << 10)
#endif /* CPTR_EL2_TFP_BIT */
#endif /* CPTR_EL2_TFP_BIT */
#ifndef CPTR_EL2_RESET_VAL
#ifndef CPTR_EL2_RESET_VAL
#define CPTR_EL2_RESET_VAL	CPTR_EL2_RES1
#endif /* CPTR_EL2_RESET_VAL */
#endif /* CPTR_EL2_RESET_VAL */

/* CPSR/SPSR definitions */
#ifndef DAIF_FIQ_BIT
#ifndef DAIF_FIQ_BIT
#define DAIF_FIQ_BIT		(U(1) << 0)
#endif /* DAIF_FIQ_BIT */
#endif /* DAIF_FIQ_BIT */
#ifndef DAIF_IRQ_BIT
#ifndef DAIF_IRQ_BIT
#define DAIF_IRQ_BIT		(U(1) << 1)
#endif /* DAIF_IRQ_BIT */
#endif /* DAIF_IRQ_BIT */
#ifndef DAIF_ABT_BIT
#ifndef DAIF_ABT_BIT
#define DAIF_ABT_BIT		(U(1) << 2)
#endif /* DAIF_ABT_BIT */
#endif /* DAIF_ABT_BIT */
#ifndef DAIF_DBG_BIT
#ifndef DAIF_DBG_BIT
#define DAIF_DBG_BIT		(U(1) << 3)
#endif /* DAIF_DBG_BIT */
#endif /* DAIF_DBG_BIT */
#ifndef SPSR_DAIF_SHIFT
#ifndef SPSR_DAIF_SHIFT
#define SPSR_DAIF_SHIFT		U(6)
#endif /* SPSR_DAIF_SHIFT */
#endif /* SPSR_DAIF_SHIFT */
#ifndef SPSR_DAIF_MASK
#ifndef SPSR_DAIF_MASK
#define SPSR_DAIF_MASK		U(0xf)
#endif /* SPSR_DAIF_MASK */
#endif /* SPSR_DAIF_MASK */

#ifndef SPSR_AIF_SHIFT
#ifndef SPSR_AIF_SHIFT
#define SPSR_AIF_SHIFT		U(6)
#endif /* SPSR_AIF_SHIFT */
#endif /* SPSR_AIF_SHIFT */
#ifndef SPSR_AIF_MASK
#ifndef SPSR_AIF_MASK
#define SPSR_AIF_MASK		U(0x7)
#endif /* SPSR_AIF_MASK */
#endif /* SPSR_AIF_MASK */

#ifndef SPSR_E_SHIFT
#ifndef SPSR_E_SHIFT
#define SPSR_E_SHIFT		U(9)
#endif /* SPSR_E_SHIFT */
#endif /* SPSR_E_SHIFT */
#ifndef SPSR_E_MASK
#ifndef SPSR_E_MASK
#define SPSR_E_MASK		U(0x1)
#endif /* SPSR_E_MASK */
#endif /* SPSR_E_MASK */
#ifndef SPSR_E_LITTLE
#ifndef SPSR_E_LITTLE
#define SPSR_E_LITTLE		U(0x0)
#endif /* SPSR_E_LITTLE */
#endif /* SPSR_E_LITTLE */
#ifndef SPSR_E_BIG
#ifndef SPSR_E_BIG
#define SPSR_E_BIG		U(0x1)
#endif /* SPSR_E_BIG */
#endif /* SPSR_E_BIG */

#ifndef SPSR_T_SHIFT
#ifndef SPSR_T_SHIFT
#define SPSR_T_SHIFT		U(5)
#endif /* SPSR_T_SHIFT */
#endif /* SPSR_T_SHIFT */
#ifndef SPSR_T_MASK
#ifndef SPSR_T_MASK
#define SPSR_T_MASK		U(0x1)
#endif /* SPSR_T_MASK */
#endif /* SPSR_T_MASK */
#ifndef SPSR_T_ARM
#ifndef SPSR_T_ARM
#define SPSR_T_ARM		U(0x0)
#endif /* SPSR_T_ARM */
#endif /* SPSR_T_ARM */
#ifndef SPSR_T_THUMB
#ifndef SPSR_T_THUMB
#define SPSR_T_THUMB		U(0x1)
#endif /* SPSR_T_THUMB */
#endif /* SPSR_T_THUMB */

#ifndef DISABLE_ALL_EXCEPTIONS
#ifndef DISABLE_ALL_EXCEPTIONS
#define DISABLE_ALL_EXCEPTIONS \
		(DAIF_FIQ_BIT | DAIF_IRQ_BIT | DAIF_ABT_BIT | DAIF_DBG_BIT)
#endif /* DISABLE_ALL_EXCEPTIONS */
#endif /* DISABLE_ALL_EXCEPTIONS */

/*
 * RMR_EL3 definitions
 */
#ifndef RMR_EL3_RR_BIT
#ifndef RMR_EL3_RR_BIT
#define RMR_EL3_RR_BIT		(U(1) << 1)
#endif /* RMR_EL3_RR_BIT */
#endif /* RMR_EL3_RR_BIT */
#ifndef RMR_EL3_AA64_BIT
#ifndef RMR_EL3_AA64_BIT
#define RMR_EL3_AA64_BIT	(U(1) << 0)
#endif /* RMR_EL3_AA64_BIT */
#endif /* RMR_EL3_AA64_BIT */

/*
 * HI-VECTOR address for AArch32 state
 */
#ifndef HI_VECTOR_BASE
#ifndef HI_VECTOR_BASE
#define HI_VECTOR_BASE	U(0xFFFF0000)
#endif /* HI_VECTOR_BASE */
#endif /* HI_VECTOR_BASE */

/*
 * TCR defintions
 */
#ifndef TCR_EL3_RES1
#ifndef TCR_EL3_RES1
#define TCR_EL3_RES1		((1UL << 31) | (1UL << 23))
#endif /* TCR_EL3_RES1 */
#endif /* TCR_EL3_RES1 */
#ifndef TCR_EL1_IPS_SHIFT
#ifndef TCR_EL1_IPS_SHIFT
#define TCR_EL1_IPS_SHIFT	U(32)
#endif /* TCR_EL1_IPS_SHIFT */
#endif /* TCR_EL1_IPS_SHIFT */
#ifndef TCR_EL3_PS_SHIFT
#ifndef TCR_EL3_PS_SHIFT
#define TCR_EL3_PS_SHIFT	U(16)
#endif /* TCR_EL3_PS_SHIFT */
#endif /* TCR_EL3_PS_SHIFT */

#ifndef TCR_TxSZ_MIN
#ifndef TCR_TxSZ_MIN
#define TCR_TxSZ_MIN		U(16)
#endif /* TCR_TxSZ_MIN */
#endif /* TCR_TxSZ_MIN */
#ifndef TCR_TxSZ_MAX
#ifndef TCR_TxSZ_MAX
#define TCR_TxSZ_MAX		U(39)
#endif /* TCR_TxSZ_MAX */
#endif /* TCR_TxSZ_MAX */

/* (internal) physical address size bits in EL3/EL1 */
#ifndef TCR_PS_BITS_4GB
#ifndef TCR_PS_BITS_4GB
#define TCR_PS_BITS_4GB		U(0x0)
#endif /* TCR_PS_BITS_4GB */
#endif /* TCR_PS_BITS_4GB */
#ifndef TCR_PS_BITS_64GB
#ifndef TCR_PS_BITS_64GB
#define TCR_PS_BITS_64GB	U(0x1)
#endif /* TCR_PS_BITS_64GB */
#endif /* TCR_PS_BITS_64GB */
#ifndef TCR_PS_BITS_1TB
#ifndef TCR_PS_BITS_1TB
#define TCR_PS_BITS_1TB		U(0x2)
#endif /* TCR_PS_BITS_1TB */
#endif /* TCR_PS_BITS_1TB */
#ifndef TCR_PS_BITS_4TB
#ifndef TCR_PS_BITS_4TB
#define TCR_PS_BITS_4TB		U(0x3)
#endif /* TCR_PS_BITS_4TB */
#endif /* TCR_PS_BITS_4TB */
#ifndef TCR_PS_BITS_16TB
#ifndef TCR_PS_BITS_16TB
#define TCR_PS_BITS_16TB	U(0x4)
#endif /* TCR_PS_BITS_16TB */
#endif /* TCR_PS_BITS_16TB */
#ifndef TCR_PS_BITS_256TB
#ifndef TCR_PS_BITS_256TB
#define TCR_PS_BITS_256TB	U(0x5)
#endif /* TCR_PS_BITS_256TB */
#endif /* TCR_PS_BITS_256TB */

#ifndef ADDR_MASK_48_TO_63
#ifndef ADDR_MASK_48_TO_63
#define ADDR_MASK_48_TO_63	ULL(0xFFFF000000000000)
#endif /* ADDR_MASK_48_TO_63 */
#endif /* ADDR_MASK_48_TO_63 */
#ifndef ADDR_MASK_44_TO_47
#ifndef ADDR_MASK_44_TO_47
#define ADDR_MASK_44_TO_47	ULL(0x0000F00000000000)
#endif /* ADDR_MASK_44_TO_47 */
#endif /* ADDR_MASK_44_TO_47 */
#ifndef ADDR_MASK_42_TO_43
#ifndef ADDR_MASK_42_TO_43
#define ADDR_MASK_42_TO_43	ULL(0x00000C0000000000)
#endif /* ADDR_MASK_42_TO_43 */
#endif /* ADDR_MASK_42_TO_43 */
#ifndef ADDR_MASK_40_TO_41
#ifndef ADDR_MASK_40_TO_41
#define ADDR_MASK_40_TO_41	ULL(0x0000030000000000)
#endif /* ADDR_MASK_40_TO_41 */
#endif /* ADDR_MASK_40_TO_41 */
#ifndef ADDR_MASK_36_TO_39
#ifndef ADDR_MASK_36_TO_39
#define ADDR_MASK_36_TO_39	ULL(0x000000F000000000)
#endif /* ADDR_MASK_36_TO_39 */
#endif /* ADDR_MASK_36_TO_39 */
#ifndef ADDR_MASK_32_TO_35
#ifndef ADDR_MASK_32_TO_35
#define ADDR_MASK_32_TO_35	ULL(0x0000000F00000000)
#endif /* ADDR_MASK_32_TO_35 */
#endif /* ADDR_MASK_32_TO_35 */

#ifndef TCR_RGN_INNER_NC
#ifndef TCR_RGN_INNER_NC
#define TCR_RGN_INNER_NC	(U(0x0) << 8)
#endif /* TCR_RGN_INNER_NC */
#endif /* TCR_RGN_INNER_NC */
#ifndef TCR_RGN_INNER_WBA
#ifndef TCR_RGN_INNER_WBA
#define TCR_RGN_INNER_WBA	(U(0x1) << 8)
#endif /* TCR_RGN_INNER_WBA */
#endif /* TCR_RGN_INNER_WBA */
#ifndef TCR_RGN_INNER_WT
#ifndef TCR_RGN_INNER_WT
#define TCR_RGN_INNER_WT	(U(0x2) << 8)
#endif /* TCR_RGN_INNER_WT */
#endif /* TCR_RGN_INNER_WT */
#ifndef TCR_RGN_INNER_WBNA
#ifndef TCR_RGN_INNER_WBNA
#define TCR_RGN_INNER_WBNA	(U(0x3) << 8)
#endif /* TCR_RGN_INNER_WBNA */
#endif /* TCR_RGN_INNER_WBNA */

#ifndef TCR_RGN_OUTER_NC
#ifndef TCR_RGN_OUTER_NC
#define TCR_RGN_OUTER_NC	(U(0x0) << 10)
#endif /* TCR_RGN_OUTER_NC */
#endif /* TCR_RGN_OUTER_NC */
#ifndef TCR_RGN_OUTER_WBA
#ifndef TCR_RGN_OUTER_WBA
#define TCR_RGN_OUTER_WBA	(U(0x1) << 10)
#endif /* TCR_RGN_OUTER_WBA */
#endif /* TCR_RGN_OUTER_WBA */
#ifndef TCR_RGN_OUTER_WT
#ifndef TCR_RGN_OUTER_WT
#define TCR_RGN_OUTER_WT	(U(0x2) << 10)
#endif /* TCR_RGN_OUTER_WT */
#endif /* TCR_RGN_OUTER_WT */
#ifndef TCR_RGN_OUTER_WBNA
#ifndef TCR_RGN_OUTER_WBNA
#define TCR_RGN_OUTER_WBNA	(U(0x3) << 10)
#endif /* TCR_RGN_OUTER_WBNA */
#endif /* TCR_RGN_OUTER_WBNA */

#ifndef TCR_SH_NON_SHAREABLE
#ifndef TCR_SH_NON_SHAREABLE
#define TCR_SH_NON_SHAREABLE	(U(0x0) << 12)
#endif /* TCR_SH_NON_SHAREABLE */
#endif /* TCR_SH_NON_SHAREABLE */
#ifndef TCR_SH_OUTER_SHAREABLE
#ifndef TCR_SH_OUTER_SHAREABLE
#define TCR_SH_OUTER_SHAREABLE	(U(0x2) << 12)
#endif /* TCR_SH_OUTER_SHAREABLE */
#endif /* TCR_SH_OUTER_SHAREABLE */
#ifndef TCR_SH_INNER_SHAREABLE
#ifndef TCR_SH_INNER_SHAREABLE
#define TCR_SH_INNER_SHAREABLE	(U(0x3) << 12)
#endif /* TCR_SH_INNER_SHAREABLE */
#endif /* TCR_SH_INNER_SHAREABLE */

#ifndef TCR_TG0_4KB
#ifndef TCR_TG0_4KB
#define TCR_TG0_4KB	(U(0x0) << 14)
#endif /* TCR_TG0_4KB */
#endif /* TCR_TG0_4KB */
#ifndef TCR_TG0_64KB
#ifndef TCR_TG0_64KB
#define TCR_TG0_64KB	(U(0x1) << 14)
#endif /* TCR_TG0_64KB */
#endif /* TCR_TG0_64KB */

#ifndef MODE_SP_SHIFT
#ifndef MODE_SP_SHIFT
#define MODE_SP_SHIFT		U(0x0)
#endif /* MODE_SP_SHIFT */
#endif /* MODE_SP_SHIFT */
#ifndef MODE_SP_MASK
#ifndef MODE_SP_MASK
#define MODE_SP_MASK		U(0x1)
#endif /* MODE_SP_MASK */
#endif /* MODE_SP_MASK */
#ifndef MODE_SP_EL0
#ifndef MODE_SP_EL0
#define MODE_SP_EL0		U(0x0)
#endif /* MODE_SP_EL0 */
#endif /* MODE_SP_EL0 */
#ifndef MODE_SP_ELX
#ifndef MODE_SP_ELX
#define MODE_SP_ELX		U(0x1)
#endif /* MODE_SP_ELX */
#endif /* MODE_SP_ELX */

#ifndef MODE_RW_SHIFT
#ifndef MODE_RW_SHIFT
#define MODE_RW_SHIFT		U(0x4)
#endif /* MODE_RW_SHIFT */
#endif /* MODE_RW_SHIFT */
#ifndef MODE_RW_MASK
#ifndef MODE_RW_MASK
#define MODE_RW_MASK		U(0x1)
#endif /* MODE_RW_MASK */
#endif /* MODE_RW_MASK */
#ifndef MODE_RW_64
#ifndef MODE_RW_64
#define MODE_RW_64		U(0x0)
#endif /* MODE_RW_64 */
#endif /* MODE_RW_64 */
#ifndef MODE_RW_32
#ifndef MODE_RW_32
#define MODE_RW_32		U(0x1)
#endif /* MODE_RW_32 */
#endif /* MODE_RW_32 */

#ifndef MODE_EL_SHIFT
#ifndef MODE_EL_SHIFT
#define MODE_EL_SHIFT		U(0x2)
#endif /* MODE_EL_SHIFT */
#endif /* MODE_EL_SHIFT */
#ifndef MODE_EL_MASK
#ifndef MODE_EL_MASK
#define MODE_EL_MASK		U(0x3)
#endif /* MODE_EL_MASK */
#endif /* MODE_EL_MASK */
#ifndef MODE_EL3
#ifndef MODE_EL3
#define MODE_EL3		U(0x3)
#endif /* MODE_EL3 */
#endif /* MODE_EL3 */
#ifndef MODE_EL2
#ifndef MODE_EL2
#define MODE_EL2		U(0x2)
#endif /* MODE_EL2 */
#endif /* MODE_EL2 */
#ifndef MODE_EL1
#ifndef MODE_EL1
#define MODE_EL1		U(0x1)
#endif /* MODE_EL1 */
#endif /* MODE_EL1 */
#ifndef MODE_EL0
#ifndef MODE_EL0
#define MODE_EL0		U(0x0)
#endif /* MODE_EL0 */
#endif /* MODE_EL0 */

#ifndef MODE32_SHIFT
#ifndef MODE32_SHIFT
#define MODE32_SHIFT		U(0)
#endif /* MODE32_SHIFT */
#endif /* MODE32_SHIFT */
#ifndef MODE32_MASK
#ifndef MODE32_MASK
#define MODE32_MASK		U(0xf)
#endif /* MODE32_MASK */
#endif /* MODE32_MASK */
#ifndef MODE32_usr
#ifndef MODE32_usr
#define MODE32_usr		U(0x0)
#endif /* MODE32_usr */
#endif /* MODE32_usr */
#ifndef MODE32_fiq
#ifndef MODE32_fiq
#define MODE32_fiq		U(0x1)
#endif /* MODE32_fiq */
#endif /* MODE32_fiq */
#ifndef MODE32_irq
#ifndef MODE32_irq
#define MODE32_irq		U(0x2)
#endif /* MODE32_irq */
#endif /* MODE32_irq */
#ifndef MODE32_svc
#ifndef MODE32_svc
#define MODE32_svc		U(0x3)
#endif /* MODE32_svc */
#endif /* MODE32_svc */
#ifndef MODE32_mon
#ifndef MODE32_mon
#define MODE32_mon		U(0x6)
#endif /* MODE32_mon */
#endif /* MODE32_mon */
#ifndef MODE32_abt
#ifndef MODE32_abt
#define MODE32_abt		U(0x7)
#endif /* MODE32_abt */
#endif /* MODE32_abt */
#ifndef MODE32_hyp
#ifndef MODE32_hyp
#define MODE32_hyp		U(0xa)
#endif /* MODE32_hyp */
#endif /* MODE32_hyp */
#ifndef MODE32_und
#ifndef MODE32_und
#define MODE32_und		U(0xb)
#endif /* MODE32_und */
#endif /* MODE32_und */
#ifndef MODE32_sys
#ifndef MODE32_sys
#define MODE32_sys		U(0xf)
#endif /* MODE32_sys */
#endif /* MODE32_sys */

#ifndef GET_RW
#define GET_RW(mode)		(((mode) >> MODE_RW_SHIFT) & MODE_RW_MASK)
#endif /* GET_RW */
#ifndef GET_EL
#define GET_EL(mode)		(((mode) >> MODE_EL_SHIFT) & MODE_EL_MASK)
#endif /* GET_EL */
#ifndef GET_SP
#define GET_SP(mode)		(((mode) >> MODE_SP_SHIFT) & MODE_SP_MASK)
#endif /* GET_SP */
#ifndef GET_M32
#define GET_M32(mode)		(((mode) >> MODE32_SHIFT) & MODE32_MASK)
#endif /* GET_M32 */

#ifndef SPSR_64
#define SPSR_64(el, sp, daif)				\
	(MODE_RW_64 << MODE_RW_SHIFT |			\
	((el) & MODE_EL_MASK) << MODE_EL_SHIFT |	\
	((sp) & MODE_SP_MASK) << MODE_SP_SHIFT |	\
	((daif) & SPSR_DAIF_MASK) << SPSR_DAIF_SHIFT)
#endif /* SPSR_64 */

#ifndef SPSR_MODE32
#define SPSR_MODE32(mode, isa, endian, aif)		\
	((MODE_RW_32 << MODE_RW_SHIFT) |		\
	(((mode) & MODE32_MASK) << MODE32_SHIFT) |	\
	(((isa) & SPSR_T_MASK) << SPSR_T_SHIFT) |	\
	(((endian) & SPSR_E_MASK) << SPSR_E_SHIFT) |	\
	(((aif) & SPSR_AIF_MASK) << SPSR_AIF_SHIFT))
#endif /* SPSR_MODE32 */

/*
 * CTR_EL0 definitions
 */
#ifndef CTR_CWG_SHIFT
#ifndef CTR_CWG_SHIFT
#define CTR_CWG_SHIFT		U(24)
#endif /* CTR_CWG_SHIFT */
#endif /* CTR_CWG_SHIFT */
#ifndef CTR_CWG_MASK
#ifndef CTR_CWG_MASK
#define CTR_CWG_MASK		U(0xf)
#endif /* CTR_CWG_MASK */
#endif /* CTR_CWG_MASK */
#ifndef CTR_ERG_SHIFT
#ifndef CTR_ERG_SHIFT
#define CTR_ERG_SHIFT		U(20)
#endif /* CTR_ERG_SHIFT */
#endif /* CTR_ERG_SHIFT */
#ifndef CTR_ERG_MASK
#ifndef CTR_ERG_MASK
#define CTR_ERG_MASK		U(0xf)
#endif /* CTR_ERG_MASK */
#endif /* CTR_ERG_MASK */
#ifndef CTR_DMINLINE_SHIFT
#ifndef CTR_DMINLINE_SHIFT
#define CTR_DMINLINE_SHIFT	U(16)
#endif /* CTR_DMINLINE_SHIFT */
#endif /* CTR_DMINLINE_SHIFT */
#ifndef CTR_DMINLINE_MASK
#ifndef CTR_DMINLINE_MASK
#define CTR_DMINLINE_MASK	U(0xf)
#endif /* CTR_DMINLINE_MASK */
#endif /* CTR_DMINLINE_MASK */
#ifndef CTR_L1IP_SHIFT
#ifndef CTR_L1IP_SHIFT
#define CTR_L1IP_SHIFT		U(14)
#endif /* CTR_L1IP_SHIFT */
#endif /* CTR_L1IP_SHIFT */
#ifndef CTR_L1IP_MASK
#ifndef CTR_L1IP_MASK
#define CTR_L1IP_MASK		U(0x3)
#endif /* CTR_L1IP_MASK */
#endif /* CTR_L1IP_MASK */
#ifndef CTR_IMINLINE_SHIFT
#ifndef CTR_IMINLINE_SHIFT
#define CTR_IMINLINE_SHIFT	U(0)
#endif /* CTR_IMINLINE_SHIFT */
#endif /* CTR_IMINLINE_SHIFT */
#ifndef CTR_IMINLINE_MASK
#ifndef CTR_IMINLINE_MASK
#define CTR_IMINLINE_MASK	U(0xf)
#endif /* CTR_IMINLINE_MASK */
#endif /* CTR_IMINLINE_MASK */

#ifndef MAX_CACHE_LINE_SIZE
#ifndef MAX_CACHE_LINE_SIZE
#define MAX_CACHE_LINE_SIZE	U(0x800) /* 2KB */
#endif /* MAX_CACHE_LINE_SIZE */
#endif /* MAX_CACHE_LINE_SIZE */

/* Physical timer control register bit fields shifts and masks */
#ifndef CNTP_CTL_ENABLE_SHIFT
#ifndef CNTP_CTL_ENABLE_SHIFT
#define CNTP_CTL_ENABLE_SHIFT   U(0)
#endif /* CNTP_CTL_ENABLE_SHIFT */
#endif /* CNTP_CTL_ENABLE_SHIFT */
#ifndef CNTP_CTL_IMASK_SHIFT
#ifndef CNTP_CTL_IMASK_SHIFT
#define CNTP_CTL_IMASK_SHIFT    U(1)
#endif /* CNTP_CTL_IMASK_SHIFT */
#endif /* CNTP_CTL_IMASK_SHIFT */
#ifndef CNTP_CTL_ISTATUS_SHIFT
#ifndef CNTP_CTL_ISTATUS_SHIFT
#define CNTP_CTL_ISTATUS_SHIFT  U(2)
#endif /* CNTP_CTL_ISTATUS_SHIFT */
#endif /* CNTP_CTL_ISTATUS_SHIFT */

#ifndef CNTP_CTL_ENABLE_MASK
#ifndef CNTP_CTL_ENABLE_MASK
#define CNTP_CTL_ENABLE_MASK    U(1)
#endif /* CNTP_CTL_ENABLE_MASK */
#endif /* CNTP_CTL_ENABLE_MASK */
#ifndef CNTP_CTL_IMASK_MASK
#ifndef CNTP_CTL_IMASK_MASK
#define CNTP_CTL_IMASK_MASK     U(1)
#endif /* CNTP_CTL_IMASK_MASK */
#endif /* CNTP_CTL_IMASK_MASK */
#ifndef CNTP_CTL_ISTATUS_MASK
#ifndef CNTP_CTL_ISTATUS_MASK
#define CNTP_CTL_ISTATUS_MASK   U(1)
#endif /* CNTP_CTL_ISTATUS_MASK */
#endif /* CNTP_CTL_ISTATUS_MASK */

#ifndef get_cntp_ctl_enable
#define get_cntp_ctl_enable(x)  (((x) >> CNTP_CTL_ENABLE_SHIFT) & \
					CNTP_CTL_ENABLE_MASK)
#endif /* get_cntp_ctl_enable */
#ifndef get_cntp_ctl_imask
#define get_cntp_ctl_imask(x)   (((x) >> CNTP_CTL_IMASK_SHIFT) & \
					CNTP_CTL_IMASK_MASK)
#endif /* get_cntp_ctl_imask */
#ifndef get_cntp_ctl_istatus
#define get_cntp_ctl_istatus(x) (((x) >> CNTP_CTL_ISTATUS_SHIFT) & \
					CNTP_CTL_ISTATUS_MASK)
#endif /* get_cntp_ctl_istatus */

#ifndef set_cntp_ctl_enable
#define set_cntp_ctl_enable(x)  ((x) |= (U(1) << CNTP_CTL_ENABLE_SHIFT))
#endif /* set_cntp_ctl_enable */
#ifndef set_cntp_ctl_imask
#define set_cntp_ctl_imask(x)   ((x) |= (U(1) << CNTP_CTL_IMASK_SHIFT))
#endif /* set_cntp_ctl_imask */

#ifndef clr_cntp_ctl_enable
#define clr_cntp_ctl_enable(x)  ((x) &= ~(U(1) << CNTP_CTL_ENABLE_SHIFT))
#endif /* clr_cntp_ctl_enable */
#ifndef clr_cntp_ctl_imask
#define clr_cntp_ctl_imask(x)   ((x) &= ~(U(1) << CNTP_CTL_IMASK_SHIFT))
#endif /* clr_cntp_ctl_imask */

/* Exception Syndrome register bits and bobs */
#ifndef ESR_EC_SHIFT
#ifndef ESR_EC_SHIFT
#define ESR_EC_SHIFT			U(26)
#endif /* ESR_EC_SHIFT */
#endif /* ESR_EC_SHIFT */
#ifndef ESR_EC_MASK
#ifndef ESR_EC_MASK
#define ESR_EC_MASK			U(0x3f)
#endif /* ESR_EC_MASK */
#endif /* ESR_EC_MASK */
#ifndef ESR_EC_LENGTH
#ifndef ESR_EC_LENGTH
#define ESR_EC_LENGTH			U(6)
#endif /* ESR_EC_LENGTH */
#endif /* ESR_EC_LENGTH */
#ifndef EC_UNKNOWN
#ifndef EC_UNKNOWN
#define EC_UNKNOWN			U(0x0)
#endif /* EC_UNKNOWN */
#endif /* EC_UNKNOWN */
#ifndef EC_WFE_WFI
#ifndef EC_WFE_WFI
#define EC_WFE_WFI			U(0x1)
#endif /* EC_WFE_WFI */
#endif /* EC_WFE_WFI */
#ifndef EC_AARCH32_CP15_MRC_MCR
#ifndef EC_AARCH32_CP15_MRC_MCR
#define EC_AARCH32_CP15_MRC_MCR		U(0x3)
#endif /* EC_AARCH32_CP15_MRC_MCR */
#endif /* EC_AARCH32_CP15_MRC_MCR */
#ifndef EC_AARCH32_CP15_MRRC_MCRR
#ifndef EC_AARCH32_CP15_MRRC_MCRR
#define EC_AARCH32_CP15_MRRC_MCRR	U(0x4)
#endif /* EC_AARCH32_CP15_MRRC_MCRR */
#endif /* EC_AARCH32_CP15_MRRC_MCRR */
#ifndef EC_AARCH32_CP14_MRC_MCR
#ifndef EC_AARCH32_CP14_MRC_MCR
#define EC_AARCH32_CP14_MRC_MCR		U(0x5)
#endif /* EC_AARCH32_CP14_MRC_MCR */
#endif /* EC_AARCH32_CP14_MRC_MCR */
#ifndef EC_AARCH32_CP14_LDC_STC
#ifndef EC_AARCH32_CP14_LDC_STC
#define EC_AARCH32_CP14_LDC_STC		U(0x6)
#endif /* EC_AARCH32_CP14_LDC_STC */
#endif /* EC_AARCH32_CP14_LDC_STC */
#ifndef EC_FP_SIMD
#ifndef EC_FP_SIMD
#define EC_FP_SIMD			U(0x7)
#endif /* EC_FP_SIMD */
#endif /* EC_FP_SIMD */
#ifndef EC_AARCH32_CP10_MRC
#ifndef EC_AARCH32_CP10_MRC
#define EC_AARCH32_CP10_MRC		U(0x8)
#endif /* EC_AARCH32_CP10_MRC */
#endif /* EC_AARCH32_CP10_MRC */
#ifndef EC_AARCH32_CP14_MRRC_MCRR
#ifndef EC_AARCH32_CP14_MRRC_MCRR
#define EC_AARCH32_CP14_MRRC_MCRR	U(0xc)
#endif /* EC_AARCH32_CP14_MRRC_MCRR */
#endif /* EC_AARCH32_CP14_MRRC_MCRR */
#ifndef EC_ILLEGAL
#ifndef EC_ILLEGAL
#define EC_ILLEGAL			U(0xe)
#endif /* EC_ILLEGAL */
#endif /* EC_ILLEGAL */
#ifndef EC_AARCH32_SVC
#ifndef EC_AARCH32_SVC
#define EC_AARCH32_SVC			U(0x11)
#endif /* EC_AARCH32_SVC */
#endif /* EC_AARCH32_SVC */
#ifndef EC_AARCH32_HVC
#ifndef EC_AARCH32_HVC
#define EC_AARCH32_HVC			U(0x12)
#endif /* EC_AARCH32_HVC */
#endif /* EC_AARCH32_HVC */
#ifndef EC_AARCH32_SMC
#ifndef EC_AARCH32_SMC
#define EC_AARCH32_SMC			U(0x13)
#endif /* EC_AARCH32_SMC */
#endif /* EC_AARCH32_SMC */
#ifndef EC_AARCH64_SVC
#ifndef EC_AARCH64_SVC
#define EC_AARCH64_SVC			U(0x15)
#endif /* EC_AARCH64_SVC */
#endif /* EC_AARCH64_SVC */
#ifndef EC_AARCH64_HVC
#ifndef EC_AARCH64_HVC
#define EC_AARCH64_HVC			U(0x16)
#endif /* EC_AARCH64_HVC */
#endif /* EC_AARCH64_HVC */
#ifndef EC_AARCH64_SMC
#ifndef EC_AARCH64_SMC
#define EC_AARCH64_SMC			U(0x17)
#endif /* EC_AARCH64_SMC */
#endif /* EC_AARCH64_SMC */
#ifndef EC_AARCH64_SYS
#ifndef EC_AARCH64_SYS
#define EC_AARCH64_SYS			U(0x18)
#endif /* EC_AARCH64_SYS */
#endif /* EC_AARCH64_SYS */
#ifndef EC_IABORT_LOWER_EL
#ifndef EC_IABORT_LOWER_EL
#define EC_IABORT_LOWER_EL		U(0x20)
#endif /* EC_IABORT_LOWER_EL */
#endif /* EC_IABORT_LOWER_EL */
#ifndef EC_IABORT_CUR_EL
#ifndef EC_IABORT_CUR_EL
#define EC_IABORT_CUR_EL		U(0x21)
#endif /* EC_IABORT_CUR_EL */
#endif /* EC_IABORT_CUR_EL */
#ifndef EC_PC_ALIGN
#ifndef EC_PC_ALIGN
#define EC_PC_ALIGN			U(0x22)
#endif /* EC_PC_ALIGN */
#endif /* EC_PC_ALIGN */
#ifndef EC_DABORT_LOWER_EL
#ifndef EC_DABORT_LOWER_EL
#define EC_DABORT_LOWER_EL		U(0x24)
#endif /* EC_DABORT_LOWER_EL */
#endif /* EC_DABORT_LOWER_EL */
#ifndef EC_DABORT_CUR_EL
#ifndef EC_DABORT_CUR_EL
#define EC_DABORT_CUR_EL		U(0x25)
#endif /* EC_DABORT_CUR_EL */
#endif /* EC_DABORT_CUR_EL */
#ifndef EC_SP_ALIGN
#ifndef EC_SP_ALIGN
#define EC_SP_ALIGN			U(0x26)
#endif /* EC_SP_ALIGN */
#endif /* EC_SP_ALIGN */
#ifndef EC_AARCH32_FP
#ifndef EC_AARCH32_FP
#define EC_AARCH32_FP			U(0x28)
#endif /* EC_AARCH32_FP */
#endif /* EC_AARCH32_FP */
#ifndef EC_AARCH64_FP
#ifndef EC_AARCH64_FP
#define EC_AARCH64_FP			U(0x2c)
#endif /* EC_AARCH64_FP */
#endif /* EC_AARCH64_FP */
#ifndef EC_SERROR
#ifndef EC_SERROR
#define EC_SERROR			U(0x2f)
#endif /* EC_SERROR */
#endif /* EC_SERROR */

#ifndef EC_BITS
#define EC_BITS(x)			(((x) >> ESR_EC_SHIFT) & ESR_EC_MASK)
#endif /* EC_BITS */

/* Reset bit inside the Reset management register for EL3 (RMR_EL3) */
#ifndef RMR_RESET_REQUEST_SHIFT
#ifndef RMR_RESET_REQUEST_SHIFT
#define RMR_RESET_REQUEST_SHIFT 	U(0x1)
#endif /* RMR_RESET_REQUEST_SHIFT */
#endif /* RMR_RESET_REQUEST_SHIFT */
#ifndef RMR_WARM_RESET_CPU
#ifndef RMR_WARM_RESET_CPU
#define RMR_WARM_RESET_CPU		(U(1) << RMR_RESET_REQUEST_SHIFT)
#endif /* RMR_WARM_RESET_CPU */
#endif /* RMR_WARM_RESET_CPU */

/*******************************************************************************
 * Definitions of register offsets, fields and macros for CPU system
 * instructions.
 ******************************************************************************/

#ifndef TLBI_ADDR_SHIFT
#ifndef TLBI_ADDR_SHIFT
#define TLBI_ADDR_SHIFT		U(12)
#endif /* TLBI_ADDR_SHIFT */
#endif /* TLBI_ADDR_SHIFT */
#ifndef TLBI_ADDR_MASK
#ifndef TLBI_ADDR_MASK
#define TLBI_ADDR_MASK		ULL(0x00000FFFFFFFFFFF)
#endif /* TLBI_ADDR_MASK */
#endif /* TLBI_ADDR_MASK */
#ifndef TLBI_ADDR
#define TLBI_ADDR(x)		(((x) >> TLBI_ADDR_SHIFT) & TLBI_ADDR_MASK)
#endif /* TLBI_ADDR */

/*******************************************************************************
 * Definitions of register offsets and fields in the CNTCTLBase Frame of the
 * system level implementation of the Generic Timer.
 ******************************************************************************/
#ifndef CNTNSAR
#ifndef CNTNSAR
#define CNTNSAR			U(0x4)
#endif /* CNTNSAR */
#endif /* CNTNSAR */
#ifndef CNTNSAR_NS_SHIFT
#define CNTNSAR_NS_SHIFT(x)	(x)
#endif /* CNTNSAR_NS_SHIFT */

#ifndef CNTACR_BASE
#define CNTACR_BASE(x)		(U(0x40) + ((x) << 2))
#endif /* CNTACR_BASE */
#ifndef CNTACR_RPCT_SHIFT
#ifndef CNTACR_RPCT_SHIFT
#define CNTACR_RPCT_SHIFT	U(0x0)
#endif /* CNTACR_RPCT_SHIFT */
#endif /* CNTACR_RPCT_SHIFT */
#ifndef CNTACR_RVCT_SHIFT
#ifndef CNTACR_RVCT_SHIFT
#define CNTACR_RVCT_SHIFT	U(0x1)
#endif /* CNTACR_RVCT_SHIFT */
#endif /* CNTACR_RVCT_SHIFT */
#ifndef CNTACR_RFRQ_SHIFT
#ifndef CNTACR_RFRQ_SHIFT
#define CNTACR_RFRQ_SHIFT	U(0x2)
#endif /* CNTACR_RFRQ_SHIFT */
#endif /* CNTACR_RFRQ_SHIFT */
#ifndef CNTACR_RVOFF_SHIFT
#ifndef CNTACR_RVOFF_SHIFT
#define CNTACR_RVOFF_SHIFT	U(0x3)
#endif /* CNTACR_RVOFF_SHIFT */
#endif /* CNTACR_RVOFF_SHIFT */
#ifndef CNTACR_RWVT_SHIFT
#ifndef CNTACR_RWVT_SHIFT
#define CNTACR_RWVT_SHIFT	U(0x4)
#endif /* CNTACR_RWVT_SHIFT */
#endif /* CNTACR_RWVT_SHIFT */
#ifndef CNTACR_RWPT_SHIFT
#ifndef CNTACR_RWPT_SHIFT
#define CNTACR_RWPT_SHIFT	U(0x5)
#endif /* CNTACR_RWPT_SHIFT */
#endif /* CNTACR_RWPT_SHIFT */

/* PMCR_EL0 definitions */
#ifndef PMCR_EL0_N_SHIFT
#ifndef PMCR_EL0_N_SHIFT
#define PMCR_EL0_N_SHIFT	U(11)
#endif /* PMCR_EL0_N_SHIFT */
#endif /* PMCR_EL0_N_SHIFT */
#ifndef PMCR_EL0_N_MASK
#ifndef PMCR_EL0_N_MASK
#define PMCR_EL0_N_MASK		U(0x1f)
#endif /* PMCR_EL0_N_MASK */
#endif /* PMCR_EL0_N_MASK */
#ifndef PMCR_EL0_N_BITS
#ifndef PMCR_EL0_N_BITS
#define PMCR_EL0_N_BITS		(PMCR_EL0_N_MASK << PMCR_EL0_N_SHIFT)
#endif /* PMCR_EL0_N_BITS */
#endif /* PMCR_EL0_N_BITS */

#endif /* ARCH_H */
