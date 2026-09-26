/*
 * BL31-specific cv_common implementation for cv186x.
 * Keep this file minimal to avoid pulling BL2-only dependencies.
 */

#include <debug.h>
#include <platform_common_def.h>
#include <stdint.h>
#include <lib/xlat_tables/xlat_tables_v2.h>
#include <common/build_message.h>
#include <lib/mmio.h>

/*
 * Do not fill a static mmap_region_t[] with the v1 MAP_REGION_FLAT() (4 fields).
 * xlat_tables_v2 mmap_add() walks entries by .granularity; a v1 initializer
 * makes the next region's PA look like a 4GB granule and fails the L1/L2/L3 check.
 * mmap_add_region() is compiled against v2 and sets granularity itself.
 */
#define MAP_DEVICE_BASE		0x0ULL
#define MAP_DEVICE_SIZE		0x100000000ULL

#define DEFINE_CONFIGURE_MMU_EL(_el)					\
	void bm_configure_mmu_el##_el(unsigned long total_base,	\
				   unsigned long total_size,		\
				   unsigned long ro_start,		\
				   unsigned long ro_limit,		\
				   unsigned long coh_start,		\
				   unsigned long coh_limit)		\
	{								\
		if (total_size != 0UL) {					\
			mmap_add_region(total_base, total_base,		\
					total_size,			\
					MT_MEMORY | MT_RW | MT_SECURE);	\
		}							\
		if (ro_limit > ro_start) {					\
			mmap_add_region(ro_start, ro_start,		\
					ro_limit - ro_start,		\
					MT_MEMORY | MT_RO | MT_SECURE);	\
		}							\
		if (coh_limit > coh_start) {					\
			mmap_add_region(coh_start, coh_start,		\
					coh_limit - coh_start,		\
					MT_DEVICE | MT_RW | MT_SECURE);	\
		}							\
		mmap_add_region(MAP_DEVICE_BASE, MAP_DEVICE_BASE,	\
				MAP_DEVICE_SIZE,			\
				MT_DEVICE | MT_RW | MT_SECURE);		\
		mmap_add_region(DRAM_BASE, DRAM_BASE, DRAM_SIZE,	\
				MT_MEMORY | MT_RW | MT_SECURE);		\
		init_xlat_tables();					\
		enable_mmu_el##_el(0);					\
	}

DEFINE_CONFIGURE_MMU_EL(1)
DEFINE_CONFIGURE_MMU_EL(3)

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

