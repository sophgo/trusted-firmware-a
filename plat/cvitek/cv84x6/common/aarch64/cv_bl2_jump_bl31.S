cv_disable_mmu_icache_el3:
	mrs	x0, sctlr_el3
	mov	x1, #0x1005
	bic	x0, x0, x1
	msr	sctlr_el3, x0
	isb
	dsb	sy
	ret

.global jump_bl31
jump_bl31:
	mov	x20, x0
	mov	x21, x1

	bl	cv_disable_mmu_icache_el3
	tlbi	alle3

	mov	x0, x21
	mov	x1, xzr
	blr	x20
	ret
