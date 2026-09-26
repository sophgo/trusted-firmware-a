#ifndef _MSG_BLOCK_LPDDR5_H
#define _MSG_BLOCK_LPDDR5_H

#pragma pack(1)

#define MAX_CHANS 2
#define MAX_RANKS 2

typedef uint8_t MR_t[MAX_CHANS][MAX_RANKS];

typedef struct _PMU_SMB_DDR_1D_t {
	uint16_t version;                           // CSR Address: 0x4000, Byte offset 0x0
														// firmware revision ID
	uint8_t  dram_type;                         // CSR Address: 0x4000, Byte offset 0x2
														///< Value | Description
														///< ----- | ------
														///<   0x0 | DDR4
														///<   0x1 | DDR5
														///<   0x2 | LPDDR4\LPDDR4X
														///<   0x3 | LPDDR5\LPDDR5X
	uint8_t  ch_num;                            // CSR Address: 0x4000, Byte offset 0x3
	uint8_t  rank_num;                          // CSR Address: 0x4001, Byte offset 0x4
	uint8_t  dram_width;                        // CSR Address: 0x4001, Byte offset 0x5
														///< Value | Description
														//< ----- | ------
														//<   0x0 | X4
														//<   0x1 | X8
														//<   0x2 | X16
														//<   0x3 | X32
	uint8_t  pstate;                            // CSR Address: 0x4001, Byte offset 0x6
														// Must be set to the target Pstate to be trained
														//    0x0 = Pstate 0
														//    0x1 = Pstate 1
														//    0x2 = Pstate 2
														//    0x3 = Pstate 3
														//    0x4 = Pstate 4
														//    0x5 = Pstate 5
														//    0x6 = Pstate 6
														//    0x7 = Pstate 7
														//    0x8 = Pstate 8
														//    0x9 = Pstate 9
														//    0xa = Pstate 10
														//    0xb = Pstate 11
														//    0xc = Pstate 12
														//    0xd = Pstate 13
														//    All other encodings are reserved
	uint8_t  pstate_num;                        // CSR Address: 0x4001, Byte offset 0x7
	uint8_t  pll_bypass;                        // CSR Address: 0x4002, Byte offset 0x8
														// Set according to whether target Pstate uses PHY PLL bypass
														//    0x0 = PHY PLL is enabled for target Pstate
														//    0x1 = PHY PLL is bypassed for target Pstate

	uint8_t  freq_ratio;                        // CSR Address: 0x4002, Byte offset 0x9
														// freq ratio betwen dfi clk and DRAM memclk.
														//    0x0 = 1:1
														//    0x1 = 1:2
														//    0x2 = 1:4
	uint16_t dram_freq;                         // CSR Address: 0x4002, Byte offset 0xa
														// DDR data rate for the target Pstate in units of MT/s.
														// For example enter 0x12C0 for DDR4800.
	uint8_t verbosity;                          // CSR Address: 0x4003, Byte offset 0xc
														//  log print level
	uint8_t dbyte_width;                        // CSR Address: 0x4003, Byte offset 0xd

	uint16_t train_opt;                         // CSR Address: 0x4003, Byte offset 0xe
														// train option
	uint32_t flow_ctrl;                         // CSR Address: 0x4004, Byte offset 0x10
														// Controls the training steps to be run. Each bit corresponds to a training step.
														// If the bit is set to 1, the training step will run.
														// If the bit is set to 0, the training step will be skipped.
														// Training step to bit mapping:
														//    flow_ctrl[0] = Run DevInit - Device/PHY initialization. Should always be set.
														//    flow_ctrl[1] = CS Training    (once cs and ca )
														//    flow_ctrl[2] = CA Training
														//    flow_ctrl[3] = RXEN Training (
														//    flow_ctrl[4] = Write leveling Training
														//    flow_ctrl[5] = Read 1D Training
														//    flow_ctrl[6] = Write 1D Training
														//    flow_ctrl[7] = MRL Training
														//    flow_ctrl[8] = DRAM DCA Train
														//    flow_ctrl[9] = PHY DCA Train
														//    flow_ctrl[10] = DRAM DFE Train(TBD)
														//    flow_ctrl[11] = PHY RX DFE Training
														//    flow_ctrl[12] = READ 2D Training
														//    flow_ctrl[13] = WRITE 2D Training
														//    flow_ctrl[14] = Read 1D ReTraining - long pattern
														//    flow_ctrl[15] = TRAIN TEST - simple and bubble read write check train passfail
														//    flow_ctrl[31:16] TBD


	uint8_t sim_speed;                          // CSR Address: 0x4005, Byte offset 0x14

	uint8_t max_csr_pstate;                     // CSR Address: 0x4005, Byte offset 0x15

	uint8_t  ca_terminating_rank[2];            // CSR Address: 0x4005, Byte offset 0x16
														// Terminating Rank for Command bus train on Channel 0/1
														//    0x0 = Rank 0 is terminating rank
														//    0x1 = Rank 1 is terminating rank
	uint8_t  channel_byteswap_en[2];            // CSR Address: 0x4006, Byte offset 0x18
														// Channel 0 : Dbyte0 and Dbyte1 swap en
														// Channel 1 : Dbyte2 and Dbyte3 swap en
														// when dram is X16 mode, It must be set
														// only for Command bus train
	uint16_t train_prbs_type;                   // CSR Address: 0x4006, Byte offset 0x1a
												//  2D train use prbs type
												//  0 : PRBS31; 1 PRBS15; 2: PRBS8

	uint32_t dbwrap_disable_bits;               // CSR Address: 0x4007, Byte offset 0x1c
													// bit 0 ~ 3 : Phy Dbyte0 ~ 3
													// other bits reserved
													// set 1 to disable dbwrap

	MR_t  mr1;                                  // CSR Address: 0x4008, Byte offset 0x20
													// OP[7:4] Write Latency
	MR_t  mr2;                                  // CSR Address: 0x4009, Byte offset 0x24
													// OP[3:0] Read Latency
													// OP[7:4] Write recvery
	MR_t  mr3;                                  // CSR Address: 0x400a, Byte offset 0x28
													// OP[4:3] BG Group mode
													// OP[6] Read DBI en
													// OP[7] Write DBI en
	MR_t  mr10;                                 // CSR Address: 0x400b, Byte offset 0x2c
													// OP[0] RDQS Postamble Mode
													// OP[1] RD Oreamble Length
													// OP[3:2] WCK Postamble Length
													// OP[5:4] RD Preamble Length OP[7:6] Postamble Length
	MR_t  mr11;                                 // CSR Address: 0x400c, Byte offset 0x30
													// OP[2:0] DQ_ODT
													// OP[6:4] CA ODT
	MR_t  mr12;                                 // CSR Address: 0x400d, Byte offset 0x34
													// OP[6:0] VrefCA Setting
	MR_t  mr13;                                 // CSR Address: 0x400e, Byte offset 0x38
													// OP[4] DMI I/O Control
													// OP[5] Data Mask Disable
													// OP[6] CBT Mode
													// OP[7] Dual VDD2
	MR_t  mr14;                                 // CSR Address: 0x400f, Byte offset 0x3c
													// OP[6:0] Vref DQ[7:0] Setting
	MR_t  mr15;                                 // CSR Address: 0x4010, Byte offset 0x40
													// OP[6:0] Vref DQ[15:8] Setting
	MR_t  mr16;                                 // CSR Address: 0x4011, Byte offset 0x44
													// OP[1:0] FSP_WR
													// OP[3:2] FSP_OP
													// OP[5:4] CBT
	MR_t  mr17;                                 // CSR Address: 0x4012, Byte offset 0x48
													// OP[2:0] Soc ODT
													// OP[3] ODTD-CK
													// OP[4] ODTD-CS
													// OP[5] ODTD-CA
													// OP[7:6] ODTD-CA x8
	MR_t  mr18;                                 // CSR Address: 0x4013, Byte offset 0x4c
													// OP[2:0] WCK ODT
													// OP[3] WCK FM
													// OP[4] WCK ON
													// OP[6] WCK2CK Leveling
													// OP[7] CKR
	MR_t  mr19;                                 // CSR Address: 0x4014, Byte offset 0x50
													// OP[1:0] DVFSC
													// OP[3:2] DVFSQ
													// OP[4] WCK2DQ OSC FM
													// OP[5] WCK2DQ OSC FM support
													// OP[7:6] CS ODT
	MR_t  mr20;                                 // CSR Address: 0x4015, Byte offset 0x54
													// OP[1:0] RDQS
													// OP[3:2] WCK mode
													// OP[4] MRWDL
													// OP[5] MRWDU
													// OP[6] RDC DMI mode
													// OP[7] RDC DQ mode
	MR_t  mr21;                                 // CSR Address: 0x4016, Byte offset 0x58
													// OP[4] WDCFE Write data copy function disable
													// OP[5] RDCFE Read data copy function disable
	MR_t  mr22;                                 // CSR Address: 0x4017, Byte offset 0x5c
													// OP[5:4] WECC, Write link ECC
													// OP[7:6] RECC, Read link ECC
	MR_t  mr24;                                 // CSR Address: 0x4018, Byte offset 0x60
													// OP[2:0] DFEQL
													// OP[6:4] DFEQH

	MR_t  mr28;                                 // CSR Address: 0x4019, Byte offset 0x64
													// OP[3:2] ZQ Interval
													// OP[5] ZQ Mode
	MR_t  mr41;                                 // CSR Address: 0x401a, Byte offset 0x68
													// OP[7:5] DQ ODT
	MR_t  mr58;                                 // CSR Address: 0x401b, Byte offset 0x6c
													// OP[1:0] DQ UP Emphasis LB
													// OP[3:2] DQ Dn Emphasis LB
													// OP[5:4] DQ UP Emphasis UB
													// OP[7:6] DQ Dn Emphasis UB

	uint8_t msg_misc;                           // CSR Address: 0x401c, Byte offset 0x70
													// global options for training.

	int8_t rxen_offset;                         // CSR Address: 0x401c, Byte offset 0x71
	int8_t retrain_loop_cnt;                    // CSR Address: 0x401c, Byte offset 0x72
	int8_t dqs_train_step;                      // CSR Address: 0x401c, Byte offset 0x73
	int8_t dqs_ttu_vib_num;                     // CSR Address: 0x401d, Byte offset 0x74
	int8_t dqs_train_min_step;                  // CSR Address: 0x401d, Byte offset 0x75
	int8_t dich_optz_en;                        // CSR Address: 0x401d, Byte offset 0x76
	int8_t dich_lock_left_edge;                 // CSR Address: 0x401d, Byte offset 0x77
	uint32_t reserved;                          // CSR Address: 0x401e, Byte offset 0x78

} __attribute__ ((packed)) PMU_SMB_DDR_1D_t;     // Structure size 124

#pragma pack()

#endif
