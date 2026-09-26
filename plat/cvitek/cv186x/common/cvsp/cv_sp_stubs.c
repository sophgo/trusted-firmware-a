#include <errno.h>
#include <stddef.h>
#include <stdint.h>

#include <arch_helpers.h>
#include <delay_timer.h>
#include <mmio.h>
#include <string.h>

#include "cv_efuse.h"
#include "cv_private.h"

static struct {
	uint32_t start;
	uint32_t size;
	uint32_t wlock_shift;
	uint32_t rlock_shift;
} efuse_lock_map[] = {
	[EFUSE_LOCK_KPUB0] = {0xA8, 32, 0, 8},
	[EFUSE_LOCK_DEBUG_PW] = {0xC8, 16, 2, 10},
	[EFUSE_LOCK_LOADER_EK] = {0xD8, 16, 4, 12},
	[EFUSE_LOCK_DEVICE_EK] = {0xE8, 16, 6, 14}
};

static int plat_efuse_is_not_secret(uint32_t addr)
{
	uint32_t lock_value = mmio_read_32(SEC_EFUSE_SHADOW_REG + EFUSE_LOCK_ADDR);
	int i;

	if (addr == EFUSE_LOCK_ADDR)
		return 1;

	for (i = 0; i < (int)(sizeof(efuse_lock_map) / sizeof(efuse_lock_map[0])); i++) {
		uint32_t start = efuse_lock_map[i].start;
		uint32_t end = efuse_lock_map[i].start + efuse_lock_map[i].size;
		uint32_t lock = (lock_value >> efuse_lock_map[i].rlock_shift) & 0x3;

		if (addr >= start && addr < end && !lock)
			return 1;
	}

	if (addr < EFUSE_SECRET_START_OFFSET)
		return 1;
	if (addr >= EFUSE_SECRET_END_OFFSET)
		return 1;

	return 0;
}

int plat_cryptodma_exec(uintptr_t src, uintptr_t dst, uint64_t len, spacc_exec_config *config)
{
	E_MODE mode;
	E_KEY_MODE key_mode;

	if (config == NULL)
		return -1;

	switch (config->mode) {
	case AES_ECB:
		mode = ECB;
		break;
	case AES_CBC:
		mode = CBC;
		break;
	case AES_CTR:
		mode = CTR;
		break;
	default:
		return -1;
	}

	switch (config->key_mode) {
	case AES_128BIT:
		key_mode = KEY_128BITS;
		break;
	case AES_192BIT:
		key_mode = KEY_192BITS;
		break;
	case AES_256BIT:
		key_mode = KEY_256BITS;
		break;
	default:
		return -1;
	}

	return plat_cryptodma_do(config->action == ENCRYPTION, src, dst, len,
				 (unsigned char *)config->key, key_mode,
				 (unsigned char *)config->iv, AES, mode, NULL, config->otp);
}

int plat_cryptodma_do(int is_encrypt, uintptr_t in, uintptr_t out, uint64_t len,
		      unsigned char *key, E_KEY_MODE key_mode, unsigned char *iv,
		      E_ALGO algo, E_MODE mode, uint32_t *state, CRYPTODMA_KEY_SOURCE_E otp)
{
	uint32_t data;
	uint32_t i;
	uint32_t des_ctrl = 0xF;
	uint32_t dma_descriptor[32] __attribute__((aligned(64))) = {0};

	if (in == 0 || len == 0)
		return -1;
	if (algo != SHA256 && algo != SHA1 && algo != SM3 && out == 0)
		return -1;

	switch (algo) {
	case AES:
		des_ctrl |= DES_USE_DESCRIPTOR_IV | DES_USE_AES | IV_OUT0_SELECT;
		des_ctrl |= (otp == CRYPTODMA_KEY_SOURCE_OTP) ? OTP_KEY_SEL : DES_USE_DESCRIPTOR_KEY;
		dma_descriptor[CRYPTODMA_CTRL] = des_ctrl;
		if (mode == CBC)
			dma_descriptor[CRYPTODMA_CIPHER] = CBC_ENABLE << 1;
		else if (mode == CTR)
			dma_descriptor[CRYPTODMA_CIPHER] = 0x1 << 2;
		else if (mode != ECB)
			return -1;

		if (key_mode == KEY_128BITS)
			dma_descriptor[CRYPTODMA_CIPHER] |= (0x4 << 3);
		else if (key_mode == KEY_192BITS)
			dma_descriptor[CRYPTODMA_CIPHER] |= (0x2 << 3);
		else if (key_mode == KEY_256BITS)
			dma_descriptor[CRYPTODMA_CIPHER] |= (0x1 << 3);
		break;
	case SM3:
		dma_descriptor[CRYPTODMA_CTRL] = des_ctrl | DES_USE_SM3 | DES_USE_DESCRIPTOR_KEY | DES_USE_DESCRIPTOR_IV;
		dma_descriptor[CRYPTODMA_CIPHER] = 0x1;
		break;
	case SM4:
		des_ctrl |= DES_USE_DESCRIPTOR_IV | DES_USE_SM4 | IV_OUT0_SELECT;
		des_ctrl |= (otp == CRYPTODMA_KEY_SOURCE_OTP) ? OTP_KEY_SEL : DES_USE_DESCRIPTOR_KEY;
		dma_descriptor[CRYPTODMA_CTRL] = des_ctrl;
		if (mode == CBC)
			dma_descriptor[CRYPTODMA_CIPHER] = CBC_ENABLE << 1;
		else if (mode == CTR)
			dma_descriptor[CRYPTODMA_CIPHER] = 0x1 << 2;
		else if (mode == OFB)
			dma_descriptor[CRYPTODMA_CIPHER] = 0x1 << 3;
		else if (mode != ECB)
			return -1;
		break;
	case BYPASS:
		dma_descriptor[CRYPTODMA_CTRL] = DES_USE_BYPASS | 0xF;
		break;
	case SHA256:
		dma_descriptor[CRYPTODMA_CIPHER] = (0x1 << 1);
		dma_descriptor[CRYPTODMA_CTRL] = DES_USE_DESCRIPTOR_KEY | DES_USE_SHA | 0xF;
		dma_descriptor[CRYPTODMA_CIPHER] |= 0x1;
		break;
	case SHA1:
		dma_descriptor[CRYPTODMA_CTRL] = DES_USE_DESCRIPTOR_KEY | DES_USE_SHA | 0xF;
		dma_descriptor[CRYPTODMA_CIPHER] = 0x1;
		break;
	case BASE64_CUSTOMER:
	case BASE64:
		dma_descriptor[CRYPTODMA_CTRL] = DES_USE_BASE64 | 0xF;
		dma_descriptor[BASE64_SIZE] = is_encrypt ? ((len + 2) / 3) * 4 : (len / 4) * 3;
		break;
	case TDES:
		dma_descriptor[CRYPTODMA_CIPHER] = (0x1 << 3);
		dma_descriptor[CRYPTODMA_CTRL] = des_ctrl | DES_USE_DESCRIPTOR_IV | DES_USE_DES | DES_USE_DESCRIPTOR_KEY;
		if (mode == CBC)
			dma_descriptor[CRYPTODMA_CIPHER] |= (CBC_ENABLE << 1);
		else if (mode == CTR)
			dma_descriptor[CRYPTODMA_CIPHER] |= (0x1 << 2);
		else if (mode != ECB)
			return -1;
		break;
	case DES:
		dma_descriptor[CRYPTODMA_CTRL] = des_ctrl | DES_USE_DESCRIPTOR_IV | DES_USE_DES | DES_USE_DESCRIPTOR_KEY;
		if (mode == CBC)
			dma_descriptor[CRYPTODMA_CIPHER] |= (CBC_ENABLE << 1);
		else if (mode == CTR)
			dma_descriptor[CRYPTODMA_CIPHER] |= (0x1 << 2);
		else if (mode != ECB)
			return -1;
		break;
	default:
		return -1;
	}

	if (is_encrypt)
		dma_descriptor[CRYPTODMA_CIPHER] |= 0x1;

	dma_descriptor[CRYPTODMA_SRC_ADDR_L] = (uint32_t)(in & 0xFFFFFFFF);
	dma_descriptor[CRYPTODMA_SRC_ADDR_H] = (uint32_t)(in >> 32);
	dma_descriptor[CRYPTODMA_DATA_AMOUNT_L] = (uint32_t)(len & 0xFFFFFFFF);
	dma_descriptor[CRYPTODMA_DATA_AMOUNT_H] = (uint32_t)(len >> 32);

	if (algo != SHA256 && algo != SHA1 && algo != SM3) {
		dma_descriptor[CRYPTODMA_DST_ADDR_L] = (uint32_t)(out & 0xFFFFFFFF);
		dma_descriptor[CRYPTODMA_DST_ADDR_H] = (uint32_t)(out >> 32);
	}

	if (algo == AES || algo == SM4 || algo == DES || algo == TDES) {
		uint32_t key_size = 0;
		if (key_mode == KEY_128BITS)
			key_size = 16;
		else if (key_mode == KEY_192BITS)
			key_size = 24;
		else if (key_mode == KEY_256BITS)
			key_size = 32;
		if (otp != CRYPTODMA_KEY_SOURCE_OTP && key != NULL && key_size)
			memcpy(&dma_descriptor[CRYPTODMA_KEY], key, key_size);
		if (mode != ECB && iv != NULL)
			memcpy(&dma_descriptor[CRYPTODMA_IV], iv, 16);
	} else if (algo == SHA256) {
		for (i = 0; i < 8; i++)
			mmio_write_32(SEC_CRYPTODMA_BASE + CRYPTODMA_SHA_PARA + i * 4, 0x12345678);
		if (state != NULL) {
			for (i = 0; i < 8; i++)
				dma_descriptor[CRYPTODMA_KEY + i] = state[i];
		}
	} else if (algo == SM3) {
		for (i = 0; i < 8; i++)
			mmio_write_32(SEC_CRYPTODMA_BASE + CRYPTODMA_SM3_PARA + i * 4, 0x12345678);
		if (state != NULL) {
			for (i = 0; i < 8; i++)
				dma_descriptor[CRYPTODMA_KEY + i] = state[i];
		}
	} else if (algo == SHA1 && state != NULL) {
		for (i = 0; i < 5; i++)
			mmio_write_32(SEC_CRYPTODMA_BASE + CRYPTODMA_SHA_PARA + i * 4, 0x12345678);
		for (i = 0; i < 5; i++)
			dma_descriptor[CRYPTODMA_KEY + i] = state[i];
	}

	flush_dcache_range((unsigned long)dma_descriptor, sizeof(dma_descriptor));
	flush_dcache_range((unsigned long)in, len);
	mmio_write_32(SEC_CRYPTODMA_BASE + CRYPTODMA_DES_BASE_L, (uint32_t)(uintptr_t)dma_descriptor);
	mmio_write_32(SEC_CRYPTODMA_BASE + CRYPTODMA_DES_BASE_H, (uint32_t)((uint64_t)(uintptr_t)dma_descriptor >> 32));
	mmio_write_32(SEC_CRYPTODMA_BASE + CRYPTODMA_INT_MASK, 0x0);
	mmio_write_32(SEC_CRYPTODMA_BASE + CRYPTODMA_WR_INT, 0x7);
	if (algo == BASE64_CUSTOMER)
		mmio_write_32(SEC_CRYPTODMA_BASE + CRYPTODMA_BASE64_CUSTOMIZE_CODE, ('!' << 8) | '#');
	mmio_write_32(SEC_CRYPTODMA_BASE + CRYPTODMA_DMA_CTRL,
		      DMA_WRITE_MAX_BURST << 24 | DMA_READ_MAX_BURST << 16 |
			      DMA_DESCRIPTOR_MODE << 1 | DMA_ENABLE);

	do {
		data = mmio_read_32(SEC_CRYPTODMA_BASE + CRYPTODMA_WR_INT);
	} while (data == 0);

	if (algo == SHA256 && state != NULL) {
		for (i = 0; i < 8; i++)
			state[i] = mmio_read_32(SEC_CRYPTODMA_BASE + CRYPTODMA_SHA_PARA + i * 4);
		return 32;
	}
	if (algo == SHA1 && state != NULL) {
		for (i = 0; i < 5; i++)
			state[i] = mmio_read_32(SEC_CRYPTODMA_BASE + CRYPTODMA_SHA_PARA + i * 4);
		return 20;
	}
	if (algo == SM3 && state != NULL) {
		for (i = 0; i < 8; i++)
			state[i] = mmio_read_32(SEC_CRYPTODMA_BASE + CRYPTODMA_SM3_PARA + i * 4);
		return 32;
	}

	if (algo != SHA256 && algo != SHA1 && algo != SM3)
		inv_dcache_range(out, len);

	return (int)len;
}

int plat_cryptodma_base64(uintptr_t src, uint64_t len, uintptr_t dst,
			  uint32_t customer_code, uint32_t action)
{
	int ret;
	E_ALGO algo = customer_code ? BASE64_CUSTOMER : BASE64;

	ret = plat_cryptodma_do(action == 1, src, dst, len, NULL, KEY_128BITS,
				NULL, algo, ECB, NULL, CRYPTODMA_KEY_SOURCE_DESCRIPTOR);
	if (ret < 0)
		return ret;
	return action == 1 ? (int)(((len + 2) / 3) * 4) : (int)((len / 4) * 3);
}

int plat_cryptodma_sha256(const void *msg, uint64_t len, uint8_t digest[32])
{
	uint32_t state[8] = {
		0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A,
		0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19
	};
	int ret;

	ret = plat_cryptodma_do(1, (uintptr_t)msg, 0, len, NULL, KEY_128BITS, NULL,
				SHA256, ECB, state, CRYPTODMA_KEY_SOURCE_DESCRIPTOR);
	if (ret < 0)
		return ret;
	memcpy(digest, state, 32);
	return 0;
}

int64_t plat_efuse_read_safe(uint32_t addr)
{
	if (addr >= EFUSE_SIZE || (addr % 4) != 0)
		return -EFAULT;

	if (plat_efuse_is_not_secret(addr))
		return mmio_read_32(SEC_EFUSE_SHADOW_REG + addr);

	return -EFAULT;
}

int plat_efuse_write_safe(uint32_t addr, uint32_t value)
{
	uint32_t ts;

	if (addr >= EFUSE_SIZE || (addr % 4) != 0)
		return -EFAULT;

	if (!plat_efuse_is_not_secret(addr))
		return -EFAULT;

	mmio_write_32(EFUSE_ADR, addr);
	mmio_write_32(EFUSE_ONE_WAY, value);
	mmio_write_32(EFUSE_MODE, BIT_PRG | BIT_CMD);

	ts = get_timer(0);
	while (mmio_read_32(EFUSE_STATUS) & BIT_BUSY) {
		if (get_timer(ts) >= 1000)
			return -EIO;
	}

	return 0;
}

int plat_get_image_source(unsigned int image_id, uintptr_t *dev_handle,
			  uintptr_t *image_spec)
{
	(void)image_id;
	if (dev_handle != NULL)
		*dev_handle = 0;
	if (image_spec != NULL)
		*image_spec = 0;
	return -ENOENT;
}


