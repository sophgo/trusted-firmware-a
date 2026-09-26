#
# cv186x platform (migrated from legacy fsbl tree).
#

CV186X_ROOT		:=	plat/cvitek/cv186x
CV186X_INC		:=	${CV186X_ROOT}/include

DISABLE_BIN_GENERATION	:=	0
include lib/libfdt/libfdt.mk
include lib/xlat_tables_v2/xlat_tables.mk
include drivers/arm/gic/v2/gicv2.mk
include drivers/cvitek/ddr/ddr.mk

CHIP_ARCH		:=	cv186x
SUBTYPE			:=	asic
SPD			?=	opteed
BL31_LINKERFILE		:=	bl31/bl31.ld.S
BL31_DEFAULT_LINKER_SCRIPT_SOURCE := bl31/bl31.ld.S
BL32_DEFAULT_LINKER_SCRIPT_SOURCE	:=	bl32/cvsp/cvsp.ld.S
BL32_LDFLAGS		+=	-Wl,-no-pie
BL2_CFLAGS		+=	-fno-pie -fno-PIE
BL2_CPPFLAGS		+=	-DDIRECT_BL31_FROM_BL2=1
BL2_ASFLAGS		+=	-fno-pie
BL2_DEFAULT_LINKER_SCRIPT_SOURCE := bl2/bl2.ld.S
BL2_LDFLAGS		+=	-Wl,-no-pie
# fixup_gdt_reloc is linked via bl2.ld *( .text* ); bl2.ld has no RELA_SECTION — define bounds.
BL2_LDFLAGS		+=	-Wl,--defsym=__RELA_START__=0 -Wl,--defsym=__RELA_END__=0

PLAT_INCLUDES		+=	-I${CV186X_INC}					\
				-Iinclude/arch					\
				-I${CV186X_INC}/bl31				\
				-I${CV186X_INC}/bl32/cvsp			\
				-I${CV186X_INC}/bl2				\
				-I${CV186X_INC}/bl2/ddr				\
				-I${CV186X_INC}/bl2/emmc			\
				-I${CV186X_INC}/drivers/cvitek/cv186x		\
				-I${CV186X_INC}/drivers/cvitek/cv186x/tpu	\
				-I${CV186X_INC}/drivers/io			\
				-I${CV186X_INC}/drivers/cvitek/cv186x/spinor	\
				-I${CV186X_INC}/drivers/cvitek/cv186x/pinmux	\
				-I${CV186X_INC}/drivers/cvitek/cv186x/emmc	\
				-I${CV186X_INC}/arch				\
				-I${CV186X_INC}/arch/${ARCH}		\
				-I${CV186X_ROOT}/common/lzma			\
				-Iinclude/common				\
				-I${CV186X_INC}/common				\
				-Iinclude/tools_share				\
				-I${CV186X_INC}/tools_share			\
				-Iinclude/drivers/io				\
				-Iinclude/lib					\
				-Iservices/spd/opteed				\
				-Iinclude/lib/psci				\
				-Iinclude/lib/xlat_tables/			\
				-Iinclude/lib/xlat_tables/aarch64/		\
				-Iinclude/drivers				\
				-Iinclude/drivers/arm				\
				-Iinclude/drivers/ti/uart			\
				-Iinclude/plat/common				\
				-Iinclude/plat/arm/common			\
				-Iinclude/plat/arm/common/aarch64/		\
				-I${CV186X_ROOT}/common/include	\
				-I${CV186X_ROOT}/${SUBTYPE}/include		\
				-I${CV186X_INC}/lib

PLAT_BL_COMMON_SOURCES	+=	drivers/cvitek/uart/uart_dw.c			\
				lib/xlat_tables/xlat_tables_common.c		\
				lib/xlat_tables/aarch64/xlat_tables.c		\
				${CV186X_ROOT}/common/cv_plat_clock.c		\
				${CV186X_ROOT}/common/cv_plat_bl_common_stubs.c	\
				${CV186X_ROOT}/common/aarch64/plat_helpers.S

BL2_SOURCES		+=	${CV186X_ROOT}/common/aarch64/cv_bl2_crash_stubs.S	\
				bl2/${ARCH}/bl2_run_next_image.S	\
				${CV186X_ROOT}/common/aarch64/cv_bl2_jump_bl31.S	\
				drivers/io/io_storage.c			\
				drivers/cvitek/emmc/emmc.c		\
				drivers/cvitek/emmc/cv_emmc.c		\
				common/desc_image_load.c			\
				${CV186X_ROOT}/common/lzma/LzmaDec.c		\
				${CV186X_ROOT}/common/board_cv_trusted_boot.c	\
				${CV186X_ROOT}/common/bl2_load.c		\
				${CV186X_ROOT}/common/cv_bl2_helper.c		\
				${CV186X_ROOT}/common/cv_bl2_setup.c		\
				${CV186X_ROOT}/common/cv_decompress_stub.c	\
				${CV186X_ROOT}/common/cv_tempsen.c		\
				${CV186X_ROOT}/common/cv_bl2_delay_timer.c	\
				${CV186X_ROOT}/common/cv_bl2_misc_stubs.c


BL31_SOURCES		+=	lib/cpus/aarch64/aem_generic.S			\
				lib/cpus/aarch64/cortex_a53.S			\
				drivers/arm/gic/v2/gicv2_helpers.c		\
				drivers/arm/gic/v2/gicv2_main.c			\
				drivers/arm/gic/common/gic_common.c		\
				plat/common/plat_gicv2.c			\
				drivers/delay_timer/${BOOT_CPU}/delay_timer.c	\
				${CV186X_ROOT}/common/cv_pm.c			\
				${CV186X_ROOT}/common/plat_topology.c		\
				${CV186X_ROOT}/common/cv_bl31_common.c		\
				${CV186X_ROOT}/common/cv_gicv2_compat.c	\
				${CV186X_ROOT}/common/cv_bl31_stubs.c	\
				${CV186X_ROOT}/${SUBTYPE}/aarch64/plat_suspend.S \
				${CV186X_ROOT}/common/cv_bl31_setup.c

BL32_SOURCES		+=	bl32/cvsp/cvsp_main.c				\
				bl32/cvsp/aarch64/cvsp_entrypoint.S		\
				bl32/cvsp/aarch64/cvsp_exceptions.S		\
				bl32/cvsp/cvsp_interrupt.c			\
				bl32/cvsp/cvsp_timer.c				\
				bl32/cvsp/cvsp_private.c			\
				bl32/cvsp/cvsp_debug.c				\
				drivers/io/io_storage.c			\
				drivers/cvitek/timer/timer_dw.c			\
				common/aarch64/early_exceptions.S		\
				lib/locks/exclusive/aarch64/spinlock.S		\
				plat/common/plat_gicv2.c			\
				plat/common/aarch64/platform_mp_stack.S		\
				drivers/arm/gic/v2/gicv2_helpers.c		\
				drivers/arm/gic/v2/gicv2_main.c			\
				drivers/arm/gic/common/gic_common.c		\
				${CV186X_ROOT}/common/topology.c		\
				${CV186X_ROOT}/common/aarch64/plat_helpers.S	\
				${CV186X_ROOT}/common/aarch64/cv_bl2_crash_stubs.S \
				${CV186X_ROOT}/common/cv_bl2_delay_timer.c	\
				${CV186X_ROOT}/common/cvsp/cv_sp_stubs.c	\
				${CV186X_ROOT}/common/cv_gicv2_compat.c	\
				${CV186X_ROOT}/common/cvsp/cv_sp_setup.c	\
				common/tf_log.c

BL32_SOURCES		:=	$(filter-out common/bl_common.c,$(BL32_SOURCES))

ENABLE_PLAT_COMPAT	:=	0
USE_COHERENT_MEM	:=	0
ERRATA_A53_835769	:=	1
ERRATA_A53_843419	:=	1
ERRATA_A53_855873	:=	1
