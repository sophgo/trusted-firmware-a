#ifndef __DDR_SYS_H__
#define __DDR_SYS_H__

#include <mmio.h>
#include <debug.h>
#include <stdbool.h>
#include <ddr_asic.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

extern uint32_t  freq_in;

extern uint8_t ddr_rank_num;
extern uint8_t ddr_sys_num;
extern uint8_t board_ddr_type;

extern u8  uWdq_Trained;
extern u16 tx_winsize_min, rx_winsize_min;
extern u8  tx_train_cnt, rx_train_cnt;
extern u32 ca_winsize_min;
extern u16 ca_train_cnt, ca_vref_min, ca_vref_max;

extern uint32_t rddata;

enum mem_cs_e {
	JEDEC_DDR4_16G_X4_3200AA,
	JEDEC_DDR4_16G_X8_3200AA,
	JEDEC_DDR4_2G_X8_3200AC,
};

enum cl_e {
	CL_18 = 18,
	CL_20 = 20,
	CL_24 = 24,
};

enum bl_e {
	BL_2 = 1,
	BL_4 = 2,
	BL_8 = 3,
};

enum rdimm_mode_e {
	SUPPORT,
	UNSUPPORT,
};

enum low_power_state_e {
	IDLE = 0,
	ACT_POWER_DOWN = 1,
	ACT_POWER_DOWN_MEM_CG = 2,
	PRE_CHARGE_PD = 3,
	PRE_CHARGE_PD_MEM_CG = 4,
	SELF_REFRESH_SHORT = 5,
	SELF_REFRESH_SHORT_MEM_CG = 6,
	SELF_REFRESH_LONG = 8,
	SELF_REFRESH_LONG_MEM_CG = 9,
	SELF_REFRESH_LONG_MC_CG = 10,
};

extern uint8_t  current_sys;

extern ddr_ctx lpddr_ctx;

extern uint32_t ddr_phy_base, ddr_ctl_base, ddr_top_base, ddr_bist_base, ddr_axi_monitor_base;
extern uint32_t ddr_ctl_offset;
extern uint32_t ddr_bist_offset;

/********************DDR Feature List***********************/
#define SG_MDF
/*ddr common feature define*/
//#define DDR_EXT_LOOP_BACK_TEST
//#define DBG_BRING_UP
//#define FULL_MEM_BIST
//#define FULL_MEM_BIST_FOREVER

//#define OLD_DB
//#define DRAM_CTX_DUMP
//#define LINKECC_TEST
//#define INLINEECC_TEST
//#define LINK_INLINE_ECC_TEST
//#define CLK_GATE_TEST

#define NO_ECC_REGION 0
#define ECC_REGION_ALL 1
#define ECC_REGION_PART 2

//#define SHOW_DDR_INIT_TIME

//#define DBG_INFO_L1
//#define DBG_INFO_L2
//#define DBG_INFO_L3

enum bist_mode {
	E_PRBS,
	E_SRAM,
};

enum ddr_ssc_enum {
	SSC_EN,
	SSC_BYPASS,
	SSC_OFF
};
enum ddr_mode_enum {
	X8_MODE,
	X16_MODE,
	X32_MODE,
	X64_MODE
};

enum board_ddr_type_e {
	BOARD_LPDDR4X,
	BOARD_LPDDR5,
	BOARD_LPDDR5x
};

enum rank_num_e {
	SINGLE_RANK,
	DUAL_RANK,
};

enum ddrsys_num_e {
	SINGLE_SYS,
	DUAL_SYS,
	TRIP_SYS,
	QUAD_SYS
};

typedef enum {
	DWC_PHY_TRAINING = 0,
	DWC_PHY_SKIP_TRAINING = 1,
	DWC_PHY_DEV_INIT = 2
} dwc_ddrctl_phy_training_e;

#define CEILING_POS(X) ((X - (int)(X)) > 0 ? (int)(X + 1) : (int)(X))
#define CEILING_NEG(X) ((X - (int)(X)) < 0 ? (int)(X - 1) : (int)(X))
#define ceil(X) (((X) > 0) ? CEILING_POS(X) : CEILING_NEG(X))

#define mmio_wr32	mmio_write_32
#define mmio_rd32	mmio_read_32

// #define ddr_mmio_rd32(a, b)	do { if (1) b = mmio_rd32(a); } while (0)
// #define ddr_sram_rd32(a, b)	do { if (1) b = mmio_rd32(a); } while (0)

#define ddr_sram_wr32(a, b)	mmio_wr32(a, b)
#define ddr_debug_wr32(b)

#ifdef DBG_INFO_L1
#define DBG1(...)		tf_printf(__VA_ARGS__)
#else
#define DBG1(...)
#endif

extern uint32_t    REG_DDRPLL_MAS_STEP;
extern uint8_t     REG_DDRPLL_MAS_ICTRL;
extern uint64_t    REG_DDRPLL_MAS_SET;
extern uint32_t    REG_DDRPLL_MAS_SET33;
extern uint32_t    REG_DDRPLL_MAS_SET32_0;
extern uint8_t     REG_TX_MPLL_ICTRL;   //[7:4]
extern uint8_t     REG_TX_MPLL_DIV_SEL; //[14:8]
extern uint8_t     REG_TX_MPLL_POST_DIV;
extern uint8_t     REG_TX_MPLL_POST_DIV_SEL; //[21:15]
extern uint16_t    REG_DDRPLL_MAS_SPAN; //[15:0]
extern uint8_t     REG_DDRPLL_MAS_SW_UP; //[0:0]
extern uint8_t     REG_DDRPLL_MAS_SYN_MODE; //[1:1]
extern uint8_t     REG_DDRPLL_MAS_SYN_RST; //[2:2]
extern uint8_t     REG_TX_MPLL_EN_LCKDET; //22:22
extern uint8_t     REG_VPROT_H_LEVEL_SEL;//25:23

extern uint32_t    REG_DDRPLL_MAS_N;
extern uint32_t    REG_DDRPLL_MAS_F;
extern uint32_t    REG_DDRPLL_MAS_DIV2N;
extern uint32_t    REG_DDRPLL_MAS_KPD;
extern uint32_t    DDR_SSO_PERIOD;  // mem_freq/200
extern uint32_t		ave_dll_code_shmoo;

void cvx32_pll_init(void);
void cvx32_ctrl_init(uintptr_t base_addr_ctrl);
void cvx32_phy_init(uintptr_t base_addr_phyd);
void cvx32_setting_check(void);
void cvx32_bist_tx_shift_delay(uint32_t shift_delay);
void cvx32_bist_rx_delay(uint32_t delay);
void cvx32_bist_rx_deskew_delay(uint32_t delay);
void cvx32_synp_mrw(uint32_t addr, uint32_t data, uint8_t mpr_en);
void cvx32_synp_mrr(uint32_t addr, uint8_t mpr_en);
void cvx32_synp_mrr_lp4(uint32_t addr, uint32_t rank);
void cvx32_synp_mrw_lp4 (uint32_t addr, uint32_t data, uint32_t rank);
void cvx32_dfi_sw_mrw(uint32_t addr, uint32_t data, uint32_t rank, uint32_t channel_sel);
void cvx32_dfi_sw_cmd(uint32_t cmd_word, uint32_t cmd_addr_data);
void cvx32_ddrck_ctrl(uint32_t stop);
void cvx32_chg_pll_freq(void);
void cvx32_clk_normal(void);
void cvx32_clk_div2(void);
void cvx32_INT_ISR_08(void);
void cvx32_clk_div40(void);
void cvx32_ddr_phy_power_on_seq1(void);
void cvx32_ddr_phy_power_on_seq2(void);
void cvx32_ddr_phy_power_on_seq3(void);
void cvx32_wait_for_dfi_init_complete(void);
void cvx32_ctrlupd_short(void);
void cvx32_polling_dfi_init_start(void);
void cvx32_set_dfi_init_complete(void);
void cvx32_polling_synp_normal_mode(void);

void cvx32_set_dfi_init_start(void);
void cvx32_dll_sw_upd(void);
void cvx32_bist_mask_shift_delay(uint32_t shift_delay, uint32_t  en_lead);
void cvx32_lb_0_external(void);
void cvx32_clk_gating_disable(void);
void cvx32_clk_gating_enable(void);
void cvx32_pinmux(void);
void cvx32_pinmux_sys2(void);
void cvx32_dfi_phyupd_req(void);
void cvx32_dfi_phyupd_req_clr(void);
void cvx32_dfi_phymstr_req(void);
void cvx32_dfi_phymstr_req_clr(void);
void cvx32_dll_sw_clr(void);
uint32_t ctrl_init_detect_dram_size(ddr_ctx *lpddr_ctx);
void cvx32_change_to_calvl_freq(void);

void cvx32_chg_pll_freq_div2(uint8_t div2_en);
//void ddr_bist_all(uint32_t capacity, uint32_t x16_mode);
void cvx32_rdglvl_training_check(void);
void cvx32_rdlvl_sw_patch(int rank);//for corner case
void ddr_phya_multi_rank(void);
void disable_low_power_function(void);
#endif /* __DDR_SYS_H__ */
