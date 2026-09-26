#include <console.h>
#include <platform.h>
#include <string.h>
#include <aarch64/bl2_helper.h>
#include <arch_helpers.h>
#include <bl2.h>

	static struct {
		bl31_params_t bl31_params;
		entry_point_info_t bl33_ep_info;
		entry_point_info_t bl32_ep_info;
	} next_info;

void jump_to_monitor(uintptr_t monitor_entry, uintptr_t next_addr)
{
	bl31_params_t *from_bl2 = &next_info.bl31_params;
	from_bl2->h.type = PARAM_BL31;
	from_bl2->h.version = VERSION_1;
	from_bl2->bl33_ep_info = &next_info.bl33_ep_info;
	from_bl2->bl32_ep_info = &next_info.bl32_ep_info;

	SET_SECURITY_STATE(from_bl2->bl32_ep_info->h.attr, SECURE);
	from_bl2->bl32_ep_info->pc = monitor_entry + 0x50000;
	from_bl2->bl32_ep_info->args.arg0 = 0;
	from_bl2->bl32_ep_info->spsr = 0;
	SET_SECURITY_STATE(from_bl2->bl33_ep_info->h.attr, NON_SECURE);
	from_bl2->bl33_ep_info->pc = next_addr ? next_addr : NS_IMAGE_OFFSET;
	from_bl2->bl33_ep_info->args.arg0 = 0;
	from_bl2->bl33_ep_info->spsr = SPSR_64(MODE_EL2, MODE_SP_ELX, DISABLE_ALL_EXCEPTIONS);

	flush_dcache_range((uintptr_t)&next_info, sizeof(next_info));
	jump_bl31(monitor_entry, from_bl2);
}
