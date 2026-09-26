/* SPDX-License-Identifier: GPL-2.0 */
#ifdef DDR_FW_IN_FIP
extern struct fw_data *lpddr5_train_itim;
extern struct fw_data *lpddr4_train_itim;
extern struct fw_data *lpddr5_train_dtim;
extern struct fw_data *lpddr4_train_dtim;
extern uint32_t g_lpddr5_train_itim_num;
extern uint32_t g_lpddr4_train_itim_num;
extern uint32_t g_lpddr5_train_dtim_num;
extern uint32_t g_lpddr4_train_dtim_num;
#else
extern struct fw_data lpddr5_train_itim[];
extern struct fw_data lpddr4_train_itim[];
extern struct fw_data lpddr5_train_dtim[];
extern struct fw_data lpddr4_train_dtim[];
#endif
extern struct train_msg lp5_stream_messages[];
extern struct train_msg lp4_stream_messages[];

uint32_t get_lpddr5_train_itim_num(void);
uint32_t get_lpddr5_train_dtim_num(void);
uint32_t get_lpddr4_train_itim_num(void);
uint32_t get_lpddr4_train_dtim_num(void);
uint32_t get_lpddr5_stream_message_num(void);
uint32_t get_lpddr4_stream_message_num(void);

#ifdef DDR_FW_IN_FIP
#include <bl2.h>
void ddr_fw_init_from_fip(uintptr_t sram_base, const struct fip_param2 *param2);
#endif
