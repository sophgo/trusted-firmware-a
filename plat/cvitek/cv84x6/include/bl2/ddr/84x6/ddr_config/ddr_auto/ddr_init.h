#ifndef __DDR_PI_PHY_H__
#define __DDR_PI_PHY_H__

extern uint32_t ddr_data_rate;

void lp5_ddrc_init(uint32_t ddrc_base_addr, ddr_ctx *lpddr_ctx);
void enable_ctl_mdf(uint32_t ddrc_base_addr, ddr_ctx *lpddr_ctx);
void lp4_ddrc_init(uint32_t ddrc_base_addr, ddr_ctx *lpddr_ctx);
void ctrl_init_update_by_dram_size(uintptr_t base, uint8_t dram_cap_in_mbyte);

#endif /* __DDR_PI_PHY_H__ */
