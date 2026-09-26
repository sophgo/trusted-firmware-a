#ifndef AKS_DDRPHY_SETUP_USER_H
#define AKS_DDRPHY_SETUP_USER_H

#include <stdint.h>
#include "aks_ddrphy_csr_defines.h"
//#include <aks_ddrphy_setup_struct.h>
/** \file
 * \brief structures and enumeration definitions
 */


#define DB_BDCAST                (14)    //DB broadcast channel
#define CA_BDCAST                (5)     //CA broadcast channel
#define DQ_LANE_BDCAST           (14)    //DQ lane broadcast channel
#define DQS_LANE_BDCAST          (6)     //DQS lane broadcast channel
#define CA_OUT_LANE_BDCAST       (47)    //CA_OUT lane broadcast channel
#define CA_CK_LANE_BDCAST        (15)    //CA_CK lane broadcast channel
//-------------------------------------------------------------
// Define some basic min/max macro's
//-------------------------------------------------------------
#define max(a,b) \
  ({ typeof (a) _a = (a); \
     typeof (b) _b = (b); \
    _a > _b ? _a : _b; })

#define min(a,b) \
  ({ typeof (a) _a = (a); \
     typeof (b) _b = (b); \
    _a < _b ? _a : _b; })


#define	CLEAR_BIT(x, bit)	      (x &= ~(1ULL << bit))	/* Clear the bit-th bit */
#define	SET_BIT(x, bit)	    ((x) |= (1ULL << (bit)))
#define SET_BITS(v,e,s,m) ((v) = (((m)<<(s) | ((v) & ~(((1<<((e)-(s)+1))-1)<<(s))))))


#define	GET_BIT(x, bit)          (((x) >> (bit))&1)
/* Used to extract a bit field of specified width from U32 data by mask, bit_lsb is the LSB of the data to be extracted */
//#define GET_BITS(x,mask,bit_lsb)  ((x & (mask << bit_lsb) ) >> bit_lsb)
#define GET_BITS(v,e,s)  (((v)>>(s)) & ((1<<((e) - (s) + 1)) -1))

//-------------------------------------------------------------
// Defines for Firmware Images
// - point to IMEM/DMEM incv files,
// - indicate IMEM/DMEM size (bytes)
//-------------------------------------------------------------
/*! \def FW_FILES_LOC
  \brief set the location of training firmware uncompressed path.

  PhyInit will use this path to load the imem and dmem incv files of the
    firmware image.
 */
/*! \def IMEM_INCV_FILENAME
  \brief firmware instruction memory (imem) filename for 1D training
 */
/*! \def DMEM_INCV_FILENAME
  \brief firmware instruction memory (imem) filename for 1D training.
 */
/*! \def IMEM_SIZE
  \brief max size of instruction memory.
 */
/*! \def DMEM_SIZE
  \brief max size of data memory.
 */
/*! \def DMEM_ST_ADDR
  \brief start of DMEM address in memory.
 */
#ifndef FW_FILES_LOC
#define FW_FILES_LOC "./fw"
#endif

#define IMEM_SIZE 16384
#define DMEM_SIZE 8192
#define DMEM_ST_ADDR 0x4000

///> Added for multiple PHY interface.
#ifndef AKS_DDRPHY_NUM_PHY
#define AKS_DDRPHY_NUM_PHY (1)
#endif
//-------------------------------------------------------------
// Defines for SR Firmware Images
// - point to IMEM incv files,
// - indicate IMEM size (bytes)
//-------------------------------------------------------------
/*! \def SR_FW_FILES_LOC
  \brief location of optional retention save restore firmware image.
 */
/*! \def SR_IMEM_SIZE
  \brief max IMEM size of retention save/restore firmware.
 */
/*! \def SR_IMEM_INCV_FILENAME
  \brief file name of retention save/restore IMEM image.
 */
#ifndef SR_FW_FILES_LOC
#define SR_FW_FILES_LOC FW_FILES_LOC"/save_restore"
#endif

#define SR_IMEM_SIZE 16384
#define SR_IMEM_INCV_FILENAME       SR_FW_FILES_LOC"/aks_ddrphy_io_retention_save_restore_imem.incv"

//------------------
// Type definitions
//------------------

/// A structure used to SRAM memory address space.
typedef enum {return_offset, return_lastaddr} return_offset_lastaddr_t;

/// A structure to store the sequence function runtime input variables.
typedef struct runtime_config {
  int do_2d_train;      ///< train2d input parameter
  int skip_train;   ///< skip_train input parameter
  int debug;        ///< print debug messages
  int ret_en;        ///< Retention Enable input parameter, instructs phyinit to \n
                    ///< issue register reads during initialization to retention registers.
} runtime_config_t;

/// enumeration of instructions for PhyInit Register Interface
typedef enum {
  startTrack,       ///< start register tracking
  stopTrack,        ///< stop register tracking
  saveRegs,         ///< save(read) tracked register values
  restoreRegs,      ///< restore (write) saved register values
  dumpRegs,         ///< write register address,value pairs to file
  importRegs        ///< import register address,value pairs to file
} regInstr;

/// data structure to store register address, value pairs
typedef struct reg_addr_val {

  uint32_t address; ///< register address
  uint16_t value;   ///< register value
} reg_addr_val_t;

typedef enum {
  TRAIN_TYPE_1D,
  TRAIN_TYPE_2D
} TRAIN_TYPE;

//-------------------------------
// Global variables - defined in aks_ddrphy_setup_globals.c
//-------------------------------

/*! \def MAX_NUM_RET_REGS
 *  \brief default Max number of retention registers
 *
 * This define is only used by the PhyInit Register interface to define the max
 * amount of registered that can be saved. The user may increase this variable
 * as desired if a larger number of registers need to be restored.
*/
#define MAX_NUM_RET_REGS 5000

/**  Array of address/value pairs used to store register values for the purpose
 * of retention restore.
 */
extern reg_addr_val_t g_reg_list[MAX_NUM_RET_REGS];

//-------------------------------------------------------------
// Fixed Function prototypes
//-------------------------------------------------------------
int aks_ddrphy_setup_flow(int skip_training, int do_2d_train, int debug);
int aks_ddrphy_setup_restore_sequence();
void aks_ddrphy_set_real_ratio(int pstate);
void aks_ddrphy_setup_config (void);
void aks_ddrphy_setup_load_itim(int do_2d_train);
void aks_ddrphy_setup_prog_csr_skip_train(int skip_training);
void aks_ddrphy_setup_load_dtim(int pstate, int do_2d_train);
void aks_ddrphy_setup_load_mitim();
void aks_ddrphy_setup_start_firmware(void);
void aks_ddrphy_setup_read_msgblock(int do_2d_train);
void aks_ddrphy_assert(int Svrty, const char *fmt,...);
void aks_ddrphy_print_comment(const char *fmt,...);
void aks_ddrphy_setup_print(const char *fmt,...);
void aks_ddrphy_setup_debug_print(void);
void aks_ddrphy_setup_store_msgblock (void *msg_block_ptr,  int size, uint32_t mem[]);
void aks_ddrphy_setup_user_default (int do_2d_train);
void aks_ddrphy_setup_calc_msgblock(int do_2d_train);
int  aks_ddrphy_setup_store_mem_file (char * incv_file_name, int mem[], return_offset_lastaddr_t return_type);
void aks_ddrphy_setup_write_mem (int mem[], int mem_offset, int mem_size);
int aks_ddrphy_setup_get_user_input (char *field);
int aks_ddrphy_setup_get_msgblock (int ps, char *field, int do_2d_train);
int aks_ddrphy_setup_set_user_input (char *field, int value);
int aks_ddrphy_setup_track_csr(uint32_t adr) ;
int aks_ddrphy_setup_reg_interface(regInstr myRegInstr, uint32_t adr, uint16_t dat);
void mr_cfg(uint8_t pstate);
void rw_cfg(uint8_t pstate);
void aks_ddrphy_setup_fpstate_cfg(int pstate);
void aks_ddrphy_setup_config_default_dly(int pstate);
void aks_ddrphy_dma_xfer_cr_to_fpram(int pstate);

extern void aks_ddrphy_setup_override_default_byuser(void);
extern void aks_ddrphy_setup_powerup_byuser(void);
extern void aks_ddrphy_setup_start_clk_rst_byuser(void);
extern void aks_ddrphy_setup_pre_train_byuser(void);
extern void aks_ddrphy_setup_post_train_byuser(void);
extern int aks_ddrphy_setup_user_set_dfi_clk(int dfi_freq);
extern void aks_ddrphy_setup_user_wait_firmware_done(void);
extern void aks_ddrphy_setup_user_read_msgblock(int do_2d_train);
extern void aks_ddrphy_setup_user_enter_mission_mode(void);
extern uint32_t aks_ddrphy_setup_user_io_read32(uint32_t addr);
extern void aks_ddrphy_setup_user_io_write32(uint32_t addr, uint32_t data);
extern void aks_ddrphy_setup_user_save_regs(void);
extern void aks_ddrphy_setup_user_wait_dfi_clk(uint32_t clk);

#endif



