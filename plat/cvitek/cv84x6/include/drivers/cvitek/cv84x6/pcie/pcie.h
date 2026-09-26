#ifndef __PCIE_H__
#define __PCIE_H__

#include <stdbool.h>
#include <stdint.h>

#define PCIE_DBI_BASE 0x20000000
#define PCIE_DBI_SIZE 0x400000

#define PCIE_CFG_BASE 0x21300000
#define PCIE_CFG_SIZE 0x4000

#define PCIE_PHY_INTF 0x21390000
#define PCIE_PHY_CR_BASE 0x21400000
#define PCIE_PHY_CR_SIZE 0x40000
/*
h1B8	PHY_MODE_REG_00
			pipe_pwr_stable			18	18	1	h0	rw
			phy0_vddcore_pwr_stable	19	19	1	h0	rw
			phy0_vpdig_pwr_stable	20	20	1	h0	rw
			phy1_vddcore_pwr_stable	21	21	1	h0	rw
			phy1_vpdig_pwr_stable	22	22	1	h0	rw
			pipe_pwr_en				23	23	1	h0	ro
			phy0_vddcore_pwr_en		24	24	1	h0	ro
			phy1_vddcore_pwr_en		25	25	1	h0	ro
			phy0_vpdig_pwr_en		26	26	1	h0	ro
			phy1_vpdig_pwr_en		27	27	1	h0	ro
			pipe_pwr_gate_support	28	28	1	h0	rw
h1BC	PHY_MODE_REG_01
			phy0_sram_init_byp	0	0	1	h0	rw
			phy0_sram_startup_byp	1	1	1	h0	rw
			phy0_sram_load_done	2	2	1	h0	rw
			phy1_sram_init_byp	3	3	1	h0	rw
			phy1_sram_startup_byp	4	4	1	h0	rw
			phy1_sram_load_done	5	5	1	h0	rw
			phy0_sram_init_done	6	6	1	h0	ro
			phy1_sram_init_done	7	7	1	h0	ro
h1c0	SS_MODE_REG_00
			ss_mode					2	0	3	h0	rw
			phy_reset			5	5	1	h1	rw
*/
#define PCIE_PHY_MODE_REG0_OFFSET 0x1B8
#define PCIE_PHY_MODE_REG0 (PCIE_PHY_INTF + PCIE_PHY_MODE_REG0_OFFSET)
#define PCIE_PIPE_PWR_STABLE (1 << 18)
#define PCIE_PHY0_VDDCORE_PWR_STABLE (1 << 19)
#define PCIE_PHY0_VPDIG_PWR_STABLE (1 << 20)
#define PCIE_PHY0_PWR_STABLE (PCIE_PHY0_VDDCORE_PWR_STABLE | PCIE_PHY0_VPDIG_PWR_STABLE)
#define PCIE_PHY1_VDDCORE_PWR_STABLE (1 << 21)
#define PCIE_PHY1_VPDIG_PWR_STABLE (1 << 22)
#define PCIE_PHY1_PWR_STABLE (PCIE_PHY1_VDDCORE_PWR_STABLE | PCIE_PHY1_VPDIG_PWR_STABLE)
#define PCIE_PIPE_PWR_EN (1 << 23)
#define PCIE_PHY0_VDDCORE_PWR_EN (1 << 24)
#define PCIE_PHY0_VPDIG_PWR_EN (1 << 26)
#define PCIE_PHY0_PWR_EN (PCIE_PHY0_VDDCORE_PWR_EN | PCIE_PHY0_VPDIG_PWR_EN)
#define PCIE_PHY1_VDDCORE_PWR_EN (1 << 25)
#define PCIE_PHY1_VPDIG_PWR_EN (1 << 27)
#define PCIE_PHY1_PWR_EN (PCIE_PHY1_VDDCORE_PWR_EN | PCIE_PHY1_VPDIG_PWR_EN)
#define PCIE_PIPE_PWR_GATE_SUPPORT (1 << 28)

#define PCIE_PHY_PWR_STABLE (PCIE_PIPE_PWR_STABLE | PCIE_PHY0_PWR_STABLE | PCIE_PHY1_PWR_STABLE)
#define PCIE_PHY_PWR_EN (PCIE_PIPE_PWR_EN | PCIE_PHY0_PWR_EN | PCIE_PHY1_PWR_EN)

#define PCIE_PHY_MODE_REG1_OFFSET 0x1BC
#define PCIE_PHY_MODE_REG1 (PCIE_PHY_INTF + PCIE_PHY_MODE_REG1_OFFSET)
#define PCIE_PHY0_BL_BYPASS_EN (3 << 0)
#define PCIE_PHY0_SRAM_INIT_BYP (1 << 0)
#define PCIE_PHY0_SRAM_STARTUP_BYP (1 << 1)
#define PCIE_PHY0_SRAM_LOAD_DONE (1 << 2)
#define PCIE_PHY1_BL_BYPASS_EN (3 << 3)
#define PCIE_PHY1_SRAM_INIT_BYP (1 << 3)
#define PCIE_PHY1_SRAM_STARTUP_BYP (1 << 4)
#define PCIE_PHY1_SRAM_LOAD_DONE (1 << 5)
#define PCIE_PHY0_SRAM_INIT_DONE (1 << 6)
#define PCIE_PHY1_SRAM_INIT_DONE (1 << 7)
#define PCIE_PHY_SRAM_LOAD_DONE (PCIE_PHY0_SRAM_LOAD_DONE | PCIE_PHY1_SRAM_LOAD_DONE)

#define PCIE_SS_MODE_REG0_OFFSET 0x1C0
#define PCIE_SS_MODE_REG0 (PCIE_PHY_INTF + PCIE_SS_MODE_REG0_OFFSET)
#define PCIE_SS_MODE_REG0_SS_MODE_MASK (0x7 << 0)
#define PCIE_PHY_RESET (1 << 5)

#define PCIE_PHY_REF0_REPEAT_CLK_MASK (3 << 17)
#define PCIE_PHY_REF1_REPEAT_CLK_MASK (3 << 27)
#define PCIE_PHY_REF_REPEAT_CLK_MASK (PCIE_PHY_REF0_REPEAT_CLK_MASK | PCIE_PHY_REF1_REPEAT_CLK_MASK)

#define PCIE_PHY0_MISC_REG0_OFFSET 0x1CC
#define PCIE_PHY0_MISC_REG0 (PCIE_PHY_INTF + PCIE_PHY0_MISC_REG0_OFFSET)

#define PCIE_PHY0_MISC_REG1_OFFSET 0x1D0
#define PCIE_PHY0_MISC_REG1 (PCIE_PHY_INTF + PCIE_PHY0_MISC_REG1_OFFSET)
#define PCIE_PHY0_REF_REPEAT_CLK_EN (1 << 2)
#define PCIE_PHY0_REF_REPEAT_CLK_MASK (0x3 << 3)

#define PCIE_PHY1_MISC_REG0_OFFSET 0x1F4
#define PCIE_PHY1_MISC_REG0 (PCIE_PHY_INTF + PCIE_PHY1_MISC_REG0_OFFSET)

#define PCIE_PHY1_MISC_REG1_OFFSET 0x1F8
#define PCIE_PHY1_MISC_REG1 (PCIE_PHY_INTF + PCIE_PHY1_MISC_REG1_OFFSET)
#define PCIE_PHY1_REF_REPEAT_CLK_EN (1 << 2)
#define PCIE_PHY1_REF_REPEAT_CLK_MASK (0x3 << 3)

#define PCIE_PHY_PERST_MUX_REG_OFFSET 0x288
#define PCIE_PHY_PERST_MUX_REG (PCIE_PHY_INTF + PCIE_PHY_PERST_MUX_REG_OFFSET)

#define PCIE_SUB_CFG 0x21790000

#define PCIE_CFG_AXI_CTRL_OFFSET 0
#define PCIE_CFG_CORE_CTRL_OFFSET 0x1000
#define PCIE_CFG_SII_OFFSET 0x2000

#define PCIE_DBI(id) (PCIE_DBI_BASE + (id) * PCIE_DBI_SIZE)
#define PCIE_CFG(id) (PCIE_CFG_BASE + (id) * PCIE_CFG_SIZE)

#define PCIE_CFG_AXI_CTRL(id) (PCIE_CFG(id) + PCIE_CFG_AXI_CTRL_OFFSET)
#define REG_PCIE_RST_CTRL0_OFFSET 0x60
#define REG_AXI_MASTER_ACELITE_CTL_OFFSET 0xe8
#define REG_CORE_CLK_STATUS_OFFSET 0x100
#define REG_CORE_DBG_IRQ0_OFFSET 0x160

#define PCIE_CFG_CORE_CTRL(id) (PCIE_CFG(id) + PCIE_CFG_CORE_CTRL_OFFSET)
#define PCIE_CFG_SII(id) (PCIE_CFG(id) + PCIE_CFG_SII_OFFSET)

#define PCIE_PHY_CR(phy) (PCIE_PHY_CR_BASE + (phy) * PCIE_PHY_CR_SIZE)
#define PCIE_PHY_ICP(phy) (PCIE_PHY_CR(phy) + 0x20000)

#define PCIE_SSMODE_MAX 6
#define PCIE_CTRL_MAX 4

#define PCIE_DBI2_OFFSET 0x100000
#define PCIE_iATU_OFFSET 0x300000

#define LTSSM_STATE_S_L0 0x11

/* wait timeouts (us) */
#define PCIE_CORE_CLK_TIMEOUT_US   100000   /* 100ms: core clock active */
#define PCIE_LINKUP_TIMEOUT_US     500000   /* 500ms: LTSSM link up (EP waits host) */

#define PCIE_REG_TOP_LINK_HOLD 0x281003a0
//#define PCIE_BOOT_REG 0x281000D0

#define OTP_BAR_SIZE_CONFIG0 0x34060400
#define OTP_BAR_SIZE_CONFIG1 0x34060408
#define OTP_PCIE_CUSTOM_ID   0x3406017C
#define PCIE_BAR_PREFETCHABLE (0x1 << 3)
#define PCIE_BAR_64BIT        (0x2 << 1)
#define PCIE_BAR_IO           (0x1 << 0)

#define PCIE_PHY_EXT_FW_MAGIC (SPIF1_BASE + 0x40000)
#define PCIE_PHY_EXT_FW_BASE  (SPIF1_BASE + 0x41000)
#define PCIE_PHY_EXT_FW_SIZE  (0x10000)

#define PCIE_TRAP_REG 0x28100004
#define PCIE_TRAP_BOOT_MODE_SHIFT 18
#define PCIE_TRAP_BOOT_MODE_MASK (0x7 << PCIE_TRAP_BOOT_MODE_SHIFT)
#define PCIE_TRAP_BOARD_ID_SHIFT 20
#define PCIE_TRAP_BOARD_ID_MASK (0x1 << PCIE_TRAP_BOARD_ID_SHIFT)
#define PCIE_TRAP_SSMODE_SHIFT 21
#define PCIE_TRAP_SSMODE_MASK (0x7 << PCIE_TRAP_SSMODE_SHIFT)
#define PCIE_TRAP_REPEAT_CLK_EN_SHIFT 24
#define PCIE_TRAP_REPEAT_CLK_EN_MASK (0x1 << PCIE_TRAP_REPEAT_CLK_EN_SHIFT)
#define PCIE_TRAP_CTRL_SEL_SHIFT 25
#define PCIE_TRAP_CTRL_SEL_MASK (0x3 << PCIE_TRAP_CTRL_SEL_SHIFT)
#define PCIE_TRAP_FUNC_NUM_SHIFT 27
#define PCIE_TRAP_FUNC_NUM_MASK (0x3 << PCIE_TRAP_FUNC_NUM_SHIFT)
#define PCIE_TRAP_BAR4_SET_SHIFT 29
#define PCIE_TRAP_BAR4_SET_MASK (0x3 << PCIE_TRAP_BAR4_SET_SHIFT)

#define PCIE_DEFAULT_DEVICE_ID 0x16941f1c

enum pcie_ctrl_id {
	PCIE_CTRL_X8_0 = 0,
	PCIE_CTRL_X2_0 = 1,
	PCIE_CTRL_X4_0 = 2,
	PCIE_CTRL_X2_1 = 3,

	//ID for invalid ep config
	PCIE_CTRL_INVALID = 4,
	PCIE_CTRL_SATA = 7,
};

enum pcie_phy_select {
	PCIE_PHY_0_SEL = (1 << 0),
	PCIE_PHY_1_SEL = (1 << 1),
};

enum pcie_ctrl_mode {
    PCIE_CTRL_DISABLED     = 0,
    PCIE_CTRL_MODE_RC      = 1,
    PCIE_CTRL_MODE_EP      = 2,
};

enum {
	FIP_LOADED = (1 << 0),
	SKIP_PCIEI = (1 << 1),
	PCIE_EP_LINKED = (1 << 2),
	SOC_EP = (1 << 3),
};

enum pcie_link_capable {
	PCIE_LINK_CAPABLE_X1 = 0x1,
	PCIE_LINK_CAPABLE_X2 = 0x3,
	PCIE_LINK_CAPABLE_X4 = 0x7,
	PCIE_LINK_CAPABLE_X8 = 0xF,
};

struct pcie_chip_config {
	enum pcie_phy_select phy_select;
	enum pcie_link_capable link_capable;
	uint32_t link_width;
};

/*
 * Global boot-time configuration, decoded once from the trap register (or from
 * compile-time overrides when CONFIG_IGNORE_PCIE_TRAP is set). Replaces passing
 * the raw trap word around and re-reading PCIE_TRAP_REG in several places.
 */
struct pcie_boot_config {
	bool                   ignore_trap;   /* true: compile-time cfg, ignore trap  */
	uint32_t               ss_mode;       /* authoritative ss_mode for this boot  */
	bool                   repeat_clk_en; /* reference repeat clock requested     */
	enum pcie_ctrl_id      rom_ctrl_id;   /* ctrl the ROM already brought up      */
	uint32_t               func_num;      /* EP function count (trap decoded)     */
	uint32_t               bar4_set;      /* EP BAR4 size selector (trap decoded) */
	uint32_t               board_id;      /* board id bit from trap reg           */
};

/* Per-controller parameters derived from pcie_boot_config + pcie_chip_configs. */
struct pcie_ctrl_setup {
	enum pcie_ctrl_id      ctrl_id;
	bool                   is_rc;
	enum pcie_phy_select   phy_select;
	uint32_t               link_width;
	enum pcie_link_capable link_cap;
	uint32_t               func_num;
	uint32_t               bar4_set;
	uint32_t               board_id;
};

struct pcie_range_regs {
	uint32_t up_en;
	uint32_t up_start_low;
	uint32_t up_start_high;
	uint32_t up_end_low;
	uint32_t up_end_high;
	uint32_t dw_en;
	uint32_t dw_start;
	uint32_t dw_end;
};

struct pcie_addr_range_group {
	uint32_t up_start_high;
	uint32_t up_end_high;
	uint32_t dw_start;
	uint32_t dw_end;
	uint32_t ecam_high;
	uint32_t ecam_low;
};

struct pcie_bus_resource {
    uint8_t primary;
    uint8_t secondary;
    uint8_t subordinate;
};

void pcie_init(void);

#endif /* __PCIE_H__ */
