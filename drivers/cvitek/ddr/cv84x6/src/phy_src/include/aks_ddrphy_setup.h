/** \file */

#include <aks_ddrphy_setup_user.h>
#include "phyinit_def.h"

//#############################################################################
// Global Structures : instantiated in aks_ddrphy_globals.c
//#############################################################################
//extern runtime_config_t             runtime_config;

//extern user_info_basic_t            user_info_basic;
//extern user_info_advanced_t         user_info_advanced;
//extern user_info_sim_t              user_info_sim;

//extern PMU_SMB_DDR_1D_t        msgblock_DDR_1D[MAX_PSTATE_NUM];
//extern PMU_SMB_DDR_1D_t        shdw_DDR_1D[MAX_PSTATE_NUM];

// Function definitions
int aks_ddrphy_setup_set_msgblock (int ps, char *field, int value, int do_2d_train);
int aks_ddrphy_setup_soft_set_msgblock (int ps, char *field, int value, int do_2d_train);
void aks_ddrphy_setup_init_struct(int do_2d_train);
