/*
 * BL2 links plat/common platform_helpers which branch to report_unhandled_exception;
 * BL31 provides the full implementation via crash_reporting.S — provide a minimal stub.
 */
#include <asm_macros.S>

	.globl	report_unhandled_exception

func report_unhandled_exception
report_unhandled_interrupt:
	no_ret	plat_panic_handler
endfunc report_unhandled_exception
