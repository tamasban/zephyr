/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <errno.h>
#include <kernel.h>
#include <device.h>
#include <init.h>
#include <soc.h>
#include <flash.h>
#include <string.h>

struct flash_mps2_dev_config {
	u32_t base;
	u32_t size;
};

#define FLASH_MPS2_SIZE 0x00400000

static const struct flash_mps2_dev_config flash_device_config = {
	.base = (u32_t)CONFIG_FLASH_BASE_ADDRESS,
	.size = (u32_t)FLASH_MPS2_SIZE
};

struct flash_mps2_dev_data {
	/* For future use. */
};

static struct flash_mps2_dev_data flash_device_data = {

};

static int flash_mps2_erase(struct device *dev, off_t offset, size_t size)
{
	const struct flash_mps2_dev_config *flash =
	((const struct flash_mps2_dev_config * const)(dev)->config->config_info);

	u32_t address = flash->base + offset;

	memset((void *)address, 0xff, size);
	return 0;
}

static int flash_mps2_read(struct device *dev, off_t offset,
				void *data, size_t len)
{
	const struct flash_mps2_dev_config *flash =
	((const struct flash_mps2_dev_config * const)(dev)->config->config_info);

	u32_t address = flash->base + offset;

	memcpy(data, (void *)address, len);
	return 0;
}

static int flash_mps2_write(struct device *dev, off_t offset,
				const void *data, size_t len)
{
	const struct flash_mps2_dev_config *flash =
	((const struct flash_mps2_dev_config * const)(dev)->config->config_info);

	u32_t address = flash->base + offset;

	memcpy((void *)address, data, len);
	return 0;
}

static int flash_mps2_protection_set(struct device *dev, bool enable)
{
	/* Do nothing */
	return 0;
}

static int flash_mps2_init(struct device *dev)
{
	/* Do nothing */
	return 0;
}

#if defined(CONFIG_FLASH_PAGE_LAYOUT)
static const struct flash_pages_layout dev_layout = {
	.pages_count = FLASH_MPS2_SIZE / FLASH_ERASE_BLOCK_SIZE,
	.pages_size  = FLASH_ERASE_BLOCK_SIZE,
};

static void flash_mps2_pages_layout(struct device *dev,
					const struct flash_pages_layout **layout,
					size_t *layout_size)
{
	*layout = &dev_layout;
	*layout_size = 1;
}
#endif /* CONFIG_FLASH_PAGE_LAYOUT */

static const struct flash_driver_api flash_mps2_api = {
	.read = flash_mps2_read,
	.write = flash_mps2_write,
	.erase = flash_mps2_erase,
	.write_protection = flash_mps2_protection_set,
	.write_block_size = 1,
#if defined(CONFIG_FLASH_PAGE_LAYOUT)
	 .page_layout = flash_mps2_pages_layout,
#endif
};

DEVICE_AND_API_INIT(flash_mps2, CONFIG_SOC_FLASH_MPS2_DEV_NAME,
			flash_mps2_init,
			&flash_device_data,
			&flash_device_config,
			POST_KERNEL,
			CONFIG_KERNEL_INIT_PRIORITY_DEVICE,
			&flash_mps2_api);

