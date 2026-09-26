$(call print_var,DDR_CFG)
$(call print_var,CHIP_ARCH)

BL2_CFLAGS += -Wno-error=redundant-decls -Wno-error=shadow -Wno-error
BL2_TF_CFLAGS_REMOVE_ddr_sys.o += -mgeneral-regs-only
BL2_TF_CFLAGS_REMOVE_aks_ddrphy_setup_api.o += -mgeneral-regs-only

ifeq (${CHIP_ARCH},cv84x6)
ifeq (${DDR_CFG},none)
DDR_CFG =
endif

ifeq ($(DDR_CFG), )
PLAT_INCLUDES += \
	-I${CV186X_INC}/bl2/ddr/84x6

BL2_MODULE_OBJS += \
	drivers/cvitek/ddr/cv84x6/ddr.o

$(eval $(call add_define,NO_DDR_CFG))

else ifeq (${BOARD},palladium)

$(eval $(call add_define,PLD_DDR_CFG))
$(eval $(call add_define,SUPPORT_DDR_INIT))
$(eval $(call add_define,DDR_2_RANK))

PLAT_INCLUDES += \
	-I${CV186X_INC}/bl2/ddr/84x6 \
	-I${CV186X_INC}/drivers/cvitek/${CHIP_ARCH}/ddr \
	-I${CV186X_INC}/bl2/ddr/84x6/pld_ddr_config/${DDR_CFG}

BL2_MODULE_OBJS += \
	drivers/cvitek/ddr/cv84x6/ddr.o \
	drivers/cvitek/ddr/cv84x6/pld_src/ddr_sys_bring_up_pld.o \
	drivers/cvitek/ddr/cv84x6/pld_src/ddr_sys_pld.o \
	drivers/cvitek/ddr/cv84x6/ddr_test.o \
	drivers/cvitek/ddr/cv84x6/pld_ddr_config/${DDR_CFG}/ddrc_init.o

else

$(eval $(call add_define,SUPPORT_DDR_INIT))
$(eval $(call add_define,DDR_FW_IN_FIP))
DDR_FW_BIN_DIR ?= $(CURDIR)/drivers/cvitek/ddr/cv84x6/fw_bin

PLAT_INCLUDES += \
	-I${CV186X_INC}/bl2/ddr/84x6 \
	-I${CV186X_INC}/drivers/cvitek/${CHIP_ARCH}/ddr \
	-Idrivers/cvitek/ddr/cv84x6/src/phy_src/include \
	-I${CV186X_INC}/bl2/ddr/84x6/ddr_config/${DDR_CFG}

BL2_MODULE_OBJS += \
	drivers/cvitek/ddr/cv84x6/ddr.o \
	drivers/cvitek/ddr/cv84x6/ddr_test.o \
	drivers/cvitek/ddr/cv84x6/ddr_pkg_info.o \
	drivers/cvitek/ddr/cv84x6/ddr_config/${DDR_CFG}/ddrc_init.o \
	drivers/cvitek/ddr/cv84x6/src/ddr_sys_bring_up.o \
	drivers/cvitek/ddr/cv84x6/src/ddr_sys.o \
	drivers/cvitek/ddr/cv84x6/src/pll_init.o \
	drivers/cvitek/ddr/cv84x6/src/phy_src/aks_ddrphy_pinswap.o \
	drivers/cvitek/ddr/cv84x6/src/phy_src/aks_ddrphy_setup_api.o \
	drivers/cvitek/ddr/cv84x6/src/phy_src/aks_ddrphy_setup_flow.o \
	drivers/cvitek/ddr/cv84x6/src/phy_src/aks_ddrphy_setup_user_api.o \
	drivers/cvitek/ddr/cv84x6/src/phy_src/aks_ddrphy_print_comment.o \
	drivers/cvitek/ddr/cv84x6/src/phy_src/aks_ddrphy_setup_reg_interface.o \
	drivers/cvitek/ddr/cv84x6/src/phy_src/firmware/1.02a_patch4/lpddr5/lpddr5_train_itim.o \
	drivers/cvitek/ddr/cv84x6/src/phy_src/firmware/1.02a_patch4/lpddr5/lpddr5_train_dtim.o \
	drivers/cvitek/ddr/cv84x6/src/phy_src/firmware/1.02a_patch4/lpddr5/lpddr5_train_string.o \
	drivers/cvitek/ddr/cv84x6/src/phy_src/firmware/1.02a_patch4/lpddr4/lpddr4_train_itim.o \
	drivers/cvitek/ddr/cv84x6/src/phy_src/firmware/1.02a_patch4/lpddr4/lpddr4_train_dtim.o \
	drivers/cvitek/ddr/cv84x6/src/phy_src/firmware/1.02a_patch4/lpddr4/lpddr4_train_string.o \
	drivers/cvitek/ddr/cv84x6/src/phy_src/aks_ddrphy_register_dump.o \
	drivers/cvitek/ddr/cv84x6/src/phy_src/aks_ddrphy_fw_fip.o

ifneq ($(findstring lp5, ${DDR_CFG}),)
    $(eval $(call add_define,DDR_CFG_LPDDR5))
endif

ifneq ($(findstring 9600, ${DDR_CFG}),)
    $(eval $(call add_define,DDR_FREQ_9600))
endif

ifneq ($(findstring lp4, ${DDR_CFG}),)
    $(eval $(call add_define,DDR_CFG_LPDDR4))
endif

ifneq ($(findstring auto, ${DDR_CFG}),)
    $(eval $(call add_define,DDR_CFG_AUTO))
endif

endif

else
ifeq (${DDR_CFG},none)
DDR_CFG =
endif

ifeq ($(DDR_CFG), )
PLAT_INCLUDES += \
	-I${CV186X_INC}/bl2/ddr

BL2_MODULE_OBJS += \
	drivers/cvitek/ddr/cv186x/ddr.o

$(eval $(call add_define,NO_DDR_CFG))

else ifeq (${BOARD},palladium)

$(eval $(call add_define,PLD_DDR_CFG))

$(eval $(call add_define,DDR_2_RANK))

PLAT_INCLUDES += \
	-I${CV186X_INC}/bl2/ddr \
	-I${CV186X_INC}/drivers/cvitek/${CHIP_ARCH}/ddr \
	-I${CV186X_INC}/bl2/ddr/ddr_config/${DDR_CFG} \
	-I${CV186X_INC}/bl2/ddr/pld_ddr_config/${DDR_CFG}

BL2_MODULE_OBJS += \
	drivers/cvitek/ddr/cv186x/ddr.o \
	drivers/cvitek/ddr/cv186x/ddr_pkg_info.o \
	drivers/cvitek/ddr/pld_src/ddr_sys_bring_up_pld.o \
	drivers/cvitek/ddr/pld_src/ddr_sys_pld.o \
	drivers/cvitek/ddr/cv186x/phy_pll_init.o \
	drivers/cvitek/ddr/cv186x/ddr_resume.o \
	drivers/cvitek/ddr/cv186x/ddr_config/${DDR_CFG}/ddrc_init.o \
	drivers/cvitek/ddr/cv186x/ddr_config/${DDR_CFG}/phy_init.o \
	drivers/cvitek/ddr/cv186x/ddr_config/${DDR_CFG}/ddr_patch_regs.o

ifneq ($(findstring ddr4, ${DDR_CFG}),)
    $(eval $(call add_define,DDR4))
else ifneq ($(findstring lp4, ${DDR_CFG}),)
    $(eval $(call add_define,LPDDR4))
endif
else
PLAT_INCLUDES += \
	-I${CV186X_INC}/bl2/ddr \
	-I${CV186X_INC}/drivers/cvitek/${CHIP_ARCH}/ddr \
	-I${CV186X_INC}/bl2/ddr/ddr_config/${DDR_CFG}

BL2_MODULE_OBJS += \
	drivers/cvitek/ddr/cv186x/ddr.o \
	drivers/cvitek/ddr/cv186x/ddr_pkg_info.o \
	drivers/cvitek/ddr/cv186x/ddr_sys_bring_up.o \
	drivers/cvitek/ddr/cv186x/ddr_sys.o \
	drivers/cvitek/ddr/cv186x/ddr_shmoo.o \
	drivers/cvitek/ddr/cv186x/ddr_pm.o \
	drivers/cvitek/ddr/cv186x/ddr_resume.o \
	drivers/cvitek/ddr/cv186x/ddr_config/${DDR_CFG}/ddrc_init.o \
	drivers/cvitek/ddr/cv186x/ddr_config/${DDR_CFG}/phy_init.o \
	drivers/cvitek/ddr/cv186x/ddr_config/${DDR_CFG}/ddr_patch_regs.o

BLDS_MODULE_OBJS += \
	drivers/cvitek/ddr/cv186x/ddr_suspend.o

ifneq ($(findstring ddr4_3200_x8, ${DDR_CFG}),)
    $(eval $(call add_define,DDR4_X8))
else ifneq ($(findstring ddr4_3200_x16, ${DDR_CFG}),)
    $(eval $(call add_define,DDR4_X16))
else ifneq ($(findstring lp4x, ${DDR_CFG}),)
    $(eval $(call add_define,LPDDR4X))
else ifneq ($(findstring lp4, ${DDR_CFG}),)
    $(eval $(call add_define,LPDDR4))
endif

ifneq ($(findstring 2r, ${DDR_CFG}),)
    $(eval $(call add_define,DDR_2_RANK))
endif
ifneq ($(findstring 2s, ${DDR_CFG}),)
    $(eval $(call add_define,DDR_2SYS))
endif

ifneq ($(findstring ddr_auto, ${DDR_CFG}),)
    $(eval $(call add_define,DDR_AUTO))
endif

$(eval $(call add_define,REAL_LOCK))

endif

endif
