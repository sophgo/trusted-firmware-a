#include <debug.h>
#include <arch_helpers.h>
#include <utils_def.h>
#include <bl2/platform.h>
//#if defined(CV186X)
//#include <bl2/ddr/88a2/bitwise_ops.h>
//#include <bl2/ddr/88a2/ddr.h>
//#endif
#if defined(CV84X6)
#include <bl2/ddr/84x6/bitwise_ops.h>
#if defined(SUPPORT_DDR_INIT)
#ifdef PLD_DDR_CFG
#include <bl2/ddr/84x6/ddr_pld.h>
#else
#include <bl2/ddr/84x6/ddr_asic.h>
#endif
#endif
#endif
#include <console.h>
#include <platform.h>
#include <rom_api.h>
#include <bl2.h>
#include <cli.h>
#include <string.h>
#include <decompress.h>
#include <delay_timer.h>
#include <security/security.h>
#include <tempsen.h>
#include <emmc/emmc.h>
#include <pinmux/cv84x6_pinmux.h>
#ifdef DDR_FW_IN_FIP
#include <aks_ddrphy_fw.h>
#endif

#define ADC_BASE 0x270E0000
#define ADC_CTL 0x04
#define ADC_BUSY_STATUS 0x8
#define ADC_CYC_SET 0xc
#define ADC_RESULT3 0x1c

uint32_t adc_val_ddr_array[7];
uint32_t adc_val_ddr_array_sum = 0;
uint32_t adc_val_ddr_array_min = 4096;
uint32_t adc_val_ddr_array_max = 0;
uint32_t adc_val_ddr;
uint32_t vol_val_ddr;
extern uint32_t  uCap_s0, uCap_s1, uCap_s2, uCap_s3;

unsigned int is_emmc_init;

struct rom_api rom_api_peri = {
	.get_boot_src = (void *)0x00000000292c0020,
	.set_boot_src = (void *)0x00000000292c0040,
	.rw_emmc_boot2 = (void *)0x00000000292c0060,
	.load_image = (void *)0x00000000292c0080,
	.flash_init = (void *)0x00000000292c00a0,
	.image_crc = (void *)0x00000000292c00c0,
	.get_number_of_retries = (void *)0x00000000292c00e0,
	.verify_rsa = (void *)0x00000000292c0100,
	.cryptodma_aes_decrypt = (void *)0x00000000292c0120
};

struct rom_api rom_api_nor = {
	.get_boot_src = (void *)0x0000000005400020,
	.set_boot_src = (void *)0x0000000005400040,
	.rw_emmc_boot2 = (void *)0x0000000005400060,
	.load_image = (void *)0x0000000005400080,
	.flash_init = (void *)0x00000000054000a0,
	.image_crc = (void *)0x00000000054000c0,
	.get_number_of_retries = (void *)0x00000000054000e0,
	.verify_rsa = (void *)0x0000000005400100,
	.cryptodma_aes_decrypt = (void *)0x0000000005400120
};

struct rom_api *p_rom_api = &rom_api_peri;

//#define BL2_USE_CLI
//#define BL2_ACCESS_BT256MB //bl2 cpu access bigthan 256M byte addrspace
struct _time_records *time_records = (void *)TIME_RECORDS_ADDR;
struct fip_param1 *fip_param1 = (void *)PARAM1_BASE;
static struct fip_param2 fip_param2 __aligned(BLOCK_SIZE);
static union {
//#if defined(CV84X6)
//	struct ddr_param ddr_param;
//#endif
	struct loader_2nd_header loader_2nd_header;
	uint8_t buf[BLOCK_SIZE];
} sram_union_buf __aligned(BLOCK_SIZE);

void print_sram_log(void)
{
	uint32_t *const log_size = (void *)BOOT_LOG_LEN_ADDR;
	uint8_t *const log_buf = (void *)phys_to_dma(BOOT_LOG_BUF_BASE);
	uint32_t i;

	const char m1[] = "\nSRAM Log: ========================================\n";
	const char m2[] = "\nSRAM Log end: ====================================\n";

	for (i = 0; m1[i]; i++)
		console_putc(m1[i]);

	for (i = 0; i < *log_size; i++)
		console_putc(log_buf[i]);

	for (i = 0; m2[i]; i++)
		console_putc(m2[i]);
}

void rom_api_redirect(void)
{
	if (mmio_read_32(REG_TOP_CONF_INFO) & 0x10000)
		p_rom_api = &rom_api_nor;
}

int load_param2(int retry)
{
#ifndef FSBL_FASTBOOT_SUPPORT
	uint32_t crc;
#endif
	int ret = -1;

	NOTICE("P2S/0x%lx/%p.\n", sizeof(fip_param2), &fip_param2);

	ret = p_rom_api->load_image(&fip_param2, fip_param1->param2_loadaddr, PARAM2_SIZE, retry);
	if (ret < 0) {
		return ret;
	}

	if (fip_param2.magic1 != FIP_PARAM2_MAGIC1) {
		ERROR("LP2_NOMAGIC fip_param2.magic1 = 0x%lx, expect 0x%llx\n", fip_param2.magic1, FIP_PARAM2_MAGIC1);
		return -1;
	}

#ifndef FSBL_FASTBOOT_SUPPORT
	crc = p_rom_api->image_crc(&fip_param2.reserved1, sizeof(fip_param2) - 12);
	if (crc != fip_param2.param2_cksum) {
		ERROR("param2_cksum (0x%x/0x%x)\n", crc, fip_param2.param2_cksum);
		return -1;
	}
#endif

	NOTICE("P2E.\n");

	return 0;
}

int load_ddr_param(int retry)
{
	return 0;
}

void get_adc3(void) {
	// read adc3 value
	PINMUX_CONFIG(ADC3, ADC3, G3);
	// mmio_clrbits_32(0x28104c34, 1 << 14);
	mmio_write_32(ADC_BASE + ADC_CTL, 0x80);
	mmio_clrsetbits_32(ADC_BASE + ADC_CYC_SET, 0xf000, 0xf << 12);
	mmio_clrsetbits_32(ADC_BASE + ADC_CYC_SET, 0xf000, 0x2 << 12);
	mmio_setbits_32(ADC_BASE + ADC_CTL, 0x01);
	for(int i = 0 ; i < 7 ; i++){
		// check adc busy
		mmio_clrbits_32(ADC_BASE + ADC_CTL, 0x01);
		mmio_setbits_32(ADC_BASE + ADC_CTL, 0x01);
		while(mmio_read_32(ADC_BASE + ADC_BUSY_STATUS) & 0x1);
		adc_val_ddr_array[i] = mmio_read_32(ADC_BASE + ADC_RESULT3) & 0xfff;
		adc_val_ddr_array_sum += adc_val_ddr_array[i];
		adc_val_ddr_array_min = (adc_val_ddr_array_min < adc_val_ddr_array[i] ? adc_val_ddr_array_min : adc_val_ddr_array[i]);
		adc_val_ddr_array_max = (adc_val_ddr_array_max > adc_val_ddr_array[i] ? adc_val_ddr_array_max : adc_val_ddr_array[i]);
	}
	adc_val_ddr = (adc_val_ddr_array_sum - adc_val_ddr_array_min - adc_val_ddr_array_max)/5;
	vol_val_ddr = (adc_val_ddr * 1500) / 4096;
}

int load_ddr(void)
{
#if !defined(PALLADIUM_PLAT)
	int retry = 0;

retry_from_flash:
	for (retry = 0; retry < p_rom_api->get_number_of_retries(); retry++) {
		if (load_param2(retry) < 0)
			continue;

		if (load_ddr_param(retry) < 0)
			continue;

		break;
	}

	if (retry >= p_rom_api->get_number_of_retries()) {
		switch (p_rom_api->get_boot_src()) {
		case BOOT_SRC_UART:
		case BOOT_SRC_SD:
		case BOOT_SRC_USB:
			WARN("DL cancelled. Load flash. (%d).\n", retry);
			// Continue to boot from flash if boot from external source
			p_rom_api->flash_init();
			goto retry_from_flash;
		default:
			ERROR("Failed to load DDR param (%d).\n", retry);
			plat_panic_handler();
		}
	}
#endif
// 	time_records->ddr_init_start = read_time_ms();
// 	VERBOSE("\n#ddr_int_start at %d ms#\n", time_records->ddr_init_start);

// 	get_adc3();

#if 1//ndef CONFIG_BOARD_fpga

// #ifdef BL2_ACCESS_BT256MB
// 	disable_mmu_el3();//close mmu
// #endif
#ifdef SUPPORT_DDR_INIT
	NOTICE("Enter DDR INIT\n");

#ifdef DDR_FW_IN_FIP
	{
		/*
		 * Placed above the xlat_table pool that BL2 now allocates
		 * (see bl2_mmu_reconfig() in cv_bl2_setup.c) instead of the
		 * former 0x24030000.
		 */
		#define FW_SRAM_BASE	0x24080000
		uint8_t *fw_ptr = (uint8_t *)FW_SRAM_BASE;
		uint32_t fw_size, fw_off;
		int fw_ret;
		uint32_t *fw_data;
		uint32_t mid;
		uint32_t fw_crc;

#ifdef DDR_CFG_LPDDR4
		NOTICE("DDR FW: loading LPDDR4 firmware to 0x%x\n", FW_SRAM_BASE);

		fw_size = fip_param2.ddr_fw_lp4_itim_size;
		if (fw_size) {
			fw_off = fip_param2.ddr_fw_lp4_itim_offset;
			fw_ret = p_rom_api->load_image(fw_ptr, fw_off, fw_size, 0);
			if (fw_ret < 0) {
				ERROR("LP4 ITIM load failed\n");
				return -1;
			}
			fw_data = (uint32_t *)fw_ptr;
			mid = (fw_size / 8 / 2) * 2;
			fw_crc = p_rom_api->image_crc(fw_ptr, fw_size);
			NOTICE("LP4 ITIM: %uB dst=0x%lx off=0x%x crc=0x%x [0]=0x%08x [%u]=0x%08x [%u]=0x%08x\n",
				fw_size, (uintptr_t)fw_ptr, fw_off, fw_crc,
				fw_data[0], mid, fw_data[mid],
				(fw_size/4)-2, fw_data[(fw_size/4)-2]);
			if (fw_crc == fip_param2.ddr_fw_lp4_itim_cksum)
				fw_ptr += fw_size;
			else {
				ERROR("LP4 ITIM CRC mismatch: 0x%x != 0x%x\n",
					fw_crc, fip_param2.ddr_fw_lp4_itim_cksum);
				return -1;
			}
		}

		fw_size = fip_param2.ddr_fw_lp4_dtim_size;
		if (fw_size) {
			fw_off = fip_param2.ddr_fw_lp4_dtim_offset;
			fw_ret = p_rom_api->load_image(fw_ptr, fw_off, fw_size, 0);
			if (fw_ret < 0) {
				ERROR("LP4 DTIM load failed\n");
				return -1;
			}
			fw_data = (uint32_t *)fw_ptr;
			mid = (fw_size / 8 / 2) * 2;
			fw_crc = p_rom_api->image_crc(fw_ptr, fw_size);
			NOTICE("LP4 DTIM: %uB dst=0x%lx off=0x%x crc=0x%x [0]=0x%08x [%u]=0x%08x [%u]=0x%08x\n",
				fw_size, (uintptr_t)fw_ptr, fw_off, fw_crc,
				fw_data[0], mid, fw_data[mid],
				(fw_size/4)-2, fw_data[(fw_size/4)-2]);
			if (fw_crc == fip_param2.ddr_fw_lp4_dtim_cksum)
				fw_ptr += fw_size;
			else {
				ERROR("LP4 DTIM CRC mismatch: 0x%x != 0x%x\n",
					fw_crc, fip_param2.ddr_fw_lp4_dtim_cksum);
				return -1;
			}
		}
#elif defined(DDR_CFG_LPDDR5)
		NOTICE("DDR FW: loading LPDDR5 firmware to 0x%x\n", FW_SRAM_BASE);

		fw_size = fip_param2.ddr_fw_lp5_itim_size;
		if (fw_size) {
			fw_off = fip_param2.ddr_fw_lp5_itim_offset;
			fw_ret = p_rom_api->load_image(fw_ptr, fw_off, fw_size, 0);
			if (fw_ret < 0) {
				ERROR("LP5 ITIM load failed\n");
				return -1;
			}
			fw_data = (uint32_t *)fw_ptr;
			mid = (fw_size / 8 / 2) * 2;
			fw_crc = p_rom_api->image_crc(fw_ptr, fw_size);
			NOTICE("LP5 ITIM: %uB dst=0x%lx off=0x%x crc=0x%x [0]=0x%08x [%u]=0x%08x [%u]=0x%08x\n",
				fw_size, (uintptr_t)fw_ptr, fw_off, fw_crc,
				fw_data[0], mid, fw_data[mid],
				(fw_size/4)-2, fw_data[(fw_size/4)-2]);
			if (fw_crc == fip_param2.ddr_fw_lp5_itim_cksum)
				fw_ptr += fw_size;
			else {
				ERROR("LP5 ITIM CRC mismatch: 0x%x != 0x%x\n",
					fw_crc, fip_param2.ddr_fw_lp5_itim_cksum);
				return -1;
			}
		}

		fw_size = fip_param2.ddr_fw_lp5_dtim_size;
		if (fw_size) {
			fw_off = fip_param2.ddr_fw_lp5_dtim_offset;
			fw_ret = p_rom_api->load_image(fw_ptr, fw_off, fw_size, 0);
			if (fw_ret < 0) {
				ERROR("LP5 DTIM load failed\n");
				return -1;
			}
			fw_data = (uint32_t *)fw_ptr;
			mid = (fw_size / 8 / 2) * 2;
			fw_crc = p_rom_api->image_crc(fw_ptr, fw_size);
			NOTICE("LP5 DTIM: %uB dst=0x%lx off=0x%x crc=0x%x [0]=0x%08x [%u]=0x%08x [%u]=0x%08x\n",
				fw_size, (uintptr_t)fw_ptr, fw_off, fw_crc,
				fw_data[0], mid, fw_data[mid],
				(fw_size/4)-2, fw_data[(fw_size/4)-2]);
			if (fw_crc != fip_param2.ddr_fw_lp5_dtim_cksum) {
				ERROR("LP5 DTIM CRC mismatch: 0x%x != 0x%x\n",
					fw_crc, fip_param2.ddr_fw_lp5_dtim_cksum);
				return -1;
			}
		}
#endif
		ddr_fw_init_from_fip(FW_SRAM_BASE, &fip_param2);
	}
#endif

//#if defined(CV186X)
	ddr_init();
//#endif
#endif
#ifdef  BL2_USE_CLI
	tempsen_init();
	cli_simple_loop(1);
#endif
#endif
	time_records->ddr_init_end = read_time_ms();
	VERBOSE("\n#ddr_int_end at %d ms#\n", time_records->ddr_init_end);
	return 0;
}

int load_blcp_2nd(int retry)
{
	uint32_t rtos_base;
#ifndef FSBL_FASTBOOT_SUPPORT
	uint32_t crc;
#endif
	int ret = -1;

	// if no blcp_2nd, release_blcp_2nd should be ddr_init_end

	NOTICE("C2S/0x%x/0x%x/0x%x.\n", fip_param2.blcp_2nd_loadaddr, fip_param2.blcp_2nd_runaddr,
	       fip_param2.blcp_2nd_size);

	if (!fip_param2.blcp_2nd_runaddr) {
		NOTICE("No C906L image.\n");
		return 0;
	}

	if (!IN_RANGE(fip_param2.blcp_2nd_runaddr, DRAM_BASE, DRAM_SIZE)) {
		ERROR("blcp_2nd_runaddr (0x%x) is not in DRAM.\n", fip_param2.blcp_2nd_runaddr);
		plat_panic_handler();
	}

	if (!IN_RANGE(fip_param2.blcp_2nd_runaddr + fip_param2.blcp_2nd_size, DRAM_BASE, DRAM_SIZE)) {
		ERROR("blcp_2nd_size (0x%x) is not in DRAM.\n", fip_param2.blcp_2nd_size);
		plat_panic_handler();
	}

// #ifdef USB_DL_BY_FSBL
// 	if (p_rom_api->get_boot_src() == BOOT_SRC_USB)
// 		ret = load_image_by_usb((void *)(uintptr_t)fip_param2.blcp_2nd_runaddr, fip_param2.blcp_2nd_loadaddr,
// 					fip_param2.blcp_2nd_size, retry);
// 	else
// #endif
	ret = p_rom_api->load_image((void *)(uintptr_t)fip_param2.blcp_2nd_runaddr, fip_param2.blcp_2nd_loadaddr,
				   fip_param2.blcp_2nd_size, retry);
	if (ret < 0) {
		return ret;
	}

#ifndef FSBL_FASTBOOT_SUPPORT
	crc = p_rom_api->image_crc((void *)(uintptr_t)fip_param2.blcp_2nd_runaddr, fip_param2.blcp_2nd_size);
	if (crc != fip_param2.blcp_2nd_cksum) {
		ERROR("blcp_2nd_cksum (0x%x/0x%x)\n", crc, fip_param2.blcp_2nd_cksum);
		return -1;
	}
#endif

	ret = dec_verify_image((void *)(uintptr_t)fip_param2.blcp_2nd_runaddr, fip_param2.blcp_2nd_size, 0, fip_param1);
	if (ret < 0) {
		ERROR("verify blcp 2nd (%d)\n", ret);
		return ret;
	}

	flush_dcache_range(fip_param2.blcp_2nd_runaddr, fip_param2.blcp_2nd_size);

	rtos_base = mmio_read_32(AXI_SRAM_RTOS_BASE);
	init_comm_info();

	switch (p_rom_api->get_boot_src()) {
		case BOOT_SRC_UART:
		// case BOOT_SRC_SD:
		// case BOOT_SRC_USB:
			break;
		default:
			if (rtos_base == CVI_RTOS_MAGIC_CODE) {
				mmio_write_32(AXI_SRAM_RTOS_BASE, fip_param2.blcp_2nd_runaddr);
			} else {
				reset_c906l(fip_param2.blcp_2nd_runaddr);
			}
	}

	NOTICE("C2E.\n");
	return 0;
}

int load_monitor(int retry, uint64_t *monitor_entry)
{
#ifndef FSBL_FASTBOOT_SUPPORT
	uint32_t crc;
#endif
	int ret = -1;

	NOTICE("MS/0x%lx/0x%lx/0x%x.\n", fip_param2.monitor_loadaddr, fip_param2.monitor_runaddr,
	       fip_param2.monitor_size);

	if (!fip_param2.monitor_runaddr) {
		NOTICE("No monitor.\n");
		return 0;
	}

	if (!IN_RANGE(fip_param2.monitor_runaddr, DRAM_BASE, DRAM_SIZE)) {
		ERROR("monitor_runaddr (0x%lx) is not in DRAM.\n", fip_param2.monitor_runaddr);
		plat_panic_handler();
	}

	if (!IN_RANGE(fip_param2.monitor_runaddr + fip_param2.monitor_size, DRAM_BASE, DRAM_SIZE)) {
		ERROR("monitor_size (0x%x) is not in DRAM.\n", fip_param2.monitor_size);
		plat_panic_handler();
	}

// #ifdef USB_DL_BY_FSBL
// 	if (p_rom_api->get_boot_src() == BOOT_SRC_USB)
// 		ret = load_image_by_usb((void *)(uintptr_t)fip_param2.monitor_runaddr, fip_param2.monitor_loadaddr,
// 					fip_param2.monitor_size, retry);
// 	else
// #endif
	ret = p_rom_api->load_image((void *)(uintptr_t)fip_param2.monitor_runaddr, fip_param2.monitor_loadaddr,
				   fip_param2.monitor_size, retry);
	if (ret < 0) {
		return ret;
	}

#ifndef FSBL_FASTBOOT_SUPPORT
	crc = p_rom_api->image_crc((void *)(uintptr_t)fip_param2.monitor_runaddr, fip_param2.monitor_size);
	if (crc != fip_param2.monitor_cksum) {
		ERROR("monitor_cksum (0x%x/0x%x)\n", crc, fip_param2.monitor_cksum);
		return -1;
	}
#endif

	ret = dec_verify_image((void *)(uintptr_t)fip_param2.monitor_runaddr, fip_param2.monitor_size, 0, fip_param1);
	if (ret < 0) {
		ERROR("verify monitor (%d)\n", ret);
		return ret;
	}

	flush_dcache_range(fip_param2.monitor_runaddr, fip_param2.monitor_size);
	NOTICE("ME.\n");

	*monitor_entry = fip_param2.monitor_runaddr;

	return 0;
}

int load_bl32(int retry)
{
#ifndef FSBL_FASTBOOT_SUPPORT
	uint32_t crc;
#endif
	int ret = -1;

	NOTICE("BL32/0x%lx/0x%lx/0x%x.\n", fip_param2.bl32_loadaddr, fip_param2.bl32_runaddr,
	       fip_param2.bl32_size);

	if (!fip_param2.bl32_runaddr) {
		NOTICE("No monitor.\n");
		return 0;
	}

	if (!IN_RANGE(fip_param2.bl32_runaddr, DRAM_BASE, DRAM_SIZE)) {
		ERROR("bl32_runaddr (0x%lx) is not in DRAM.\n", fip_param2.bl32_runaddr);
		plat_panic_handler();
	}

	if (!IN_RANGE(fip_param2.bl32_runaddr + fip_param2.bl32_size, DRAM_BASE, DRAM_SIZE)) {
		ERROR("bl32_size (0x%x) is not in DRAM.\n", fip_param2.bl32_size);
		plat_panic_handler();
	}

// #ifdef USB_DL_BY_FSBL
// 	if (p_rom_api->get_boot_src() == BOOT_SRC_USB)
// 		ret = load_image_by_usb((void *)(uintptr_t)fip_param2.bl32_runaddr, fip_param2.monitor_loadaddr,
// 					fip_param2.bl32_size, retry);
// 	else
// #endif
	ret = p_rom_api->load_image((void *)(uintptr_t)fip_param2.bl32_runaddr, fip_param2.bl32_loadaddr,
				   fip_param2.bl32_size, retry);
	if (ret < 0) {
		return ret;
	}

#ifndef FSBL_FASTBOOT_SUPPORT
	crc = p_rom_api->image_crc((void *)(uintptr_t)fip_param2.bl32_runaddr, fip_param2.bl32_size);
	if (crc != fip_param2.bl32_cksum) {
		ERROR("monitor_cksum (0x%x/0x%x)\n", crc, fip_param2.bl32_cksum);
		return -1;
	}
#endif

	ret = dec_verify_image((void *)(uintptr_t)fip_param2.bl32_runaddr, fip_param2.bl32_size, 0, fip_param1);
	if (ret < 0) {
		ERROR("verify monitor (%d)\n", ret);
		return ret;
	}

	flush_dcache_range(fip_param2.bl32_runaddr, fip_param2.bl32_size);
	NOTICE("BL32.E.\n");

	return 0;
}

int load_blmcu(int retry)
{
#ifndef FSBL_FASTBOOT_SUPPORT
	uint32_t crc;
#endif
	int ret = -1;

	NOTICE("BLMCU/0x%lx/0x%lx/0x%x.\n", fip_param2.blmcu_loadaddr, fip_param2.blmcu_runaddr,
	       fip_param2.blmcu_size);

	if (!fip_param2.blmcu_runaddr) {
		NOTICE("No blmcu.\n");
		return 0;
	}

	ret = p_rom_api->load_image((void *)(uintptr_t)fip_param2.blmcu_runaddr, fip_param2.blmcu_loadaddr,
				   fip_param2.blmcu_size, retry);
	if (ret < 0) {
		return ret;
	}

#ifndef FSBL_FASTBOOT_SUPPORT
	crc = p_rom_api->image_crc((void *)(uintptr_t)fip_param2.blmcu_runaddr, fip_param2.blmcu_size);
	if (crc != fip_param2.blmcu_cksum) {
		ERROR("blmcu_cksum (0x%x/0x%x)\n", crc, fip_param2.blmcu_cksum);
		return -1;
	}
#endif

	ret = dec_verify_image((void *)(uintptr_t)fip_param2.blmcu_runaddr, fip_param2.blmcu_size, 0, fip_param1);
	if (ret < 0) {
		ERROR("verify blmcu (%d)\n", ret);
		return ret;
	}

	mmio_write_32(0x28100248, 0x1);
	mmio_write_32(0x05025020, 0x5200080);
	mmio_write_32(0x05025024, 0x5200000);

	time_records->blmcu_start = read_time_ms();
	VERBOSE("\n#blmcu start at %dms#\n", time_records->blmcu_start);
	NOTICE("BLMCU.E.\n");

	return 0;
}

int load_loader_2nd(int retry, uint64_t *loader_2nd_entry)
{
	struct loader_2nd_header *loader_2nd_header = &sram_union_buf.loader_2nd_header;
#ifndef FSBL_FASTBOOT_SUPPORT
	uint32_t crc;
#endif
	int ret = -1;

	const int cksum_offset =
		offsetof(struct loader_2nd_header, cksum) + sizeof(((struct loader_2nd_header *)0)->cksum);

	enum COMPRESS_TYPE comp_type = COMP_NONE;
	int reading_size;
	void *image_buf;

	NOTICE("L2/0x%lx.\n", fip_param2.loader_2nd_loadaddr);

// #ifdef USB_DL_BY_FSBL
// 	if (p_rom_api->get_boot_src() == BOOT_SRC_USB)
// 		ret = load_image_by_usb(loader_2nd_header, fip_param2.loader_2nd_loadaddr, BLOCK_SIZE, retry);
// 	else
// #endif
	ret = p_rom_api->load_image(loader_2nd_header, fip_param2.loader_2nd_loadaddr, BLOCK_SIZE, retry);
	if (ret < 0) {
		return ret;
	}

	reading_size = ROUND_UP_2EVAL(loader_2nd_header->size, BLOCK_SIZE);

	NOTICE("L2/0x%x/0x%x/0x%lx/0x%x/0x%x\n", loader_2nd_header->magic, loader_2nd_header->cksum,
	       loader_2nd_header->runaddr, loader_2nd_header->size, reading_size);

	switch (loader_2nd_header->magic) {
	case LOADER_2ND_MAGIC_LZMA:
		comp_type = COMP_LZMA;
		break;
	case LOADER_2ND_MAGIC_LZ4:
		comp_type = COMP_LZ4;
		break;
	default:
		comp_type = COMP_NONE;
		break;
	}

	if (comp_type) {
		NOTICE("COMP/%d.\n", comp_type);
		image_buf = (void *)DECOMP_BUF_ADDR;
	} else {
		image_buf = (void *)loader_2nd_header->runaddr;
	}

// #ifdef USB_DL_BY_FSBL
// 	if (p_rom_api->get_boot_src() == BOOT_SRC_USB)
// 		ret = load_image_by_usb(image_buf, fip_param2.loader_2nd_loadaddr, reading_size, retry);
// 	else
// #endif
	ret = p_rom_api->load_image(image_buf, fip_param2.loader_2nd_loadaddr, reading_size, retry);
	if (ret < 0) {
		return ret;
	}

#ifndef FSBL_FASTBOOT_SUPPORT
	crc = p_rom_api->image_crc(image_buf + cksum_offset, loader_2nd_header->size - cksum_offset);
	if (crc != loader_2nd_header->cksum) {
		ERROR("loader_2nd_cksum (0x%x/0x%x)\n", crc, loader_2nd_header->cksum);
		return -1;
	}
#endif
	ret = dec_verify_image(image_buf + cksum_offset, loader_2nd_header->size - cksum_offset,
			       sizeof(struct loader_2nd_header) - cksum_offset, fip_param1);
	if (ret < 0) {
		ERROR("verify loader 2nd (%d)\n", ret);
		return ret;
	}


	// sys_switch_all_to_pll();

	if (comp_type) {
		size_t dst_size = DECOMP_DST_SIZE;

		// header is not compressed.
		void *dst = (void *)loader_2nd_header->runaddr;

		memcpy(dst, image_buf, sizeof(struct loader_2nd_header));
		image_buf += sizeof(struct loader_2nd_header);

		ret = decompress(dst + sizeof(struct loader_2nd_header), &dst_size, image_buf, loader_2nd_header->size,
				 comp_type);
		if (ret < 0) {
			ERROR("Failed to decompress loader_2nd (%d/%lu)\n", ret, dst_size);
			return -1;
		}

		reading_size = sizeof(struct loader_2nd_header) + dst_size;
	}

	flush_dcache_range(loader_2nd_header->runaddr, reading_size);
	sync_cache();
	NOTICE("Loader_2nd loaded.\n");

	*loader_2nd_entry = loader_2nd_header->runaddr + sizeof(struct loader_2nd_header);

	return 0;
}

#if defined(BOOT_FROM_EMMC) || (defined(BOOT_FROM_SPINOR) && defined(KERNEL_BOOT_FROM_NVME))
#define OEM_INFO_MAX_BYTE_SIZE	256
int load_oem_info(void)
{
	char oem_info[EMMC_BLOCK_SIZE] __attribute__((aligned(EMMC_BLOCK_SIZE)));
	uint8_t dram_size_GB = 0;

#if defined(BOOT_FROM_EMMC)
	int ret;

	if (is_emmc_init == 0) {
			bm_emmc_init();
			is_emmc_init = 1;
	}

	/* OEM information is save in eMMC BOOT2 partition
	 * @para buf: Must be aligned EMMC_BLOCK_SIZE
	 * @para size: Must be greater than 512
	 */
	ret = emmc_boot2_read_blocks(0, (uintptr_t)oem_info, EMMC_BLOCK_SIZE);
	if (ret < 0)
		return -1;

	// /* OEM information is save in eMMC BOOT2 partition
	//  * @para buf: Must be aligned EMMC_BLOCK_SIZE
	//  * @para size: Must be greater than 512
	//  */
	// ret = p_rom_api->rw_emmc_boot2(0, (uintptr_t)oem_info, EMMC_BLOCK_SIZE, 0);
	// if (ret < 0)
	// 	return -1;

	/* Write ddr size into OEM */
	dram_size_GB = (uint8_t)((uCap_s0 + uCap_s1 + uCap_s2 + uCap_s3) / 1024);

	// if (dram_size_GB != ((uint8_t)oem_info[0xF0])) {
	oem_info[0xF0] = (dram_size_GB & 0xFF);
	// 	NOTICE("Rewrite ddr size into OEM, ddr_size = %d GB\n", oem_info[0xF0]);
	// 	ret = emmc_boot2_write_blocks(0, (uintptr_t)oem_info, EMMC_BLOCK_SIZE, 1);
	// 	if (ret < 0)
	// 		return -1;
	// }
#elif defined(BOOT_FROM_SPINOR)
	//todo: read OEM info from spinor flash

	memset(oem_info, 0, EMMC_BLOCK_SIZE);

	/* Write ddr size into OEM */
	dram_size_GB = (uint8_t)((uCap_s0 + uCap_s1 + uCap_s2 + uCap_s3) / 1024);
	if (dram_size_GB != ((uint8_t)oem_info[0xf0])) {
		oem_info[0xf0] = (dram_size_GB & 0xFF);
		NOTICE("Rewrite ddr size into OEM, ddr_size = %d GB\n", oem_info[0xf0]);
		// todo: Rewrite to spinor flash
	}
#endif

	/* Write OEM information to the last OEM_INFO_MAX_BYTE_SIZE bytes of RTC SRAM */
	for (int i = 0; i < OEM_INFO_MAX_BYTE_SIZE; i ++)
		mmio_write_8(OEM_INFO_RTC_SRAM_ADDR+i, oem_info[i]);

	NOTICE("Load OEM info end.\n");

// #define FSBL_PRINT_OEM_INFORMATION
#ifdef FSBL_PRINT_OEM_INFORMATION
	NOTICE("OEM info: \n");
	NOTICE("  SN0	: %s.\n", &oem_info[0x00]);	//SN0
	NOTICE("  SN1	: %s.\n", &oem_info[0x20]);	//SN1
	NOTICE("  MAC0	: %02x:%02x:%02x:%02x:%02x:%02x\n",
			oem_info[0x40],oem_info[0x41],oem_info[0x42],oem_info[0x43],oem_info[0x44],oem_info[0x45]);	//MAC0
	NOTICE("  MAC1	: %02x:%02x:%02x:%02x:%02x:%02x\n",
			oem_info[0x50],oem_info[0x51],oem_info[0x52],oem_info[0x53],oem_info[0x54],oem_info[0x55]);	//MAC1
	NOTICE("  product type	: %s.\n", &oem_info[0x60]);	//product type
	NOTICE("  module type	: %s.\n", &oem_info[0x70]);	//module type
	NOTICE("  inter flag	: %d.\n", oem_info[0x80]);	//interface flag
	NOTICE("  aging flag	: %d.\n", oem_info[0x81]);	//aging flag
	NOTICE("  vendor	: %s.\n", &oem_info[0x90]);		//vendor
	NOTICE("  dts type	: %s.\n", &oem_info[0xA0]);		//dts type
	NOTICE("  hw version	: %s.\n", &oem_info[0xC0]);	//hw version
	NOTICE("  produce	: %s.\n", &oem_info[0xD0]);		//product
	NOTICE("  chip		: %s.\n", &oem_info[0xE0]);		//chip
	NOTICE("  ddr size	: %s.\n", &oem_info[0xF0]);		//ddr size

	/* readback check*/
	for (int i = 0; i < OEM_INFO_MAX_BYTE_SIZE; i ++) {
		if (oem_info[i] != mmio_read_8(OEM_INFO_RTC_SRAM_ADDR+i)) {
			ERROR("cmp err %d. \n", i);
			break;
		}
	}
	NOTICE("OEM info readback check OK.\n");
#endif
	return 0;
}
#endif

static int load_param1(int retry)
{
       uint32_t fip_param1_size = PARAM1_SIZE;
       int ret = 0;

       ret = p_rom_api->load_image(fip_param1, 0, fip_param1_size, retry);
       if (ret < 0) {
               ERROR("load param1 (%d)\n", ret);
               return ret;
       }

       if (fip_param1->magic1 != FIP_PARAM1_MAGIC1) {
               ERROR("PARAM1 magic (0x%lx)\n", fip_param1->magic1);
               return -1;
       }

       return ret;
}

void fip_src_check(void)
{
	int retry = 0;

	mmio_setbits_32(PCIE_BOOT_REG, 0x3 << 16);
	if (p_rom_api->get_boot_src() != BOOT_SRC_RTC_NOR)
		return;

	NOTICE("Waiting for boot image in position\n");
	while (1) {
		feed_dog();
		if(mmio_read_32(PCIE_BOOT_REG) & 0x10) {
			NOTICE("The image is in position\n");
			mmio_clrbits_32(PCIE_BOOT_REG, 0x10010);
			p_rom_api->set_boot_src(BOOT_SRC_PCIE);

			inv_dcache_range(0x102000000, 0x80000);

			for (retry = 0; retry < p_rom_api->get_number_of_retries(); retry++) {
				if (load_param1(retry) < 0)
					continue;
				if (load_param2(retry) < 0)
					continue;
				break;
			}
			break;
		}
		mdelay(100);
	}
}


int get_loader_2nd_entry_from_header(int retry, uint64_t *loader_2nd_entry)
{
	union {
		struct loader_2nd_header hdr;
		uint8_t raw[BLOCK_SIZE];
	} hdr_buf __aligned(BLOCK_SIZE);
	int ret;

	if (loader_2nd_entry == NULL)
		return -1;

	ret = p_rom_api->load_image(&hdr_buf, fip_param2.loader_2nd_loadaddr, BLOCK_SIZE, retry);
	if (ret < 0) {
		ERROR("get_loader_2nd_entry: load header failed (%d)\n", ret);
		return ret;
	}

	if ((hdr_buf.hdr.magic != LOADER_2ND_MAGIC_RAW) && 
	    (hdr_buf.hdr.magic != LOADER_2ND_MAGIC_LZMA) && 
	    (hdr_buf.hdr.magic != LOADER_2ND_MAGIC_LZ4)) {
		ERROR("get_loader_2nd_entry: bad magic 0x%x\n", hdr_buf.hdr.magic);
		return -1;
	}

	*loader_2nd_entry = hdr_buf.hdr.runaddr + sizeof(struct loader_2nd_header);
	return 0;
}
int load_rest(void)
{

	uint64_t monitor_entry = 0;
	uint64_t loader_2nd_entry = 0;
#if !defined(PALLADIUM_PLAT)
	int retry = 0;
	fip_src_check();

	// Init sys PLL and switch clocks to PLL
	sys_pll_init();

	mmio_setbits_32(PCIE_BOOT_REG, 0x3 << 16);
	NOTICE("LR/0x%lx/0x%llx/src=0x%x.\n", (unsigned long)DRAM_BASE,
	       (unsigned long long)0x800000000ULL, p_rom_api->get_boot_src());

retry_from_flash:
	for (retry = 0; retry < p_rom_api->get_number_of_retries(); retry++) {
#if defined(BOOT_FROM_EMMC)
		if (p_rom_api->get_boot_src() == BOOT_SRC_EMMC) {
			//if (load_oem_info() < 0)
			//	ERROR("Fail to load OEM info.\n");
		}
#elif defined(BOOT_FROM_SPINOR) && defined(KERNEL_BOOT_FROM_NVME)
		if (p_rom_api->get_boot_src() != BOOT_SRC_PCIE) {
			if (load_oem_info() < 0)
				ERROR("Fail to load OEM info.\n");
		}
#endif

		if (load_blcp_2nd(retry) < 0)
			continue;

		if (load_monitor(retry, &monitor_entry) < 0)
			continue;

		if(load_bl32(retry) < 0)
			continue;

		if (load_blmcu(retry) < 0)
			continue;

		if (load_loader_2nd(retry, &loader_2nd_entry) < 0)
			continue;

		//MCU reset shoule deassert after image loaded from nor flash
		// mmio_write_32(0x05025018, 0x1fffff);
		break;
	}

	if (retry >= p_rom_api->get_number_of_retries()) {
		switch (p_rom_api->get_boot_src()) {
		case BOOT_SRC_UART:
		case BOOT_SRC_SD:
		case BOOT_SRC_USB:
			WARN("DL cancelled. Load flash. (%d).\n", retry);
			// Continue to boot from flash if boot from external source
			p_rom_api->flash_init();
			goto retry_from_flash;
		default:
			ERROR("Failed to load rest (%d).\n", retry);
			plat_panic_handler();
		}
	}

	// otp_enter_power_save_mode();

	sync_cache();
	// console_flush();
	//switch_rtc_mode_2nd_stage();

	// feed_dog();
#else
	NOTICE("M/%lx/N/%lx/", monitor_entry, loader_2nd_entry);
	monitor_entry = CVIMMAP_MONITOR_ADDR;
	loader_2nd_entry = CONFIG_SYS_TEXT_BASE;
#endif

	if (monitor_entry) {
		INFO("From BL2 jump to BL31 at 0x%lx.\n", monitor_entry);
		jump_to_monitor(monitor_entry, loader_2nd_entry);
	} else {
		INFO("Jump to loader_2nd at 0x%lx.\n", loader_2nd_entry);
		jump_to_loader_2nd(loader_2nd_entry);
	}

	return 0;
}
