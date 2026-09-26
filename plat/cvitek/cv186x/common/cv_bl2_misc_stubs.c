#include <stddef.h>
#include <stdint.h>

#include <errno.h>

#include <arch_helpers.h>
#include <common/bl_common.h>
#include <common/debug.h>
#include <common/desc_image_load.h>
#include <firmware_image_package.h>
#include <lib/mmio.h>
#include <platform_def.h>
#include <bl2.h>
#include <ddr.h>
#include <emmc/emmc.h>
#include <rom_api.h>
#include <security/security.h>
#include <common/tbbr/tbbr_img_def.h>

unsigned char cv_tkc_ek[64];
unsigned char cv_tkcpk_hash[64];

fip_toc_header_t *get_fip_header(void)
{
	return (fip_toc_header_t *)(uintptr_t)PLAT_BM_FIP_BASE;
}

int plat_get_image_source(unsigned int image_id, uintptr_t *dev_handle,
			  uintptr_t *image_spec)
{
	(void)image_id;
	(void)dev_handle;
	(void)image_spec;
	return -ENOENT;
}

static struct bl_mem_params_node cv_bl2_mem_params_descs[] = {
	{
		.image_id = BL31_IMAGE_ID,
		SET_STATIC_PARAM_HEAD(image_info, PARAM_EP, VERSION_2, image_info_t,
				      IMAGE_ATTRIB_PLAT_SETUP | IMAGE_ATTRIB_SKIP_LOADING),
		.image_info.image_base = BL31_BASE,
		.image_info.image_max_size = BL31_LIMIT - BL31_BASE,
		SET_STATIC_PARAM_HEAD(ep_info, PARAM_EP, VERSION_2, entry_point_info_t,
				      SECURE | EXECUTABLE | EP_FIRST_EXE),
		.ep_info.pc = BL31_BASE,
		.ep_info.spsr = SPSR_64(MODE_EL3, MODE_SP_ELX, DISABLE_ALL_EXCEPTIONS),
		.next_handoff_image_id = INVALID_IMAGE_ID,
	},
};
REGISTER_BL_IMAGE_DESCS(cv_bl2_mem_params_descs)

extern struct rom_api *p_rom_api;
extern void fip_src_check(void);
extern int load_blcp_2nd(int retry);
extern int load_monitor(int retry, uint64_t *monitor_entry);
extern int load_bl32(int retry);
extern int load_blmcu(int retry);
extern int load_loader_2nd(int retry, uint64_t *loader_2nd_entry);
extern int get_loader_2nd_entry_from_header(int retry, uint64_t *loader_2nd_entry);
#if defined(BOOT_FROM_EMMC) || (defined(BOOT_FROM_SPINOR) && defined(KERNEL_BOOT_FROM_NVME))
extern int load_oem_info(void);
#endif

extern uint64_t bl31_loader_2nd_entry;

struct bl_load_info *plat_get_bl_image_load_info(void)
{
	return get_bl_load_info_from_mem_params_desc();
}

struct bl_params *plat_get_next_bl_params(void)
{
	return get_next_bl_params_from_mem_params_desc();
}

void plat_flush_next_bl_params(void)
{
	flush_bl_params_desc();
}

int bl2_plat_handle_post_image_load(unsigned int image_id)
{
	static int bl31_loaded;
	bl_mem_params_node_t *bl_mem_params;
	uint64_t monitor_entry = 0;
	int retry;
	int retries;
	int ret;
	unsigned int boot_src;

	if (image_id == BL33_IMAGE_ID) {
		bl_mem_params = get_bl_mem_params_node(BL33_IMAGE_ID);
		if ((bl_mem_params != NULL) && (bl_mem_params->ep_info.pc != 0U)) {
			bl31_loader_2nd_entry = bl_mem_params->ep_info.pc;
		}
		return 0;
	}

	if (image_id != BL31_IMAGE_ID || bl31_loaded != 0)
		return 0;

	boot_src = p_rom_api->get_boot_src();
	fip_src_check();
	sys_pll_init();

retry_from_flash:
	retries = p_rom_api->get_number_of_retries();
	for (retry = 0; retry < retries; retry++) {
#if defined(BOOT_FROM_EMMC) || (defined(BOOT_FROM_SPINOR) && defined(KERNEL_BOOT_FROM_NVME))
		if (p_rom_api->get_boot_src() != BOOT_SRC_PCIE) {
			ret = load_oem_info();
			if (ret < 0)
				ERROR("Fail to load OEM info.\n");
		}
#endif
		ret = load_blcp_2nd(retry);
		if (ret < 0)
			continue;
		ret = load_monitor(retry, &monitor_entry);
		if (ret < 0)
			continue;
		ret = load_bl32(retry);
		if (ret < 0)
			continue;
		ret = load_blmcu(retry);
		if (ret < 0)
			continue;
		ret = load_loader_2nd(retry, &bl31_loader_2nd_entry);
		if (ret < 0) {
			ret = get_loader_2nd_entry_from_header(retry, &bl31_loader_2nd_entry);
			if ((ret == 0) && (bl31_loader_2nd_entry != 0U)) {
				WARN("loader_2nd load failed, use header fallback entry=0x%lx\n",
				     (unsigned long)bl31_loader_2nd_entry);
			} else {
				bl_mem_params = get_bl_mem_params_node(BL33_IMAGE_ID);
				if ((bl_mem_params != NULL) && (bl_mem_params->ep_info.pc != 0U)) {
					bl31_loader_2nd_entry = bl_mem_params->ep_info.pc;
					WARN("loader_2nd load failed, fallback BL33 ep=0x%lx\n",
					     (unsigned long)bl31_loader_2nd_entry);
				} else {
					WARN("loader_2nd load failed, keep entry=0 (NS_IMAGE_OFFSET fallback)\n");
				}
			}
		}

		break;
	}

	if (retry >= retries) {
		switch (p_rom_api->get_boot_src()) {
		case BOOT_SRC_UART:
		case BOOT_SRC_SD:
		case BOOT_SRC_USB:
			WARN("BL2: BL31 load failed, fallback to flash.\n");
			p_rom_api->flash_init();
			goto retry_from_flash;
		default:
			break;
		}
		ERROR("BL2: failed to load BL31 monitor image\n");
		return -ENOENT;
	}
	if (monitor_entry == 0U) {
		ERROR("BL2: monitor entry is zero, BL31 is not packaged/loaded\n");
		return -ENOENT;
	}

	cv_bl2_mem_params_descs[0].ep_info.pc = monitor_entry;
	bl_mem_params = get_bl_mem_params_node(BL33_IMAGE_ID);
	if ((bl_mem_params != NULL) && (bl31_loader_2nd_entry != 0U)) {
		bl_mem_params->ep_info.pc = bl31_loader_2nd_entry;
	} else if (bl31_loader_2nd_entry == 0U) {
		WARN("BL33 entry is zero, will fallback to NS_IMAGE_OFFSET\n");
	}
	bl31_loaded = 1;
	return 0;
}

int dec_verify_image(const void *image, size_t size, size_t dec_skip,
		     struct fip_param1 *fip_p1)
{
	(void)image;
	(void)size;
	(void)dec_skip;
	(void)fip_p1;
	return 0;
}

int otp_enter_power_save_mode(void)
{
	return 0;
}

void sync_cache(void)
{
	__asm__ volatile("ic iallu\nisb\n" ::: "memory");
}

void jump_to_loader_2nd(uintptr_t loader_2nd_entry)
{
	(void)loader_2nd_entry;
	ERROR("jump_to_loader_2nd: stub (link-only build)\n");
	while (1)
		;
}

