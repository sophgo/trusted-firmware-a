#include <stddef.h>
#include <common/interrupt_props.h>
#include <drivers/arm/gicv2.h>
#include <platform_def.h>

/*
 * Minimal GICv2 glue for cv186x on upstream ATF.
 * Keep interrupt_props empty for now; secure IRQ routing can be refined later.
 */
static const gicv2_driver_data_t cv186x_gic_data = {
	.gicd_base = PLAT_ARM_GICD_BASE,
	.gicc_base = PLAT_ARM_GICC_BASE,
	.interrupt_props = NULL,
	.interrupt_props_num = 0U,
};

void plat_arm_gic_driver_init(void)
{
	gicv2_driver_init(&cv186x_gic_data);
}

void plat_arm_gic_init(void)
{
	gicv2_distif_init();
	gicv2_pcpu_distif_init();
	gicv2_cpuif_enable();
}
