#include <drivers/console.h>

void plat_crash_console_flush(void)
{
	console_flush();
}
