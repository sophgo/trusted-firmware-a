/*
 * Copyright (c) 2016-2017, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __CV_UTILS_DEF_H__
#define __CV_UTILS_DEF_H__

#include <lib/utils_def.h>

/* Compute the number of elements in the given array */
#define ARRAY_SIZE(a)					(sizeof(a) / sizeof((a)[0]))

#define IS_POWER_OF_TWO(x)				(((x) & ((x) - 1)) == 0)

// Get a bit field from a value
#define GET_FIELD(var, mask, shift) (((var) >> (shift)) & (mask))

#define PTR_INC(base, offset) (void *)((uint8_t *)(base) + (offset))
#define PTR_DEC(base, offset) (void *)((uint8_t *)(base) - (offset))
#define ROUND_UP(divident, divisor) ((((divident) + (divisor) - 1) / (divisor)) * (divisor))
#define ROUND_DOWN(divident, divisor) (((divident) / (divisor)) * (divisor))

#define DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))
#define DIV_ROUND_CLOSEST(x, divisor)(			{								typeof(x) __x = x;					typeof(divisor) __d = divisor;				(((typeof(x))-1) > 0 ||					 ((typeof(divisor))-1) > 0 ||				 (((__x) > 0) == ((__d) > 0))) ?				(((__x) + ((__d) / 2)) / (__d)) :			(((__x) - ((__d) / 2)) / (__d));	}							)

#endif /* __CV_UTILS_DEF_H__ */
