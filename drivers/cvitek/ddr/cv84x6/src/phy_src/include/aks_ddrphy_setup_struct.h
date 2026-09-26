
/** \file aks_ddrphy_setup_struct.h
 *  \brief This file defines the internal data structures used in PhyInit to store user configuration.
 *
 *  Please see \ref docref to obtain necessary information for program variables
 *  correctly for your PHY variant and process technology.
 */

/**  \addtogroup structDef
 *  @{
 */
#define PSTATE_MAX  (16)

#define    AKS_DDRPHY_DBN           (4)
#define    AKS_DDRPHY_DQN           (8)
#define    AKS_DDRPHY_PAD_AW        (14)
#define    AKS_DDRPHY_CHN           (2)
#define    AKS_DDRPHY_RN            (4)
#define    AKS_DDRPHY_CS_NUM        (2)
#define    AKS_DDRPHY_CK_NUM        (1)
#define    AKS_DDRPHY_MAX_DQ_NUM    (9)
#define    AKS_DDRPHY_DQS_NUM       (2)

#define ENABLE      (1)
#define DISABLE     (0)

// /** Enumerator for DRAM Type */
typedef enum {
	DDR4,    /*!< DDR4 */
	DDR5,    /*!< DDR5 */
	LPDDR4,  /*!< LPDDR4_ */
	LPDDR5,  /*!< LPDDR5 */
} dram_type_e;

/** Enumerator for DIMM Type */
typedef enum {
	UDIMM,   /*!< UDIMM */
	SODIMM,  /*!< SODIMM */
	RDIMM,   /*!< RDIMM (DDR4/DDR5 only) */
	LRDIMM,  /*!< LRDIMM (DDR4/DDR5 only) */
	NODIMM   /*!< No DIMM (Soldered-on) */
} dimm_type_e;

typedef enum{
	BG,
	B8,
	B16
}bank_mode_e;

typedef enum{
	PRBS31 = 0,
	PRBS15,
	PRBS8
}prbs_type_e;


typedef enum{
	D5_2N_MODE,
	D5_1N_MODE
}d5_2n_mode_e;


typedef struct user_info_basic {

	dram_type_e dram_type;    ///< DRAM Module Type:

							///< - must be set as hex
							///<
							///< value | Description

							///<   0x2 | LPDDR4\LPDDR4X
							///<   0x3 | LPDDR5\LPDDR5X

	int db_num;             ///< db_num of phy
	int ac_num;             ///< ac_num of phy
	int rank_num_cha;       ///< rank num of cha
	int db_num_cha;         ///< db num of cha
	int rank_num_chb;       ///< rank num of chb
	int db_num_chb;         ///< db num of chb
	int device_type_cha;    ///< Width of the DRAM device cha. Enter 4,8,16 depending on protocol and dram type
	int device_type_chb;    ///< Width of the DRAM device chb. Enter 4,8,16 depending on protocol and dram type
	int ch_num;             ///< ch num of phy

	int lp4x_mode;           ///< indicates LPDDR4_ mode support.

							///< - must be set as hex
							///< - Only used for LPDDR4_
							///<
							///< value | Description
							///< ----- | ------
							///<   0x0 | LPDDR4_  mode, when dram_type is LPDDR4
							///<   0x1 | LPDDR4_ mode, when dram_type is LPDDR4X
	int lp5x_mode;           ///< indicates LPDDR5_ mode support.

							///< - must be set as hex
							///< - Only used for LPDDR5
							///<
							///< value | Description
							///< ----- | ------
							///<   0x0 | LPDDR5_ mode, when dram_type is LPDDR5
							///<   0x1 | LPDDR5_ mode, when dram_type is LPDDR5X

	int dram_data_width;      ///< Width of the DRAM device.

							///< Protocol | Valid Options | Default
							///< -------- | ------------- | ---
							///< DDR3     | 4,8,16        | 8
							///< DDR4     | 4,8,16        | 8
							///< LPDDR3   | 16,32         | 16
							///< LPDDR4_   | 8,16          | 16


	int pstate_num;         ///< Number of p-states used

							///< - Must be decimal integer.

	int freq[PSTATE_MAX];       ///< Memclk freq for each PState.

							///< - Must be decimal integer.
							///< - Memclk freq in MHz round up to next highest integer.  Enter 334 for 333.333, etc.
							///<
							///< [0] - P0 Memclk freq in MHz \n
							///< [1] - P1 Memclk freq in MHz \n
							///< [2] - P2 Memclk freq in MHz \n
							///< [3] - P3 Memclk freq in MHz \n

	int pll_bypass[PSTATE_MAX];       ///< indicates if PLL should be in Bypass mode.

							///< - See PUB Databook section "PLL Bypass Mode" under "Clocking and Timing" for requirements.
							///< - At datarates below DDR333 rate PLL must be in Bypass Mode.
							///< - Must be set as hex.
							///<
							///< [0] - PLL Bypass Enable for P0 \n
							///< [1] - PLL Bypass Enable for P1 \n
							///< [2] - PLL Bypass Enable for P2 \n
							///< [3] - PLL Bypass Enable for P3 \n
							///<
							///< value | Description
							///< ----- | ------
							///<   0x1 | Enabled
							///<   0x0 | Disabled

	int mc2phy_ratio[PSTATE_MAX];   ///< - Selected MC clock and phy clock ratio
							///< - must be set as hex
							///<
							///< [0] - MC clock and phy clock ratio for P0 \n
							///< [1] - MC clock and phy clock ratio for P1 \n
							///< [2] - MC clock and phy clock ratio for P2 \n
							///< [3] - MC clock and phy clock ratio for P3 \n
							///<
							///< Binary value | Description
							///<        ----- | ------
							///<        2'b00 | 1:1 MC clock and phy clock ratio (default)
							///<        2'b01 | 1:2 MC clock and phy clock ratio

	int freq_ratio[PSTATE_MAX];    ///< Selected Dfi freq ratio

							///< - Used to program the freq_ratio register. This register controls how dfi_freq_ratio
							///    input pin should be driven inaccordance with DFI Spec.
							///< - See PUB databook section "DfiClk" on detailes on how set value.
							///< - must be set as hex
							///<
							///< [0] - DFI freq Ratio for P0 \n
							///< [1] - DFI freq Ratio for P1 \n
							///< [2] - DFI freq Ratio for P2 \n
							///< [3] - DFI freq Ratio for P3 \n
							///<
							///< Binary value | Description
							///<        ----- | ------
							///<        2'b00 | 1:1 DFI freq Ratio
							///<        2'b01 | 1:2 DFI freq Ratio (defualt)
							///<        2'b1x | 1:4 DFI freq Ratio  //update in 1.02a

	int do_2d_train;       ///< Obsolete, Not used.

							///< - This variable is set via input variable to function aks_ddrphy_setup_flow()
							///< - This exists for backward compatibility and no longer used.
	int ckr[PSTATE_MAX];    // Only used for LPDDR5
							// 0:CKR=4; 1:CKR=2
	int flow_ctrl;          // Training Stage , See MessBlock.h for different DRAM type

							///< [14] - MWD Training(LRDIMM Only)
	int dbyte_width;        ///< dbyte_width of dram
	int train_opt;         // training option bit field:

	int sim_speed;
	int verbosity;        ///  debug print level,   0 for max
	int emul;             ///  used to emulate
							///< value | Description
							///< ----- | ------
							///<   0x1 | Enabled
							///<   0x0 | Disabled
} user_info_basic_t;



/** \brief Structure for advanced (optional) user inputs
 *
 *  if user does not enter a value for these parameters, a default recommended or
 *  default value will be used
 */
typedef struct user_info_advanced {

	int rd_preamble_mode[PSTATE_MAX];  //bit[n] represents rd_preamble pattern config for pstate[n]
	int rd_preamble_len[PSTATE_MAX];   //number of halfclk(UI) for pstate[0]. e.g., 4 means 2tCK.
	int rd_postamble_mode[PSTATE_MAX]; //
	int rd_postamble_len[PSTATE_MAX];  //number of halfclk(UI)
	int wr_preamble_mode[PSTATE_MAX];  //< Obsolete, Not used.
	int wr_preamble_len[PSTATE_MAX];   //number of halfclk(UI)
	int wr_postamble_mode[PSTATE_MAX]; //< Obsolete, Not used.
	int wr_postamble_len[PSTATE_MAX];  //number of halfclk(UI)
	int wdqs_ext;                      //< Obsolete, Not used.
	int BL[PSTATE_MAX];                // Burst Length; please refer to JESD79-58_v1.20, chapter 3.5.2 MR0 definition.
	int RL[PSTATE_MAX];                //represents CAS Lantency(RL)
										//please refer to JESD79-58_v1.20, chapter 3.5.2 MR0 definition.
	int WL[PSTATE_MAX];                //
	int WR[PSTATE_MAX];                //
	int TCCD_L[PSTATE_MAX];            // please refer to JESD79-58_v1.20, chapter 3.5.15 MR13 definition.
	int nWR[PSTATE_MAX];               //
	int WLS[PSTATE_MAX];               //
	int rd_dbi_en[PSTATE_MAX];         //bit[n] represents rd_dbi_en on/off for pstate[n]. 0 means, read dbi off; 1 means, read dbi on.
	int wr_dbi_en[PSTATE_MAX];         //bit[n] represents wr_dbi_en on/off for pstate[n]. 0 means, write dbi off; 1 means, write dbi on.
	int data_mask[PSTATE_MAX];      //bit[n] represents data_mask on/off for pstate[n].  see LPDDR5/4 JESD MR13, 0 means Enable , 1 means Disable
	int dq_odt[PSTATE_MAX];            //represents RTT_NOM_WR and RTT_NOM_RD configuration for pstate[n].
	int ca_odt[PSTATE_MAX];            //
	int soc_odt[PSTATE_MAX];           // dram soc odt set
	int ck_mode[PSTATE_MAX];           //0:Differential  1:Single Ended
	//lp5 specific
	bank_mode_e bank_mode[PSTATE_MAX];  //Bank group mode.
	int dvfsc[PSTATE_MAX];              //DVFSC MODE please refer to JESD209-5B chapter 7.7.1.
	int wck_mode[PSTATE_MAX];           // wck mode
										// 0:Differential
										// 1:Single Ended from WCK_t
										// 2:Single Ended from WCK_c
	int rdqs_mode[PSTATE_MAX];          //Read DQS
										//0x0:RDQS_t and RDQS_c diabled
										//0x1:RDQS_t enable and RDQS_c diabled
										//0x2:RDQS_t and RDQS_c enable
										//0x3:RDQS_t disable and RDQS_c enable
	int wck_always_on[PSTATE_MAX];      //wck mode
										//0x1: ENABLE  0x0:DISABLE.
	int wck_fm[PSTATE_MAX];             //wck_fm
										//0x0:low frequency mode  0x1:high frequency mode
	int wck_odt[PSTATE_MAX];            //wck odt
	int fsp_wr[PSTATE_MAX];             //Frequency Set Point Operation Write Enable
	int fsp_op[PSTATE_MAX];             //Frequency Set Point Operation Mode
	int vrcg[PSTATE_MAX];               //VREF Current Generator 0:Normal Operation  1:VREF Fast Response Mode
	int rd_link_ecc[PSTATE_MAX];        //read link ecc enable 1: ENABLE  0:DISABLE.
	int wr_link_ecc[PSTATE_MAX];        //write link ecc enable 1: ENABLE  0:DISABLE.
	int rd_data_cp[PSTATE_MAX];         //read data copy enable 1: ENABLE  0:DISABLE.
	int wr_data_cp[PSTATE_MAX];         //write data copy enable 1: ENABLE  0:DISABLE.
	int wrx_en[PSTATE_MAX];             //WXFE
										// 0x0:LPDDR5 write x function disable
										// 0x1:LPDDR5 write x function enable
	uint32_t phymaster_inrerval;       // phymstr interval time,real time is n*0.125us,configured at initialization.
	uint16_t phymaster_req_point;      // phymstr req time,real time is n*0.125us,configured at initialization.
	uint16_t phymaster_resp_max;       // phymstr max respone time,real time is n*0.125us,configured at initialization.
	uint16_t phymaster_mask_update;    // phymstr mask update time,,real time is n*0.125us,configured at initialization.
	uint32_t phyupd_interval;          // phy update interval  time,real time is n*0.125us,configured at initialization.
	uint16_t phyupd_req_point;         // phy update req time,real time is n*0.125us,configured at initialization.
	uint16_t phyupd_resp_max;          // phy update max respone time,real time is n*0.125us,configured at initialization.
	uint16_t remap_lndq_sel[AKS_DDRPHY_DBN][AKS_DDRPHY_DQN];  // dq remap cfg
	uint16_t remap_lnca_sel[AKS_DDRPHY_CHN][AKS_DDRPHY_PAD_AW]; // ca remap cfg

	//  phy Analog config
	uint16_t phy_dq_pu;
	uint16_t phy_dq_pd;
	uint16_t phy_dq_odt;
	uint16_t phy_wck_pu;
	uint16_t phy_wck_pd;
	uint16_t phy_wck_odt;
	uint16_t phy_rdqs_pu;
	uint16_t phy_rdqs_pd;
	uint16_t phy_rdqs_odt;
	uint16_t phy_ca_pu;
	uint16_t phy_ca_pd;
	uint16_t phy_cs_cke_pu;
	uint16_t phy_cs_cke_pd;
	uint16_t phy_ck_pu;
	uint16_t phy_ck_pd;
	uint16_t phy_ck_odt;
	uint16_t phy_se_rx_vref_dac;            // default dq vref

} user_info_advanced_t;


/*Structure for user input simulation options (Optional)*/
typedef struct user_info_sim {

	int tDQS2DQ;                 ///< Enter the value of tDQS2DQ for LPDDR4_ dram (in ps)

									///< - must be set as decimal integer.
									///< - for simulation only.

	int tDQSCK;                  ///< Enter the value of tDQSCK in ps

									///< - must be set as decimal integer.
									///< - for simulation only.
	int tWCK2DQI[PSTATE_MAX];
	int tWCK2DQO[PSTATE_MAX];
} user_info_sim_t;
/** @} */
