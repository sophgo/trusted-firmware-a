/*
 * Copyright (c) 2024, Cvitek. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <debug.h>
#include <errno.h>
#include <io_driver.h>
#include <io_storage.h>
#include <io_spi_nand.h>

static int spi_nand_dev_open(const uintptr_t dev_spec, io_dev_info_t **dev_info);
static int spi_nand_dev_close(io_dev_info_t *dev_info);
static int spi_nand_open(io_dev_info_t *dev_info, const uintptr_t spec,
			  io_entity_t *entity);
static int spi_nand_seek(io_entity_t *entity, int mode, signed long long offset);
static int spi_nand_len(io_entity_t *entity, size_t *length);
static int spi_nand_read(io_entity_t *entity, uintptr_t buffer, size_t length,
			  size_t *length_read);
static int spi_nand_close(io_entity_t *entity);

static const io_dev_connector_t spi_nand_dev_connector = {
	.dev_open = spi_nand_dev_open
};

static const io_dev_funcs_t spi_nand_dev_funcs = {
	.type		= NULL,
	.open		= spi_nand_open,
	.seek		= spi_nand_seek,
	.size		= spi_nand_len,
	.read		= spi_nand_read,
	.write		= NULL,
	.close		= spi_nand_close,
	.dev_init	= NULL,
	.dev_close	= spi_nand_dev_close,
};

static const io_dev_info_t spi_nand_dev_info = {
	.funcs = &spi_nand_dev_funcs,
	.info = (uintptr_t)NULL
};

static int spi_nand_open(io_dev_info_t *dev_info, const uintptr_t spec,
			  io_entity_t *entity)
{
	return -ENOTSUP;
}

static int spi_nand_seek(io_entity_t *entity, int mode, signed long long offset)
{
	return -ENOTSUP;
}

static int spi_nand_len(io_entity_t *entity, size_t *length)
{
	return -ENOTSUP;
}

static int spi_nand_read(io_entity_t *entity, uintptr_t buffer, size_t length,
			  size_t *length_read)
{
	return -ENOTSUP;
}

static int spi_nand_close(io_entity_t *entity)
{
	return 0;
}

static int spi_nand_dev_open(const uintptr_t dev_spec, io_dev_info_t **dev_info)
{
	assert(dev_info != NULL);

	*dev_info = (io_dev_info_t *)&spi_nand_dev_info;

	return 0;
}

static int spi_nand_dev_close(io_dev_info_t *dev_info)
{
	assert(dev_info != NULL);

	return 0;
}

/* Exported functions */

int register_io_dev_spi_nand(const io_dev_connector_t **dev_con)
{
	int result;

	assert(dev_con != NULL);

	result = io_register_device(&spi_nand_dev_info);
	if (result == 0)
		*dev_con = &spi_nand_dev_connector;

	return result;
}