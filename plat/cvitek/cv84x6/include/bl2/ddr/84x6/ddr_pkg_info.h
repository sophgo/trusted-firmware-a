#ifndef __DDR_PKG_INFO_H__
#define __DDR_PKG_INFO_H__
#include <stdint.h>
#include <stdbool.h>

#define DDR_TYPE_UNKNOWN		0
#define DDR_TYPE_DDR2			1
#define DDR_TYPE_DDR3			2

extern uint32_t ddr_data_rate;
extern uint8_t ddr_rank_num;
extern uint8_t ddr_sys_num;
extern uint8_t board_ddr_type;

#define OTP_CV186_IC0 0x11
#define OTP_CV186_IC1 0xF9
#define OTP_CV186_IC2 0xC1
#define OTP_BM1688_IC0 0x00
#define OTP_BM1688_IC1 0x3f
#define GPIO117_STATUS_LOW 0b0

void get_board_info(void);

void read_ddr_pkg_info(void);

uint8_t get_ddr_type(void);
uint8_t get_rank_num(void);
uint8_t get_sys_num(void);
bool is_byte_mode(void);

#endif /* __DDR_PKG_INFO_H__ */
