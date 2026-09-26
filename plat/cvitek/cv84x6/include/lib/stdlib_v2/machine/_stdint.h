/*-
 * Copyright (c) 2001, 2002 Mike Barcroft <mike@FreeBSD.org>
 * Copyright (c) 2001 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Klaus Klein.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * $FreeBSD$
 */

/*
 * Portions copyright (c) 2016-2017, ARM Limited and Contributors.
 * All rights reserved.
 */

#ifndef	_MACHINE__STDINT_H_
#define	_MACHINE__STDINT_H_

#if !defined(__cplusplus) || defined(__STDC_CONSTANT_MACROS)

#ifndef INT8_C
#define	INT8_C(c)		(c)
#endif
#ifndef INT16_C
#define	INT16_C(c)		(c)
#endif
#ifndef INT32_C
#define	INT32_C(c)		(c)
#endif
#ifndef INT64_C
#define	INT64_C(c)		(c ## LL)
#endif

#ifndef UINT8_C
#define	UINT8_C(c)		(c)
#endif
#ifndef UINT16_C
#define	UINT16_C(c)		(c)
#endif
#ifndef UINT32_C
#define	UINT32_C(c)		(c ## U)
#endif
#ifndef UINT64_C
#define	UINT64_C(c)		(c ## ULL)
#endif

#ifndef INTMAX_C
#define	INTMAX_C(c)		INT64_C(c)
#endif
#ifndef UINTMAX_C
#define	UINTMAX_C(c)		UINT64_C(c)
#endif

#endif /* !defined(__cplusplus) || defined(__STDC_CONSTANT_MACROS) */

#if !defined(__cplusplus) || defined(__STDC_LIMIT_MACROS)

/*
 * ISO/IEC 9899:1999
 * 7.18.2.1 Limits of exact-width integer types
 */
/* Minimum values of exact-width signed integer types. */
#ifndef INT8_MIN
#define	INT8_MIN	(-0x7f-1)
#endif
#ifndef INT16_MIN
#define	INT16_MIN	(-0x7fff-1)
#endif
#ifndef INT32_MIN
#define	INT32_MIN	(-0x7fffffff-1)
#endif
#ifndef INT64_MIN
#define	INT64_MIN	(-0x7fffffffffffffffLL-1)
#endif

/* Maximum values of exact-width signed integer types. */
#ifndef INT8_MAX
#define	INT8_MAX	0x7f
#endif
#ifndef INT16_MAX
#define	INT16_MAX	0x7fff
#endif
#ifndef INT32_MAX
#define	INT32_MAX	0x7fffffff
#endif
#ifndef INT64_MAX
#define	INT64_MAX	0x7fffffffffffffffLL
#endif

/* Maximum values of exact-width unsigned integer types. */
#ifndef UINT8_MAX
#define	UINT8_MAX	0xff
#endif
#ifndef UINT16_MAX
#define	UINT16_MAX	0xffff
#endif
#ifndef UINT32_MAX
#define	UINT32_MAX	0xffffffffU
#endif
#ifndef UINT64_MAX
#define	UINT64_MAX	0xffffffffffffffffULL
#endif

/*
 * ISO/IEC 9899:1999
 * 7.18.2.2  Limits of minimum-width integer types
 */
/* Minimum values of minimum-width signed integer types. */
#ifndef INT_LEAST8_MIN
#define	INT_LEAST8_MIN	INT8_MIN
#endif
#ifndef INT_LEAST16_MIN
#define	INT_LEAST16_MIN	INT16_MIN
#endif
#ifndef INT_LEAST32_MIN
#define	INT_LEAST32_MIN	INT32_MIN
#endif
#ifndef INT_LEAST64_MIN
#define	INT_LEAST64_MIN	INT64_MIN
#endif

/* Maximum values of minimum-width signed integer types. */
#ifndef INT_LEAST8_MAX
#define	INT_LEAST8_MAX	INT8_MAX
#endif
#ifndef INT_LEAST16_MAX
#define	INT_LEAST16_MAX	INT16_MAX
#endif
#ifndef INT_LEAST32_MAX
#define	INT_LEAST32_MAX	INT32_MAX
#endif
#ifndef INT_LEAST64_MAX
#define	INT_LEAST64_MAX	INT64_MAX
#endif

/* Maximum values of minimum-width unsigned integer types. */
#ifndef UINT_LEAST8_MAX
#define	UINT_LEAST8_MAX	 UINT8_MAX
#endif
#ifndef UINT_LEAST16_MAX
#define	UINT_LEAST16_MAX UINT16_MAX
#endif
#ifndef UINT_LEAST32_MAX
#define	UINT_LEAST32_MAX UINT32_MAX
#endif
#ifndef UINT_LEAST64_MAX
#define	UINT_LEAST64_MAX UINT64_MAX
#endif

/*
 * ISO/IEC 9899:1999
 * 7.18.2.3  Limits of fastest minimum-width integer types
 */
/* Minimum values of fastest minimum-width signed integer types. */
#ifndef INT_FAST8_MIN
#define	INT_FAST8_MIN	INT32_MIN
#endif
#ifndef INT_FAST16_MIN
#define	INT_FAST16_MIN	INT32_MIN
#endif
#ifndef INT_FAST32_MIN
#define	INT_FAST32_MIN	INT32_MIN
#endif
#ifndef INT_FAST64_MIN
#define	INT_FAST64_MIN	INT64_MIN
#endif

/* Maximum values of fastest minimum-width signed integer types. */
#ifndef INT_FAST8_MAX
#define	INT_FAST8_MAX	INT32_MAX
#endif
#ifndef INT_FAST16_MAX
#define	INT_FAST16_MAX	INT32_MAX
#endif
#ifndef INT_FAST32_MAX
#define	INT_FAST32_MAX	INT32_MAX
#endif
#ifndef INT_FAST64_MAX
#define	INT_FAST64_MAX	INT64_MAX
#endif

/* Maximum values of fastest minimum-width unsigned integer types. */
#ifndef UINT_FAST8_MAX
#define	UINT_FAST8_MAX	UINT32_MAX
#endif
#ifndef UINT_FAST16_MAX
#define	UINT_FAST16_MAX	UINT32_MAX
#endif
#ifndef UINT_FAST32_MAX
#define	UINT_FAST32_MAX	UINT32_MAX
#endif
#ifndef UINT_FAST64_MAX
#define	UINT_FAST64_MAX	UINT64_MAX
#endif

/*
 * ISO/IEC 9899:1999
 * 7.18.2.4  Limits of integer types capable of holding object pointers
 */
#ifdef AARCH32
#ifndef INTPTR_MIN
#define	INTPTR_MIN	INT32_MIN
#define	INTPTR_MAX	INT32_MAX
#define	UINTPTR_MAX	UINT32_MAX
#endif
#else
#ifndef INTPTR_MIN
#define	INTPTR_MIN	INT64_MIN
#define	INTPTR_MAX	INT64_MAX
#define	UINTPTR_MAX	UINT64_MAX
#endif
#endif

/*
 * ISO/IEC 9899:1999
 * 7.18.2.5  Limits of greatest-width integer types
 */
#ifndef INTMAX_MIN
#define	INTMAX_MIN	INT64_MIN
#define	INTMAX_MAX	INT64_MAX
#define	UINTMAX_MAX	UINT64_MAX
#endif

/*
 * ISO/IEC 9899:1999
 * 7.18.3  Limits of other integer types
 */
/* Limits of ptrdiff_t. */
#ifdef AARCH32
#ifndef PTRDIFF_MIN
#define	PTRDIFF_MIN	INT32_MIN
#define	PTRDIFF_MAX	INT32_MAX
#endif
#else
#ifndef PTRDIFF_MIN
#define	PTRDIFF_MIN	INT64_MIN
#define	PTRDIFF_MAX	INT64_MAX
#endif
#endif

/* Limits of sig_atomic_t. */
#ifndef SIG_ATOMIC_MIN
#define	SIG_ATOMIC_MIN	INT32_MIN
#define	SIG_ATOMIC_MAX	INT32_MAX
#endif

/* Limit of size_t. */
#ifndef SIZE_MAX
#ifdef AARCH32
#define	SIZE_MAX	UINT32_MAX
#else
#define	SIZE_MAX	UINT64_MAX
#endif
#endif

#ifndef WCHAR_MIN /* Also possibly defined in <wchar.h> */
/* Limits of wchar_t. */
#define	WCHAR_MIN	INT32_MIN
#define	WCHAR_MAX	INT32_MAX
#endif

/* Limits of wint_t. */
#define	WINT_MIN	INT32_MIN
#define	WINT_MAX	INT32_MAX

#endif /* !defined(__cplusplus) || defined(__STDC_LIMIT_MACROS) */

#endif /* !_MACHINE__STDINT_H_ */
