#include <console.h>
#include <debug.h>
#include <platform.h>
#include <string.h>
#include <aarch64/bl2_helper.h>
#include <arch_helpers.h>
#include <bl2.h>

typedef struct param_header {
	uint8_t type;
	uint8_t version;
	uint16_t size;
	uint32_t attr;
} param_header_t;

typedef struct image_info {
	param_header_t h;
	uintptr_t image_base;
	uint32_t image_size;
} image_info_t;

typedef struct aapcs64_params {
	u_register_t arg0;
	u_register_t arg1;
	u_register_t arg2;
	u_register_t arg3;
	u_register_t arg4;
	u_register_t arg5;
	u_register_t arg6;
	u_register_t arg7;
} aapcs64_params_t;

typedef struct entry_point_info {
	param_header_t h;
	uintptr_t pc;
	uint32_t spsr;
	aapcs64_params_t args;
} entry_point_info_t;

typedef struct bl31_params {
	param_header_t h;
	image_info_t *bl31_image_info;
	entry_point_info_t *bl32_ep_info;
	image_info_t *bl32_image_info;
	entry_point_info_t *bl33_ep_info;
	image_info_t *bl33_image_info;
} bl31_params_t;

#define PARAM_BL31 0x03
#define VERSION_1 0x01
#define MODE_EL2 U(0x2)
#define NON_SECURE U(0x1)
#define SECURE U(0x0)
#define PARAM_EP_SECURITY_MASK U(0x1)
#define SET_SECURITY_STATE(x, security) ((x) = ((x) & ~PARAM_EP_SECURITY_MASK) | (security))

struct {
	bl31_params_t bl31_params;
	entry_point_info_t bl33_ep_info;
	entry_point_info_t bl32_ep_info;
} static next_info;

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
