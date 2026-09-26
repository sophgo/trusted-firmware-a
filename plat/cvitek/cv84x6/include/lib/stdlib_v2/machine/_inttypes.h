/*
 * Copyright (c) 2015, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MACHINE_INTTYPES_H_
#define _MACHINE_INTTYPES_H_

/* Format macros for inttypes.h - required by TF-A */

/* fprintf macros for signed integers */
#define PRId8		"d"
#define PRId16		"d"
#define PRId32		"d"
#define PRId64		"ld"

#define PRIdLEAST8	"d"
#define PRIdLEAST16	"d"
#define PRIdLEAST32	"d"
#define PRIdLEAST64	"ld"

#define PRIdFAST8	"d"
#define PRIdFAST16	"d"
#define PRIdFAST32	"d"
#define PRIdFAST64	"ld"

#define PRIdMAX		"ld"
#define PRIdPTR		"ld"

/* fprintf macros for unsigned integers */
#define PRIo8		"o"
#define PRIo16		"o"
#define PRIo32		"o"
#define PRIo64		"lo"

#define PRIoLEAST8	"o"
#define PRIoLEAST16	"o"
#define PRIoLEAST32	"o"
#define PRIoLEAST64	"lo"

#define PRIoFAST8	"o"
#define PRIoFAST16	"o"
#define PRIoFAST32	"o"
#define PRIoFAST64	"lo"

#define PRIoMAX		"lo"
#define PRIoPTR		"lo"

#define PRIu8		"u"
#define PRIu16		"u"
#define PRIu32		"u"
#define PRIu64		"lu"

#define PRIuLEAST8	"u"
#define PRIuLEAST16	"u"
#define PRIuLEAST32	"u"
#define PRIuLEAST64	"lu"

#define PRIuFAST8	"u"
#define PRIuFAST16	"u"
#define PRIuFAST32	"u"
#define PRIuFAST64	"lu"

#define PRIuMAX		"lu"
#define PRIuPTR		"lu"

#define PRIx8		"x"
#define PRIx16		"x"
#define PRIx32		"x"
#define PRIx64		"lx"

#define PRIxLEAST8	"x"
#define PRIxLEAST16	"x"
#define PRIxLEAST32	"x"
#define PRIxLEAST64	"lx"

#define PRIxFAST8	"x"
#define PRIxFAST16	"x"
#define PRIxFAST32	"x"
#define PRIxFAST64	"lx"

#define PRIxMAX		"lx"
#define PRIxPTR		"lx"

#define PRIX8		"X"
#define PRIX16		"X"
#define PRIX32		"X"
#define PRIX64		"lX"

#define PRIXLEAST8	"X"
#define PRIXLEAST16	"X"
#define PRIXLEAST32	"X"
#define PRIXLEAST64	"lX"

#define PRIXFAST8	"X"
#define PRIXFAST16	"X"
#define PRIXFAST32	"X"
#define PRIXFAST64	"lX"

#define PRIXMAX		"lX"
#define PRIXPTR		"lX"

/* fscanf macros for signed integers */
#define SCNd8		"hhd"
#define SCNd16		"hd"
#define SCNd32		"d"
#define SCNd64		"ld"

#define SCNdLEAST8	"hhd"
#define SCNdLEAST16	"hd"
#define SCNdLEAST32	"d"
#define SCNdLEAST64	"ld"

#define SCNdFAST8	"hhd"
#define SCNdFAST16	"hd"
#define SCNdFAST32	"d"
#define SCNdFAST64	"ld"

#define SCNdMAX		"ld"
#define SCNdPTR		"ld"

/* fscanf macros for unsigned integers */
#define SCNo8		"hho"
#define SCNo16		"ho"
#define SCNo32		"o"
#define SCNo64		"lo"

#define SCNoLEAST8	"hho"
#define SCNoLEAST16	"ho"
#define SCNoLEAST32	"o"
#define SCNoLEAST64	"lo"

#define SCNoFAST8	"hho"
#define SCNoFAST16	"ho"
#define SCNoFAST32	"o"
#define SCNoFAST64	"lo"

#define SCNoMAX		"lo"
#define SCNoPTR		"lo"

#define SCNu8		"hhu"
#define SCNu16		"hu"
#define SCNu32		"u"
#define SCNu64		"lu"

#define SCNuLEAST8	"hhu"
#define SCNuLEAST16	"hu"
#define SCNuLEAST32	"u"
#define SCNuLEAST64	"lu"

#define SCNuFAST8	"hhu"
#define SCNuFAST16	"hu"
#define SCNuFAST32	"u"
#define SCNuFAST64	"lu"

#define SCNuMAX		"lu"
#define SCNuPTR		"lu"

#define SCNx8		"hhx"
#define SCNx16		"hx"
#define SCNx32		"x"
#define SCNx64		"lx"

#define SCNxLEAST8	"hhx"
#define SCNxLEAST16	"hx"
#define SCNxLEAST32	"x"
#define SCNxLEAST64	"lx"

#define SCNxFAST8	"hhx"
#define SCNxFAST16	"hx"
#define SCNxFAST32	"x"
#define SCNxFAST64	"lx"

#define SCNxMAX		"lx"
#define SCNxPTR		"lx"

#endif /* !_MACHINE_INTTYPES_H_ */
