#include <decompress.h>

#include <debug.h>
#include <errno.h>
#include <string.h>

#include "lzma/LzmaDec.h"

static uintptr_t comp_alloc_buf;
static size_t comp_alloc_size;

static void sz_init(void)
{
	comp_alloc_buf = DECOMP_ALLOC_ADDR;
	comp_alloc_size = DECOMP_ALLOC_SIZE;
}

static void *sz_alloc(ISzAllocPtr p, size_t size)
{
	void *x = (void *)comp_alloc_buf;
	(void)p;

	if ((size == 0U) || (size > comp_alloc_size)) {
		ERROR("%s: alloc %lu failed, remain %lu\n", __func__,
		      (unsigned long)size, (unsigned long)comp_alloc_size);
		return NULL;
	}

	comp_alloc_buf += size;
	comp_alloc_size -= size;
	return x;
}

static void sz_free(ISzAllocPtr p, void *address)
{
	(void)p;
	(void)address;
}

static int decompress_lzma(void *dst, size_t *dst_size, const void *src, size_t src_size)
{
	size_t uncomp_size;
	uint32_t tmp;
	int ret;
	ISzAlloc alloc = { sz_alloc, sz_free };
	ELzmaStatus status = 0;
	const uint8_t *prop;
	const uint8_t *payload;
	size_t payload_size;

	if ((dst == NULL) || (dst_size == NULL) || (src == NULL)) {
		return -EINVAL;
	}
	if (src_size <= (LZMA_PROPS_SIZE + 8U)) {
		return -EINVAL;
	}

	sz_init();
	prop = (const uint8_t *)src;
	payload = (const uint8_t *)src + LZMA_PROPS_SIZE;
	payload_size = src_size - LZMA_PROPS_SIZE;

	memcpy(&tmp, payload, sizeof(tmp));
	uncomp_size = tmp;

	payload += 8;
	payload_size -= 8;

	ret = LzmaDecode((uint8_t *)dst, &uncomp_size,
			 payload, &payload_size,
			 prop, LZMA_PROPS_SIZE,
			 LZMA_FINISH_END, &status, &alloc);
	if (ret != SZ_OK) {
		ERROR("LzmaDecode failed: ret=%d status=%d\n", ret, status);
		return -EFAULT;
	}

	*dst_size = uncomp_size;
	return 0;
}

int decompress(void *dst, size_t *dst_size, const void *src, size_t src_size,
	       enum COMPRESS_TYPE type)
{
	if (type == COMP_NONE) {
		if ((dst == NULL) || (dst_size == NULL) || (src == NULL))
			return -EINVAL;
		if (*dst_size < src_size)
			return -ENOMEM;
		memcpy(dst, src, src_size);
		*dst_size = src_size;
		return 0;
	}
	if (type == COMP_LZMA) {
		return decompress_lzma(dst, dst_size, src, src_size);
	}
	ERROR("Unsupported compress type: %d\n", type);
	return -ENOTSUP;
}
