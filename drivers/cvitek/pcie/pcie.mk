ifeq (${CHIP_ARCH},cv84x6)
ifeq ($(CONFIG_IGNORE_PCIE_TRAP),y)
$(eval $(call add_define,CONFIG_IGNORE_PCIE_TRAP))
ifdef CONFIG_SSMODE
$(eval $(call add_define,CONFIG_SSMODE=$(CONFIG_SSMODE)))
else
$(eval $(call add_define,CONFIG_SSMODE=0))
endif
endif

# Controller 0
ifdef CONFIG_PCIE_CTRL0_DISABLED
$(eval $(call add_define,CONFIG_PCIE_CTRL0_MODE=0))
else ifdef CONFIG_PCIE_CTRL0_MODE_RC
$(eval $(call add_define,CONFIG_PCIE_CTRL0_MODE=1))
else ifdef CONFIG_PCIE_CTRL0_MODE_EP
$(eval $(call add_define,CONFIG_PCIE_CTRL0_MODE=2))
else # disabled
$(eval $(call add_define,CONFIG_PCIE_CTRL0_MODE=0))
endif

# Controller 1
ifdef CONFIG_PCIE_CTRL1_DISABLED
$(eval $(call add_define,CONFIG_PCIE_CTRL1_MODE=0))
else ifdef CONFIG_PCIE_CTRL1_MODE_RC
$(eval $(call add_define,CONFIG_PCIE_CTRL1_MODE=1))
else ifdef CONFIG_PCIE_CTRL1_MODE_EP
$(eval $(call add_define,CONFIG_PCIE_CTRL1_MODE=2))
else
$(eval $(call add_define,CONFIG_PCIE_CTRL1_MODE=0))
endif

# Controller 2
ifdef CONFIG_PCIE_CTRL2_DISABLED
$(eval $(call add_define,CONFIG_PCIE_CTRL2_MODE=0))
else ifdef CONFIG_PCIE_CTRL2_MODE_RC
$(eval $(call add_define,CONFIG_PCIE_CTRL2_MODE=1))
else ifdef CONFIG_PCIE_CTRL2_MODE_EP
$(eval $(call add_define,CONFIG_PCIE_CTRL2_MODE=2))
else
$(eval $(call add_define,CONFIG_PCIE_CTRL2_MODE=0))
endif

# Controller 3
ifdef CONFIG_PCIE_CTRL3_DISABLED
$(eval $(call add_define,CONFIG_PCIE_CTRL3_MODE=0))
else ifdef CONFIG_PCIE_CTRL3_MODE_RC
$(eval $(call add_define,CONFIG_PCIE_CTRL3_MODE=1))
else ifdef CONFIG_PCIE_CTRL3_MODE_EP
$(eval $(call add_define,CONFIG_PCIE_CTRL3_MODE=2))
else
$(eval $(call add_define,CONFIG_PCIE_CTRL3_MODE=0))
endif

$(eval $(call add_define,CONFIG_PCIE_INIT))

# NOTE: unlike the fsbl build, the translation table library v1
# (lib/xlat_tables/) is deliberately NOT pulled in here.  The cv84x6 BL2
# already links the v2 library through PLAT_BL_COMMON_SOURCES
# (${XLAT_TABLES_LIB_SRCS} in plat/cvitek/cv84x6/platform.mk), and both
# libraries export mmap_add_region()/init_xlat_tables()/enable_mmu_el3().
# bl2_mmu_reconfig() in cv_bl2_setup.c uses the v2 API instead.
PLAT_INCLUDES += \
	-I${CVITEK_PLAT_SOC}/include/drivers/cvitek/${CHIP_ARCH}/pcie

BL2_SOURCES += \
	drivers/cvitek/pcie/84x6/pcie.c
endif
