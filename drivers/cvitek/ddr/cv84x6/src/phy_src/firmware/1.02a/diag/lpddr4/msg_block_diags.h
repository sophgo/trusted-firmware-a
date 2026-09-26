#ifndef _MSG_BLOCK_DIAGS_INTERNAL_H
#define _MSG_BLOCK_DIAGS_INTERNAL_H

typedef struct _DDR_DIAG_s {
	/********************************* <<<Data width per address increment = 32bit>>> ********************************/
	/*************************************** <<<Origin CSR Address = 0x4300>>> ***************************************/

	/***************************** <<<	Common Parameter  >>> ******************************/
    uint16_t DiagTestSelect;                    // CSR Address: 0x4300, Byte offset 0x0
								///<  Value | Description
								///<   0x00 | SIMPLE_READ_WRITE
								///<   0x01 | TX_EYE
								///<   0x02 | RX_EYE
								///<   0x03 | CONTINUE_WRITE_NO_GAP
								///<   0x04 | CONTINUE_WRITE_WITH_GAP
								///<   0x05 | CONTINUE_READ_NO_GAP
								///<   0x06 | BIST_TEST
								///<   0x09 | RX_EVEN_PATTERN_EYE
								///<   0x0a | RX_ODD_PATTERN_EYE
								///<   0x0b | RX_EVEN_ODD_PATTERN_EYE
								///<   0x0c | TX_2UI_EVEN_PATTERN_EYE
								///<   0x0d | TX_2UI_ODD_PATTERN_EYE
								///<   0x0e | TX_2UI_EVEN_ODD_PATTERN_EYE
								///<   0x0f | CONTINUE_RDQS
								///<   0x10 | DCA_EYE
								///<   0x11 | QCA_EYE
								///<   0x12 | RXENT_EYE
								///<   0x13 | RXENC_EYE
								///<   0x14 | PHY_RETRAIN
								///<   0x0c | TX_1UI_EVEN_PATTERN_EYE
								///<   0x0d | TX_1UI_ODD_PATTERN_EYE
								///<   0x0e | TX_1UI_EVEN_ODD_PATTERN_EYE
    uint16_t DiagRsvd0;                         // CSR Address: 0x4300, Byte offset 0x2

    uint8_t DiagXcount;                         // CSR Address: 0x4301, Byte offset 0x4
    uint8_t DiagLoopCount;                      // CSR Address: 0x4301, Byte offset 0x5
    uint8_t DiagRepeatCount;                    // CSR Address: 0x4301, Byte offset 0x6
								// DDR5 Default value	= 0x03
								// DDR4 Default value	= 0x03
								// LPDDR5 Default value	= 0x00
								// LPDDR4 Default value	= 0x00
    uint8_t DiagRsvd1;                          // CSR Address: 0x4301, Byte offset 0x7

    uint16_t PatternType;                       // CSR Address: 0x4302, Byte offset 0x8
								///<  Value | Description
								///<   0x00 | PATTERN_TYPE_CUSTOM
								///<   0x01 | PATTERN_TYPE_LANE_CUSTOM		//Only for Bist & 2D EYE Test
								///<   0x02 | PATTERN_TYPE_PRBS8
								///<   0x03 | PATTERN_TYPE_PRBS15
								///<   0x04 | PATTERN_TYPE_PRBS31
								///<   0x05 | PATTERN_TYPE_SOLIDBIT			//Only for Bist & 2D EYE Test
								///<   0x06 | PATTERN_TYPE_BITFLIP			//Only for Bist & 2D EYE Test
								///<   0x07 | PATTERN_TYPE_STATICBIT_HIGH	//Only for Bist & 2D EYE Test
								///<   0x08 | PATTERN_TYPE_STATICBIT_LOW	//Only for Bist & 2D EYE Test
								///<   0x09 | PATTERN_TYPE_SIPI				//Only for Bist & 2D EYE Test
								///<   0x0b | PATTERN_TYPE_ALL				//Only for Bist & 2D EYE Test

    uint16_t AllPatternBitMap;                  // CSR Address: 0x4302, Byte offset 0xa
								///<  This field valid only when PatternType set PATTERN_TYPE_ALL
								///<  Value | Description
								///<   bit0 | PATTERN_TYPE_CUSTOM
								///<   bit1 | PATTERN_TYPE_LANE_CUSTOM		//Only for Bist & 2D EYE Test
								///<   bit2 | PATTERN_TYPE_PRBS8
								///<   bit3 | PATTERN_TYPE_PRBS15
								///<   bit4 | PATTERN_TYPE_PRBS31
								///<   bit5 | PATTERN_TYPE_SOLIDBIT			//Only for Bist & 2D EYE Test
								///<   bit6 | PATTERN_TYPE_BITFLIP			//Only for Bist & 2D EYE Test
								///<   bit7 | PATTERN_TYPE_STATICBIT_HIGH	//Only for Bist & 2D EYE Test
								///<   bit8 | PATTERN_TYPE_STATICBIT_LOW	//Only for Bist & 2D EYE Test
								///<   bit9 | PATTERN_TYPE_SIPI				//Only for Bist & 2D EYE Test

    uint32_t CustomPattern;                     // CSR Address: 0x4303, Byte offset 0xc

    uint8_t DiagChannel;                        // CSR Address: 0x4304, Byte offset 0x10
								// DDR5 Default value	= 0x03
								// DDR4 Default value	= 0x03
								// LPDDR5 Default value	= 0x03
								///<  Value | Description
								///<   0x00 | DISABLE_ALL_CH
								///<   0x01 | ENABLE_CH0
								///<   0x02 | ENABLE_CH1
								///<   0x03 | ENABLE_ALL_CH
    uint8_t DiagLane;                           // CSR Address: 0x4304, Byte offset 0x11
								///<  Value | Description
								///<   0x00 | Lane0
								///<   0x01 | lane1
								///<   0x02 | lane2
								///<   0x03 | Lane3
								///<   0x04 | lane4
								///<   0x05 | lane5
								///<   0x06 | lane6
								///<   0x07 | lane7
    uint8_t DiagDByte;                          // CSR Address: 0x4304, Byte offset 0x12
								///<  Value | Description
								///<   0x00 | DByte0
								///<   0x01 | DByte1
								///<   0x02 | DByte2
								///<   0x03 | DByte3
								///<   0x04 | DByte4
								///<   0x05 | DByte5
								///<   0x06 | DByte6
								///<   0x07 | DByte7
								///<   0x08 | DByte8
								///<   0x09 | DByte9
    uint8_t DiagRank;                           // CSR Address: 0x4304, Byte offset 0x13

    uint32_t DiagRowAddr;                       // CSR Address: 0x4305, Byte offset 0x14
								// DIAG: DIAG Row
								// Bist: Test Start Row

    uint32_t DiagRowEndAddr;                    // CSR Address: 0x4306, Byte offset 0x18
								// Only for bist
								// Bist: Test End Row

    uint32_t DiagSeed;                          // CSR Address: 0x4307, Byte offset 0x1c

    uint8_t DiagVrefInc;                        // CSR Address: 0x4308, Byte offset 0x20
    uint8_t DiagDlyInc;                         // CSR Address: 0x4308, Byte offset 0x21
    uint8_t IncreatDirect;                      // CSR Address: 0x4308, Byte offset 0x22
								///<  Value | Description
								///<   bit0: 0 - Rclkt > Rclkc
								///<		 1 - Rclkt < Rclkc
								///<   bit7: 0 - auto skew
								///<		 1 - manual skew

    uint8_t RclkSkew;                           // CSR Address: 0x4308, Byte offset 0x23

    uint32_t Lane0Pat;                          // CSR Address: 0x4309, Byte offset 0x24
    uint32_t Lane1Pat;                          // CSR Address: 0x430a, Byte offset 0x28
    uint32_t Lane2Pat;                          // CSR Address: 0x430b, Byte offset 0x2c
    uint32_t Lane3Pat;                          // CSR Address: 0x430c, Byte offset 0x30
    uint32_t Lane4Pat;                          // CSR Address: 0x430d, Byte offset 0x34
    uint32_t Lane5Pat;                          // CSR Address: 0x430e, Byte offset 0x38
    uint32_t Lane6Pat;                          // CSR Address: 0x430f, Byte offset 0x3c
    uint32_t Lane7Pat;                          // CSR Address: 0x4310, Byte offset 0x40

    uint8_t SeedRandom;                         // CSR Address: 0x4311, Byte offset 0x44
								///<  Value | Description
								///<   0x00 | Dbyte Broadcast Fixed Seed
								///<   0x01 | Dbyte Broadcast Random Seed
								///<   0x02 | All DQ Random Seed
								///<   0x03 | Single-Lane Prbs and other lanes using lane custom pattern
    uint8_t GapRandom;                          // CSR Address: 0x4311, Byte offset 0x45
								// Hardware not support, TBD
    uint8_t BurstMode;                          // CSR Address: 0x4311, Byte offset 0x46
								///<  Value | Description
								///<   0x00 | SEAMLESS_BURST
								///<   0x01 | SINGLE_BURST
								///<   0x02 | SEAMLESS_AND_SINGLE_BURST
    uint8_t BankMode;                           // CSR Address: 0x4311, Byte offset 0x47
								// Only for LP5, TBD

	/* Full Address Test */
    uint32_t RowNum;                            // CSR Address: 0x4312, Byte offset 0x48
    uint16_t ColNum;                            // CSR Address: 0x4313, Byte offset 0x4c
    uint16_t DiagRsvd4;                         // CSR Address: 0x4313, Byte offset 0x4e
    uint8_t RaNum;                              // CSR Address: 0x4314, Byte offset 0x50
    uint8_t BgNum;                              // CSR Address: 0x4314, Byte offset 0x51
    uint8_t BaNum;                              // CSR Address: 0x4314, Byte offset 0x52
    uint8_t FullAddrTest;                       // CSR Address: 0x4314, Byte offset 0x53

	/* CA Eye Test */
    uint8_t CAChannel;                          // CSR Address: 0x4315, Byte offset 0x54
								///<  Value | Description
								///<   0x00 | Test CA of CH0
								///<   0x01 | Test CA of CH1
    uint8_t DimmNum;                            // CSR Address: 0x4315, Byte offset 0x55
    uint8_t PhyRreTrainCnt;                     // CSR Address: 0x4315, Byte offset 0x56
	/* TX Decision Feedback Equalizer */
    uint8_t TxDFE;                              // CSR Address: 0x4315, Byte offset 0x57

    uint8_t ReadDCA;                            // CSR Address: 0x4316, Byte offset 0x58
    uint8_t DiagMRIdx;                          // CSR Address: 0x4316, Byte offset 0x59
    uint8_t DiagMRWVal;                         // CSR Address: 0x4316, Byte offset 0x5a
    uint8_t DiagPdaID;                          // CSR Address: 0x4316, Byte offset 0x5b

    uint8_t DiagCW;                             // CSR Address: 0x4317, Byte offset 0x5c

    uint8_t DiagRsvd6[31];                      // CSR Address: 0x4317, Byte offset 0x5d

#if (!TRAIN_PLUS_DIAGS && !TRAIN_PLUS_DIAGS_INTERNAL)
	/* For TX and RX EYE and SIMPLE_READ_WRITE */
	/* Start point of the data returned by the test */
	diag_return_data_u DiagReturnData;		// Byte offset 0x7c, CSR Addr 0x431f, Direction=Out
#endif

} __attribute__ ((packed)) DDR_DIAG_t;     // Structure size 124

#pragma pack()
typedef DDR_DIAG_t diag_messgBlock_t;

#endif
