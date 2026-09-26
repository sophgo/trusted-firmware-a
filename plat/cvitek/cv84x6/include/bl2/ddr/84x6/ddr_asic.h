#ifndef __DDR_H__
#define __DDR_H__

#include <stdbool.h>
#include <aks_ddrphy_setup.h>
#include <aks_ddrphy_setup_struct.h>
#include <msg_block_lpddr4.h>
#include <msg_block_lpddr5.h>

#define MAX_PSTATE_NUM 1
//#define DDR_DEBUG
//#define AKS_DDR_PHY_DEBUG

//#define BRINGUP_CODE_VERIFY
//#define LPDDR5_BRINGUP_CODE_VERIFY
//#define LPDDR4_BRINGUP_CODE_VERIFY

struct ddr_param {
	uint8_t data[1024 * 16];
};

struct fw_data {
	uint32_t offset;
	uint32_t value;
};

struct train_msg {
	uint32_t index;
	const char *msg;
};

struct phy_reg {
	uint32_t offset;
	const char *name;
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

	bool is_lp5x;
	bool is_Bytemode;

	bool mdf_ecc_en;
	bool mdf_rdcam_en;
	bool mdf_wrcam_en;

	//ate run selection
	bool run_ate_fw;

	//lp5 special feature config
	bool nt_odt_en;
	bool txdfe;

	// // === Global Struct Defines === //
	runtime_config_t runtimeConfig;		///< Instance of runtime objects
	user_info_basic_t userInfoBasic;	///< Instance of useInputBasic
	user_info_advanced_t userInfoAdvanced;
	user_info_sim_t userInfoSim;

	// === Firmware Message Block Structs === //
	PMU_SMB_LPDDR5X_1D_t mb_LPDDR5X_1D[MAX_PSTATE_NUM];	///< 1D message block instance
	PMU_SMB_LPDDR4X_1D_t mb_LPDDR4X_1D[MAX_PSTATE_NUM];	///< 1D message block instance
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
