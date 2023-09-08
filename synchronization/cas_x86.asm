	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 13, 0	sdk_version 13, 3
	.globl	__compare_and_swap              ## -- Begin function _compare_and_swap
	.p2align	4, 0x90
__compare_and_swap:                     ## @_compare_and_swap
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-8(%rbp), %rcx
	movq	-16(%rbp), %rax
	movq	-24(%rbp), %rdx
	lock		cmpxchgq	%rdx, (%rcx)
	movq	%rax, %rcx
	sete	%al
	andb	$1, %al
	movzbl	%al, %eax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
.subsections_via_symbols
