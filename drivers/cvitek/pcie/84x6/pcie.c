/*
 * Copyright (c) 2022-2023, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <string.h>
#include <platform_def.h>
#include <bl_common.h>
#include <debug.h>
#include <utils.h>
#include <mmio.h>
#include <platform.h>
#include <delay_timer.h>
#include <pcie.h>
#include <console.h>
#include <stdbool.h>

////#define PCIE_REG_DEBUG
#ifdef PCIE_REG_DEBUG
#define PCIE_REG_INFO(...) NOTICE(__VA_ARGS__)
#else
#define PCIE_REG_INFO(...)
#endif

const struct pcie_chip_config pcie_chip_configs[PCIE_SSMODE_MAX][PCIE_CTRL_MAX] = {
	[0][PCIE_CTRL_X8_0] = {PCIE_PHY_0_SEL | PCIE_PHY_1_SEL, PCIE_LINK_CAPABLE_X8, 8},
	[0][PCIE_CTRL_X2_0] = {0, 0, 0},
	[0][PCIE_CTRL_X4_0] = {0, 0, 0},
	[0][PCIE_CTRL_X2_1] = {0, 0, 0},
	[1][PCIE_CTRL_X8_0] = {PCIE_PHY_0_SEL, PCIE_LINK_CAPABLE_X4, 4},
	[1][PCIE_CTRL_X2_0] = {0, 0, 0},
	[1][PCIE_CTRL_X4_0] = {PCIE_PHY_1_SEL, PCIE_LINK_CAPABLE_X4, 4},
	[1][PCIE_CTRL_X2_1] = {0, 0, 0},
	[2][PCIE_CTRL_X8_0] = {PCIE_PHY_0_SEL, PCIE_LINK_CAPABLE_X2, 2},
	[2][PCIE_CTRL_X2_0] = {PCIE_PHY_0_SEL, PCIE_LINK_CAPABLE_X2, 2},
	[2][PCIE_CTRL_X4_0] = {PCIE_PHY_1_SEL, PCIE_LINK_CAPABLE_X2, 2},
	[2][PCIE_CTRL_X2_1] = {PCIE_PHY_1_SEL, PCIE_LINK_CAPABLE_X2, 2},
	[3][PCIE_CTRL_X8_0] = {PCIE_PHY_0_SEL, PCIE_LINK_CAPABLE_X2, 2},
	[3][PCIE_CTRL_X2_0] = {PCIE_PHY_0_SEL, PCIE_LINK_CAPABLE_X2, 2},
	[3][PCIE_CTRL_X4_0] = {0, 0, 0},
	[3][PCIE_CTRL_X2_1] = {0, 0, 0},
	[4][PCIE_CTRL_X8_0] = {PCIE_PHY_0_SEL, PCIE_LINK_CAPABLE_X1, 1},
	[4][PCIE_CTRL_X2_0] = {PCIE_PHY_0_SEL, PCIE_LINK_CAPABLE_X1, 1},
	[4][PCIE_CTRL_X4_0] = {PCIE_PHY_0_SEL, PCIE_LINK_CAPABLE_X1, 1},
	[4][PCIE_CTRL_X2_1] = {PCIE_PHY_0_SEL, PCIE_LINK_CAPABLE_X1, 1},
	[5][PCIE_CTRL_X8_0] = {PCIE_PHY_0_SEL, PCIE_LINK_CAPABLE_X4, 4},
	[5][PCIE_CTRL_X2_0] = {0, 0, 0},
	[5][PCIE_CTRL_X4_0] = {PCIE_PHY_1_SEL, PCIE_LINK_CAPABLE_X2, 2},
	[5][PCIE_CTRL_X2_1] = {PCIE_PHY_1_SEL, PCIE_LINK_CAPABLE_X2, 2},
};

static enum pcie_ctrl_mode pcie_ctrl_modes[PCIE_CTRL_MAX] = {
	CONFIG_PCIE_CTRL0_MODE,
	CONFIG_PCIE_CTRL1_MODE,
	CONFIG_PCIE_CTRL2_MODE,
	CONFIG_PCIE_CTRL3_MODE,
};

static struct pcie_range_regs pcie_range_regs[PCIE_CTRL_MAX] = {
	// PCIe x8
	{0x18, 0x24, 0x28, 0x2C, 0x30, 0x20, 0x44, 0x48},
	// PCIe x2_0
	{0x4C, 0x58, 0x5C, 0x60, 0x64, 0x54, 0x78, 0x7C},
	// PCIe x4
	{0xB4, 0xC0, 0xC4, 0xC8, 0xCC, 0xBC, 0xE8, 0xEC},
	// PCIe x2_1
	{0x80, 0x8C, 0x90, 0x94, 0x98, 0x88, 0xAC, 0xB0}
};

static struct pcie_addr_range_group pcie_range_groups[PCIE_CTRL_MAX] = {
	// 0x8000_0000 ~ 0x9FFF_FFFF    0x30_0000_0000 ~ 0x37_FFFF_FFFF
	{0x30, 0x37, 0x80000000, 0x9FFFFFFF, 0x2, 0x00000000},
	// 0xA000_0000 ~ 0xBFFF_FFFF    0x38_0000_0000 ~ 0x3F_FFFF_FFFF
	{0x38, 0x3F, 0xA0000000, 0xBFFFFFFF, 0x2, 0x80000000},
	// 0xC000_0000 ~ 0xDFFF_FFFF    0x40_0000_0000 ~ 0x47_FFFF_FFFF
	{0x40, 0x47, 0xC0000000, 0xDFFFFFFF, 0x2, 0x40000000},
	// 0xE000_0000 ~ 0xFFFF_FFFF    0x48_0000_0000 ~ 0x4F_FFFF_FFFF
	{0x48, 0x4F, 0xE0000000, 0xFFFFFFFF, 0x2, 0xC0000000}
};

static struct pcie_bus_resource pcie_bus_resources[PCIE_CTRL_MAX] = {
	{0x00, 0x01, 0x3F},
	{0x80, 0x81, 0xCF},
	{0x40, 0x41, 0x7F},
	{0xC0, 0xC1, 0xFF},
};

static const uint64_t BAR4_SIZE_MASKS[4] = {
	[0] = 0xfffffULL,      //   1MB
	[1] = 0x7ffffffULL,    // 128MB
	[2] = 0x1ffffffffULL,  //   8GB
	[3] = 0x1fffffffffULL, // 128GB
};

static inline uint32_t pcie_reg_read(uint32_t reg) {
	uint32_t val =  mmio_read_32(reg);

	PCIE_REG_INFO("[0x%x]=0x%x\n", reg, val);
	return val;
}

static inline void pcie_reg_write(uint32_t reg, uint32_t val) {
	PCIE_REG_INFO("[0x%x]=0x%x w 0x%x\n", reg, mmio_read_32(reg), val);
	mmio_write_32(reg, val);
	PCIE_REG_INFO("[0x%x]=0x%x\n", reg, mmio_read_32(reg));
}

static inline void pcie_reg_clrbits(uint32_t reg, uint32_t val) {
	PCIE_REG_INFO("[0x%x]=0x%x clear 0x%x\n", reg, mmio_read_32(reg), val);
	mmio_clrbits_32(reg, val);
	PCIE_REG_INFO("[0x%x]=0x%x\n", reg, mmio_read_32(reg));
}

static inline void pcie_reg_setbits(uint32_t reg, uint32_t val) {
	PCIE_REG_INFO("[0x%x]=0x%x set 0x%x\n", reg, mmio_read_32(reg), val);
	mmio_setbits_32(reg, val);
	PCIE_REG_INFO("[0x%x]=0x%x\n", reg, mmio_read_32(reg));
}

static inline enum pcie_ctrl_id pcie_trap_get_ctrl_id(uint32_t trap)
{
	return (trap & PCIE_TRAP_CTRL_SEL_MASK) >> PCIE_TRAP_CTRL_SEL_SHIFT;
}

static inline uint32_t pcie_trap_get_ssmode(uint32_t trap)
{
	return (trap & PCIE_TRAP_SSMODE_MASK) >> PCIE_TRAP_SSMODE_SHIFT;
}

static inline uint32_t pcie_trap_get_func_num(uint32_t trap)
{
	return ((trap & PCIE_TRAP_FUNC_NUM_MASK) >> PCIE_TRAP_FUNC_NUM_SHIFT) + 1;
}

static inline uint32_t pcie_trap_get_bar4_set(uint32_t trap)
{
	return (trap & PCIE_TRAP_BAR4_SET_MASK) >> PCIE_TRAP_BAR4_SET_SHIFT;
}

static inline uint32_t pcie_trap_get_board_id(uint32_t trap)
{
	//TBD: read from reg_board_id if sophon-driver config this, or use macro force
	return (mmio_read_32(0x21790400) >> 20) & 0x7f;
}

static inline uint32_t pcie_trap_get_repeat_clk(uint32_t trap)
{
	return (trap & PCIE_TRAP_REPEAT_CLK_EN_MASK) >> PCIE_TRAP_REPEAT_CLK_EN_SHIFT;
}

static inline bool pcie_is_switch_mode(void)
{
#ifdef CONFIG_PCIE_SWITCH_MODE
	return true;
#else
	return false;
#endif
}

static void pcie_config_phy_bl_bypass(enum pcie_phy_select phy_select, uint32_t bypass)
{
	uint32_t val =0;

	//config phy bl no bypass
	if(phy_select & PCIE_PHY_0_SEL) {
		val = pcie_reg_read(PCIE_PHY_MODE_REG1);
		val &= ~PCIE_PHY0_BL_BYPASS_EN;
		if (bypass) {
			val |= PCIE_PHY0_SRAM_INIT_BYP;
		} else {
			val |= PCIE_PHY0_SRAM_LOAD_DONE;
		}
		pcie_reg_write(PCIE_PHY_MODE_REG1, val);
	}
	if(phy_select & PCIE_PHY_1_SEL) {
		val = pcie_reg_read(PCIE_PHY_MODE_REG1);
		val &= ~PCIE_PHY1_BL_BYPASS_EN;
		if (bypass) {
			val |= PCIE_PHY1_SRAM_INIT_BYP;
		} else {
			val |= PCIE_PHY1_SRAM_LOAD_DONE;
		}
		pcie_reg_write(PCIE_PHY_MODE_REG1, val);
	}
}

static void pcie_wait_sram_init_done(enum pcie_phy_select phy_select)
{
#ifndef CONFIG_BOARD_palladium
	uint32_t val = 0;
	uint32_t check = 0;
	uint32_t timeout = 100000;

	if(phy_select & PCIE_PHY_0_SEL)
		check |= PCIE_PHY0_SRAM_INIT_DONE;

	if(phy_select & PCIE_PHY_1_SEL)
		check |= PCIE_PHY1_SRAM_INIT_DONE;

	//wait phy sram init done
	while(timeout--) {
		val = pcie_reg_read(PCIE_PHY_MODE_REG1);
		if((val & check) == check)
			return;
		udelay(10);
	}
	ERROR("pcie_wait_sram_init_done timeout: %x %x\n", val, check);
#endif
}

#ifdef CONFIG_IGNORE_PCIE_TRAP
static void pcie_config_ssmode(uint32_t ssmode)
{
	uint32_t val = pcie_reg_read(PCIE_SS_MODE_REG0);
	val &= ~PCIE_SS_MODE_REG0_SS_MODE_MASK;
	val |= (ssmode << 0);
	pcie_reg_write(PCIE_SS_MODE_REG0, val);
}
#else
static uint32_t pcie_get_ssmode(void)
{
	uint32_t val = pcie_reg_read(PCIE_SS_MODE_REG0);
	/* ss_mode lives in bits[2:0]; return only that field (SHIFT == 0) */
	return val & PCIE_SS_MODE_REG0_SS_MODE_MASK;
}
#endif

static void pcie_phy_wait_power_en(enum pcie_phy_select phy_select)
{
#ifndef CONFIG_BOARD_palladium
	uint32_t timeout = 100000;
	uint32_t mask = PCIE_PIPE_PWR_EN;

	if (phy_select & PCIE_PHY_0_SEL)
		mask |= PCIE_PHY0_PWR_EN;
	if (phy_select & PCIE_PHY_1_SEL)
		mask |= PCIE_PHY1_PWR_EN;

	//wait phy power en done
	while(timeout--) {
		if ((pcie_reg_read(PCIE_PHY_MODE_REG0) & mask) == mask)
			return;
		udelay(10);
	}
	NOTICE("PWR EN: 0x%x.", pcie_reg_read(PCIE_PHY_MODE_REG0));
#endif
}

static void pcie_phy_load_fw(enum pcie_phy_select phy_select)
{
	uint32_t val;
	uint32_t i;
	uint32_t phy_icp_base  = 0;
	uint32_t fw_ex_base = PCIE_PHY_EXT_FW_BASE;

	if(phy_select & PCIE_PHY_0_SEL) {
		phy_icp_base = PCIE_PHY_ICP(0);
		//load fw to phy0 icp
		for(i = 0; i < PCIE_PHY_EXT_FW_SIZE/4; i++) {
			val = mmio_read_32(fw_ex_base + i*4);
			mmio_write_32(phy_icp_base + i*4, val);
		}
		val = pcie_reg_read(PCIE_PHY_MODE_REG1);
		val |= PCIE_PHY0_SRAM_LOAD_DONE;
		pcie_reg_write(PCIE_PHY_MODE_REG1, val);
		INFO("PCIE PHY0 FW loaded.\n");
	}

	if(phy_select & PCIE_PHY_1_SEL) {
		phy_icp_base = PCIE_PHY_ICP(1);
		fw_ex_base = PCIE_PHY_EXT_FW_BASE + PCIE_PHY_EXT_FW_SIZE;
		//load fw to phy1 icp
		for(i = 0; i < PCIE_PHY_EXT_FW_SIZE/4; i++) {
			val = mmio_read_32(fw_ex_base + i*4);
			mmio_write_32(phy_icp_base + i*4, val);
		}
		val = pcie_reg_read(PCIE_PHY_MODE_REG1);
		val |= PCIE_PHY1_SRAM_LOAD_DONE;
		pcie_reg_write(PCIE_PHY_MODE_REG1, val);
		INFO("PCIE PHY1 FW loaded.\n");
	}
}

static void pcie_config_repeat_clk(enum pcie_phy_select phy_select)
{
	uint32_t val =0;

	//config repeat clk
	if(phy_select & PCIE_PHY_0_SEL) {
		val = pcie_reg_read(PCIE_PHY0_MISC_REG1);
		val |= PCIE_PHY0_REF_REPEAT_CLK_EN;
		pcie_reg_write(PCIE_PHY0_MISC_REG1, val);

		val = pcie_reg_read(PCIE_PHY1_MISC_REG0);
		val &= (~PCIE_PHY_REF_REPEAT_CLK_MASK);
		pcie_reg_write(PCIE_PHY1_MISC_REG0, val);
		INFO("PCIE PHY0 REF REPEAT CLK EN.\n");
	}

	if(phy_select == PCIE_PHY_1_SEL) {
		val = pcie_reg_read(PCIE_PHY1_MISC_REG1);
		val |= PCIE_PHY1_REF_REPEAT_CLK_EN;
		pcie_reg_write(PCIE_PHY1_MISC_REG1, val);

		val = pcie_reg_read(PCIE_PHY0_MISC_REG0);
		val &= (~PCIE_PHY_REF_REPEAT_CLK_MASK);
		pcie_reg_write(PCIE_PHY0_MISC_REG0, val);
		INFO("PCIE PHY1 REF REPEAT CLK EN.\n");
	}
}

static bool pcie_phy_is_powered(enum pcie_phy_select phy)
{
	uint32_t mask = 0;

	if (phy & PCIE_PHY_0_SEL)
		mask |= PCIE_PHY0_PWR_EN;
	if (phy & PCIE_PHY_1_SEL)
		mask |= PCIE_PHY1_PWR_EN;

	if (mask == 0)
		return false;

	return (pcie_reg_read(PCIE_PHY_MODE_REG0) & mask) == mask;
}

#ifdef CONFIG_IGNORE_PCIE_TRAP
/*
 * Reverse of pcie_phy_init: clear controller resets, PERST MUX, PHY power,
 * and assert shared PHY reset.  Used when CONFIG_IGNORE_PCIE_TRAP forces a
 * cold ss_mode switch during pcie_parse_trap.
 */
static void pcie_phy_deinit(void)
{
	uint32_t ctrl_id;

	for (ctrl_id = 0; ctrl_id < PCIE_CTRL_MAX; ctrl_id++) {
		uintptr_t intf_base = PCIE_CFG_AXI_CTRL(ctrl_id);
		pcie_reg_clrbits(intf_base + REG_PCIE_RST_CTRL0_OFFSET, 0x3);
	}

	pcie_reg_write(PCIE_PHY_PERST_MUX_REG, 0);
	pcie_reg_clrbits(PCIE_PHY_MODE_REG0, PCIE_PIPE_PWR_GATE_SUPPORT | PCIE_PHY_PWR_STABLE);
	pcie_reg_setbits(PCIE_SS_MODE_REG0, PCIE_PHY_RESET);
}
#endif

static void pcie_phy_init(const struct pcie_boot_config *cfg,
			  enum pcie_phy_select phy_needed,
			  enum pcie_phy_select phy_init_done,
			  enum pcie_ctrl_id ctrl_id)
{
	uint32_t val = 0;
	uint32_t fw_exload = 0;
	uint32_t otp_repeat_clk = 0;
	enum pcie_phy_select phy_to_init = phy_needed;
	bool skip_reset = (phy_init_done != 0);
	bool repeat_clk;

	//phy power stable (idempotent; safe even if a PHY is already up)
	pcie_reg_setbits(PCIE_PHY_MODE_REG0, PCIE_PIPE_PWR_GATE_SUPPORT | PCIE_PHY_PWR_STABLE);

	/*
	 * Both PHYs share PCIE_PHY_RESET (SS_MODE_REG0 bit5). If any PHY is
	 * already initialized (tracked by phy_init_done), skip the shared reset
	 * to avoid knocking down a live PHY.
	 */
	NOTICE("PCIE PHY needed 0x%x, phy_init_done 0x%x\n", phy_needed, phy_init_done);

	if (phy_to_init == 0) {
		NOTICE("PCIE no PHY needs init\n");
		return;
	}

	if (!skip_reset)
		pcie_reg_setbits(PCIE_SS_MODE_REG0, PCIE_PHY_RESET);

	/* check if need to config repeat clk */
	//TODO: otp_repeat_clk = get_sw_info_1()->pcie_repeat_clock;
	repeat_clk = (otp_repeat_clk == 0x01) ||
		((otp_repeat_clk == 0x0 || otp_repeat_clk == 0x3) &&
		cfg->repeat_clk_en);
	// prefer phy 0 clk repeat to phy 1, force now
	if (repeat_clk) {
		pcie_config_repeat_clk(PCIE_PHY_0_SEL);
	}

	val = mmio_read_32(PCIE_PHY_EXT_FW_MAGIC); //magic number
	if (val != 0xcafebeef) {
		//sram_startup_byp=0 & sram_init_byp=0
		pcie_config_phy_bl_bypass(phy_to_init, 0);
	} else {
		//sram_startup_byp=0 & sram_init_byp=1
		INFO("PCIE PHY use external FW.\n");
		fw_exload = 1;
		pcie_config_phy_bl_bypass(phy_to_init, 1);
	}

	if (!skip_reset)
		pcie_reg_clrbits(PCIE_SS_MODE_REG0, PCIE_PHY_RESET);

	//wait phy power up done
	pcie_phy_wait_power_en(phy_to_init);

	pcie_reg_write(PCIE_PHY_PERST_MUX_REG, 0x1);
	if (ctrl_id < PCIE_CTRL_MAX) {
		uintptr_t intf_base = PCIE_CFG_AXI_CTRL(ctrl_id);
		pcie_reg_setbits(intf_base + REG_PCIE_RST_CTRL0_OFFSET, 0x3);
	}

	// wait sram init done
	pcie_wait_sram_init_done(phy_to_init);

	if (fw_exload) {
		pcie_phy_load_fw(phy_to_init);
		pcie_wait_sram_init_done(phy_to_init);
	}
}

static void pcie_config_ctrl(enum pcie_ctrl_id ctrl_id, bool is_rc)
{
	uint32_t val =0;
	uintptr_t sii_base = PCIE_CFG_SII(ctrl_id);
	uintptr_t dbi_base = PCIE_DBI(ctrl_id);

	//config device_type
	val = pcie_reg_read(sii_base + 0x50);
	val &= 0xffffe1ff; //bit[12,9]
	if (is_rc) {
		val |= (4 << 9);  //4: root complex
	} else {
		val |= (0 << 9);  //0: endpoint
	}
	pcie_reg_write((sii_base + 0x50), val);

	//config Directed Speed Change	Writing '1' to this field instructs the LTSSM to initiate
	//a speed change to Gen2 or Gen3 after the link is initialized at Gen1 speed
	if (!is_rc) {
		val = pcie_reg_read(dbi_base + 0x80c);
		val = val | 0x20000;
		pcie_reg_write((dbi_base + 0x80c), val);
	}

	////config generation_select-pcie_cap_target_link_speed
	//val = pcie_reg_read(dbi_base + 0xa0);
	//val = (val & 0xfffffff0) | 4;  //Gen4
	//pcie_reg_write((dbi_base + 0xa0), val);

	//// config ecrc generation enable
	//val = pcie_reg_read(dbi_base + 0x118);
	//val = 0x3e0;
	//pcie_reg_write((dbi_base + 0x118), val);

//#ifdef CONFIG_BOARD_palladium
//	if (is_rc) {
//		val = pcie_reg_read(dbi_base + 0x710);
//		val |= 0x1 << 7; // enable fastlink mode
//		pcie_reg_write(dbi_base + 0x710, val);
//	}
//#endif
		// lane floating
}

static void pcie_config_rc_ranges(uint32_t ctrl_id) {
	uintptr_t dbi_base = PCIE_DBI(ctrl_id);
	struct pcie_range_regs *reg = &pcie_range_regs[ctrl_id];
	struct pcie_addr_range_group *addr = &pcie_range_groups[ctrl_id];

	// config 64bits address range
	pcie_reg_write(PCIE_SUB_CFG + reg->up_start_low, 0x0);
	pcie_reg_write(PCIE_SUB_CFG + reg->up_start_high, addr->up_start_high);
	pcie_reg_write(PCIE_SUB_CFG + reg->up_end_low, 0xFFFFFFFF);
	pcie_reg_write(PCIE_SUB_CFG + reg->up_end_high, addr->up_end_high);
	pcie_reg_write(PCIE_SUB_CFG + reg->up_en, 0x1);

	//config 32bits address range
	pcie_reg_write(PCIE_SUB_CFG + reg->dw_start, addr->dw_start);
	pcie_reg_write(PCIE_SUB_CFG + reg->dw_end, addr->dw_end);
	pcie_reg_write(PCIE_SUB_CFG + reg->dw_en, 0x1);

	//config ecam address range
	pcie_reg_write(dbi_base + 0xc70, addr->ecam_low);
	pcie_reg_write(dbi_base + 0xc74, addr->ecam_high);
	pcie_reg_write(dbi_base + 0xc78, 0x1);
}

static void pcie_config_eq(enum pcie_ctrl_id ctrl_id)
{
	//uint32_t val =0;
	//uintptr_t dbi_base = PCIE_DBI(ctrl_id);

	//config eq
}

static void pcie_config_link(enum pcie_ctrl_id ctrl_id, uint32_t link_width)
{
	uint32_t val =0;
	uintptr_t dbi_base = PCIE_DBI(ctrl_id);

	//config lane_count
	val = pcie_reg_read(dbi_base + 0x8c0);
	val &= 0xffffffc0;
	val |= link_width;
	pcie_reg_write(dbi_base + 0x8c0, val);
}

static void pcie_config_ep_function(enum pcie_ctrl_id ctrl_id, uint32_t func_num,
	uint32_t link_width, enum pcie_link_capable target_link_cap)
{
	uint32_t func, val =0;
	uintptr_t dbi_base = PCIE_DBI(ctrl_id);
	uint32_t cur_link_cap;
	uint32_t vdid = PCIE_DEFAULT_DEVICE_ID;

	//enable DBI_RO_WR_EN
	val = pcie_reg_read(dbi_base + 0x8bc);
	val |= 0x1;
	pcie_reg_write((dbi_base + 0x8bc), val);

	//device id, default 0x16941f1c (customized with OTP)
	val = pcie_reg_read(OTP_PCIE_CUSTOM_ID);
	if (val != 0 && val != 0xffffffff) {
		vdid = val;
	}
	NOTICE("VDID:%x.", vdid);

	//multi-func config
	val = pcie_reg_read(dbi_base + 0xc);
	if (func_num == 1)
		pcie_reg_write(dbi_base + 0xc, val & ~(1 << 23));
	else
		pcie_reg_write(dbi_base + 0xc, val | (1 << 23));

	for (func = 0; func < func_num; func++) { 
		//class id
		val = pcie_reg_read((dbi_base + 0x8) | (func << 16));
		pcie_reg_write(((dbi_base + 0x8) | (func << 16)), (val | (0x1200 << 16)));

		pcie_reg_write((dbi_base | (func << 16)), vdid);

		//max payload size & max request size
		//val = pcie_reg_read((dbi_base | (func << 16)) + 0x78);
		//val &= 0xffff8f1f; //bit[7, 5], bit[14, 12]
		//val |= 0x1 << 5;
		//val |= 0x1 << 12;
		//pcie_reg_write(((dbi_base | (func << 16)) + 0x78), val);

		//msi-x ???
		//val = pcie_reg_read((dbi_base | (func << 16)) + 0xb0);
		//val &= 0xf800ffff; //bit[26, 16]
		//val |= 0x7 << 16;
		//pcie_reg_write(((dbi_base | (func << 16)) + 0xb0), val);

		// lane floating
		cur_link_cap = pcie_reg_read((dbi_base | (func << 16)) + 0x710);
		cur_link_cap = (cur_link_cap >> 16) & 0x3F;
		if (cur_link_cap != target_link_cap) {
			val = pcie_reg_read((dbi_base | (func << 16)) + 0x710);
			val &= 0xffc0ffff; //bit[21, 16]
			val |= target_link_cap << 16;
			pcie_reg_write(((dbi_base | (func << 16)) + 0x710), val);
			val = pcie_reg_read((dbi_base | (func << 16)) + 0x80c);
			val &= 0xffffe0ff; //bit[12, 8]
			val |= link_width << 8;
			pcie_reg_write(((dbi_base | (func << 16)) + 0x80c), val);
			val = pcie_reg_read((dbi_base | (func << 16)) + 0x7c);
			val &= 0xfffffc0f; //bit[9, 4]
			val |= link_width << 4;
			pcie_reg_write(((dbi_base | (func << 16)) + 0x7c), val);
		}
	}

	if (func_num > 1) {
		//max func_num that can be used in a request
		val = pcie_reg_read(dbi_base + 0x718);
		val &= ~0xff;
		val |= (func_num - 1);
		pcie_reg_write(dbi_base + 0x718, val);
	}

	//disable DBI_RO_WR_EN
	val = pcie_reg_read(dbi_base + 0x8bc);
	val &= ~0x1;
	pcie_reg_write((dbi_base + 0x8bc), val);
}

static void pcie_config_single_bar(uint32_t dbi_base, uint8_t bar_index,
	uint8_t bar_cfg, uint64_t size_mask)
{
	uint32_t dbi2_base = dbi_base + PCIE_DBI2_OFFSET;
	uint32_t offset = 0x10 + (bar_index * 4);
	uint32_t bar_config = ((bar_cfg & 0x80) ? PCIE_BAR_64BIT : 0) |
		((bar_cfg & 0x40) ? PCIE_BAR_PREFETCHABLE : 0);

	// Disable next BAR first if 64-bit
	if (bar_config & PCIE_BAR_64BIT) {
		pcie_reg_write(dbi2_base + offset + 4, 0);
	}

	// Write BAR config and size
	pcie_reg_write(dbi_base + offset, bar_config);
	pcie_reg_write(dbi2_base + offset, size_mask & 0xffffffff);

	// Write upper 32 bits for 64-bit BAR
	if (bar_config & PCIE_BAR_64BIT) {
		pcie_reg_write(dbi2_base + offset + 4, (size_mask >> 32) & 0xffffffff);
	}
}

#define READ_U64(addr) (((uint64_t)mmio_read_32((addr) + 4) << 32) | \
                        (uint64_t)mmio_read_32(addr))

static void pcie_config_ep_bar_size(uint32_t dbi_base, uint32_t bar4_set,
	uint32_t func_num, uint32_t board_id)
{
	uint64_t bar_config_0, bar_config_1;
	uint8_t bar_config;
	uint8_t i;
	uint64_t size_mask = 0;

	/* TODO: wire OTP bar size (OTP_BAR_SIZE_CONFIG0); primary is hardcoded */
	bar_config_1 = READ_U64(OTP_BAR_SIZE_CONFIG1);

	if (func_num == 4)
		bar_config_0 = 0;
	else
		bar_config_0 = 0xad00960096ULL;

	// Process each BAR
	for (i = 0; i < 6; i++) {
		bar_config = (bar_config_0 >> (i * 8)) & 0xff;
		// Use backup config if primary is invalid
		if (bar_config == 0xff) {
			bar_config = (bar_config_1 >> (i * 8)) & 0xff;
		}

		if (i == 4 && func_num < 4 && ((dbi_base >> 16) & 0xf) == 0) {
			if (board_id == 1)
				size_mask = 0x1ULL<< 52;

			if (func_num == 3)
				size_mask |= 0x41FFFFFFFFFFFULL;

			if (func_num == 2)
				size_mask |= 0x61FFFFFFFFFFFULL;

			if (func_num == 1)
				size_mask |= 0x81FFFFFFFFFFFULL;
			pcie_config_single_bar(dbi_base, 4, 0xC0, size_mask); // 0xC = 64-bit | prefetchable
			continue;
		}

		if (bar_config != 0 && bar_config != 0xff) {
			// Clear 64-bit flag for odd numbered BARs
			if ((i % 2) && (bar_config & 0x80)) {
				bar_config &= 0x7f;
			}

			size_mask = (1ULL << (bar_config & 0x3f)) - 1;
			pcie_config_single_bar(dbi_base, i, bar_config, size_mask);

			// Skip next BAR if 64-bit
			if (bar_config & 0x80) {
				i++;
			}
		} else if (i == 4) {
			// Special handling for BAR4
			size_mask = BAR4_SIZE_MASKS[bar4_set];
			pcie_config_single_bar(dbi_base, 4, 0xC0, size_mask); // 0xC = 64-bit | prefetchable
		}
	}
}

static void pcie_config_ep_cap(enum pcie_ctrl_id ctrl_id, uint32_t func_num,
	uint32_t bar4_set, uint32_t board_id)
{
	uint32_t dbi_base = PCIE_DBI(ctrl_id);
	uint32_t func, val =0;

	//enable DBI_RO_WR_EN
	val = pcie_reg_read(dbi_base + 0x8bc);
	val |= 0x1;
	pcie_reg_write((dbi_base + 0x8bc), val);

	for (func = 0; func < func_num; func++) {
		// 1. pcie subdevice id
		// one word all chip
		// chip0 func0 subdevice id = 0x13, chip0 func1 subdevice id = 0x12,
		//    chip0 func2 subdevice id = 0x11, chip0 func3 subdevice id = 0x10
		// chip1 func0 subdevice id = 0x12, chip1 func1 subdevice id = 0x11,
		//    chip1 func2 subdevice id = 0x10
		// chip2 func0 subdevice id = 0x11, chip2 func1 subdevice id = 0x10
		// chip3 func0 subdevice id = 0x10
		val = pcie_reg_read((dbi_base | (func << 16)) + 0x2c);
		val &= 0xffff;
		val |= (0x10 + func_num - 1) << 16;
		pcie_reg_write(((dbi_base | (func << 16)) + 0x2c), val);

		// 2. disable ATS、ACS
		if (func == 0) {
			val = pcie_reg_read((dbi_base + 0x1b8));
			val = (val & 0xfffff) | (0x1f8 << 20);
			pcie_reg_write((dbi_base + 0x1b8), val);
		} else {
			val = pcie_reg_read((dbi_base + 0x100) | (func << 16));
			val = (val & 0xfffff) | (0x220 << 20);
			pcie_reg_write(((dbi_base + 0x100) | (func << 16)), val);
		}

		// 3. enable FLR
		val = pcie_reg_read((dbi_base | (func << 16)) + 0x74);
		val |= (0x1 << 28);
		pcie_reg_write(((dbi_base | (func << 16)) + 0x74), val);

		//enable memTLP for all bars
		pcie_reg_write(((dbi_base | (func << 16)) + 0x81c), (func << 16) | 0x3F);

		// 4. bar size config
		pcie_config_ep_bar_size(dbi_base + (func << 16), bar4_set & 0x3,
			func_num, board_id);
	}

	//disable DBI_RO_WR_EN
	val = pcie_reg_read(dbi_base + 0x8bc);
	val &= ~0x1;
	pcie_reg_write((dbi_base + 0x8bc), val);

}

//static void pcie_config_axi_route(enum pcie_ctrl_id ctrl_id)
//{
//	//start: 0x20000000, end: 0x21ffffff
//	INFO("CFG_START: 0x%lx\n", mmio_read_64((PCIE_SUB_CFG + 0xfc)));
//	INFO("CFG_END: 0x%lx\n", mmio_read_64((PCIE_SUB_CFG + 0x104)));
//}
static void pcie_config_bar0_iatu(enum pcie_ctrl_id ctrl_id)
{
	uintptr_t dbi_base = PCIE_DBI(ctrl_id);

	//config bar0 iatu
	pcie_reg_write((dbi_base + PCIE_iATU_OFFSET + 0x114), dbi_base & 0xffffffff);
	pcie_reg_write((dbi_base + PCIE_iATU_OFFSET + 0x118), 0x0);
	pcie_reg_write((dbi_base + PCIE_iATU_OFFSET + 0x100), 0x0);
	pcie_reg_write((dbi_base + PCIE_iATU_OFFSET + 0x104), 0xC0080000);
}

static void pcie_disable_ltssm(enum pcie_ctrl_id ctrl_id)
{
	uint32_t val = 0;
	uintptr_t sii_base = PCIE_CFG_SII(ctrl_id);

	val = pcie_reg_read(sii_base + 0x58);
	val &= ~0x1;
	pcie_reg_write((sii_base + 0x58), val);
}

static void pcie_enable_ltssm(enum pcie_ctrl_id ctrl_id)
{
	uint32_t val = 0;
	uintptr_t sii_base = PCIE_CFG_SII(ctrl_id);

	val = pcie_reg_read(sii_base + 0x58);
	val |= 0x1;
	pcie_reg_write((sii_base + 0x58), val);
}

static int pcie_wait_core_clk_active(enum pcie_ctrl_id ctrl_id, bool is_rc)
{
	uint32_t val = 0;
	uintptr_t intf_base = PCIE_CFG_AXI_CTRL(ctrl_id);
	//uintptr_t sii_base = PCIE_CFG_SII(ctrl_id);
	uint32_t timeout;

	pcie_reg_setbits(intf_base + REG_PCIE_RST_CTRL0_OFFSET, 0x3);

	pcie_reg_write(intf_base + REG_AXI_MASTER_ACELITE_CTL_OFFSET, 0x2);

	//wait core debug irq (bit 14)
	timeout = PCIE_CORE_CLK_TIMEOUT_US / 10;
	while (timeout--) {
		if (mmio_read_32(intf_base + REG_CORE_DBG_IRQ0_OFFSET) & (0x1 << 14))
			break;
		udelay(10);
	}
	if (timeout == (uint32_t)-1) {
		ERROR("PCIE ctrl%d core dbg irq timeout\n", ctrl_id);
		return -1;
	}

	//wait core clock active
	timeout = PCIE_CORE_CLK_TIMEOUT_US / 10;
	while (timeout--) {
		val = mmio_read_32(intf_base + REG_CORE_CLK_STATUS_OFFSET);
		if (val & 0x1)
			return 0;
		udelay(10);
	}

	ERROR("PCIE ctrl%d core clk not active: 0x%x\n", ctrl_id, val);
	return -1;
}

static void pcie_config_rc_cap(enum pcie_ctrl_id ctrl_id, uint32_t link_width, enum pcie_link_capable link_cap)
{
	uint32_t dbi_base = PCIE_DBI(ctrl_id);
	uint32_t val = 0;

	//pcie rc specific config
	val = pcie_reg_read(dbi_base + 0x8bc);
	val |= 0x1;
	pcie_reg_write((dbi_base + 0x8bc), val);

	pcie_reg_setbits(dbi_base + 0x890, 0x1 << 11); //clear EQ_redo

	val = pcie_reg_read(dbi_base + 0x710);
	val &= 0xffc0ffff; //bit[21, 16]
	val |= link_cap << 16;
	pcie_reg_write(dbi_base + 0x710, val);

	val = pcie_reg_read(dbi_base + 0x80c);
	val &= 0xffffe0ff; //bit[12, 8]
	val |= link_width << 8;
	pcie_reg_write(dbi_base + 0x80c, val);

	val = pcie_reg_read(dbi_base + 0x4);
	val |= 0x7;
	pcie_reg_write(dbi_base + 0x4, val);

	val = pcie_reg_read(dbi_base + 0x78);
	val &= ~(0x7 << 5);
	val |= (0x1 << 5);
	pcie_reg_write(dbi_base + 0x78, val);

	pcie_reg_write(dbi_base + 0x10, 0x4);
	pcie_reg_write(dbi_base + 0x14, 0x0);
	pcie_reg_write(dbi_base + 0x04, 0x110107);
	pcie_reg_write(dbi_base + 0x10, 0x0);
	pcie_reg_write(dbi_base + 0x08, 0x06040001);

	val = pcie_reg_read(dbi_base + 0x8bc);
	val &= ~0x1;
	pcie_reg_write((dbi_base + 0x8bc), val);
}

static void pcie_config_rc_bus_resource(uint32_t ctrl_id)
{
	uintptr_t dbi_base = PCIE_DBI(ctrl_id);
	uintptr_t core_base = PCIE_CFG_CORE_CTRL(ctrl_id);
	uintptr_t sii_base = PCIE_CFG_SII(ctrl_id);
	struct pcie_bus_resource *bus_resource = &pcie_bus_resources[ctrl_id];
	uint32_t prim_bus = bus_resource->primary;
	uint32_t sec_bus = bus_resource->secondary;
	uint32_t sub_bus = bus_resource->subordinate;

	pcie_reg_write(sii_base + 0x54, (pcie_reg_read(sii_base + 0x54) & ~0xff) | prim_bus);
	pcie_reg_write(core_base + 0x300, prim_bus);
	pcie_reg_write(dbi_base + 0x18, (sub_bus << 16) | (sec_bus << 8) | (prim_bus));
}

#define PCIE_LINK_STATUS_MAX 64

static void pcie_wait_link_up(enum pcie_ctrl_id ctrl_id)
{
	uintptr_t sii_base = PCIE_CFG_SII(ctrl_id);
	uintptr_t dbi_base = PCIE_DBI(ctrl_id);
	uint32_t status, status_pre = 0;
	uint32_t timeout = PCIE_LINKUP_TIMEOUT_US / 10;
	bool speed_change_done = true;
	uint32_t status_log[PCIE_LINK_STATUS_MAX];
	uint32_t log_cnt = 0;

	while (timeout--) {
		status = mmio_read_32(sii_base + 0xB4);

		if (status != status_pre) {
			status_pre = status;
			if (log_cnt < PCIE_LINK_STATUS_MAX)
				status_log[log_cnt++] = status;
		}

		if ((status & 0x1f) == LTSSM_STATE_S_L0) {
			if (status == 0x2d1 && !speed_change_done) {
				//0x2d1: Gen3 L0, trigger directed speed change once again
				speed_change_done = true;
				pcie_reg_setbits(dbi_base + 0x80c, 0x20000);
			} else {
				//link up: target-gen L0, or Gen1 L0 after speed change triggered
				NOTICE("link up: 0x%x\n", status);
				NOTICE("lane: 0x%x\n", pcie_reg_read(sii_base + 0xB0));
				console_flush();
				break;
			}
		}
		udelay(10);
	}

	NOTICE("PCIE ctrl%d link status transitions(%u):\n",
		ctrl_id, log_cnt);
	for (uint32_t i = 0; i < log_cnt; i++)
		NOTICE("  [%u] 0x%x\n", i, status_log[i]);
	console_flush();
}

/* program one iATU outbound region: map [src_start, src_end] to target */
static void pcie_atu_config_region(uintptr_t atu_base, uint32_t region_off,
	uint64_t src_start, uint64_t src_end, uint64_t target)
{
	mmio_write_32(atu_base + region_off + 0x8, src_start & 0xffffffff);		/* LWR_BASE_ADDR */
	mmio_write_32(atu_base + region_off + 0xc, (src_start >> 32) & 0xffffffff);	/* UPPER_BASE_ADDR */
	mmio_write_32(atu_base + region_off + 0x10, src_end & 0xffffffff);		/* LIMIT_ADDR */
	mmio_write_32(atu_base + region_off + 0x20, (src_end >> 32) & 0xffffffff);	/* UPPER_LIMIT_ADDR, valid when INCREASE_REGION_SIZE set */
	mmio_write_32(atu_base + region_off + 0x14, target & 0xffffffff);		/* LWR_TARGET_ADDR */
	mmio_write_32(atu_base + region_off + 0x18, (target >> 32) & 0xffffffff);	/* UPPER_TARGET_ADDR */
	mmio_write_32(atu_base + region_off + 0x0, 0x2000); //Increase the maximum ATU Region size
	mmio_write_32(atu_base + region_off + 0x4, 0x80000000);
}

#define PCIE_VENDOR_ID_CV	0x1f1c

static uintptr_t pcie_ecam_base(enum pcie_ctrl_id ctrl_id)
{
	return (uint64_t)(pcie_range_groups[ctrl_id].ecam_high) << 32 |
		pcie_range_groups[ctrl_id].ecam_low;
}

/*
 * Poll the function's cfg space until it reports our vendor id (or timeout).
 * vendor_id is carried across funcs so an already-found EP is not re-polled.
 */
static int pci_scan_wait_vendor(uintptr_t cfg_base_addr, int bus_num,
	int device_num, uint32_t var_func_num, int vendor_id)
{
	uint32_t count = 600;

	while (vendor_id != PCIE_VENDOR_ID_CV) {
		NOTICE("cfg_base_addr = 0x%lx, bus num = %d, device = %d, function = %d\n",
			cfg_base_addr, bus_num, device_num, var_func_num);
		vendor_id = mmio_read_32(cfg_base_addr) & 0xffff;
		udelay(10);
		if (--count == 0) {
			ERROR("bus num = %d, device = %d, function = %d,  not find, vendor_id = 0x%x \n",
				bus_num, device_num, var_func_num, vendor_id);
			break;
		}
	}
	return vendor_id;
}

/* program bar0/bar1/bar4 of the scanned function and enable mem decode */
static void pci_scan_program_bars(uintptr_t cfg_base_addr,
	uint32_t bar0_base, uint32_t bar1_base, uint32_t bar4_base)
{
	mmio_write_32(cfg_base_addr + 0x10, bar0_base);
	mmio_write_32(cfg_base_addr + 0x14, bar1_base);
	mmio_write_32(cfg_base_addr + 0x18, 0);
	mmio_write_32(cfg_base_addr + 0x1c, 0);
	mmio_write_32(cfg_base_addr + 0x20, 0);
	mmio_write_32(cfg_base_addr + 0x24, bar4_base);
	mmio_setbits_32(cfg_base_addr + 0x4, 0x7);
	mmio_setbits_32(cfg_base_addr + 0x50, 0x1 << 16);
	//TODO: get the EP confit then config max payload size
	mmio_write_32(cfg_base_addr + 0x78, (mmio_read_32(cfg_base_addr + 0x78) & ~(0x7 << 5)) | (0x1 << 5));

	NOTICE("BAR: 0x%x 0x%x\n", mmio_read_32(cfg_base_addr + 0x10), mmio_read_32(cfg_base_addr + 0x14));
	NOTICE("BAR: 0x%x 0x%x\n", mmio_read_32(cfg_base_addr + 0x18), mmio_read_32(cfg_base_addr + 0x1c));
	NOTICE("BAR: 0x%x 0x%x\n", mmio_read_32(cfg_base_addr + 0x20), mmio_read_32(cfg_base_addr + 0x24));
}

static int pci_switch_bus_scan(enum pcie_ctrl_id ctrl_id)
{
	int bus_num = pcie_bus_resources[ctrl_id].secondary;
	uintptr_t atu_base = PCIE_DBI(ctrl_id) + PCIE_iATU_OFFSET;
	uintptr_t cfg_base = pcie_ecam_base(ctrl_id);
	uint32_t var_func_num;
	int vendor_id = 0;

	//config port code for switch mode pass-through
	mmio_write_32(PCIE_SUB_CFG + 0x400, 0x20033333);
	mmio_write_32(PCIE_SUB_CFG + 0x302c, 0x00011111);

	for (var_func_num = 0; var_func_num < 4; var_func_num++) {
		uintptr_t cfg_base_addr = cfg_base + ((bus_num << 20) | (var_func_num << 12));
		uint32_t bar0_base = cfg_base + (var_func_num + 1) * 0x400000;
		uint32_t bar1_base = cfg_base + (var_func_num + 1) * 0x800000;
		uint32_t bar4_base = (ctrl_id == PCIE_CTRL_X4_0) ?
			0x320000 + var_func_num * 0x2000 :
			0x220000 + var_func_num * 0x2000;

		vendor_id = pci_scan_wait_vendor(cfg_base_addr, bus_num, 0,
			var_func_num, vendor_id);

		NOTICE("scanning bus num = %d, device = %d, function = %d, vendor_id = 0x%x \n",
			bus_num, 0, var_func_num, vendor_id);
		udelay(10);
		if (vendor_id != PCIE_VENDOR_ID_CV)
			continue;

		pci_scan_program_bars(cfg_base_addr, bar0_base, bar1_base, bar4_base);

		//config atu for cfg space access
		pcie_atu_config_region(atu_base, 0x3c00 - var_func_num * 0x600,
			cfg_base | 0x400000 | (var_func_num << 24),
			cfg_base | 0x7fffff | (var_func_num << 24), bar0_base);
		pcie_atu_config_region(atu_base, 0x3a00 - var_func_num * 0x600,
			cfg_base | 0x800000 | (var_func_num << 24),
			cfg_base | 0xbfffff | (var_func_num << 24), bar1_base);
		pcie_atu_config_region(atu_base, 0x3800 - var_func_num * 0x600,
			(cfg_base + 0x10000000) | (var_func_num << 26),
			(cfg_base + 0x13ffffff) | (var_func_num << 26), bar4_base);
	}
	return 0;
}

static int pci_bus_scan(enum pcie_ctrl_id ctrl_id, uint32_t func_num, uint32_t board_id)
{
	int bus_num = pcie_bus_resources[ctrl_id].secondary;
	uintptr_t atu_base = PCIE_DBI(ctrl_id) + PCIE_iATU_OFFSET;
	uintptr_t cfg_base = pcie_ecam_base(ctrl_id);
	uint32_t chip_id = 0;
	uint64_t bar0_src_base = 0, bar1_src_base = 0;

	(void)board_id;
	(void)func_num;

	if (pcie_is_switch_mode())
		return pci_switch_bus_scan(ctrl_id);

	/* non-switch mode: cascade chips rc, decode chip position from port_code */
	uint32_t port_code = mmio_read_32(PCIE_SUB_CFG + 0x400);

	if ((port_code & 0xf) == 6) {
		chip_id = 1;
	} else if (((port_code >> 4) & 0xf) == 6) {
		chip_id = 2;
	} else if (((port_code >> 8) & 0xf) == 6) {
		chip_id = 3;
	} else if (((port_code >> 12) & 0xf) == 6) {
		chip_id = 4;
	}

	NOTICE("port_code=0x%x, chip_id=%d\n", port_code, chip_id);

	switch (chip_id) {
	case 1:
		bar0_src_base = 0x4600000000000ULL;
		bar1_src_base = 0x4800000000000ULL;
		break;
	case 2:
		bar0_src_base = 0x6c00000000000ULL;
		bar1_src_base = 0x6e00000000000ULL;
		break;
	case 3:
		bar0_src_base = 0x9200000000000ULL;
		bar1_src_base = 0x9400000000000ULL;
		break;
	default:
		break;
	}

	if (chip_id > 1) {
		//resize our EP bar4 to 0x400_0000_0000_0000
		uintptr_t dbi_base = PCIE_DBI(PCIE_CTRL_X8_0);

		mmio_setbits_32(dbi_base + 0x8bc, 0x1); /* DBI_RO_WR_EN */
		mmio_setbits_32(dbi_base + PCIE_DBI2_OFFSET + 0x20, 0xffffffff);
		mmio_setbits_32(dbi_base + PCIE_DBI2_OFFSET + 0x24, 0x3ffffff);
		mmio_clrbits_32(dbi_base + 0x8bc, 0x1);
	}


	uint32_t board_size = port_code >> 27 & 0x7;
	if (chip_id == board_size) {
		NOTICE("chip_id=%d, board_size=%d, port_code=0x%x\n",
			chip_id, board_size, port_code);
			return 0;
	}

	/* scan function 0 only; bar bases are fixed for all cascade funcs */
	uintptr_t cfg_base_addr = cfg_base + (bus_num << 20);
	uint32_t bar0_base = 0x80000000;
	uint32_t bar1_base = 0x80400000;
	uint32_t bar4_base = 0;
	int vendor_id = pci_scan_wait_vendor(cfg_base_addr, bus_num, 0, 0, 0);

	NOTICE("scanning bus num = %d, device = %d, function = %d, vendor_id = 0x%x \n",
		bus_num, 0, 0, vendor_id);
	udelay(10);
	if (vendor_id != PCIE_VENDOR_ID_CV)
		return 0;

	pci_scan_program_bars(cfg_base_addr, bar0_base, bar1_base, bar4_base);

	//config atu: translate chip-specific 64bit windows to bar bases
	if (bar0_src_base) {
		pcie_atu_config_region(atu_base, 0x3c00, bar0_src_base,
			bar0_src_base + 0x7fffff, bar0_base);
		pcie_atu_config_region(atu_base, 0x3a00, bar1_src_base,
			bar1_src_base + 0x3fffff, bar1_base);
	} else {
		NOTICE("no atu window for chip_id=%d, skip atu config\n", chip_id);
	}
	return 0;
}

static void pcie_chip_rc_downstream_config(enum pcie_ctrl_id ctrl_id,
					    uint32_t func_num, uint32_t board_id)
{
	uintptr_t atu_base = PCIE_DBI(ctrl_id) + PCIE_iATU_OFFSET;
	struct pcie_addr_range_group *addr = &pcie_range_groups[ctrl_id];
	uint32_t cfg_base = addr->dw_start;

	//config atu for cfg space access
	mmio_write_32(atu_base + 0x3e08, cfg_base);
	mmio_write_32(atu_base + 0x3e0c, 0x0);
	mmio_write_32(atu_base + 0x3e10, cfg_base + 0x3fffff);
	mmio_write_32(atu_base + 0x3e14, 0x0);
	mmio_write_32(atu_base + 0x3e18, 0x0);
	mmio_write_32(atu_base + 0x3e00, 0x4);
	mmio_write_32(atu_base + 0x3e04, 0x90000000);

	NOTICE("cfg access enabled\n");
	pci_bus_scan(ctrl_id, func_num, board_id);
}

static void pcie_ctrl_init(const struct pcie_ctrl_setup *setup)
{
	enum pcie_ctrl_id ctrl_id = setup->ctrl_id;

	NOTICE("PCIE ctrl%d init: is_rc=%d, phy=0x%x, link_width=%d, link_cap=0x%x\n",
		ctrl_id, setup->is_rc, setup->phy_select, setup->link_width, setup->link_cap);

	if (pcie_wait_core_clk_active(ctrl_id, setup->is_rc) != 0) {
		return;
	}

	//disbale ltssm before config
	pcie_disable_ltssm(ctrl_id);

	//check radm status
	pcie_config_ctrl(ctrl_id, setup->is_rc);
	pcie_config_eq(ctrl_id);
	pcie_config_link(ctrl_id, setup->link_width);

	if (setup->is_rc) {
		pcie_config_rc_ranges(ctrl_id);
		pcie_config_rc_bus_resource(ctrl_id);
		pcie_config_rc_cap(ctrl_id, setup->link_width, setup->link_cap);
	} else {
		pcie_config_ep_function(ctrl_id, setup->func_num, setup->link_width,
			setup->link_cap);
		pcie_config_ep_cap(ctrl_id, setup->func_num, setup->bar4_set,
			setup->board_id);
		//pcie_config_axi_route(ctrl_id);
		pcie_config_bar0_iatu(ctrl_id);
	}
	pcie_enable_ltssm(ctrl_id);
	pcie_wait_link_up(ctrl_id);

	/*
	 * For RC controllers: do downstream config so the bus can be scanned.
	 * Switch-mode port config values differ from the EP path (which uses
	 * pcie_chip_config_port_code in the ROM).
	 */
	if (setup->is_rc) {
		pcie_chip_rc_downstream_config(ctrl_id, setup->func_num, setup->board_id);
	}
}

static uint32_t pcie_get_trap_config(void)
{
	return pcie_reg_read(PCIE_TRAP_REG);
}

/*
 * Decode the raw trap word (and compile-time overrides) into pcie_boot_config
 * once, so the rest of pcie_init never re-reads PCIE_TRAP_REG or mutates the raw
 * trap value. In CONFIG_IGNORE_PCIE_TRAP builds the ss_mode is taken from the
 * build config and programmed into hardware here; otherwise it is read back from
 * SS_MODE_REG0 and rom_ctrl_id marks the controller the ROM already brought up.
 */
static void pcie_parse_trap(struct pcie_boot_config *cfg)
{
	uint32_t trap = pcie_get_trap_config();

	NOTICE("TR.0x%x.", trap);

	cfg->func_num      = pcie_trap_get_func_num(trap);
	cfg->bar4_set      = pcie_trap_get_bar4_set(trap);
	cfg->board_id      = pcie_trap_get_board_id(trap);
	cfg->repeat_clk_en = pcie_trap_get_repeat_clk(trap);

#ifdef CONFIG_IGNORE_PCIE_TRAP
	cfg->ignore_trap = true;
	cfg->ss_mode     = CONFIG_SSMODE;
	cfg->rom_ctrl_id = PCIE_CTRL_INVALID;
	/* deinit PHY before programming new ss_mode into hardware */
	pcie_phy_deinit();
	pcie_config_ssmode(cfg->ss_mode);
#else
	cfg->ignore_trap = false;
	cfg->ss_mode     = pcie_get_ssmode();
	cfg->rom_ctrl_id = pcie_trap_get_ctrl_id(trap);
#endif

	NOTICE("PCIE cfg: ignore=%d ss=%d rom_ctrl=%d func=%d bar4=%d clk=%d\n",
	       cfg->ignore_trap, cfg->ss_mode, cfg->rom_ctrl_id,
	       cfg->func_num, cfg->bar4_set, cfg->repeat_clk_en);
}

void pcie_init(void)
{
	struct pcie_boot_config cfg = {0};
	enum pcie_phy_select phy_init_done = 0;
	uint32_t ctrl_id;

	NOTICE("\nPCIe initialization start\n");

	//if (pcie_reg_read(PCIE_BOOT_REG) & SKIP_PCIEI)
	//	return;

	pcie_parse_trap(&cfg);

	/* trap mode: the ROM already brought up rom_ctrl_id, don't touch it */
	if (!cfg.ignore_trap && cfg.rom_ctrl_id < PCIE_CTRL_MAX)
		pcie_ctrl_modes[cfg.rom_ctrl_id] = PCIE_CTRL_DISABLED;

	// 1. add phy init done flag: mark ROM PHY as done if already powered
	if (!cfg.ignore_trap && cfg.rom_ctrl_id < PCIE_CTRL_MAX) {
		enum pcie_phy_select rom_phy =
			pcie_chip_configs[cfg.ss_mode][cfg.rom_ctrl_id].phy_select;

		if (pcie_phy_is_powered(rom_phy))
			phy_init_done |= rom_phy;
	}

	for (ctrl_id = 0; ctrl_id < PCIE_CTRL_MAX; ctrl_id++) {
		struct pcie_ctrl_setup setup = {0};
		const struct pcie_chip_config *chip =
			&pcie_chip_configs[cfg.ss_mode][ctrl_id];

		if (pcie_ctrl_modes[ctrl_id] == PCIE_CTRL_DISABLED)
			continue;
		if (chip->phy_select == 0)
			continue;

		setup.ctrl_id    = ctrl_id;
		setup.is_rc      = (pcie_ctrl_modes[ctrl_id] == PCIE_CTRL_MODE_RC);
		setup.phy_select = chip->phy_select;
		setup.link_width = chip->link_width;
		setup.link_cap   = chip->link_capable;
		setup.func_num   = cfg.func_num;
		setup.bar4_set   = cfg.bar4_set;
		setup.board_id   = cfg.board_id;

		// 2. perst for rc controller
		if (setup.is_rc) {
			uintptr_t intf_base = PCIE_CFG_AXI_CTRL(ctrl_id);
			pcie_reg_write(intf_base + 0x08, 0x140);
			udelay(100000);
			pcie_reg_write(intf_base + 0x08, 0x740);
			udelay(300000);
		}

		// 3. for each controller, if phy not initialized, init phy first, then init ctrl
		if (!(setup.phy_select & phy_init_done)) {
			pcie_phy_init(&cfg, setup.phy_select, phy_init_done, ctrl_id);
			phy_init_done |= setup.phy_select;
		}

		pcie_ctrl_init(&setup);
	}

	NOTICE("PCIe initialization end\n");
}
