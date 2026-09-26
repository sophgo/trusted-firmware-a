/*
 * Legacy FSBL delay/timer (cntpct-based) for BL2-only APIs in plat delay_timer.h.
 */
#include <arch_helpers.h>
#include <delay_timer.h>
#include <mmio.h>
#include <platform_def.h>

#define SYS_COUNTER_FREQ_IN_US (SYS_COUNTER_FREQ_IN_SECOND / 1000000)

static uint32_t get_timer_value(void)
{
	return (uint32_t)(~read_cntpct_el0());
}

void trig_simulation_timer(uint32_t usec)
{
	uint32_t total_delta = usec * SYS_COUNTER_FREQ_IN_US;

	mmio_write_32(REG_GP_REG2, total_delta);
}

void udelay(uint32_t usec)
{
	uint32_t start, delta, total_delta;

	start = get_timer_value();
	total_delta = usec * SYS_COUNTER_FREQ_IN_US;
	mmio_write_32(REG_GP_REG2, total_delta);
	do {
		delta = start - get_timer_value();
	} while (delta < total_delta);
}

void mdelay(uint32_t msec)
{
	udelay(msec * 1000U);
}

uint32_t get_timer(uint32_t base)
{
	if (base == 0)
		return get_timer_value();
	return (base - get_timer_value()) / SYS_COUNTER_FREQ_IN_US / 1000U;
}

uint32_t get_random_from_timer(uint32_t base)
{
	if (base == 0)
		return get_timer_value();
	return (base - get_timer_value()) / SYS_COUNTER_FREQ_IN_US % 100000U;
}

void timer_init(void)
{
}

uint32_t read_count_tick(void)
{
	return (uint32_t)(read_cntpct_el0() / SYS_COUNTER_FREQ_IN_US);
}
