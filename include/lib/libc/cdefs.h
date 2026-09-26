/*
 * Copyright (c) 2018-2025, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef CDEFS_H
#define CDEFS_H

#ifndef __dead2
#define __dead2		__attribute__((__noreturn__))
#endif
#ifndef __deprecated
#define __deprecated	__attribute__((__deprecated__))
#endif
#ifndef __packed
#define __packed	__attribute__((__packed__))
#endif
#ifndef __used
#define __used		__attribute__((__used__))
#endif
#ifndef __maybe_unused
#define __maybe_unused	__attribute__((__unused__))
#endif
#ifndef __unused
#define __unused	__attribute__((__unused__))
#endif
#ifndef __aligned
#define __aligned(x)	__attribute__((__aligned__(x)))
#endif
#ifndef __section
#define __section(x)	__attribute__((__section__(x)))
#endif
#ifndef __fallthrough
#define __fallthrough	__attribute__((__fallthrough__))
#endif
#ifndef __noinline
#define __noinline	__attribute__((__noinline__))
#endif
#ifndef __pure
#define __pure		__attribute__((__pure__))
#endif
#if ENABLE_PAUTH
#define __no_pauth	__attribute__((target("branch-protection=none")))
#else
#define __no_pauth
#endif
#if RECLAIM_INIT_CODE
/*
 * Add each function to a section that is unique so the functions can still
 * be garbage collected.
 *
 * NOTICE: for this to work, these functions will NOT be inlined.
 * TODO: the noinline attribute can be removed if RECLAIM_INIT_CODE is made
 * platform agnostic and called after bl31_main(). Then, top-level functions
 * (those that can't be inlined like bl31_main()) can be annotated with __init
 * and noinline can be removed.
 */
#define __init		__section(".text.init." __FILE__ "." __XSTRING(__LINE__)) __noinline
#else
#define __init
#endif

#define __printflike(fmtarg, firstvararg) \
		__attribute__((__format__ (__printf__, fmtarg, firstvararg)))

#ifndef __weak_reference
#define __weak_reference(sym, alias)	\
	__asm__(".weak alias");		\
	__asm__(".equ alias, sym")
#endif

#define __STRING(x)	#x
#define __XSTRING(x)	__STRING(x)

#ifndef __predict_true
#define __predict_true(exp)     (exp)
#endif
#ifndef __predict_false
#define __predict_false(exp)    (exp)
#endif

#endif /* CDEFS_H */
