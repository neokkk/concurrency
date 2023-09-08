	.arch armv8-a
	.file	"cas.c"
	.text
	.global	__aarch64_cas8_acq_rel
	.align	2
	.global	_compare_and_swap
	.type	_compare_and_swap, %function
_compare_and_swap:
.LFB0:
	.cfi_startproc
	stp	x29, x30, [sp, -64]!
	.cfi_def_cfa_offset 64
	.cfi_offset 29, -64
	.cfi_offset 30, -56
	mov	x29, sp
	str	x19, [sp, 16]
	.cfi_offset 19, -48
	str	x0, [sp, 56]
	str	x1, [sp, 48]
	str	x2, [sp, 40]
	ldr	x1, [sp, 56]
	ldr	x19, [sp, 48]
	ldr	x0, [sp, 40]
	mov	x2, x1
	mov	x1, x0
	mov	x0, x19
	bl	__aarch64_cas8_acq_rel
	cmp	x0, x19
	cset	w0, eq
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 64
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 19
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE0:
	.size	_compare_and_swap, .-_compare_and_swap
	.ident	"GCC: (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0"
	.section	.note.GNU-stack,"",@progbits
