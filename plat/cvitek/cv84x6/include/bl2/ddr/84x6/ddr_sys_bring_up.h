#ifndef __DDR_SYS_BRING_UP_H__
#define __DDR_SYS_BRING_UP_H__

void ddr_sys_bring_up(void);
int pll_init(ddr_ctx *lpddr_ctx);
void ddr_ctx_init(ddr_ctx *lpddr_ctx);
void print_ddr_ctx(const ddr_ctx *ctx);
void top_itlv_config(void);
void cv84x6_ddr_top_itlv_config(ddr_ctx *lpddr_ctx);
void cv84x6_ddr_top_pwr_on_rst(ddr_ctx *lpddr_ctx);
int cv84x6_ddrc_addr_cfg(uint8_t ddr_sys_index);
void cv84x6_ctrl_init(ddr_ctx *lpddr_ctx);
void cv84x6_axi_core_ddrc_rstn_desert(void);
void cv84x6_dfi_reset_setting(ddr_ctx *lpddr_ctx);
void cv84x6_set_dfi_init_start(ddr_ctx *lpddr_ctx);
void cv84x6_polling_normal_mode(ddr_ctx *lpddr_ctx);
void cv84x6_polling_dfi_init_complete(ddr_ctx *lpddr_ctx);
void cv84x6_set_dfi_param(ddr_ctx *lpddr_ctx);
void cv84x6_ddrc_update_refresh_status(ddr_ctx *lpddr_ctx);
void cv84x6_set_dram_param(ddr_ctx *lpddr_ctx);
void ddrc_selfref_test(uint8_t ddr_sys_index);
void lp5_inline_ecc_enable(ddr_ctx *lpddr_ctx);
void refresh_management_config(ddr_ctx *lpddr_ctx);
void cv84x6_bist_wr_prbs_init(uint32_t uCap_sys, uint32_t rank);
uint32_t cv84x6_bist_start_check(void);
uint32_t ctrl_init_detect_dram_size(ddr_ctx *lpddr_ctx);
void cv84x6_dram_mr_dump(uint8_t sys);
#endif /* __DDR_SYS_BRING_UP_H__ */
