#ifndef __DDR_H__
#define __DDR_H__

#include <stdbool.h>
#include <stdint.h>

#define LPDDR4		0
#define LPDDR5		1

#define DWC_PHY_TRAINING	0
#define SW_PHY_TRAINING		1

extern uint32_t usys0_cap_in_mbyte, usys1_cap_in_mbyte;

struct ddr_param {
	uint8_t data[1024 * 16];
};

typedef struct SubsysHdlr_priv {
	uint8_t dram_type;  // LPDDR4, LPDDR5
	uint8_t rank_num;
	uint8_t sys_num;
	uint8_t ctrl_num;
	uint16_t ddr_clk;
	uint16_t itlv_size;

	uint32_t phy_training;  // DWC_PHY_TRAINING, SW_PHY_TRAINING

	bool lp5_linkecc_en;
	bool lp5_inlineecc_en;
	bool dbi_en;
	uint8_t ecc_region;

	bool mdf_ecc_en;
	bool mdf_rdcam_en;
	bool mdf_wrcam_en;

	bool run_ate_fw;

	bool nt_odt_en;
	bool txdfe;
} ddr_ctx;

int ddr_init(const struct ddr_param *ddr_param);


#endif /* __DDR_H__ */
