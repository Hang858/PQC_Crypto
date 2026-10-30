	.build_version macos, 26, 0	sdk_version 27, 2
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_bit_ntt_init                   ## -- Begin function bit_ntt_init
	.p2align	4
_bit_ntt_init:                          ## @bit_ntt_init
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%rbx
	pushq	%rax
	.cfi_offset %rbx, -24
	leaq	_bit_ntt_roots32(%rip), %rdi
	xorl	%ebx, %ebx
	movl	$2, %esi
	movl	$7681, %edx                     ## imm = 0x1E01
	movl	$-1954291199, %ecx              ## imm = 0x8B83E201
	movl	$1912, %r8d                     ## imm = 0x778
	xorl	%r9d, %r9d
	callq	_OP_ntt256_init
	negl	%eax
	sbbl	%ebx, %ebx
	movl	%ebx, %eax
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__literal16,16byte_literals
	.p2align	4, 0x0                          ## -- Begin function ntt_forward_ws
LCPI1_0:
	.byte	0                               ## 0x0
	.byte	1                               ## 0x1
	.byte	4                               ## 0x4
	.byte	5                               ## 0x5
	.byte	8                               ## 0x8
	.byte	9                               ## 0x9
	.byte	12                              ## 0xc
	.byte	13                              ## 0xd
	.byte	8                               ## 0x8
	.byte	9                               ## 0x9
	.byte	12                              ## 0xc
	.byte	13                              ## 0xd
	.byte	12                              ## 0xc
	.byte	13                              ## 0xd
	.byte	14                              ## 0xe
	.byte	15                              ## 0xf
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_ntt_forward_ws
	.p2align	4
_ntt_forward_ws:                        ## @ntt_forward_ws
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r14
	pushq	%rbx
	.cfi_offset %rbx, -32
	.cfi_offset %r14, -24
	movq	%rdx, %rbx
	movq	%rdi, %r14
	movl	$12, %eax
	.p2align	4
LBB1_1:                                 ## =>This Inner Loop Header: Depth=1
	pmovsxwd	-24(%rsi,%rax,2), %xmm0
	pmovsxwd	-16(%rsi,%rax,2), %xmm1
	movdqu	%xmm0, -48(%rbx,%rax,4)
	movdqu	%xmm1, -32(%rbx,%rax,4)
	pmovsxwd	-8(%rsi,%rax,2), %xmm0
	pmovsxwd	(%rsi,%rax,2), %xmm1
	movdqu	%xmm0, -16(%rbx,%rax,4)
	movdqu	%xmm1, (%rbx,%rax,4)
	addq	$16, %rax
	cmpq	$268, %rax                      ## imm = 0x10C
	jne	LBB1_1
## %bb.2:
	leaq	2048(%rbx), %rdi
	movq	%rbx, %rsi
	xorl	%edx, %edx
	callq	_OP_ntt256
	movl	%eax, %ecx
	movl	$-1, %eax
	testl	%ecx, %ecx
	jne	LBB1_5
## %bb.3:
	xorl	%eax, %eax
	movdqa	LCPI1_0(%rip), %xmm0            ## xmm0 = [0,1,4,5,8,9,12,13,8,9,12,13,12,13,14,15]
	xorl	%ecx, %ecx
	.p2align	4
LBB1_4:                                 ## =>This Inner Loop Header: Depth=1
	movdqu	2048(%rbx,%rcx,4), %xmm1
	movdqu	2064(%rbx,%rcx,4), %xmm2
	pshufb	%xmm0, %xmm1
	pshufb	%xmm0, %xmm2
	punpcklqdq	%xmm2, %xmm1            ## xmm1 = xmm1[0],xmm2[0]
	movdqu	%xmm1, (%r14,%rcx,2)
	movdqu	2080(%rbx,%rcx,4), %xmm1
	movdqu	2096(%rbx,%rcx,4), %xmm2
	pshufb	%xmm0, %xmm1
	pshufb	%xmm0, %xmm2
	punpcklqdq	%xmm2, %xmm1            ## xmm1 = xmm1[0],xmm2[0]
	movdqu	%xmm1, 16(%r14,%rcx,2)
	addq	$16, %rcx
	cmpq	$256, %rcx                      ## imm = 0x100
	jne	LBB1_4
LBB1_5:
                                        ## kill: def $eax killed $eax killed $rax
	popq	%rbx
	popq	%r14
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__literal16,16byte_literals
	.p2align	4, 0x0                          ## -- Begin function ntt_inverse_ws
LCPI2_0:
	.byte	0                               ## 0x0
	.byte	1                               ## 0x1
	.byte	4                               ## 0x4
	.byte	5                               ## 0x5
	.byte	8                               ## 0x8
	.byte	9                               ## 0x9
	.byte	12                              ## 0xc
	.byte	13                              ## 0xd
	.byte	8                               ## 0x8
	.byte	9                               ## 0x9
	.byte	12                              ## 0xc
	.byte	13                              ## 0xd
	.byte	12                              ## 0xc
	.byte	13                              ## 0xd
	.byte	14                              ## 0xe
	.byte	15                              ## 0xf
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_ntt_inverse_ws
	.p2align	4
_ntt_inverse_ws:                        ## @ntt_inverse_ws
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r14
	pushq	%rbx
	.cfi_offset %rbx, -32
	.cfi_offset %r14, -24
	movq	%rdx, %rbx
	movq	%rdi, %r14
	movl	$12, %eax
	.p2align	4
LBB2_1:                                 ## =>This Inner Loop Header: Depth=1
	pmovsxwd	-24(%rsi,%rax,2), %xmm0
	pmovsxwd	-16(%rsi,%rax,2), %xmm1
	movdqu	%xmm0, -48(%rbx,%rax,4)
	movdqu	%xmm1, -32(%rbx,%rax,4)
	pmovsxwd	-8(%rsi,%rax,2), %xmm0
	pmovsxwd	(%rsi,%rax,2), %xmm1
	movdqu	%xmm0, -16(%rbx,%rax,4)
	movdqu	%xmm1, (%rbx,%rax,4)
	addq	$16, %rax
	cmpq	$268, %rax                      ## imm = 0x10C
	jne	LBB2_1
## %bb.2:
	leaq	2048(%rbx), %rdi
	movq	%rbx, %rsi
	movl	$1, %edx
	callq	_OP_intt256
	movl	%eax, %ecx
	movl	$-1, %eax
	testl	%ecx, %ecx
	jne	LBB2_5
## %bb.3:
	xorl	%eax, %eax
	movdqa	LCPI2_0(%rip), %xmm0            ## xmm0 = [0,1,4,5,8,9,12,13,8,9,12,13,12,13,14,15]
	xorl	%ecx, %ecx
	.p2align	4
LBB2_4:                                 ## =>This Inner Loop Header: Depth=1
	movdqu	2048(%rbx,%rcx,4), %xmm1
	movdqu	2064(%rbx,%rcx,4), %xmm2
	pshufb	%xmm0, %xmm1
	pshufb	%xmm0, %xmm2
	punpcklqdq	%xmm2, %xmm1            ## xmm1 = xmm1[0],xmm2[0]
	movdqu	%xmm1, (%r14,%rcx,2)
	movdqu	2080(%rbx,%rcx,4), %xmm1
	movdqu	2096(%rbx,%rcx,4), %xmm2
	pshufb	%xmm0, %xmm1
	pshufb	%xmm0, %xmm2
	punpcklqdq	%xmm2, %xmm1            ## xmm1 = xmm1[0],xmm2[0]
	movdqu	%xmm1, 16(%r14,%rcx,2)
	addq	$16, %rcx
	cmpq	$256, %rcx                      ## imm = 0x100
	jne	LBB2_4
LBB2_5:
                                        ## kill: def $eax killed $eax killed $rax
	popq	%rbx
	popq	%r14
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__literal16,16byte_literals
	.p2align	4, 0x0                          ## -- Begin function ntt_basemul_raw_ws
LCPI3_0:
	.byte	0                               ## 0x0
	.byte	1                               ## 0x1
	.byte	4                               ## 0x4
	.byte	5                               ## 0x5
	.byte	8                               ## 0x8
	.byte	9                               ## 0x9
	.byte	12                              ## 0xc
	.byte	13                              ## 0xd
	.byte	8                               ## 0x8
	.byte	9                               ## 0x9
	.byte	12                              ## 0xc
	.byte	13                              ## 0xd
	.byte	12                              ## 0xc
	.byte	13                              ## 0xd
	.byte	14                              ## 0xe
	.byte	15                              ## 0xf
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_ntt_basemul_raw_ws
	.p2align	4
_ntt_basemul_raw_ws:                    ## @ntt_basemul_raw_ws
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r14
	pushq	%rbx
	.cfi_offset %rbx, -32
	.cfi_offset %r14, -24
	movq	%rcx, %rbx
	movq	%rdx, %rax
	movq	%rdi, %r14
	leaq	1024(%rcx), %rdx
	movl	$4, %ecx
	.p2align	4
LBB3_1:                                 ## =>This Inner Loop Header: Depth=1
	pmovsxwd	-8(%rsi,%rcx,2), %xmm0
	pmovsxwd	(%rsi,%rcx,2), %xmm1
	movdqu	%xmm0, -16(%rbx,%rcx,4)
	movdqu	%xmm1, (%rbx,%rcx,4)
	pmovsxwd	-8(%rax,%rcx,2), %xmm0
	pmovsxwd	(%rax,%rcx,2), %xmm1
	movdqu	%xmm0, 1008(%rbx,%rcx,4)
	movdqu	%xmm1, 1024(%rbx,%rcx,4)
	addq	$8, %rcx
	cmpq	$260, %rcx                      ## imm = 0x104
	jne	LBB3_1
## %bb.2:
	leaq	2048(%rbx), %rdi
	movq	%rbx, %rsi
	movl	$256, %ecx                      ## imm = 0x100
	movl	$7681, %r8d                     ## imm = 0x1E01
	xorl	%r9d, %r9d
	callq	_OP_cwm
	movl	%eax, %ecx
	movl	$-1, %eax
	testl	%ecx, %ecx
	jne	LBB3_5
## %bb.3:
	xorl	%eax, %eax
	movdqa	LCPI3_0(%rip), %xmm0            ## xmm0 = [0,1,4,5,8,9,12,13,8,9,12,13,12,13,14,15]
	xorl	%ecx, %ecx
	.p2align	4
LBB3_4:                                 ## =>This Inner Loop Header: Depth=1
	movdqu	2048(%rbx,%rcx,4), %xmm1
	movdqu	2064(%rbx,%rcx,4), %xmm2
	pshufb	%xmm0, %xmm1
	pshufb	%xmm0, %xmm2
	punpcklqdq	%xmm2, %xmm1            ## xmm1 = xmm1[0],xmm2[0]
	movdqu	%xmm1, (%r14,%rcx,2)
	movdqu	2080(%rbx,%rcx,4), %xmm1
	movdqu	2096(%rbx,%rcx,4), %xmm2
	pshufb	%xmm0, %xmm1
	pshufb	%xmm0, %xmm2
	punpcklqdq	%xmm2, %xmm1            ## xmm1 = xmm1[0],xmm2[0]
	movdqu	%xmm1, 16(%r14,%rcx,2)
	addq	$16, %rcx
	cmpq	$256, %rcx                      ## imm = 0x100
	jne	LBB3_4
LBB3_5:
                                        ## kill: def $eax killed $eax killed $rax
	popq	%rbx
	popq	%r14
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__literal16,16byte_literals
	.p2align	4, 0x0                          ## -- Begin function ntt_montgomery_lift_ws
LCPI4_0:
	.short	5569                            ## 0x15c1
	.short	0                               ## 0x0
	.short	5569                            ## 0x15c1
	.short	0                               ## 0x0
	.short	5569                            ## 0x15c1
	.short	0                               ## 0x0
	.short	5569                            ## 0x15c1
	.short	0                               ## 0x0
LCPI4_1:
	.long	559168                          ## 0x88840
	.long	559168                          ## 0x88840
	.long	559168                          ## 0x88840
	.long	559168                          ## 0x88840
LCPI4_2:
	.long	4294959615                      ## 0xffffe1ff
	.long	4294959615                      ## 0xffffe1ff
	.long	4294959615                      ## 0xffffe1ff
	.long	4294959615                      ## 0xffffe1ff
LCPI4_3:
	.long	7681                            ## 0x1e01
	.long	7681                            ## 0x1e01
	.long	7681                            ## 0x1e01
	.long	7681                            ## 0x1e01
LCPI4_4:
	.long	57855                           ## 0xe1ff
	.long	57855                           ## 0xe1ff
	.long	57855                           ## 0xe1ff
	.long	57855                           ## 0xe1ff
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_ntt_montgomery_lift_ws
	.p2align	4
_ntt_montgomery_lift_ws:                ## @ntt_montgomery_lift_ws
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	xorl	%eax, %eax
	movdqa	LCPI4_0(%rip), %xmm2            ## xmm2 = [5569,0,5569,0,5569,0,5569,0]
	movdqa	LCPI4_1(%rip), %xmm3            ## xmm3 = [559168,559168,559168,559168]
	movdqa	LCPI4_2(%rip), %xmm4            ## xmm4 = [4294959615,4294959615,4294959615,4294959615]
	movdqa	LCPI4_3(%rip), %xmm5            ## xmm5 = [7681,7681,7681,7681]
	movdqa	LCPI4_4(%rip), %xmm6            ## xmm6 = [57855,57855,57855,57855]
	pxor	%xmm7, %xmm7
	.p2align	4
LBB4_1:                                 ## =>This Inner Loop Header: Depth=1
	pmovsxwd	(%rdi,%rax,2), %xmm0
	pmovsxwd	8(%rdi,%rax,2), %xmm1
	pmaddwd	%xmm2, %xmm1
	pmaddwd	%xmm2, %xmm0
	pshufd	$245, %xmm0, %xmm8              ## xmm8 = xmm0[1,1,3,3]
	pmuldq	%xmm3, %xmm8
	movdqa	%xmm0, %xmm9
	pmuldq	%xmm3, %xmm9
	pshufd	$245, %xmm9, %xmm9              ## xmm9 = xmm9[1,1,3,3]
	pblendw	$204, %xmm8, %xmm9              ## xmm9 = xmm9[0,1],xmm8[2,3],xmm9[4,5],xmm8[6,7]
	pshufd	$245, %xmm1, %xmm8              ## xmm8 = xmm1[1,1,3,3]
	pmuldq	%xmm3, %xmm8
	movdqa	%xmm1, %xmm10
	pmuldq	%xmm3, %xmm10
	pshufd	$245, %xmm10, %xmm10            ## xmm10 = xmm10[1,1,3,3]
	pblendw	$204, %xmm8, %xmm10             ## xmm10 = xmm10[0,1],xmm8[2,3],xmm10[4,5],xmm8[6,7]
	pmulld	%xmm4, %xmm10
	pmulld	%xmm4, %xmm9
	paddd	%xmm0, %xmm9
	paddd	%xmm1, %xmm10
	movdqa	%xmm10, %xmm8
	psrad	$31, %xmm8
	pand	%xmm5, %xmm8
	movdqa	%xmm9, %xmm11
	psrad	$31, %xmm11
	pand	%xmm5, %xmm11
	paddd	%xmm9, %xmm11
	paddd	%xmm10, %xmm8
	movdqa	%xmm8, %xmm9
	paddd	%xmm6, %xmm9
	movdqa	%xmm11, %xmm10
	paddd	%xmm6, %xmm10
	movdqa	%xmm5, %xmm1
	pcmpgtd	%xmm8, %xmm1
	movdqa	%xmm5, %xmm0
	pcmpgtd	%xmm11, %xmm0
	blendvps	%xmm0, %xmm11, %xmm10
	movdqa	%xmm1, %xmm0
	blendvps	%xmm0, %xmm8, %xmm9
	pblendw	$170, %xmm7, %xmm9              ## xmm9 = xmm9[0],xmm7[1],xmm9[2],xmm7[3],xmm9[4],xmm7[5],xmm9[6],xmm7[7]
	pblendw	$170, %xmm7, %xmm10             ## xmm10 = xmm10[0],xmm7[1],xmm10[2],xmm7[3],xmm10[4],xmm7[5],xmm10[6],xmm7[7]
	packusdw	%xmm9, %xmm10
	movdqu	%xmm10, (%rdi,%rax,2)
	addq	$8, %rax
	cmpq	$256, %rax                      ## imm = 0x100
	jne	LBB4_1
## %bb.2:
	xorl	%eax, %eax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_ntt_forward                    ## -- Begin function ntt_forward
	.p2align	4
_ntt_forward:                           ## @ntt_forward
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, %rdx
	xorl	%edi, %edi
	movq	%rdx, %rsi
	xorl	%ecx, %ecx
	popq	%rbp
	jmp	_standalone                     ## TAILCALL
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__literal16,16byte_literals
	.p2align	4, 0x0                          ## -- Begin function standalone
LCPI6_0:
	.long	57855                           ## 0xe1ff
	.long	57855                           ## 0xe1ff
	.long	57855                           ## 0xe1ff
	.long	57855                           ## 0xe1ff
LCPI6_1:
	.long	7681                            ## 0x1e01
	.long	7681                            ## 0x1e01
	.long	7681                            ## 0x1e01
	.long	7681                            ## 0x1e01
LCPI6_2:
	.byte	0                               ## 0x0
	.byte	1                               ## 0x1
	.byte	4                               ## 0x4
	.byte	5                               ## 0x5
	.byte	8                               ## 0x8
	.byte	9                               ## 0x9
	.byte	12                              ## 0xc
	.byte	13                              ## 0xd
	.byte	8                               ## 0x8
	.byte	9                               ## 0x9
	.byte	12                              ## 0xc
	.byte	13                              ## 0xd
	.byte	12                              ## 0xc
	.byte	13                              ## 0xd
	.byte	14                              ## 0xe
	.byte	15                              ## 0xf
LCPI6_3:
	.short	5569                            ## 0x15c1
	.short	0                               ## 0x0
	.short	5569                            ## 0x15c1
	.short	0                               ## 0x0
	.short	5569                            ## 0x15c1
	.short	0                               ## 0x0
	.short	5569                            ## 0x15c1
	.short	0                               ## 0x0
LCPI6_4:
	.long	559168                          ## 0x88840
	.long	559168                          ## 0x88840
	.long	559168                          ## 0x88840
	.long	559168                          ## 0x88840
LCPI6_5:
	.long	4294959615                      ## 0xffffe1ff
	.long	4294959615                      ## 0xffffe1ff
	.long	4294959615                      ## 0xffffe1ff
	.long	4294959615                      ## 0xffffe1ff
	.section	__TEXT,__text,regular,pure_instructions
	.p2align	4
_standalone:                            ## @standalone
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	pushq	%rax
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	movq	%rcx, %r13
	movq	%rdx, %r12
	movq	%rsi, %r15
	movl	%edi, %r14d
	movl	$3072, %edi                     ## imm = 0xC00
	callq	_malloc
	testq	%rax, %rax
	je	LBB6_1
## %bb.2:
	movq	%rax, %rbx
	leaq	_bit_ntt_roots32(%rip), %rdi
	movl	$2, %esi
	movl	$7681, %edx                     ## imm = 0x1E01
	movl	$-1954291199, %ecx              ## imm = 0x8B83E201
	movl	$1912, %r8d                     ## imm = 0x778
	xorl	%r9d, %r9d
	callq	_OP_ntt256_init
	testl	%eax, %eax
	je	LBB6_4
## %bb.3:
	movl	$-1, %r14d
LBB6_27:
	movl	$3072, %esi                     ## imm = 0xC00
	movq	%rbx, %rdi
	callq	_bit_secure_zero
	movq	%rbx, %rdi
	callq	_free
	jmp	LBB6_28
LBB6_1:
	movl	$-1, %r14d
LBB6_28:
	movl	%r14d, %eax
	addq	$8, %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
LBB6_4:
	cmpl	$1, %r14d
	ja	LBB6_13
## %bb.5:
	movl	$12, %eax
	.p2align	4
LBB6_6:                                 ## =>This Inner Loop Header: Depth=1
	pmovsxwd	-24(%r12,%rax,2), %xmm0
	pmovsxwd	-16(%r12,%rax,2), %xmm1
	movdqu	%xmm0, -48(%rbx,%rax,4)
	movdqu	%xmm1, -32(%rbx,%rax,4)
	pmovsxwd	-8(%r12,%rax,2), %xmm0
	pmovsxwd	(%r12,%rax,2), %xmm1
	movdqu	%xmm0, -16(%rbx,%rax,4)
	movdqu	%xmm1, (%rbx,%rax,4)
	addq	$16, %rax
	cmpq	$268, %rax                      ## imm = 0x10C
	jne	LBB6_6
## %bb.7:
	movq	%rbx, %rdi
	addq	$2048, %rdi                     ## imm = 0x800
	movq	%rbx, %rsi
	testl	%r14d, %r14d
	je	LBB6_9
## %bb.8:
	movl	$1, %edx
	callq	_OP_intt256
	jmp	LBB6_10
LBB6_13:
	cmpl	$2, %r14d
	je	LBB6_22
## %bb.14:
	cmpl	$3, %r14d
	jne	LBB6_15
## %bb.17:
	leaq	1024(%rbx), %rdx
	movl	$4, %eax
	.p2align	4
LBB6_18:                                ## =>This Inner Loop Header: Depth=1
	pmovsxwd	-8(%r12,%rax,2), %xmm0
	pmovsxwd	(%r12,%rax,2), %xmm1
	movdqu	%xmm0, -16(%rbx,%rax,4)
	movdqu	%xmm1, (%rbx,%rax,4)
	pmovsxwd	-8(%r13,%rax,2), %xmm0
	pmovsxwd	(%r13,%rax,2), %xmm1
	movdqu	%xmm0, 1008(%rbx,%rax,4)
	movdqu	%xmm1, 1024(%rbx,%rax,4)
	addq	$8, %rax
	cmpq	$260, %rax                      ## imm = 0x104
	jne	LBB6_18
## %bb.19:
	movq	%rbx, %rdi
	addq	$2048, %rdi                     ## imm = 0x800
	movq	%rbx, %rsi
	movl	$256, %ecx                      ## imm = 0x100
	movl	$7681, %r8d                     ## imm = 0x1E01
	xorl	%r9d, %r9d
	callq	_OP_cwm
	testl	%eax, %eax
	movl	$-1, %r14d
	jne	LBB6_27
## %bb.20:
	xorl	%r14d, %r14d
	movdqa	LCPI6_0(%rip), %xmm2            ## xmm2 = [57855,57855,57855,57855]
	movdqa	LCPI6_1(%rip), %xmm3            ## xmm3 = [7681,7681,7681,7681]
	movdqa	LCPI6_2(%rip), %xmm4            ## xmm4 = [0,1,4,5,8,9,12,13,8,9,12,13,12,13,14,15]
	xorl	%eax, %eax
	.p2align	4
LBB6_21:                                ## =>This Inner Loop Header: Depth=1
	pmovsxwd	(%r15,%rax,2), %xmm0
	pmovsxwd	8(%r15,%rax,2), %xmm1
	movdqu	2048(%rbx,%rax,4), %xmm5
	paddd	%xmm0, %xmm5
	movdqu	2064(%rbx,%rax,4), %xmm6
	paddd	%xmm1, %xmm6
	movdqa	%xmm5, %xmm7
	paddd	%xmm2, %xmm7
	movdqa	%xmm6, %xmm8
	paddd	%xmm2, %xmm8
	movdqa	%xmm3, %xmm0
	pcmpgtd	%xmm5, %xmm0
	movdqa	%xmm3, %xmm1
	pcmpgtd	%xmm6, %xmm1
	blendvps	%xmm0, %xmm5, %xmm7
	movdqa	%xmm1, %xmm0
	blendvps	%xmm0, %xmm6, %xmm8
	pshufb	%xmm4, %xmm7
	pshufb	%xmm4, %xmm8
	movq	%xmm7, (%r15,%rax,2)
	movq	%xmm8, 8(%r15,%rax,2)
	addq	$8, %rax
	cmpq	$256, %rax                      ## imm = 0x100
	jne	LBB6_21
	jmp	LBB6_27
LBB6_9:
	xorl	%edx, %edx
	callq	_OP_ntt256
LBB6_10:
	movl	$-1, %r14d
	testl	%eax, %eax
	jne	LBB6_27
## %bb.11:
	xorl	%r14d, %r14d
	movdqa	LCPI6_2(%rip), %xmm0            ## xmm0 = [0,1,4,5,8,9,12,13,8,9,12,13,12,13,14,15]
	xorl	%eax, %eax
	.p2align	4
LBB6_12:                                ## =>This Inner Loop Header: Depth=1
	movdqu	2048(%rbx,%rax,4), %xmm1
	movdqu	2064(%rbx,%rax,4), %xmm2
	pshufb	%xmm0, %xmm1
	pshufb	%xmm0, %xmm2
	punpcklqdq	%xmm2, %xmm1            ## xmm1 = xmm1[0],xmm2[0]
	movdqu	%xmm1, (%r15,%rax,2)
	movdqu	2080(%rbx,%rax,4), %xmm1
	movdqu	2096(%rbx,%rax,4), %xmm2
	pshufb	%xmm0, %xmm1
	pshufb	%xmm0, %xmm2
	punpcklqdq	%xmm2, %xmm1            ## xmm1 = xmm1[0],xmm2[0]
	movdqu	%xmm1, 16(%r15,%rax,2)
	addq	$16, %rax
	cmpq	$256, %rax                      ## imm = 0x100
	jne	LBB6_12
	jmp	LBB6_27
LBB6_22:
	leaq	1024(%rbx), %rdx
	movl	$4, %eax
	.p2align	4
LBB6_23:                                ## =>This Inner Loop Header: Depth=1
	pmovsxwd	-8(%r12,%rax,2), %xmm0
	pmovsxwd	(%r12,%rax,2), %xmm1
	movdqu	%xmm0, -16(%rbx,%rax,4)
	movdqu	%xmm1, (%rbx,%rax,4)
	pmovsxwd	-8(%r13,%rax,2), %xmm0
	pmovsxwd	(%r13,%rax,2), %xmm1
	movdqu	%xmm0, 1008(%rbx,%rax,4)
	movdqu	%xmm1, 1024(%rbx,%rax,4)
	addq	$8, %rax
	cmpq	$260, %rax                      ## imm = 0x104
	jne	LBB6_23
## %bb.24:
	movq	%rbx, %rdi
	addq	$2048, %rdi                     ## imm = 0x800
	movq	%rbx, %rsi
	movl	$256, %ecx                      ## imm = 0x100
	movl	$7681, %r8d                     ## imm = 0x1E01
	xorl	%r9d, %r9d
	callq	_OP_cwm
	testl	%eax, %eax
	movl	$-1, %r14d
	jne	LBB6_27
## %bb.25:
	xorl	%r14d, %r14d
	movdqa	LCPI6_2(%rip), %xmm0            ## xmm0 = [0,1,4,5,8,9,12,13,8,9,12,13,12,13,14,15]
	xorl	%eax, %eax
	.p2align	4
LBB6_26:                                ## =>This Inner Loop Header: Depth=1
	movdqu	2048(%rbx,%rax,4), %xmm1
	movdqu	2064(%rbx,%rax,4), %xmm2
	pshufb	%xmm0, %xmm1
	pshufb	%xmm0, %xmm2
	punpcklqdq	%xmm2, %xmm1            ## xmm1 = xmm1[0],xmm2[0]
	movdqu	%xmm1, (%r15,%rax,2)
	movdqu	2080(%rbx,%rax,4), %xmm1
	movdqu	2096(%rbx,%rax,4), %xmm2
	pshufb	%xmm0, %xmm1
	pshufb	%xmm0, %xmm2
	punpcklqdq	%xmm2, %xmm1            ## xmm1 = xmm1[0],xmm2[0]
	movdqu	%xmm1, 16(%r15,%rax,2)
	addq	$16, %rax
	cmpq	$256, %rax                      ## imm = 0x100
	jne	LBB6_26
	jmp	LBB6_27
LBB6_15:
	xorl	%r14d, %r14d
	movdqa	LCPI6_3(%rip), %xmm2            ## xmm2 = [5569,0,5569,0,5569,0,5569,0]
	movdqa	LCPI6_4(%rip), %xmm3            ## xmm3 = [559168,559168,559168,559168]
	movdqa	LCPI6_5(%rip), %xmm4            ## xmm4 = [4294959615,4294959615,4294959615,4294959615]
	movdqa	LCPI6_1(%rip), %xmm5            ## xmm5 = [7681,7681,7681,7681]
	movdqa	LCPI6_0(%rip), %xmm6            ## xmm6 = [57855,57855,57855,57855]
	pxor	%xmm7, %xmm7
	xorl	%eax, %eax
	.p2align	4
LBB6_16:                                ## =>This Inner Loop Header: Depth=1
	pmovsxwd	(%r15,%rax,2), %xmm0
	pmovsxwd	8(%r15,%rax,2), %xmm1
	pmaddwd	%xmm2, %xmm1
	pmaddwd	%xmm2, %xmm0
	pshufd	$245, %xmm0, %xmm8              ## xmm8 = xmm0[1,1,3,3]
	pmuldq	%xmm3, %xmm8
	movdqa	%xmm0, %xmm9
	pmuldq	%xmm3, %xmm9
	pshufd	$245, %xmm9, %xmm9              ## xmm9 = xmm9[1,1,3,3]
	pblendw	$204, %xmm8, %xmm9              ## xmm9 = xmm9[0,1],xmm8[2,3],xmm9[4,5],xmm8[6,7]
	pshufd	$245, %xmm1, %xmm8              ## xmm8 = xmm1[1,1,3,3]
	pmuldq	%xmm3, %xmm8
	movdqa	%xmm1, %xmm10
	pmuldq	%xmm3, %xmm10
	pshufd	$245, %xmm10, %xmm10            ## xmm10 = xmm10[1,1,3,3]
	pblendw	$204, %xmm8, %xmm10             ## xmm10 = xmm10[0,1],xmm8[2,3],xmm10[4,5],xmm8[6,7]
	pmulld	%xmm4, %xmm10
	pmulld	%xmm4, %xmm9
	paddd	%xmm0, %xmm9
	paddd	%xmm1, %xmm10
	movdqa	%xmm10, %xmm8
	psrad	$31, %xmm8
	pand	%xmm5, %xmm8
	movdqa	%xmm9, %xmm11
	psrad	$31, %xmm11
	pand	%xmm5, %xmm11
	paddd	%xmm9, %xmm11
	paddd	%xmm10, %xmm8
	movdqa	%xmm8, %xmm9
	paddd	%xmm6, %xmm9
	movdqa	%xmm11, %xmm10
	paddd	%xmm6, %xmm10
	movdqa	%xmm5, %xmm1
	pcmpgtd	%xmm8, %xmm1
	movdqa	%xmm5, %xmm0
	pcmpgtd	%xmm11, %xmm0
	blendvps	%xmm0, %xmm11, %xmm10
	movdqa	%xmm1, %xmm0
	blendvps	%xmm0, %xmm8, %xmm9
	pblendw	$170, %xmm7, %xmm9              ## xmm9 = xmm9[0],xmm7[1],xmm9[2],xmm7[3],xmm9[4],xmm7[5],xmm9[6],xmm7[7]
	pblendw	$170, %xmm7, %xmm10             ## xmm10 = xmm10[0],xmm7[1],xmm10[2],xmm7[3],xmm10[4],xmm7[5],xmm10[6],xmm7[7]
	packusdw	%xmm9, %xmm10
	movdqu	%xmm10, (%r15,%rax,2)
	addq	$8, %rax
	cmpq	$256, %rax                      ## imm = 0x100
	jne	LBB6_16
	jmp	LBB6_27
	.cfi_endproc
                                        ## -- End function
	.globl	_ntt_inverse                    ## -- Begin function ntt_inverse
	.p2align	4
_ntt_inverse:                           ## @ntt_inverse
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, %rdx
	movl	$1, %edi
	movq	%rdx, %rsi
	xorl	%ecx, %ecx
	popq	%rbp
	jmp	_standalone                     ## TAILCALL
	.cfi_endproc
                                        ## -- End function
	.globl	_ntt_basemul_raw                ## -- Begin function ntt_basemul_raw
	.p2align	4
_ntt_basemul_raw:                       ## @ntt_basemul_raw
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdx, %rcx
	movq	%rsi, %rdx
	movq	%rdi, %rsi
	movl	$2, %edi
	popq	%rbp
	jmp	_standalone                     ## TAILCALL
	.cfi_endproc
                                        ## -- End function
	.globl	_ntt_basemul_acc_raw            ## -- Begin function ntt_basemul_acc_raw
	.p2align	4
_ntt_basemul_acc_raw:                   ## @ntt_basemul_acc_raw
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdx, %rcx
	movq	%rsi, %rdx
	movq	%rdi, %rsi
	movl	$3, %edi
	popq	%rbp
	jmp	_standalone                     ## TAILCALL
	.cfi_endproc
                                        ## -- End function
	.globl	_ntt_montgomery_lift            ## -- Begin function ntt_montgomery_lift
	.p2align	4
_ntt_montgomery_lift:                   ## @ntt_montgomery_lift
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, %rsi
	movl	$4, %edi
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	popq	%rbp
	jmp	_standalone                     ## TAILCALL
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__const
	.p2align	4, 0x0                          ## @bit_ntt_roots32
_bit_ntt_roots32:
	.long	5569                            ## 0x15c1
	.long	6115                            ## 0x17e3
	.long	5330                            ## 0x14d2
	.long	4083                            ## 0xff3
	.long	2336                            ## 0x920
	.long	6620                            ## 0x19dc
	.long	3415                            ## 0xd57
	.long	721                             ## 0x2d1
	.long	4320                            ## 0x10e0
	.long	5298                            ## 0x14b2
	.long	5158                            ## 0x1426
	.long	5963                            ## 0x174b
	.long	808                             ## 0x328
	.long	6709                            ## 0x1a35
	.long	3838                            ## 0xefe
	.long	3064                            ## 0xbf8
	.long	4753                            ## 0x1291
	.long	3066                            ## 0xbfa
	.long	1454                            ## 0x5ae
	.long	3042                            ## 0xbe2
	.long	1842                            ## 0x732
	.long	2195                            ## 0x893
	.long	4909                            ## 0x132d
	.long	825                             ## 0x339
	.long	3196                            ## 0xc7c
	.long	4901                            ## 0x1325
	.long	7500                            ## 0x1d4c
	.long	2157                            ## 0x86d
	.long	3215                            ## 0xc8f
	.long	49                              ## 0x31
	.long	5670                            ## 0x1626
	.long	2153                            ## 0x869
	.long	4981                            ## 0x1375
	.long	6290                            ## 0x1892
	.long	2537                            ## 0x9e9
	.long	2994                            ## 0xbb2
	.long	7176                            ## 0x1c08
	.long	4448                            ## 0x1160
	.long	3362                            ## 0xd22
	.long	5766                            ## 0x1686
	.long	6221                            ## 0x184d
	.long	7384                            ## 0x1cd8
	.long	746                             ## 0x2ea
	.long	4350                            ## 0x10fe
	.long	6270                            ## 0x187e
	.long	4169                            ## 0x1049
	.long	2899                            ## 0xb53
	.long	6361                            ## 0x18d9
	.long	1843                            ## 0x733
	.long	5578                            ## 0x15ca
	.long	6834                            ## 0x1ab2
	.long	7293                            ## 0x1c7d
	.long	871                             ## 0x367
	.long	4770                            ## 0x12a2
	.long	2217                            ## 0x8a9
	.long	3455                            ## 0xd7f
	.long	769                             ## 0x301
	.long	5349                            ## 0x14e5
	.long	5573                            ## 0x15c5
	.long	4285                            ## 0x10bd
	.long	4852                            ## 0x12f4
	.long	19                              ## 0x13
	.long	4                               ## 0x4
	.long	5851                            ## 0x16db
	.long	2862                            ## 0xb2e
	.long	4086                            ## 0xff6
	.long	2073                            ## 0x819
	.long	206                             ## 0xce
	.long	5912                            ## 0x1718
	.long	6653                            ## 0x19fd
	.long	5039                            ## 0x13af
	.long	2798                            ## 0xaee
	.long	4620                            ## 0x120c
	.long	6306                            ## 0x18a2
	.long	6583                            ## 0x19b7
	.long	3070                            ## 0xbfe
	.long	2571                            ## 0xa0b
	.long	2801                            ## 0xaf1
	.long	2611                            ## 0xa33
	.long	7544                            ## 0x1d78
	.long	7110                            ## 0x1bc6
	.long	3919                            ## 0xf4f
	.long	6889                            ## 0x1ae9
	.long	1333                            ## 0x535
	.long	2610                            ## 0xa32
	.long	4161                            ## 0x1041
	.long	876                             ## 0x36c
	.long	6323                            ## 0x18b3
	.long	6405                            ## 0x1905
	.long	14                              ## 0xe
	.long	1620                            ## 0x654
	.long	3907                            ## 0xf43
	.long	6532                            ## 0x1984
	.long	7200                            ## 0x1c20
	.long	303                             ## 0x12f
	.long	3476                            ## 0xd94
	.long	953                             ## 0x3b9
	.long	5660                            ## 0x161c
	.long	6447                            ## 0x192f
	.long	3842                            ## 0xf02
	.long	5114                            ## 0x13fa
	.long	3050                            ## 0xbea
	.long	5089                            ## 0x13e1
	.long	2966                            ## 0xb96
	.long	3986                            ## 0xf92
	.long	4483                            ## 0x1183
	.long	7412                            ## 0x1cf4
	.long	4012                            ## 0xfac
	.long	3505                            ## 0xdb1
	.long	5632                            ## 0x1600
	.long	3207                            ## 0xc87
	.long	3709                            ## 0xe7d
	.long	4638                            ## 0x121e
	.long	5752                            ## 0x1678
	.long	2828                            ## 0xb0c
	.long	4279                            ## 0x10b7
	.long	7439                            ## 0x1d0f
	.long	3181                            ## 0xc6d
	.long	2691                            ## 0xa83
	.long	1668                            ## 0x684
	.long	289                             ## 0x121
	.long	2200                            ## 0x898
	.long	3293                            ## 0xcdd
	.long	2769                            ## 0xad1
	.long	495                             ## 0x1ef
	.long	127                             ## 0x7f
	.long	431                             ## 0x1af
	.long	6364                            ## 0x18dc
	.long	813                             ## 0x32d
	.long	581                             ## 0x245
	.long	5782                            ## 0x1696
	.long	4680                            ## 0x1248
	.long	2243                            ## 0x8c3
	.long	6922                            ## 0x1b0a
	.long	1053                            ## 0x41d
	.long	5996                            ## 0x176c
	.long	781                             ## 0x30d
	.long	7540                            ## 0x1d74
	.long	5630                            ## 0x15fe
	.long	5091                            ## 0x13e3
	.long	5537                            ## 0x15a1
	.long	5393                            ## 0x1511
	.long	5178                            ## 0x143a
	.long	4494                            ## 0x118e
	.long	7237                            ## 0x1c45
	.long	3424                            ## 0xd60
	.long	5572                            ## 0x15c4
	.long	902                             ## 0x386
	.long	5379                            ## 0x1503
	.long	868                             ## 0x364
	.long	587                             ## 0x24b
	.long	4123                            ## 0x101b
	.long	3003                            ## 0xbbb
	.long	4867                            ## 0x1303
	.long	4663                            ## 0x1237
	.long	5836                            ## 0x16cc
	.long	519                             ## 0x207
	.long	4509                            ## 0x119d
	.long	545                             ## 0x221
	.long	295                             ## 0x127
	.long	472                             ## 0x1d8
	.long	6809                            ## 0x1a99
	.long	2242                            ## 0x8c2
	.long	3539                            ## 0xdd3
	.long	1340                            ## 0x53c
	.long	1430                            ## 0x596
	.long	6365                            ## 0x18dd
	.long	2952                            ## 0xb88
	.long	5319                            ## 0x14c7
	.long	5275                            ## 0x149b
	.long	302                             ## 0x12e
	.long	93                              ## 0x5d
	.long	2147                            ## 0x863
	.long	4756                            ## 0x1294
	.long	597                             ## 0x255
	.long	7229                            ## 0x1c3d
	.long	4844                            ## 0x12ec
	.long	3679                            ## 0xe5f
	.long	7647                            ## 0x1ddf
	.long	193                             ## 0xc1
	.long	2556                            ## 0x9fc
	.long	5823                            ## 0x16bf
	.long	4460                            ## 0x116c
	.long	2696                            ## 0xa88
	.long	3359                            ## 0xd1f
	.long	3298                            ## 0xce2
	.long	6354                            ## 0x18d2
	.long	4144                            ## 0x1030
	.long	358                             ## 0x166
	.long	5197                            ## 0x144d
	.long	5541                            ## 0x15a5
	.long	3563                            ## 0xdeb
	.long	5030                            ## 0x13a6
	.long	3075                            ## 0xc03
	.long	4690                            ## 0x1252
	.long	5005                            ## 0x138d
	.long	1652                            ## 0x674
	.long	4629                            ## 0x1215
	.long	166                             ## 0xa6
	.long	865                             ## 0x361
	.long	1582                            ## 0x62e
	.long	5930                            ## 0x172a
	.long	3674                            ## 0xe5a
	.long	1284                            ## 0x504
	.long	7095                            ## 0x1bb7
	.long	6941                            ## 0x1b1d
	.long	1057                            ## 0x421
	.long	4166                            ## 0x1046
	.long	5926                            ## 0x1726
	.long	248                             ## 0xf8
	.long	1265                            ## 0x4f1
	.long	1178                            ## 0x49a
	.long	1592                            ## 0x638
	.long	1355                            ## 0x54b
	.long	7562                            ## 0x1d8a
	.long	4516                            ## 0x11a4
	.long	6732                            ## 0x1a4c
	.long	191                             ## 0xbf
	.long	1253                            ## 0x4e5
	.long	6668                            ## 0x1a0c
	.long	235                             ## 0xeb
	.long	3862                            ## 0xf16
	.long	6877                            ## 0x1add
	.long	6823                            ## 0x1aa7
	.long	4772                            ## 0x12a4
	.long	5895                            ## 0x1707
	.long	7305                            ## 0x1c89
	.long	3038                            ## 0xbde
	.long	6127                            ## 0x17ef
	.long	4303                            ## 0x10cf
	.long	4140                            ## 0x102c
	.long	3157                            ## 0xc55
	.long	2808                            ## 0xaf8
	.long	5748                            ## 0x1674
	.long	5657                            ## 0x1619
	.long	4260                            ## 0x10a4
	.long	6670                            ## 0x1a0e
	.long	5513                            ## 0x1589
	.long	4799                            ## 0x12bf
	.long	5064                            ## 0x13c8
	.long	7314                            ## 0x1c92
	.long	2761                            ## 0xac9
	.long	177                             ## 0xb1
	.long	7354                            ## 0x1cba
	.long	6574                            ## 0x19ae
	.long	3347                            ## 0xd13
	.long	4343                            ## 0x10f7
	.long	6297                            ## 0x1899
	.long	6686                            ## 0x1a1e
	.long	5874                            ## 0x16f2
	.long	4875                            ## 0x130b
	.long	1018                            ## 0x3fa
	.long	4010                            ## 0xfaa
	.long	1184                            ## 0x4a0
	.long	7526                            ## 0x1d66
	.long	5624                            ## 0x15f8
	.long	5569                            ## 0x15c1
	.long	1566                            ## 0x61e
	.long	3598                            ## 0xe0e
	.long	2351                            ## 0x92f
	.long	6960                            ## 0x1b30
	.long	4266                            ## 0x10aa
	.long	1061                            ## 0x425
	.long	5345                            ## 0x14e1
	.long	4617                            ## 0x1209
	.long	3843                            ## 0xf03
	.long	972                             ## 0x3cc
	.long	6873                            ## 0x1ad9
	.long	1718                            ## 0x6b6
	.long	2523                            ## 0x9db
	.long	2383                            ## 0x94f
	.long	3361                            ## 0xd21
	.long	5528                            ## 0x1598
	.long	2011                            ## 0x7db
	.long	7632                            ## 0x1dd0
	.long	4466                            ## 0x1172
	.long	5524                            ## 0x1594
	.long	181                             ## 0xb5
	.long	2780                            ## 0xadc
	.long	4485                            ## 0x1185
	.long	6856                            ## 0x1ac8
	.long	2772                            ## 0xad4
	.long	5486                            ## 0x156e
	.long	5839                            ## 0x16cf
	.long	4639                            ## 0x121f
	.long	6227                            ## 0x1853
	.long	4615                            ## 0x1207
	.long	2928                            ## 0xb70
	.long	1830                            ## 0x726
	.long	7677                            ## 0x1dfd
	.long	7662                            ## 0x1dee
	.long	2829                            ## 0xb0d
	.long	3396                            ## 0xd44
	.long	2108                            ## 0x83c
	.long	2332                            ## 0x91c
	.long	6912                            ## 0x1b00
	.long	4226                            ## 0x1082
	.long	5464                            ## 0x1558
	.long	2911                            ## 0xb5f
	.long	6810                            ## 0x1a9a
	.long	388                             ## 0x184
	.long	847                             ## 0x34f
	.long	2103                            ## 0x837
	.long	5838                            ## 0x16ce
	.long	1320                            ## 0x528
	.long	4782                            ## 0x12ae
	.long	3512                            ## 0xdb8
	.long	1411                            ## 0x583
	.long	3331                            ## 0xd03
	.long	6935                            ## 0x1b17
	.long	297                             ## 0x129
	.long	1460                            ## 0x5b4
	.long	1915                            ## 0x77b
	.long	4319                            ## 0x10df
	.long	3233                            ## 0xca1
	.long	505                             ## 0x1f9
	.long	4687                            ## 0x124f
	.long	5144                            ## 0x1418
	.long	1391                            ## 0x56f
	.long	2700                            ## 0xa8c
	.long	1317                            ## 0x525
	.long	7250                            ## 0x1c52
	.long	7554                            ## 0x1d82
	.long	7186                            ## 0x1c12
	.long	4912                            ## 0x1330
	.long	4388                            ## 0x1124
	.long	5481                            ## 0x1569
	.long	7392                            ## 0x1ce0
	.long	6013                            ## 0x177d
	.long	4990                            ## 0x137e
	.long	4500                            ## 0x1194
	.long	242                             ## 0xf2
	.long	3402                            ## 0xd4a
	.long	4853                            ## 0x12f5
	.long	1929                            ## 0x789
	.long	3043                            ## 0xbe3
	.long	3972                            ## 0xf84
	.long	4474                            ## 0x117a
	.long	2049                            ## 0x801
	.long	4176                            ## 0x1050
	.long	3669                            ## 0xe55
	.long	269                             ## 0x10d
	.long	3198                            ## 0xc7e
	.long	3695                            ## 0xe6f
	.long	4715                            ## 0x126b
	.long	2592                            ## 0xa20
	.long	4631                            ## 0x1217
	.long	2567                            ## 0xa07
	.long	3839                            ## 0xeff
	.long	1234                            ## 0x4d2
	.long	2021                            ## 0x7e5
	.long	6728                            ## 0x1a48
	.long	4205                            ## 0x106d
	.long	7378                            ## 0x1cd2
	.long	481                             ## 0x1e1
	.long	1149                            ## 0x47d
	.long	3774                            ## 0xebe
	.long	6061                            ## 0x17ad
	.long	7667                            ## 0x1df3
	.long	1276                            ## 0x4fc
	.long	1358                            ## 0x54e
	.long	6805                            ## 0x1a95
	.long	3520                            ## 0xdc0
	.long	5071                            ## 0x13cf
	.long	6348                            ## 0x18cc
	.long	792                             ## 0x318
	.long	3762                            ## 0xeb2
	.long	571                             ## 0x23b
	.long	137                             ## 0x89
	.long	5070                            ## 0x13ce
	.long	4880                            ## 0x1310
	.long	5110                            ## 0x13f6
	.long	4611                            ## 0x1203
	.long	1098                            ## 0x44a
	.long	1375                            ## 0x55f
	.long	3061                            ## 0xbf5
	.long	4883                            ## 0x1313
	.long	2642                            ## 0xa52
	.long	1028                            ## 0x404
	.long	1769                            ## 0x6e9
	.long	7475                            ## 0x1d33
	.long	5608                            ## 0x15e8
	.long	3595                            ## 0xe0b
	.long	4819                            ## 0x12d3
	.long	2057                            ## 0x809
	.long	155                             ## 0x9b
	.long	6497                            ## 0x1961
	.long	3671                            ## 0xe57
	.long	6663                            ## 0x1a07
	.long	2806                            ## 0xaf6
	.long	1807                            ## 0x70f
	.long	995                             ## 0x3e3
	.long	1384                            ## 0x568
	.long	3338                            ## 0xd0a
	.long	4334                            ## 0x10ee
	.long	1107                            ## 0x453
	.long	327                             ## 0x147
	.long	7504                            ## 0x1d50
	.long	4920                            ## 0x1338
	.long	367                             ## 0x16f
	.long	2617                            ## 0xa39
	.long	2882                            ## 0xb42
	.long	2168                            ## 0x878
	.long	1011                            ## 0x3f3
	.long	3421                            ## 0xd5d
	.long	2024                            ## 0x7e8
	.long	1933                            ## 0x78d
	.long	4873                            ## 0x1309
	.long	4524                            ## 0x11ac
	.long	3541                            ## 0xdd5
	.long	3378                            ## 0xd32
	.long	1554                            ## 0x612
	.long	4643                            ## 0x1223
	.long	376                             ## 0x178
	.long	1786                            ## 0x6fa
	.long	2909                            ## 0xb5d
	.long	858                             ## 0x35a
	.long	804                             ## 0x324
	.long	3819                            ## 0xeeb
	.long	7446                            ## 0x1d16
	.long	1013                            ## 0x3f5
	.long	6428                            ## 0x191c
	.long	7490                            ## 0x1d42
	.long	949                             ## 0x3b5
	.long	3165                            ## 0xc5d
	.long	119                             ## 0x77
	.long	6326                            ## 0x18b6
	.long	6089                            ## 0x17c9
	.long	6503                            ## 0x1967
	.long	6416                            ## 0x1910
	.long	7433                            ## 0x1d09
	.long	1755                            ## 0x6db
	.long	3515                            ## 0xdbb
	.long	6624                            ## 0x19e0
	.long	740                             ## 0x2e4
	.long	586                             ## 0x24a
	.long	6397                            ## 0x18fd
	.long	4007                            ## 0xfa7
	.long	1751                            ## 0x6d7
	.long	6099                            ## 0x17d3
	.long	6816                            ## 0x1aa0
	.long	7515                            ## 0x1d5b
	.long	3052                            ## 0xbec
	.long	6029                            ## 0x178d
	.long	2676                            ## 0xa74
	.long	2991                            ## 0xbaf
	.long	4606                            ## 0x11fe
	.long	2651                            ## 0xa5b
	.long	4118                            ## 0x1016
	.long	2140                            ## 0x85c
	.long	2484                            ## 0x9b4
	.long	7323                            ## 0x1c9b
	.long	3537                            ## 0xdd1
	.long	1327                            ## 0x52f
	.long	4383                            ## 0x111f
	.long	4322                            ## 0x10e2
	.long	4985                            ## 0x1379
	.long	3221                            ## 0xc95
	.long	1858                            ## 0x742
	.long	5125                            ## 0x1405
	.long	7488                            ## 0x1d40
	.long	34                              ## 0x22
	.long	4002                            ## 0xfa2
	.long	2837                            ## 0xb15
	.long	452                             ## 0x1c4
	.long	7084                            ## 0x1bac
	.long	2925                            ## 0xb6d
	.long	5534                            ## 0x159e
	.long	7588                            ## 0x1da4
	.long	7379                            ## 0x1cd3
	.long	2406                            ## 0x966
	.long	2362                            ## 0x93a
	.long	4729                            ## 0x1279
	.long	1316                            ## 0x524
	.long	6251                            ## 0x186b
	.long	6341                            ## 0x18c5
	.long	4142                            ## 0x102e
	.long	5439                            ## 0x153f
	.long	872                             ## 0x368
	.long	7209                            ## 0x1c29
	.long	7386                            ## 0x1cda
	.long	7136                            ## 0x1be0
	.long	3172                            ## 0xc64
	.long	7162                            ## 0x1bfa
	.long	1845                            ## 0x735
	.long	3018                            ## 0xbca
	.long	2814                            ## 0xafe
	.long	4678                            ## 0x1246
	.long	3558                            ## 0xde6
	.long	7094                            ## 0x1bb6
	.long	6813                            ## 0x1a9d
	.long	2302                            ## 0x8fe
	.long	6779                            ## 0x1a7b
	.long	2109                            ## 0x83d
	.long	4257                            ## 0x10a1
	.long	444                             ## 0x1bc
	.long	3187                            ## 0xc73
	.long	2503                            ## 0x9c7
	.long	2288                            ## 0x8f0
	.long	2144                            ## 0x860
	.long	2590                            ## 0xa1e
	.long	2051                            ## 0x803
	.long	141                             ## 0x8d
	.long	6900                            ## 0x1af4
	.long	1685                            ## 0x695
	.long	6628                            ## 0x19e4
	.long	759                             ## 0x2f7
	.long	5438                            ## 0x153e
	.long	3001                            ## 0xbb9
	.long	1899                            ## 0x76b
	.long	7100                            ## 0x1bbc
	.long	6868                            ## 0x1ad4

.subsections_via_symbols
