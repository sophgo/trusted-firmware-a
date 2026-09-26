#ifndef __DDR_H__
#define __DDR_H__

#include <stdbool.h>

extern uint32_t usys0_cap_in_mbyte, usys1_cap_in_mbyte;

struct ddr_param {
	uint8_t data[1024 * 16];
};

typedef struct SubsysHdlr_priv {
	//sys basic config
	uint8_t dram_type;
	uint8_t rank_num; //!< Number of ranks to setup
	uint8_t sys_num;
	uint8_t ctrl_num;
	uint16_t ddr_clk;
	uint16_t itlv_size;

	uint32_t phy_training; //!< 0 - full training, 1 - skip training, 2 - dev_inti

	//ECC config
	bool lp5_linkecc_en;
	bool lp5_inlineecc_en;
	bool dbi_en;
	bool dm_en;
	uint8_t ecc_region;

	bool mdf_ecc_en;
	bool mdf_rdcam_en;
	bool mdf_wrcam_en;

	//ate run selection
	bool run_ate_fw;

	//lp5 special feature config
	bool nt_odt_en;
	bool txdfe;

	// // === Global Struct Defines === //
	// runtime_config_t runtimeConfig;		///< Instance of runtime objects
	// user_input_basic_t userInputBasic;	///< Instance of useInputBasic
	// user_input_advanced_t userInputAdvanced;
	// // === Firmware Message Block Structs === //
	// PMU_SMB_LPDDR5X_1D_t mb_LPDDR5X_1D[DWC_DDRPHY_PHYINIT_MAX_NUM_PSTATE];	///< 1D message block instance
	// PMU_SMB_LPDDR4X_1D_t mb_LPDDR4X_1D[DWC_DDRPHY_PHYINIT_MAX_NUM_PSTATE];	///< 1D message block instance
	// PMU_SMB_DIAG_t mb_diag;
	// PMU_SMB_ATE_t mb_ate;

	// bool remap0_2G;
	// bool ddr_no_strip;
	// bool rxu_ccn;
	// bool run_diag_fw;

	// uint8_t ddr_sys_type; // 0 full 1 location0 2 location1
	// uint16_t *ddr_sys_list;
	// uint8_t ddr_intlv; // 0 256B 1 64B

	// uint8_t addr_map;
	// uint64_t total_size;
	// uint8_t share_percent;
} ddr_ctx;

int ddr_init(void);


#endif /* __DDR_H__ */
