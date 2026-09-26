#include <cpu.h>
#include <mmio.h>
#include <debug.h>
#include <assert.h>
#include <errno.h>
#include <arch_helpers.h>
#include <xlat_tables.h>
#include <bl_common.h>
#include <platform.h>
#include <bl2/platform.h>
#include <delay_timer.h>
#include <console.h>
#include <string.h>
#include <rom_api.h>
#include <cv_atf_err_state.h>
#include <common/build_message.h>
#include <pinmux/cv84x6_pinmux.h>

#ifdef CONFIG_PCIE_INIT
#include <pcie.h>
#endif

#if defined(CV186X)
#include "ddr_pkg_info.h"
#include "ddr_sys.h"
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

#define TPU_BONDING_LMT0 0x1
#define TPU_BONDING_LMT1 0x2

#define TPU_SYS_REG_BASE 0x26D09000
#define TPU_SYS_GATING_CTRL 0x0
#define TPU_SYS_SOFT_RESET_CTRL 0x4
#define ITLV0_CFG_0 0x14

#define BITMASK(msb, lsb)	((2U << (msb)) - (1U << (lsb)))

static inline uint32_t modified_bits_by_value(uint32_t orig, uint32_t value,
					      uint32_t msb, uint32_t lsb)
{
	uint32_t bitmask = BITMASK(msb, lsb);

	orig &= ~bitmask;
	return (orig | ((value << lsb) & bitmask));
}

extern struct rom_api *p_rom_api;

void plat_panic_handler(void)
{
	void *ra;

	ATF_ERR = ATF_ERR_PLAT_PANIC;

	ra = __builtin_return_address(0);
	mmio_write_32(ATF_ERR_INFO0, ((uint64_t)ra) & 0xFFFFFFFFUL);

	ERROR("ra=0x%lx\n", (uint64_t)ra);

	__system_reset("panic", -1);
	__builtin_unreachable();
}

void __system_reset(const char *file, unsigned int line)
{
	ATF_ERR = ATF_ERR_PLAT_SYSTEM_RESET;
	ERROR("RESET:%s:%d\n", file, line);

	console_flush();

	ATF_STATE = ATF_STATE_RESET_WAIT;
	mdelay(5000);

	// enable rtc wdt reset
	mmio_write_32(0x050260E0, 0x0001); //enable rtc_core wathdog reset enable
	mmio_write_32(0x050260C8, 0x0001); //enable rtc_core power cycle   enable

	// sw delay 100us
	ATF_STATE = ATF_STATE_RESET_RTC_WAIT;
	udelay(100);

	// mmio_write_32(0x05025018,0x00FFFFFF); //Mercury rtcsys_rstn_src_sel
	mmio_write_32(0x050250AC, 0x00000000); //cv181x rtcsys_rstn_src_sel
	mmio_write_32(0x05025004, 0x0000AB18);
	mmio_write_32(0x05025008, 0x00400040); //enable rtc_ctrl wathdog reset enable

	// printf("Enable TOP_WDT\n");
	// mmio_write_32(0x03010004,0x00000000); //config watch dog 2.6ms
	// mmio_write_32(0x03010004,0x00000022); //config watch dog 166ms
	mmio_write_32(0x03010004, 0x00000066); //config watch dog 166ms
	mmio_write_32(0x0301001c, 0x00000020);
	mmio_write_32(0x0301000c, 0x00000076);
	mmio_write_32(0x03010000, 0x00000011);

	// ROM_PWR_CYC
	if (get_sw_info()->reset_type == 1) {
		ATF_ERR = ATF_ERR_PLAT_SYSTEM_PWR_CYC;
		// printf("Issue RTCSYS_PWR_CYC\n");

		// wait pmu state to ON
		while (mmio_read_32(0x050260D4) != 0x00000003) {
			;
		}

		mmio_write_32(0x05025008, 0x00080008);
	}

	while (1)
		;

	__builtin_unreachable();
}

void reset_c906l(uintptr_t reset_address)
{
	NOTICE("RSC.\n");

	mmio_clrbits_32(0x3003024, 1 << 6);

	mmio_setbits_32(SEC_SYS_BASE + 0x04, 1 << 13);
	mmio_write_32(SEC_SYS_BASE + 0x20, reset_address);
	mmio_write_32(SEC_SYS_BASE + 0x24, reset_address >> 32);

	mmio_setbits_32(0x3003024, 1 << 6);
}

void setup_dl_flag(void)
{
	uint32_t v = p_rom_api->get_boot_src();
	switch (v) {
	case BOOT_SRC_SD:
		mmio_write_32(BOOT_SOURCE_FLAG_ADDR, MAGIC_NUM_SD_DL);
		break;
	case BOOT_SRC_USB:
		mmio_write_32(BOOT_SOURCE_FLAG_ADDR, MAGIC_NUM_USB_DL);
		break;
	case BOOT_SRC_UART:
		mmio_write_32(BOOT_SOURCE_FLAG_ADDR, MAGIC_NUM_UART_DL);
		break;
	default:
		mmio_write_32(BOOT_SOURCE_FLAG_ADDR, v);
		break;
	}
}

void config_core_power(uint32_t low_period)
{
	/*
	 * low_period = 0x42; // 0.90V
	 * low_period = 0x48; // 0.93V
	 * low_period = 0x4F; // 0.96V
	 * low_period = 0x58; // 1.00V
	 * low_period = 0x5C; // 1.02V
	 * low_period = 0x62; // 1.05V
	 * low_period = 0x62; // 1.05V
	 */
	// mmio_write_32(PWM0_BASE + PWM_HLPERIOD0, low_period);
	// mmio_write_32(PWM0_BASE + PWM_PERIOD0, 0x64);
	// mmio_write_32(PINMUX_BASE + 0xEC, 0x0); // set pinmux for pwm0
	// mmio_write_32(PWM0_BASE + PWM_START, 0x1); // enable bit0:pwm0 and bit3:pwm3
	// mmio_write_32(PWM0_BASE + PWM_OE, 0x1); // output enable bit0:pwm0 and bit3:pwm3
	mdelay(10);
}

void config_rgmii_power(void)
{
	mmio_write_32(0x281051e0, mmio_read_32(0x281051e0) | 0x300); //RGMII 1.8V
	mmio_write_32(0x281001cc, mmio_read_32(0x281001cc) | 0x101); //rx delay 2ns
}

void sys_switch_all_to_pll(void)
{
}

void sys_pll_init(void)
{
	NOTICE("PLLS.\n");

	// mpll0 config, ca55 clk
	mmio_clrbits_32(0x28102020, 0x1);
	mmio_write_32(0x28100220, 0x118105ce);  //2300MHz
	mmio_write_32(0x28100220, 0x11c105ce);  //2300MHz
	// wait for mpll0 lock (0x2810001c bit10 == 1) before switching
	while ((mmio_read_32(0x2810001c) & BIT(10)) == 0)
		;
	mmio_setbits_32(0x28102020, 0x1);

	// mpll1 config, tpu_sys clk
	mmio_clrbits_32(0x28102020, 0x1 << 2);
	mmio_write_32(0x28100224, 0x1181050e);
	mmio_write_32(0x28100224, 0x11c1050e);  //2000MHz
	// wait for mpll1 lock (0x2810001c bit11 == 1) before switching
	while ((mmio_read_32(0x2810001c) & BIT(11)) == 0)
		;
	mmio_setbits_32(0x28102020, 0x1 << 2);

	// mpll2 default 2500MHz
	mmio_write_32(0x28100228, 0x1181064e);
	mmio_write_32(0x28100228, 0x11c1064e);  //2500MHz
	// wait for mpll2 lock (0x2810001c bit12 == 1) before switching
	while ((mmio_read_32(0x2810001c) & BIT(12)) == 0)
		;

	// mpll3 default 2000MHz
	mmio_write_32(0x2810022c, 0x1181050e);
	mmio_write_32(0x2810022c, 0x11c1050e);  //2000MHz
	// wait for mpll3 lock (0x2810001c bit13 == 1) before switching
	while ((mmio_read_32(0x2810001c) & BIT(13)) == 0)
		;

	// mpll4 default 1200MHz
	mmio_write_32(0x28100230, 0x11c1030e);

	// fpll5 sw update
	mmio_setbits_32(0x281002a0, 0x1);

	// fpll6 sw update
	mmio_setbits_32(0x281002b0, 0x1);

	NOTICE("PLLE.\n");
}

/*
 * Bring up TPU subsystem: configure interleaving (ITLV) for all 6 channels,
 * release global soft reset, then enable TXPSYS1/2/3 clusters.
 *
 * TXPSYS0 is NOT touched - BL2 itself runs on TXPSYS0 LMEM,
 * resetting it would crash the system.
 *
 * Clock source: TPLL (1.4GHz) / div2 = 700MHz.
 */
void bl2_tpu_enable_single(uint32_t tpu_reg_base, uint32_t sys_reg_base);
void bl2_tpu_enable(void);

static int bringup_tpusys(void)
{
	uint32_t rddata;
	uint8_t l1_itlv_size = 3;
	uint8_t l1_itlv_bypass = 0;
	uint8_t l1_itlv_bypass_ch = 0;
	uint16_t l1_addr_itlv = 0x7fff;
	uint16_t l1_addr_ddr1 = 0xffff;

	uint8_t l2_itlv_size = 2;
	uint8_t l2_itlv_bypass = 0;
	uint8_t l2_itlv_bypass_ch = 0;
	uint16_t l2_addr_itlv = 0x7fff;
	uint16_t l2_addr_ddr1 = 0xffff;

	NOTICE("bringup tpusys\n");
	mmio_write_32(TPU_SYS_REG_BASE + TPU_SYS_GATING_CTRL, 0xFFFFFFFF);
	NOTICE("config itlv\n");
	for (int i = 0; i < 6; i++) {
		/* L1_0 */
		rddata = mmio_read_32(TPU_SYS_REG_BASE + ITLV0_CFG_0 + 0x14 * i);
		rddata = modified_bits_by_value(rddata, l1_itlv_bypass, 1, 1);
		rddata = modified_bits_by_value(rddata, l1_itlv_bypass_ch, 2, 2);
		rddata = modified_bits_by_value(rddata, l1_itlv_size, 5, 3);
		rddata = modified_bits_by_value(rddata, l1_addr_itlv, 20, 6);
		mmio_write_32(TPU_SYS_REG_BASE + ITLV0_CFG_0 + 0x14 * i, rddata);
		/* L1_0 && L2_0 */
		rddata = mmio_read_32(TPU_SYS_REG_BASE + ITLV0_CFG_0 + 0x4 +
				      0x14 * i);
		rddata = modified_bits_by_value(rddata, l1_addr_ddr1, 15, 0);
		rddata = modified_bits_by_value(rddata, l2_itlv_bypass, 16, 16);
		rddata = modified_bits_by_value(rddata, l2_itlv_bypass_ch, 17, 17);
		rddata = modified_bits_by_value(rddata, l2_itlv_size, 20, 18);
		mmio_write_32(TPU_SYS_REG_BASE + ITLV0_CFG_0 + 0x4 + 0x14 * i,
			      rddata);
		/* L2_0 */
		rddata = mmio_read_32(TPU_SYS_REG_BASE + ITLV0_CFG_0 + 0x8 +
				      0x14 * i);
		rddata = modified_bits_by_value(rddata, l2_addr_itlv, 14, 0);
		rddata = modified_bits_by_value(rddata, l2_addr_ddr1, 30, 15);
		mmio_write_32(TPU_SYS_REG_BASE + ITLV0_CFG_0 + 0x8 + 0x14 * i,
			      rddata);
		/* L2_1 */
		rddata = mmio_read_32(TPU_SYS_REG_BASE + ITLV0_CFG_0 + 0xc +
				      0x14 * i);
		rddata = modified_bits_by_value(rddata, l2_itlv_bypass, 0, 0);
		rddata = modified_bits_by_value(rddata, l2_itlv_bypass_ch, 1, 1);
		rddata = modified_bits_by_value(rddata, l2_itlv_size, 4, 2);
		rddata = modified_bits_by_value(rddata, l2_addr_itlv, 19, 5);
		mmio_write_32(TPU_SYS_REG_BASE + ITLV0_CFG_0 + 0xc + 0x14 * i,
			      rddata);
		/* L2_1 */
		rddata = mmio_read_32(TPU_SYS_REG_BASE + ITLV0_CFG_0 + 0x10 +
				      0x14 * i);
		rddata = modified_bits_by_value(rddata, l2_addr_ddr1, 15, 0);
		mmio_write_32(TPU_SYS_REG_BASE + ITLV0_CFG_0 + 0x10 + 0x14 * i,
			      rddata);
	}
	mmio_write_32(TPU_SYS_REG_BASE + TPU_SYS_SOFT_RESET_CTRL, 0x0);

	/* enable TXPSYS1/2/3 clusters */
	bl2_tpu_enable();

	return 0;
}

/*
 * Enable TXPSYS1/2/3 (TPU accelerator clusters).
 *
 * TXPSYS0 is NOT touched here - BL2 itself runs on TXPSYS0 LMEM,
 * resetting it would crash the system.
 *
 * Clock source: TPLL (1.4GHz) / div2 = 700MHz.
 *
 * Register offsets:
 *   TXPSYS1: REG=0x24D09000, TPU_REG=0x24C10000
 *   TXPSYS2: REG=0x25509000, TPU_REG=0x25410000
 *   TXPSYS3: REG=0x25D09000, TPU_REG=0x25C10000
 */
void bl2_tpu_enable_single(uint32_t tpu_reg_base, uint32_t sys_reg_base)
{
	/* clock gating enable */
	mmio_setbits_32(sys_reg_base + TXPSYS_REG_GATING_CTRL, BIT(0));
	/* release software reset */
	mmio_clrbits_32(sys_reg_base + TXPSYS_REG_SOFTRESET_CTRL, BIT(0));
	/* tpu core enable */
	mmio_setbits_32(tpu_reg_base + TXPSYS_CFG_OFFSET, BIT(0));
}

void bl2_tpu_enable(void)
{
	NOTICE("TPUS.\n");

	/* set tpu clock to PLL(TPLL)(1.4G) div by 2 = 700MHz */
	mmio_write_32(TPU_CLK_DIV_CTL_REG, 0x00020009);

	/*
	 * Enable clocks for TXPSYS1/2/3.
	 * TXPSYS0 clock is already enabled by ROM bl1_tpu_enable().
	 */
	mmio_setbits_32(TPU_CLK_ENABLE_REG, BIT(16) | BIT(17) | BIT(18));

	/* TXPSYS1 */
	bl2_tpu_enable_single(TXPSYS1_TPU_REG_BASE, TXPSYS1_SYS_REG_BASE);
	/* TXPSYS2 */
	bl2_tpu_enable_single(TXPSYS2_TPU_REG_BASE, TXPSYS2_SYS_REG_BASE);
	/* TXPSYS3 */
	bl2_tpu_enable_single(TXPSYS3_TPU_REG_BASE, TXPSYS3_SYS_REG_BASE);

	NOTICE("TPUE.\n");
}

#if 0
void switch_rtc_mode_1st_stage(void)
{
	uint32_t read_data;
	uint32_t write_data;
	uint32_t rtc_mode;

#ifdef CV181X_SUPPORT_SUSPEND_RESUME
	void (*warmboot_entry)(void) = get_warmboot_entry();

	if (warmboot_entry == (void *)BL31_WARMBOOT_ENTRY)
		return;
#endif

	// reg_rtc_mode = rtc_ctrl0[10]
	read_data = mmio_read_32(REG_RTC_CTRL_BASE + RTC_CTRL0);
	rtc_mode = (read_data >> 10) & 0x1;
	if (rtc_mode == 0x1) {
		NOTICE("By pass rtc mode switch\n");
		return;
	}

	mmio_write_32(REG_RTC_CTRL_BASE + RTC_CTRL0_UNLOCKKEY, 0xAB18);
	read_data = mmio_read_32(REG_RTC_CTRL_BASE + RTC_CTRL0);

	// reg_clk32k_cg_en = rtc_ctrl0[11] -> 0
	write_data = 0x08000000 | (read_data & 0xfffff7ff);
	mmio_write_32(REG_RTC_CTRL_BASE + RTC_CTRL0, write_data);

	//cg_en_out_clk_32k = rtc_ctrl_status0[25]
	read_data = mmio_read_32(REG_RTC_CTRL_BASE + RTC_CTRL0_STATUS0);
	while ((read_data & 0x02000000) != 0x00)
		read_data = mmio_read_32(REG_RTC_CTRL_BASE + RTC_CTRL0_STATUS0);

	read_data = mmio_read_32(REG_RTC_CTRL_BASE + RTC_CTRL0);
	//r eg_rtc_mode = rtc_ctrl0[10];
	write_data = 0x04000000 | (read_data & 0xfffffbff) | (0x1 << 10);
	mmio_write_32(REG_RTC_CTRL_BASE + RTC_CTRL0, write_data);

	// DA_SOC_READY = 1
	mmio_write_32(RTC_MACRO_BASE + 0x8C, 0x01);
	// DA_SOC_READY = 0
	mmio_write_32(RTC_MACRO_BASE + 0x8C, 0x0);

	udelay(200); // delay ~200us

	read_data = mmio_read_32(REG_RTC_CTRL_BASE + RTC_CTRL0);
	// reg_clk32k_cg_en = rtc_ctrl0[11] -> 1
	write_data = 0x0C000000 | (read_data & 0xffffffff) | (0x1 << 11);
	mmio_write_32(REG_RTC_CTRL_BASE + RTC_CTRL0, write_data); //rtc_ctrl0
}

void switch_rtc_mode_2nd_stage(void)
{
	uint32_t read_data;
	uint32_t write_data;

	// mdelay(50);
	read_data = mmio_read_32(REG_RTC_CTRL_BASE + RTC_CTRL0_STATUS0);
#ifndef NO_DDR_CFG
	if (get_pkg() == PKG_QFN || (read_data & 0x02000000) == 0x00) {
#else
	if ((read_data & 0x02000000) == 0x00) {
#endif
		read_data = mmio_read_32(REG_RTC_CTRL_BASE + RTC_CTRL0);
		// reg_rtc_mode = rtc_ctrl0[10]
		write_data = 0x0C000000 | (read_data & 0xfffffbff);
		mmio_write_32(REG_RTC_CTRL_BASE + RTC_CTRL0, write_data);
		//DA_SOC_READY = 1
		mmio_write_32(RTC_MACRO_BASE + 0x8C, 0x01);
		//DA_SOC_READY = 0
		mmio_write_32(RTC_MACRO_BASE + 0x8C, 0x00);
		NOTICE("Use internal 32k\n");
	} else
		NOTICE("Switch RTC mode to xtal32k\n");
}
#endif

void set_rtc_en_registers(void)
{
	uint32_t write_data;
	uint32_t read_data;
	uint32_t fsm_state;

	read_data = mmio_read_32(REG_RTC_BASE + RTC_ST_ON_REASON);
	NOTICE("st_on_reason=%x\n", read_data);

	fsm_state = read_data & 0xF;

	read_data = mmio_read_32(REG_RTC_BASE + RTC_ST_OFF_REASON);
	NOTICE("st_off_reason=%x\n", read_data);

	if (fsm_state == ST_ON_FROM_SUSP)
		return;

	mmio_write_32(REG_RTC_BASE + RTC_EN_SHDN_REQ, 0x01);
	while (mmio_read_32(REG_RTC_BASE + RTC_EN_SHDN_REQ) != 0x01)
		;

	mmio_write_32(REG_RTC_BASE + RTC_EN_WARM_RST_REQ, 0x01);
	while (mmio_read_32(REG_RTC_BASE + RTC_EN_WARM_RST_REQ) != 0x01)
		;

	mmio_write_32(REG_RTC_BASE + RTC_EN_PWR_CYC_REQ, 0x01);
	while (mmio_read_32(REG_RTC_BASE + RTC_EN_PWR_CYC_REQ) != 0x01)
		;

	mmio_write_32(REG_RTC_BASE + RTC_EN_WDT_RST_REQ, 0x01);
	while (mmio_read_32(REG_RTC_BASE + RTC_EN_WDT_RST_REQ) != 0x01)
		;

	// Set rtcsys_rst_ctrl[24] = 1; bit 24 is reg_rtcsys_reset_en
	mmio_setbits_32(REG_RTC_CTRL_BASE + RTC_POR_RST_CTRL, 0X1);

	// rtc_ctrl0_unlockkey
	mmio_write_32(REG_RTC_CTRL_BASE + RTC_CTRL0_UNLOCKKEY, 0xAB18);

	// Enable hw_wdg_rst_en
	write_data = mmio_read_32(REG_RTC_CTRL_BASE + RTC_CTRL0);
	write_data = 0xffff0000 | write_data | (0x1 << 11) | (0x01 << 6);
	mmio_write_32(REG_RTC_CTRL_BASE + RTC_CTRL0, write_data);

	// Avoid power up again after poweroff
	mmio_clrbits_32(REG_RTC_BASE + RTC_EN_PWR_VBAT_DET, BIT(2));
}

void init_comm_info(void)
{
#ifdef RTOS_ENABLE_FREERTOS
	struct transfer_config_t *transfer_config = (struct transfer_config_t *)MAILBOX_FIELD;
	struct transfer_config_t transfer_config_s;
	unsigned char *ptr = (unsigned char *)&transfer_config_s;
	unsigned short checksum = 0;
	/* mailbox field is 4 byte write access, and can not access byte by byte.
	 * so init parameters and copy all to mailbox field together.
	 */
	transfer_config_s.conf_magic = RTOS_MAGIC_HEADER;
	transfer_config_s.conf_size = ((uint64_t)&transfer_config_s.checksum - (uint64_t)&transfer_config_s.conf_magic);
	transfer_config_s.isp_buffer_addr = CVIMMAP_ISP_MEM_BASE_ADDR;
	transfer_config_s.isp_buffer_size = CVIMMAP_ISP_MEM_BASE_SIZE;
	transfer_config_s.encode_img_addr = CVIMMAP_H26X_BITSTREAM_ADDR;
	transfer_config_s.encode_img_size = CVIMMAP_H26X_BITSTREAM_SIZE;
	transfer_config_s.encode_buf_addr = CVIMMAP_H26X_ENC_BUFF_ADDR;
	transfer_config_s.encode_buf_size = CVIMMAP_H26X_ENC_BUFF_SIZE;
	transfer_config_s.dump_print_enable = RTOS_DUMP_PRINT_ENABLE;
	transfer_config_s.dump_print_size_idx = RTOS_DUMP_PRINT_SZ_IDX;
	transfer_config_s.image_type = RTOS_FAST_IMAGE_TYPE;
	transfer_config_s.mcu_status = MCU_STATUS_NONOS_DONE;
	for (int i = 0; i < transfer_config_s.conf_size; i++) {
		checksum += ptr[i];
	}

	transfer_config_s.checksum = checksum;
	memcpy(transfer_config, &transfer_config_s, sizeof(struct transfer_config_t));
#endif
}

void apply_analog_trimming_data(void)
{
}

void bm_storage_boot_loader_version(uint32_t addr)
{
	int size_version, size_time;

	size_version = strlen(build_version_string);
	for (int i = 0; i < size_version; i++) {
		mmio_write_8(addr + i, build_version_string[i]);
	}

	mmio_write_8(addr + size_version, ' ');

	size_time = strlen(build_message);
	for (int i = 0; i < size_time; i++) {
		mmio_write_8(addr + size_version + 1 + i, build_message[i]);
	}

	mmio_write_8(addr + size_version + 1 + size_time, '\0');
}

void enable_top_wdt(void)
{
	// config top wdt 30s
	mmio_write_32((PERI_WDT0_BASE + 0x4), 0xEE);
	mmio_write_32((PERI_WDT0_BASE + 0x1c), 0x5969);
	mmio_write_32(PERI_WDT0_BASE, 0xD1);
}

void vddc_pwm_init(void)
{
	/*
	 * low_period = 0x55; // 0.98V
	 * low_period = 0x50; // 0.96V
	 * low_period = 0x4B; // 0.94V
	 * low_period = 0x46; // 0.92V
         * low_period = 0x42; // 0.90V
	 * low_period = 0x41; // 0.89V
	 * low_period = 0x3c; // 0.87V
         */
	int low_period = 0x42;

        // set pin/func mux GPIO0 to PWM8
        mmio_clrsetbits_32(G12_PINMUX_BASE + 0x64, 0xF << 4, 0x7 << 4);
        mmio_clrsetbits_32(CORE_IPMUX_BASE + 0xc4, 0xF, 0xC);

        // init PWM8 with 34% duty cycle
        mmio_clrsetbits_32(PWM2_BASE + HLPERIOD0, 0x3FFFFFFF, low_period);
        mmio_clrsetbits_32(PWM2_BASE + PERIOD0, 0x3FFFFFFF, 0x64);
        mmio_clrsetbits_32(PWM2_BASE + POLARITY, 0xF << 8, 0);
        mmio_clrsetbits_32(PWM2_BASE + PWMSTART, 0xF, 0x1);
        mmio_clrsetbits_32(PWM2_BASE + PWM_OE, 0xF, 0x1);
}

static void *get_warmboot_entry(void)
{
#define WANTED_STATE ST_ON_FROM_SUSP
	NOTICE("\nRTC_ST_ON_REASON 0x%x\n", mmio_read_32(REG_RTC_ST_ON_REASON));
	NOTICE("\nSRAM_FLAG_ADDR 0x%lx\n", mmio_read_64(PM_SRAM_FLAG_ADDR));
	if((mmio_read_32(REG_RTC_ST_ON_REASON) & 0xF) == WANTED_STATE)
		return (void *)(intptr_t)mmio_read_64(PM_SRAM_FLAG_ADDR);

	return 0;
}

#if defined(CV186X)
static void ddr_resume(void)
{
	cvx32_ddr_resume();
}
#endif

extern void get_adc3(void);
void platform_warmentry(void)
{
	void (*warmboot_entry)(void) = get_warmboot_entry();

	mmio_write_64(PM_SRAM_FLAG_ADDR, 0);
	if (warmboot_entry) {

		//enable WDG_RTS_REQ when resume
		mmio_write_32(REG_RTC_CTRL_BASE + RTC_CTRL0, 0x200000);
		mmio_write_32(REG_RTC_BASE + RTC_EN_WDT_RST_REQ, 0x1);

		get_adc3();

		sys_pll_init();

#if defined(CV186X)
		ddr_resume();
		NOTICE("ddr resumed\n");
#endif
		sys_switch_all_to_pll();

		console_flush();

#ifdef AARCH64
		disable_mmu_icache_el3();
		__asm__ volatile("tlbi alle3\n");
#endif

		warmboot_entry();
	}
}
/**
 * @brief Built-In Self-Repair RESET
 *
 */
void reset_rtc_bisr(void)
{
	// NOTICE("%s\n", __func__);
	// All the RAM on the chip is bisr in the rom code, which is reset here
	// bisr_pdgroup_en0 1 2 3 reset 0
	mmio_write_32(0x050250dc, 0x0);
	mmio_write_32(0x050250e0, 0x0);
	mmio_write_32(0x050250e4, 0x0);
	mmio_write_32(0x050250e8, 0x0);
	// bisr_repair_en reset 0
	mmio_write_32(0x050250d8, 0x0);
}

int pwm0_ch0_duty50(void)
{
	const int period = 1000;
	const int hlp = period / 2; /* 50% */
	const int pola = 0;

	mmio_write_32(REG_PWM0_BASE + PERIOD0, period);
	mmio_write_32(REG_PWM0_BASE + HLPERIOD0, period - hlp);
	mmio_write_32(REG_PWM0_BASE + POLARITY, pola);
	mmio_write_32(REG_PWM0_BASE + PWM_OE, 0x1); /* enable ch0 output */
	mmio_write_32(REG_PWM0_BASE + PWMSTART, 0x1); /* start ch0 */
	NOTICE("PWM0 CH0: 50%% duty, clk=pclk/%d\n", period);
	return 0;
}

void pwm_init(void)
{
	PINMUX_CONFIG(PWM0, PWM0, G3);
	pwm0_ch0_duty50();
}

/*
 * Rebuild the translation tables for BL2.
 *
 * The ROM enters BL2 with an MMU configuration of its own that does not cover
 * the PCIe ECAM window, so the tables are thrown away and rebuilt here from an
 * explicit region list.  Ported from the fsbl bl2_mmu_reconfig(), but using the
 * translation table library v2 that this BL2 already links through
 * PLAT_BL_COMMON_SOURCES (${XLAT_TABLES_LIB_SRCS}) - the v1 library the
 * original used is deliberately not linked, as both export mmap_add_region()
 * and init_xlat_tables().
 *
 * v2 keeps the mmap list and table pools of the (single) tf_xlat_ctx in
 * .bss / .xlat_table, both of which BL2 zeroes before reaching here, so the
 * v1-only "clear __XLAT_VARS_START__..__BSS_START__" step has no equivalent and
 * is not needed.  init_xlat_tables() asserts xlat_regime == EL_REGIME_INVALID,
 * which holds because nothing has called it yet at this point.
 */
static void bl2_mmu_reconfig(void)
{
	INFO("MMU: reconfig start\n");

	flush_dcache_range(TPU_SRAM_BASE, 0x400000);
	__asm__ volatile("dsb ish");
	__asm__ volatile("isb");
	INFO("MMU: flush done\n");

	disable_mmu_el3();
	__asm__ volatile("tlbi alle3");
	__asm__ volatile("dsb sy");
	__asm__ volatile("isb");
	INFO("MMU: disable done\n");

	mmap_add_region(BL1_ROM_BASE, BL1_ROM_BASE, BL1_ROM_SIZE,
			MT_MEMORY | MT_RO | MT_SECURE);
#ifndef CONFIG_SIMULATION
	mmap_add_region(CV_IO_BUF_BASE, CV_IO_BUF_BASE, CV_IO_BUF_SIZE,
			MT_DEVICE | MT_RW | MT_SECURE);
#endif
	/*
	 * TPU SRAM: code RO (executable, XN not set), data/bss RW.
	 * __RO_END__ may not be page-aligned, so round up for mmap_add_region.
	 */
	uintptr_t ro_end_aligned = (uintptr_t)__RO_END__;
	ro_end_aligned = (ro_end_aligned + 0xfff) & ~0xfff;

	mmap_add_region(TPU_SRAM_BASE, TPU_SRAM_BASE,
			ro_end_aligned - TPU_SRAM_BASE,
			MT_MEMORY | MT_RO | MT_SECURE);
	mmap_add_region(ro_end_aligned, ro_end_aligned,
			TPU_SRAM_BASE + 0x400000 - ro_end_aligned,
			MT_MEMORY | MT_RW | MT_SECURE);
	/* IO device space */
	mmap_add_region(0, 0, 0x80000000, MT_DEVICE | MT_RW | MT_SECURE);
	/*
	 * DRAM - Non-cacheable so the ROM image loader's DMA (writes physical
	 * DRAM) stays coherent with CPU read-back/verify inside load_image.
	 * A cacheable (MT_MEMORY) mapping made load_image fail -> load_rest
	 * retried and panicked. Kept NS to match ROM/cvsp layout.
	 */
	mmap_add_region(DRAM_BASE, DRAM_BASE, 0x200000000ULL,
			MT_NON_CACHEABLE | MT_RW | MT_NS);
	/* ECAM for PCIe */
	mmap_add_region(0x200000000, 0x200000000, 0x100000000,
			MT_DEVICE | MT_RW | MT_SECURE);

	INFO("MMU: regions added\n");

	init_xlat_tables();
	INFO("MMU: tables init done\n");

	enable_mmu_el3(0);
	INFO("MMU reconfigured\n");
}

void platform_setup(void)
{
	// reset rtc bisr which occurs in rom
	// reset_rtc_bisr();

	time_records->bl2_start = read_time_ms();
	VERBOSE("\n#bl2 start at: %d ms#\n", time_records->bl2_start);

	console_init(PLAT_BOOT_UART_BASE, PLAT_BM_BOOT_UART_CLK_IN_HZ,
		     PLAT_BM_CONSOLE_BAUDRATE);

	// vddc_pwm_init();

	// config_rgmii_power();
	//NOTICE("\nFSBL %s:%s\n", version_string, build_message);
	INFO("sw_info=0x%x\n", get_sw_info()->value);

	INFO("fip_param1: param_cksum=0x%x param2_loadaddr=0x%x\n",
	     fip_param1->param_cksum, fip_param1->param2_loadaddr);

	bl2_mmu_reconfig();

	INFO("CP_STATE_REG=0x%x\n", mmio_read_32(0x25047018));
#ifndef FSBL_FASTBOOT_SUPPORT
	print_sram_log();
#endif

	rom_api_redirect();
	pwm_init();

#ifdef CONFIG_PCIE_INIT
	pcie_init();
#endif

	setup_dl_flag();
	bringup_tpusys();
	//switch_rtc_mode_1st_stage();

	set_rtc_en_registers();
	
	bm_storage_boot_loader_version(BL2_VERSION_BASE);
	// if (BOOT_SRC_UART != p_rom_api->get_boot_src())
	// 	enable_top_wdt();
}

void plat_setup_early_console(void)
{
	console_init(PLAT_BOOT_UART_BASE, PLAT_BM_BOOT_UART_CLK_IN_HZ,
		     PLAT_BM_CONSOLE_BAUDRATE);
}

void bl2_early_platform_setup2(u_register_t arg0, u_register_t arg1,
			       u_register_t arg2, u_register_t arg3)
{
	(void)arg0;
	(void)arg1;
	(void)arg2;
	(void)arg3;

	platform_setup();
	//platform_warmentry();
	load_ddr();
}

void bl2_plat_arch_setup(void)
{
}

void bl2_platform_setup(void)
{
}

#pragma GCC diagnostic pop
