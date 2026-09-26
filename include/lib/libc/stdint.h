/*
 * Copyright (c) 2012-2017 Roberto E. Vargas Caballero
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*
 * Portions copyright (c) 2018-2019, Arm Limited and Contributors.
 * All rights reserved.
 */

#ifndef STDINT_H
#define STDINT_H

#include <limits.h>
#include <stdint_.h>

#ifndef INT8_MAX
#define INT8_MAX  CHAR_MAX
#define INT8_MIN  CHAR_MIN
#define UINT8_MAX UCHAR_MAX
#endif

#ifndef INT16_MAX
#define INT16_MAX  SHRT_MAX
#define INT16_MIN  SHRT_MIN
#define UINT16_MAX USHRT_MAX
#endif

#ifndef INT32_MAX
#define INT32_MAX  INT_MAX
#define INT32_MIN  INT_MIN
#define UINT32_MAX UINT_MAX
#endif

#ifndef INT_LEAST8_MIN
#define INT_LEAST8_MIN  INT8_MIN
#define INT_LEAST8_MAX  INT8_MAX
#define UINT_LEAST8_MAX UINT8_MAX
#endif

#ifndef INT_LEAST16_MIN
#define INT_LEAST16_MIN  INT16_MIN
#define INT_LEAST16_MAX  INT16_MAX
#define UINT_LEAST16_MAX UINT16_MAX
#endif

#ifndef INT_LEAST32_MIN
#define INT_LEAST32_MIN  INT32_MIN
#define INT_LEAST32_MAX  INT32_MAX
#define UINT_LEAST32_MAX UINT32_MAX
#endif

#ifndef INT_LEAST64_MIN
#define INT_LEAST64_MIN  INT64_MIN
#define INT_LEAST64_MAX  INT64_MAX
#define UINT_LEAST64_MAX UINT64_MAX
#endif

#ifndef INT_FAST8_MIN
#define INT_FAST8_MIN  INT32_MIN
#define INT_FAST8_MAX  INT32_MAX
#define UINT_FAST8_MAX UINT32_MAX
#endif

#ifndef INT_FAST16_MIN
#define INT_FAST16_MIN  INT32_MIN
#define INT_FAST16_MAX  INT32_MAX
#define UINT_FAST16_MAX UINT32_MAX
#endif

#ifndef INT_FAST32_MIN
#define INT_FAST32_MIN  INT32_MIN
#define INT_FAST32_MAX  INT32_MAX
#define UINT_FAST32_MAX UINT32_MAX
#endif

#ifndef INT_FAST64_MIN
#define INT_FAST64_MIN  INT64_MIN
#define INT_FAST64_MAX  INT64_MAX
#define UINT_FAST64_MAX UINT64_MAX
#endif

#ifndef INTPTR_MIN
#define INTPTR_MIN  LONG_MIN
#define INTPTR_MAX  LONG_MAX
#define UINTPTR_MAX ULONG_MAX
#endif

#ifndef INTMAX_MIN
#define INTMAX_MIN  LLONG_MIN
#define INTMAX_MAX  LLONG_MAX
#define UINTMAX_MAX ULLONG_MAX
#endif

#ifndef PTRDIFF_MIN
#define PTRDIFF_MIN LONG_MIN
#define PTRDIFF_MAX LONG_MAX
#endif

#ifndef SIZE_MAX
#define SIZE_MAX ULONG_MAX
#endif

#ifndef INT8_C
#define INT8_C(x)  x
#define INT16_C(x) x
#define INT32_C(x) x
#endif

#ifndef UINT8_C
#define UINT8_C(x)  x
#define UINT16_C(x) x
#define UINT32_C(x) x ## U
#endif

#ifndef INTMAX_C
#define INTMAX_C(x)  x ## LL
#define UINTMAX_C(x) x ## ULL
#endif

typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef signed char int8_least_t;
typedef short int16_least_t;
typedef int int32_least_t;

typedef unsigned char uint8_least_t;
typedef unsigned short uint16_least_t;
typedef unsigned int uint32_least_t;

typedef int int8_fast_t;
typedef int int16_fast_t;
typedef int int32_fast_t;

typedef unsigned int uint8_fast_t;
typedef unsigned int uint16_fast_t;
typedef unsigned int uint32_fast_t;

typedef long intptr_t;
typedef unsigned long uintptr_t;

/*
* Conceptually, these are supposed to be the largest integers representable in C,
* but GCC and Clang define them as long long for compatibility.
*/
#ifndef _INTMAX_T_DECLARED
typedef long long intmax_t;
#define _INTMAX_T_DECLARED
#endif
#ifndef _UINTMAX_T_DECLARED
typedef unsigned long long uintmax_t;
#define _UINTMAX_T_DECLARED
#endif

typedef long register_t;
typedef unsigned long u_register_t;

#endif /* STDINT_H */
