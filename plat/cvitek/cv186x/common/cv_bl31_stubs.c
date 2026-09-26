#include <arch_helpers.h>
#include <common/debug.h>
#include <drivers/console.h>
#include <drivers/io/io_storage.h>
#include <plat/common/platform.h>

int plat_get_image_source(unsigned int image_id, uintptr_t *dev_handle,
			  uintptr_t *image_spec)
{
	(void)image_id;
	(void)dev_handle;
	(void)image_spec;
	return -1;
}

int io_dev_close(uintptr_t dev_handle)
{
	(void)dev_handle;
	return -1;
}

int io_open(uintptr_t dev_handle, const uintptr_t spec, uintptr_t *handle)
{
	(void)dev_handle;
	(void)spec;
	(void)handle;
	return -1;
}

int io_size(uintptr_t handle, size_t *length)
{
	(void)handle;
	(void)length;
	return -1;
}

int io_read(uintptr_t handle, uintptr_t buffer, size_t length,
	    size_t *length_read)
{
	(void)handle;
	(void)buffer;
	(void)length;
	(void)length_read;
	return -1;
}

int io_close(uintptr_t handle)
{
	(void)handle;
	return -1;
}

plat_local_state_t plat_get_target_pwr_state(unsigned int lvl,
					     const plat_local_state_t *states,
					     unsigned int ncpu)
{
	(void)lvl;
	(void)states;
	(void)ncpu;
	return PLAT_MAX_OFF_STATE;
}

bool is_dcache_enabled(void)
{
	return true;
}
