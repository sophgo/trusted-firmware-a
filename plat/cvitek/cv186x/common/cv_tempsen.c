#include <tempsen.h>

void tempsen_init(void)
{
	unsigned int regval;

	regval = TEMPSEN_GET(sta_tempsen_intr_raw);
	TEMPSEN_SET(sta_tempsen_intr_clr, regval);

	TEMPSEN_SET(clr_tempsen_ch0_max_result, 1);
	TEMPSEN_SET(clr_tempsen_ch1_max_result, 1);

	TEMPSEN_SET(reg_tempsen_chopsel, 0x3);
	TEMPSEN_SET(reg_tempsen_accsel, 0x2);
	TEMPSEN_SET(reg_tempsen_cyc_clkdiv, 0x31);
	TEMPSEN_SET(reg_tempsen_auto_cycle, 0x100000);

	TEMPSEN_SET(reg_tempsen_sel, 0x1);
	TEMPSEN_SET(reg_tempsen_en, 1);
}

unsigned int read_temp(void)
{
	unsigned int temp, result;

	while (!(TEMPSEN_GET(sta_tempsen_intr_raw) & CHAN0))
		__asm__ volatile("nop");

	temp = TEMPSEN_GET(sta_tempsen_ch0_result);
	result = (temp * 1000) * 716 / 2048 - 273000;
	return result;
}
