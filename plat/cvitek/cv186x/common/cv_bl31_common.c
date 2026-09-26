/*
 * BL31-specific cv_common implementation for cv186x.
 * Keep this file minimal to avoid pulling BL2-only dependencies.
 */

#include <debug.h>
#include <platform_common_def.h>
#include <stdint.h>
#include <xlat_tables.h>

#define MAP_DEVICE	MAP_REGION_FLAT(0, 0x100000000, \
					MT_DEVICE | MT_RW | MT_SECURE)
#define MAP_DRAM	MAP_REGION_FLAT(0x100000000, 0x80000000, \
					MT_MEMORY | MT_RW | MT_SECURE)

static const mmap_region_t plat_bm_mmap[] = {
	MAP_DEVICE,
	MAP_DRAM,
	{0},
};

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
		mmap_add(plat_bm_mmap);					\
		init_xlat_tables();					\
		enable_mmu_el##_el(0);					\
	}

DEFINE_CONFIGURE_MMU_EL(1)
DEFINE_CONFIGURE_MMU_EL(3)

