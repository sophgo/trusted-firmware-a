#
# Copyright (c) 2024, CVITEK, Inc. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

CVITEK_PLAT		:=	plat/cvitek
CVITEK_PLAT_SOC		:=	${CVITEK_PLAT}/${PLAT}
CVITEK_PLAT_COMMON	:=	${CVITEK_PLAT_SOC}/common

CVITEK_PLAT_SOC_INC	:=	${CVITEK_PLAT_SOC}/include
CV186X_INC		:=	${CVITEK_PLAT_SOC_INC}

CHIP_ARCH		:=	cv84x6

$(eval $(call add_define,CV84X6))

DISABLE_BIN_GENERATION	:=	0
include lib/libfdt/libfdt.mk
include lib/xlat_tables_v2/xlat_tables.mk

# GIC-600 configuration
GICV3_IMPL		:=	GIC600
GICV3_SUPPORT_GIC600	:=	1
GIC_ENABLE_V4_EXTN	:=	1
# GICV2_IMPL		:=	GIC200
# GICV2_SUPPORT_GIC200	:=	1

# Include GICv3 driver files
include drivers/arm/gic/v3/gicv3.mk

BL31_LINKERFILE		:=	bl31/bl31.ld.S
BL31_DEFAULT_LINKER_SCRIPT_SOURCE := bl31/bl31.ld.S
BL32_DEFAULT_LINKER_SCRIPT_SOURCE := bl32/cvsp/cvsp.ld.S
BL32_LDFLAGS		+=	-Wl,-no-pie
BL2_CFLAGS		+=	-fno-pie -fno-PIE
BL2_CPPFLAGS		+=	-DDIRECT_BL31_FROM_BL2=1
BL2_ASFLAGS		+=	-fno-pie
BL2_DEFAULT_LINKER_SCRIPT_SOURCE := bl2/bl2.ld.S
# Removed -Wl,-no-pie: old ld (6.3.1) treats -no-pie as -n -o pie, breaking link
BL2_LDFLAGS		+=	-Wl,--defsym=__RELA_START__=0 -Wl,--defsym=__RELA_END__=0

PLAT_INCLUDES		:=	-I${CVITEK_PLAT_SOC}/include/arch/		\
				-Iinclude/plat/common				\
				-Iinclude/plat/arm/common/aarch64/		\
				-Iinclude/plat/arm/common/			\
				-Iinclude/bl31					\
				-Iinclude/common				\
				-Iinclude/drivers				\
				-Iinclude/drivers/arm				\
				-Iinclude/drivers/auth				\
				-Iinclude/lib/psci				\
				-Iinclude/drivers/ti/uart			\
				-Iinclude/lib/xlat_tables/			\
				-Iinclude/lib/xlat_tables/aarch64/		\
				-Iinclude/lib					\
				-Iinclude/lib/cpus/${ARCH}			\
				-Iinclude/lib/el3_runtime			\
				-Iinclude/services				\
				-Idrivers/arm/gic/v3/				\
				-I${CVITEK_PLAT_COMMON}/aarch64/		\
				-I${CVITEK_PLAT_SOC}/				\
				-I${CVITEK_PLAT_SOC}/${SUBTYPE}/include/	\
				-I${CVITEK_PLAT_SOC}/include/			\
				-I${CVITEK_PLAT_SOC}/include/drivers/		\
				-I${CVITEK_PLAT_SOC}/include/drivers/cvitek/${PLAT}/emmc/	\
				-I${CVITEK_PLAT_SOC}/include/bl2/emmc/			\
				-I${CVITEK_PLAT_SOC}/include/drivers/cvitek/${PLAT}/usb/	\
				-I${CVITEK_PLAT_SOC}/include/tools_share/	\
				-I${CVITEK_PLAT_COMMON}/include/		\
				-I${CVITEK_PLAT_SOC}/include/lib/		\
				-I${CVITEK_PLAT_SOC}/include/bl2/		\
				-I${CVITEK_PLAT_SOC}/include/bl2/ddr/		\
				-I${CVITEK_PLAT_SOC}/include/drivers/cvitek/${PLAT}/spinor/	\
				-I${CVITEK_PLAT_SOC}/include/drivers/cvitek/${PLAT}/pinmux/	\
				-I${CVITEK_PLAT_SOC}/include/drivers/cvitek/${CHIP_ARCH}/pcie/	\
				-I${CVITEK_PLAT_SOC}/include/drivers/cvitek/${PLAT}/emmc/	\
					-I${CVITEK_PLAT_SOC}/include/drivers/cvitek/${PLAT}/spinand/	\
				-I${CVITEK_PLAT_SOC}/include/drivers/io/	\
				-I${CVITEK_PLAT_SOC}/include/drivers/cvitek/${PLAT}/i2c/	\
				-I${CVITEK_PLAT_SOC}/include/drivers/cvitek/${PLAT}/	\
					-I${CVITEK_PLAT_SOC}/include/lib/stdlib_v2/	\
					-I${CVITEK_PLAT_SOC}/include/bl32/		\
				-I${CVITEK_PLAT_SOC}/include/bl32/cvsp/	\
				-I${CVITEK_PLAT_SOC}/include/common/		\
				-I${CVITEK_PLAT_SOC}/include/lib/		\
				-Iinclude/lib/xlat_tables/			\
				-Iinclude/drivers/io				\
				-Iinclude/tools_share				\
				-Iservices/spd/opteed

include drivers/cvitek/ddr/ddr.mk
include drivers/cvitek/pcie/pcie.mk

CVITEK_GIC_SOURCES	:=	${GICV3_SOURCES}				\
				plat/common/plat_gicv3.c			\
				plat/arm/common/arm_gicv3.c

PLAT_BL_COMMON_SOURCES	:=	${XLAT_TABLES_LIB_SRCS}				\
				common/desc_image_load.c			\
				lib/bl_aux_params/bl_aux_params.c		\
				plat/common/plat_psci_common.c			\
				drivers/cvitek/uart/uart_dw.c			\
				${CVITEK_PLAT_COMMON}/cv_uart_clock.c

BL2_SOURCES		+=	${CVITEK_PLAT_COMMON}/aarch64/cv_bl2_crash_stubs.S	\
				${CVITEK_PLAT_COMMON}/aarch64/plat_helpers.S	\
				bl2/${ARCH}/bl2_run_next_image.S		\
				${CVITEK_PLAT_COMMON}/aarch64/cv_bl2_jump_bl31.S	\
				drivers/io/io_storage.c				\
				drivers/cvitek/emmc/cv84x6/emmc.c			\
				drivers/cvitek/emmc/cv84x6/cv_emmc.c			\
				common/desc_image_load.c			\
				${CVITEK_PLAT_COMMON}/lzma/LzmaDec.c		\
				${CVITEK_PLAT_COMMON}/board_cv_trusted_boot.c	\
				${CVITEK_PLAT_COMMON}/bl2_load.c		\
				${CVITEK_PLAT_COMMON}/cv_bl2_helper.c		\
				${CVITEK_PLAT_COMMON}/cv_bl2_setup.c		\
				${CVITEK_PLAT_COMMON}/cv_decompress_stub.c	\
				${CVITEK_PLAT_COMMON}/cv_tempsen.c		\
				${CVITEK_PLAT_COMMON}/cv_bl2_delay_timer.c	\
				${CVITEK_PLAT_COMMON}/cv_bl2_misc_stubs.c

BL31_SOURCES		+=	${CVITEK_GIC_SOURCES}				\
				drivers/delay_timer/delay_timer.c		\
				drivers/delay_timer/generic_delay_timer.c	\
				lib/cpus/aarch64/cortex_a55.S			\
				${CVITEK_PLAT_COMMON}/aarch64/plat_helpers.S	\
				${CVITEK_PLAT_COMMON}/cv_bl31_setup.c		\
				${CVITEK_PLAT_COMMON}/topology.c		\
				${CVITEK_PLAT_COMMON}/cv_pm.c			\
				${CVITEK_PLAT_COMMON}/cv_bl31_common.c		\
				${CVITEK_PLAT_COMMON}/cv_bl31_stubs.c		\
				${CVITEK_PLAT_SOC}/${SUBTYPE}/aarch64/plat_suspend.S

BL32_SOURCES		+=	bl32/cvsp/cvsp_main.c				\
				bl32/cvsp/aarch64/cvsp_entrypoint.S		\
				bl32/cvsp/aarch64/cvsp_exceptions.S		\
				bl32/cvsp/cvsp_interrupt.c			\
				bl32/cvsp/cvsp_timer.c				\
				bl32/cvsp/cvsp_private.c			\
				bl32/cvsp/cvsp_debug.c				\
				drivers/io/io_storage.c				\
				drivers/cvitek/timer/timer_dw.c			\
				common/aarch64/early_exceptions.S		\
				lib/locks/exclusive/aarch64/spinlock.S		\
				plat/common/plat_gicv3.c			\
				plat/common/aarch64/platform_mp_stack.S		\
				drivers/arm/gic/v3/gicv3_helpers.c		\
				drivers/arm/gic/v3/gicv3_main.c			\
				drivers/arm/gic/common/gic_common.c		\
				${CVITEK_PLAT_COMMON}/topology.c		\
				${CVITEK_PLAT_COMMON}/aarch64/plat_helpers.S	\
				${CVITEK_PLAT_COMMON}/aarch64/cv_bl2_crash_stubs.S	\
				${CVITEK_PLAT_COMMON}/cv_bl2_delay_timer.c	\
				${CVITEK_PLAT_COMMON}/cvsp/cv_sp_stubs.c	\
				plat/arm/common/arm_gicv3.c			\
				${CVITEK_PLAT_COMMON}/cvsp/cv_sp_setup.c	\
				common/tf_log.c

BL31_SOURCES		:=	$(filter-out common/bl_common.c common/tf_log.c,$(BL31_SOURCES))
BL32_SOURCES		:=	$(filter-out common/bl_common.c,$(BL32_SOURCES))

CTX_INCLUDE_AARCH32_REGS :=	0
ENABLE_PLAT_COMPAT	:=	0
MULTI_CONSOLE_API	:=	0
USE_DSU_DRIVER		:=	1

# Cortex-A55 errata (MIDR: 0x412fd050, variant=2, revision=r0p0)
ERRATA_A55_768277	:=	1
ERRATA_A55_778703	:=	1
ERRATA_A55_798797	:=	1
ERRATA_A55_846532	:=	1
ERRATA_A55_903758	:=	1
ERRATA_A55_1221012	:=	1
ERRATA_A55_1530923	:=	1

# DSU errata (required when USE_DSU_DRIVER=1)
ERRATA_DSU_798953	:=	1
ERRATA_DSU_936184	:=	1

RESET_TO_BL31		:=	0

# System coherency is managed in hardware
HW_ASSISTED_COHERENCY	:=	1
USE_COHERENT_MEM	:=	0

ENABLE_SPE_FOR_LOWER_ELS :=	0