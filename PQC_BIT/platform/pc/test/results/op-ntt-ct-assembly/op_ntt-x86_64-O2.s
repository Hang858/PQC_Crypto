	.build_version macos, 26, 0	sdk_version 27, 2
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_OP_ntt256_init                 ## -- Begin function OP_ntt256_init
	.p2align	4
_OP_ntt256_init:                        ## @OP_ntt256_init
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	testq	%rdi, %rdi
	sete	%al
	xorl	$2, %esi
	xorl	$7681, %edx                     ## imm = 0x1E01
	orl	%esi, %edx
	xorl	$-1954291199, %ecx              ## imm = 0x8B83E201
	xorl	$1912, %r8d                     ## imm = 0x778
	orl	%ecx, %r8d
	orl	%edx, %r8d
	orl	%r9d, %r8d
	setne	%cl
	xorl	%edx, %edx
	orb	%al, %cl
	cmoveq	%rdi, %rdx
	movzbl	%cl, %eax
	negl	%eax
	movq	%rdx, _ntt_omega(%rip)
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__literal16,16byte_literals
	.p2align	4, 0x0                          ## -- Begin function OP_ntt256
LCPI1_0:
	.long	7681                            ## 0x1e01
	.long	7681                            ## 0x1e01
	.long	7681                            ## 0x1e01
	.long	7681                            ## 0x1e01
LCPI1_1:
	.long	559168                          ## 0x88840
	.long	559168                          ## 0x88840
	.long	559168                          ## 0x88840
	.long	559168                          ## 0x88840
LCPI1_2:
	.long	4294959615                      ## 0xffffe1ff
	.long	4294959615                      ## 0xffffe1ff
	.long	4294959615                      ## 0xffffe1ff
	.long	4294959615                      ## 0xffffe1ff
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_OP_ntt256
	.p2align	4
_OP_ntt256:                             ## @OP_ntt256
	.cfi_startproc
## %bb.0:
	movl	$-1, %eax
	testl	%edx, %edx
	jne	LBB1_20
## %bb.1:
	testq	%rsi, %rsi
	je	LBB1_20
## %bb.2:
	testq	%rdi, %rdi
	je	LBB1_20
## %bb.3:
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
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	movq	_ntt_omega(%rip), %rcx
	movq	%rcx, -72(%rbp)                 ## 8-byte Spill
	testq	%rcx, %rcx
	je	LBB1_19
## %bb.4:
	movq	%rdi, %rax
	subq	%rsi, %rax
	cmpq	$32, %rax
	jae	LBB1_5
## %bb.7:
	xorl	%eax, %eax
	.p2align	4
LBB1_8:                                 ## =>This Inner Loop Header: Depth=1
	movl	(%rsi,%rax,4), %ecx
	movl	%ecx, (%rdi,%rax,4)
	movl	4(%rsi,%rax,4), %ecx
	movl	%ecx, 4(%rdi,%rax,4)
	movl	8(%rsi,%rax,4), %ecx
	movl	%ecx, 8(%rdi,%rax,4)
	movl	12(%rsi,%rax,4), %ecx
	movl	%ecx, 12(%rdi,%rax,4)
	addq	$4, %rax
	cmpq	$256, %rax                      ## imm = 0x100
	jne	LBB1_8
	jmp	LBB1_9
LBB1_5:
	xorl	%eax, %eax
	.p2align	4
LBB1_6:                                 ## =>This Inner Loop Header: Depth=1
	movups	(%rsi,%rax,4), %xmm0
	movups	16(%rsi,%rax,4), %xmm1
	movups	%xmm0, (%rdi,%rax,4)
	movups	%xmm1, 16(%rdi,%rax,4)
	movups	32(%rsi,%rax,4), %xmm0
	movups	48(%rsi,%rax,4), %xmm1
	movups	%xmm0, 32(%rdi,%rax,4)
	movups	%xmm1, 48(%rdi,%rax,4)
	movups	64(%rsi,%rax,4), %xmm0
	movups	80(%rsi,%rax,4), %xmm1
	movups	%xmm0, 64(%rdi,%rax,4)
	movups	%xmm1, 80(%rdi,%rax,4)
	movdqu	96(%rsi,%rax,4), %xmm0
	movups	112(%rsi,%rax,4), %xmm1
	movdqu	%xmm0, 96(%rdi,%rax,4)
	movups	%xmm1, 112(%rdi,%rax,4)
	addq	$32, %rax
	cmpq	$256, %rax                      ## imm = 0x100
	jne	LBB1_6
LBB1_9:
	movl	$1, %esi
	movl	$256, %r9d                      ## imm = 0x100
	xorl	%edx, %edx
	movdqa	LCPI1_0(%rip), %xmm1            ## xmm1 = [7681,7681,7681,7681]
	movdqa	LCPI1_1(%rip), %xmm2            ## xmm2 = [559168,559168,559168,559168]
	movl	$256, %r8d                      ## imm = 0x100
	jmp	LBB1_10
	.p2align	4
LBB1_16:                                ##   in Loop: Header=BB1_10 Depth=1
	addl	%esi, %esi
	movq	-56(%rbp), %r8                  ## 8-byte Reload
	movl	%r8d, %r9d
	cmpl	$256, %esi                      ## imm = 0x100
	jge	LBB1_17
LBB1_10:                                ## =>This Loop Header: Depth=1
                                        ##     Child Loop BB1_11 Depth 2
                                        ##       Child Loop BB1_13 Depth 3
                                        ##       Child Loop BB1_21 Depth 3
	shrl	%r8d
	movl	%r9d, %eax
	andl	$-2, %eax
	movq	%rax, -96(%rbp)                 ## 8-byte Spill
	movl	%esi, %esi
	movq	-72(%rbp), %rax                 ## 8-byte Reload
	leaq	(%rax,%rsi,4), %rax
	movq	%rax, -88(%rbp)                 ## 8-byte Spill
	leaq	(%rdi,%r8,4), %r11
	movl	%r8d, %eax
	andl	$252, %eax
	movq	%rax, -64(%rbp)                 ## 8-byte Spill
	movl	%r9d, %r14d
	andl	$-8, %r14d
	addq	%r14, %r14
	leaq	(,%r8,8), %rax
	movq	%rax, -80(%rbp)                 ## 8-byte Spill
	movq	%rdi, %r12
	movq	%r11, %r13
	xorl	%ecx, %ecx
	movq	%r8, -56(%rbp)                  ## 8-byte Spill
	xorl	%eax, %eax
	movq	%rsi, -104(%rbp)                ## 8-byte Spill
	movl	%r9d, -44(%rbp)                 ## 4-byte Spill
	jmp	LBB1_11
	.p2align	4
LBB1_15:                                ##   in Loop: Header=BB1_11 Depth=2
	incq	%rcx
	movq	-96(%rbp), %rsi                 ## 8-byte Reload
	addq	%rsi, %rax
	addq	%rsi, %r8
	movq	-80(%rbp), %rsi                 ## 8-byte Reload
	addq	%rsi, %r13
	addq	%rsi, %r12
	movq	-104(%rbp), %rsi                ## 8-byte Reload
	cmpq	%rsi, %rcx
	movl	-44(%rbp), %r9d                 ## 4-byte Reload
	je	LBB1_16
LBB1_11:                                ##   Parent Loop BB1_10 Depth=1
                                        ## =>  This Loop Header: Depth=2
                                        ##       Child Loop BB1_13 Depth 3
                                        ##       Child Loop BB1_21 Depth 3
	movq	-88(%rbp), %rsi                 ## 8-byte Reload
	imull	$-1954291199, (%rsi,%rcx,4), %r10d ## imm = 0x8B83E201
	movq	%rax, %rsi
	cmpl	$8, %r9d
	jb	LBB1_21
## %bb.12:                              ##   in Loop: Header=BB1_11 Depth=2
	movq	-64(%rbp), %rsi                 ## 8-byte Reload
	addq	%rax, %rsi
	movd	%r10d, %xmm0
	pshufd	$0, %xmm0, %xmm0                ## xmm0 = xmm0[0,0,0,0]
	xorl	%ebx, %ebx
	.p2align	4
LBB1_13:                                ##   Parent Loop BB1_10 Depth=1
                                        ##     Parent Loop BB1_11 Depth=2
                                        ## =>    This Inner Loop Header: Depth=3
	movdqu	(%r12,%rbx), %xmm3
	movdqu	(%r13,%rbx), %xmm4
	pmulld	%xmm0, %xmm4
	pshufd	$245, %xmm4, %xmm5              ## xmm5 = xmm4[1,1,3,3]
	pmuludq	%xmm1, %xmm5
	movdqa	%xmm4, %xmm6
	pmaxud	%xmm2, %xmm6
	pcmpeqd	%xmm4, %xmm6
	pmuludq	%xmm1, %xmm4
	pshufd	$245, %xmm4, %xmm4              ## xmm4 = xmm4[1,1,3,3]
	pblendw	$204, %xmm5, %xmm4              ## xmm4 = xmm4[0,1],xmm5[2,3],xmm4[4,5],xmm5[6,7]
	pand	%xmm1, %xmm6
	psubd	%xmm4, %xmm6
	movdqa	%xmm6, %xmm4
	paddd	%xmm3, %xmm4
	movdqu	%xmm4, (%r12,%rbx)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, (%r13,%rbx)
	addq	$16, %rbx
	cmpq	%rbx, %r14
	jne	LBB1_13
## %bb.14:                              ##   in Loop: Header=BB1_11 Depth=2
	movq	-64(%rbp), %r9                  ## 8-byte Reload
	cmpl	-56(%rbp), %r9d                 ## 4-byte Folded Reload
	je	LBB1_15
	.p2align	4
LBB1_21:                                ##   Parent Loop BB1_10 Depth=1
                                        ##     Parent Loop BB1_11 Depth=2
                                        ## =>    This Inner Loop Header: Depth=3
	movl	(%rdi,%rsi,4), %ebx
	movl	(%r11,%rsi,4), %r9d
	imull	%r10d, %r9d
	imulq	$7681, %r9, %r15                ## imm = 0x1E01
	shrq	$32, %r15
	cmpl	$559168, %r9d                   ## imm = 0x88840
	movl	$7681, %r9d                     ## imm = 0x1E01
	cmovbl	%edx, %r9d
	subl	%r15d, %r9d
	leal	(%r9,%rbx), %r15d
	movl	%r15d, (%rdi,%rsi,4)
	subl	%r9d, %ebx
	addl	$7681, %ebx                     ## imm = 0x1E01
	movl	%ebx, (%r11,%rsi,4)
	incq	%rsi
	cmpq	%rsi, %r8
	jne	LBB1_21
	jmp	LBB1_15
LBB1_17:
	xorl	%eax, %eax
	movdqa	LCPI1_2(%rip), %xmm3            ## xmm3 = [4294959615,4294959615,4294959615,4294959615]
	xorl	%ecx, %ecx
	.p2align	4
LBB1_18:                                ## =>This Inner Loop Header: Depth=1
	movdqu	(%rdi,%rcx,4), %xmm0
	pshufd	$245, %xmm0, %xmm4              ## xmm4 = xmm0[1,1,3,3]
	pmuldq	%xmm2, %xmm4
	movdqa	%xmm0, %xmm5
	pmuldq	%xmm2, %xmm5
	pshufd	$245, %xmm5, %xmm5              ## xmm5 = xmm5[1,1,3,3]
	pblendw	$204, %xmm4, %xmm5              ## xmm5 = xmm5[0,1],xmm4[2,3],xmm5[4,5],xmm4[6,7]
	pmulld	%xmm3, %xmm5
	paddd	%xmm0, %xmm5
	movdqa	%xmm5, %xmm4
	psrad	$31, %xmm4
	pand	%xmm1, %xmm4
	paddd	%xmm5, %xmm4
	movdqa	%xmm4, %xmm5
	paddd	%xmm3, %xmm5
	movdqa	%xmm1, %xmm0
	pcmpgtd	%xmm4, %xmm0
	blendvps	%xmm0, %xmm4, %xmm5
	movups	%xmm5, (%rdi,%rcx,4)
	addq	$4, %rcx
	cmpq	$256, %rcx                      ## imm = 0x100
	jne	LBB1_18
LBB1_19:
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
LBB1_20:
                                        ## kill: def $eax killed $eax killed $rax
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__literal16,16byte_literals
	.p2align	4, 0x0                          ## -- Begin function OP_intt256
LCPI2_0:
	.quad	7681                            ## 0x1e01
	.quad	7681                            ## 0x1e01
LCPI2_1:
	.long	7681                            ## 0x1e01
	.long	7681                            ## 0x1e01
	.long	7681                            ## 0x1e01
	.long	7681                            ## 0x1e01
LCPI2_2:
	.long	16775032                        ## 0xfff778
	.long	16775032                        ## 0xfff778
	.long	16775032                        ## 0xfff778
	.long	16775032                        ## 0xfff778
LCPI2_3:
	.long	559168                          ## 0x88840
	.long	559168                          ## 0x88840
	.long	559168                          ## 0x88840
	.long	559168                          ## 0x88840
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_OP_intt256
	.p2align	4
_OP_intt256:                            ## @OP_intt256
	.cfi_startproc
## %bb.0:
	movl	$-1, %eax
	cmpl	$1, %edx
	jne	LBB2_20
## %bb.1:
	testq	%rsi, %rsi
	je	LBB2_20
## %bb.2:
	testq	%rdi, %rdi
	je	LBB2_20
## %bb.3:
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
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	movq	_ntt_omega(%rip), %rcx
	movq	%rcx, -56(%rbp)                 ## 8-byte Spill
	testq	%rcx, %rcx
	je	LBB2_19
## %bb.4:
	movq	%rdi, %rax
	subq	%rsi, %rax
	cmpq	$32, %rax
	jae	LBB2_5
## %bb.7:
	xorl	%eax, %eax
	.p2align	4
LBB2_8:                                 ## =>This Inner Loop Header: Depth=1
	movl	(%rsi,%rax,4), %ecx
	movl	%ecx, (%rdi,%rax,4)
	movl	4(%rsi,%rax,4), %ecx
	movl	%ecx, 4(%rdi,%rax,4)
	movl	8(%rsi,%rax,4), %ecx
	movl	%ecx, 8(%rdi,%rax,4)
	movl	12(%rsi,%rax,4), %ecx
	movl	%ecx, 12(%rdi,%rax,4)
	addq	$4, %rax
	cmpq	$256, %rax                      ## imm = 0x100
	jne	LBB2_8
	jmp	LBB2_9
LBB2_5:
	xorl	%eax, %eax
	.p2align	4
LBB2_6:                                 ## =>This Inner Loop Header: Depth=1
	movups	(%rsi,%rax,4), %xmm0
	movups	16(%rsi,%rax,4), %xmm1
	movups	%xmm0, (%rdi,%rax,4)
	movups	%xmm1, 16(%rdi,%rax,4)
	movups	32(%rsi,%rax,4), %xmm0
	movups	48(%rsi,%rax,4), %xmm1
	movups	%xmm0, 32(%rdi,%rax,4)
	movups	%xmm1, 48(%rdi,%rax,4)
	movups	64(%rsi,%rax,4), %xmm0
	movups	80(%rsi,%rax,4), %xmm1
	movups	%xmm0, 64(%rdi,%rax,4)
	movups	%xmm1, 80(%rdi,%rax,4)
	movups	96(%rsi,%rax,4), %xmm0
	movups	112(%rsi,%rax,4), %xmm1
	movups	%xmm0, 96(%rdi,%rax,4)
	movups	%xmm1, 112(%rdi,%rax,4)
	addq	$32, %rax
	cmpq	$256, %rax                      ## imm = 0x100
	jne	LBB2_6
LBB2_9:
	addq	$1024, -56(%rbp)                ## 8-byte Folded Spill
                                        ## imm = 0x400
	movl	$256, %eax                      ## imm = 0x100
	movl	$1, %r9d
	movdqa	LCPI2_0(%rip), %xmm1            ## xmm1 = [7681,7681]
	movdqa	LCPI2_1(%rip), %xmm0            ## xmm0 = [7681,7681,7681,7681]
	movl	$256, %esi                      ## imm = 0x100
	jmp	LBB2_10
	.p2align	4
LBB2_16:                                ##   in Loop: Header=BB2_10 Depth=1
	addl	%r9d, %r9d
	cmpl	$3, -44(%rbp)                   ## 4-byte Folded Reload
	movl	%esi, %eax
	jbe	LBB2_17
LBB2_10:                                ## =>This Loop Header: Depth=1
                                        ##     Child Loop BB2_11 Depth 2
                                        ##       Child Loop BB2_13 Depth 3
                                        ##       Child Loop BB2_21 Depth 3
	movl	%eax, -44(%rbp)                 ## 4-byte Spill
	shrl	%esi
	movl	%r9d, %r9d
	leaq	(%r9,%r9), %rax
	movq	%rax, -96(%rbp)                 ## 8-byte Spill
	movq	-56(%rbp), %rax                 ## 8-byte Reload
	leaq	(%rax,%rsi,4), %rax
	movq	%rax, -88(%rbp)                 ## 8-byte Spill
	leaq	(%rdi,%r9,4), %r11
	movl	%r9d, %eax
	andl	$-4, %eax
	movq	%rax, -64(%rbp)                 ## 8-byte Spill
	leaq	(,%r9,4), %r14
	andq	$-16, %r14
	leaq	(,%r9,8), %rax
	movq	%rax, -80(%rbp)                 ## 8-byte Spill
	movq	%rdi, %r12
	movq	%r11, %r13
	xorl	%ecx, %ecx
	movq	%r9, %rax
	xorl	%edx, %edx
	movq	%r9, -72(%rbp)                  ## 8-byte Spill
	movq	%rsi, -104(%rbp)                ## 8-byte Spill
	jmp	LBB2_11
	.p2align	4
LBB2_15:                                ##   in Loop: Header=BB2_11 Depth=2
	incq	%rcx
	movq	-96(%rbp), %rsi                 ## 8-byte Reload
	addq	%rsi, %rdx
	addq	%rsi, %rax
	movq	-80(%rbp), %rsi                 ## 8-byte Reload
	addq	%rsi, %r13
	addq	%rsi, %r12
	movq	-104(%rbp), %rsi                ## 8-byte Reload
	cmpq	%rsi, %rcx
	movq	-72(%rbp), %r9                  ## 8-byte Reload
	je	LBB2_16
LBB2_11:                                ##   Parent Loop BB2_10 Depth=1
                                        ## =>  This Loop Header: Depth=2
                                        ##       Child Loop BB2_13 Depth 3
                                        ##       Child Loop BB2_21 Depth 3
	movq	-88(%rbp), %rsi                 ## 8-byte Reload
	movslq	(%rsi,%rcx,4), %r8
	movq	%r8, %r10
	movl	$2340676097, %esi               ## imm = 0x8B83E201
	imulq	%rsi, %r10
	movq	%rdx, %rsi
	cmpl	$4, %r9d
	jb	LBB2_21
## %bb.12:                              ##   in Loop: Header=BB2_11 Depth=2
	movq	-64(%rbp), %rsi                 ## 8-byte Reload
	addq	%rdx, %rsi
	movq	%r8, %xmm2
	pshufd	$68, %xmm2, %xmm2               ## xmm2 = xmm2[0,1,0,1]
	movq	%r10, %xmm3
	pshufd	$68, %xmm3, %xmm3               ## xmm3 = xmm3[0,1,0,1]
	movdqa	%xmm2, %xmm4
	psrlq	$32, %xmm4
	xorl	%ebx, %ebx
	.p2align	4
LBB2_13:                                ##   Parent Loop BB2_10 Depth=1
                                        ##     Parent Loop BB2_11 Depth=2
                                        ## =>    This Inner Loop Header: Depth=3
	movdqu	(%r12,%rbx), %xmm5
	movdqu	(%r13,%rbx), %xmm6
	movdqa	%xmm6, %xmm7
	paddd	%xmm5, %xmm7
	movdqu	%xmm7, (%r12,%rbx)
	psubd	%xmm6, %xmm5
	pshufd	$238, %xmm5, %xmm6              ## xmm6 = xmm5[2,3,2,3]
	pmovsxdq	%xmm6, %xmm6
	pmovsxdq	%xmm5, %xmm5
	movdqa	%xmm4, %xmm7
	pmuludq	%xmm5, %xmm7
	movdqa	%xmm5, %xmm8
	psrlq	$32, %xmm8
	pmuludq	%xmm2, %xmm8
	paddq	%xmm7, %xmm8
	psllq	$32, %xmm8
	movdqa	%xmm2, %xmm7
	pmuludq	%xmm5, %xmm7
	paddq	%xmm8, %xmm7
	movdqa	%xmm4, %xmm8
	pmuludq	%xmm6, %xmm8
	movdqa	%xmm6, %xmm9
	psrlq	$32, %xmm9
	pmuludq	%xmm2, %xmm9
	paddq	%xmm8, %xmm9
	psllq	$32, %xmm9
	movdqa	%xmm2, %xmm8
	pmuludq	%xmm6, %xmm8
	paddq	%xmm9, %xmm8
	shufps	$221, %xmm8, %xmm7              ## xmm7 = xmm7[1,3],xmm8[1,3]
	pmuludq	%xmm3, %xmm6
	pmuludq	%xmm3, %xmm5
	pmuludq	%xmm1, %xmm5
	pmuludq	%xmm1, %xmm6
	shufps	$221, %xmm6, %xmm5              ## xmm5 = xmm5[1,3],xmm6[1,3]
	psubd	%xmm5, %xmm7
	movdqa	%xmm7, %xmm5
	psrad	$31, %xmm5
	pand	%xmm0, %xmm5
	paddd	%xmm7, %xmm5
	movdqa	%xmm5, %xmm6
	psrad	$31, %xmm6
	pand	%xmm0, %xmm6
	paddd	%xmm5, %xmm6
	movdqu	%xmm6, (%r13,%rbx)
	addq	$16, %rbx
	cmpq	%rbx, %r14
	jne	LBB2_13
## %bb.14:                              ##   in Loop: Header=BB2_11 Depth=2
	movq	-64(%rbp), %r9                  ## 8-byte Reload
	cmpl	-72(%rbp), %r9d                 ## 4-byte Folded Reload
	je	LBB2_15
	.p2align	4
LBB2_21:                                ##   Parent Loop BB2_10 Depth=1
                                        ##     Parent Loop BB2_11 Depth=2
                                        ## =>    This Inner Loop Header: Depth=3
	movl	(%rdi,%rsi,4), %ebx
	movl	(%r11,%rsi,4), %r9d
	leal	(%r9,%rbx), %r15d
	movl	%r15d, (%rdi,%rsi,4)
	subl	%r9d, %ebx
	movslq	%ebx, %r9
	movq	%r9, %rbx
	imulq	%r8, %rbx
	imull	%r10d, %r9d
	shrq	$32, %rbx
	imulq	$7681, %r9, %r9                 ## imm = 0x1E01
	shrq	$32, %r9
	subl	%r9d, %ebx
	movl	%ebx, %r9d
	sarl	$31, %r9d
	andl	$7681, %r9d                     ## imm = 0x1E01
	addl	%ebx, %r9d
	movl	%r9d, %ebx
	sarl	$31, %ebx
	andl	$7681, %ebx                     ## imm = 0x1E01
	addl	%r9d, %ebx
	movl	%ebx, (%r11,%rsi,4)
	incq	%rsi
	cmpq	%rsi, %rax
	jne	LBB2_21
	jmp	LBB2_15
LBB2_17:
	xorl	%eax, %eax
	movdqa	LCPI2_2(%rip), %xmm1            ## xmm1 = [16775032,16775032,16775032,16775032]
	movdqa	LCPI2_3(%rip), %xmm2            ## xmm2 = [559168,559168,559168,559168]
	xorl	%ecx, %ecx
	.p2align	4
LBB2_18:                                ## =>This Inner Loop Header: Depth=1
	movdqu	(%rdi,%rcx,4), %xmm4
	movdqu	16(%rdi,%rcx,4), %xmm3
	pmulld	%xmm1, %xmm4
	pmulld	%xmm1, %xmm3
	pshufd	$245, %xmm4, %xmm5              ## xmm5 = xmm4[1,1,3,3]
	pmuludq	%xmm0, %xmm5
	movdqa	%xmm4, %xmm6
	pmaxud	%xmm2, %xmm6
	pcmpeqd	%xmm4, %xmm6
	pmuludq	%xmm0, %xmm4
	pshufd	$245, %xmm4, %xmm4              ## xmm4 = xmm4[1,1,3,3]
	pblendw	$204, %xmm5, %xmm4              ## xmm4 = xmm4[0,1],xmm5[2,3],xmm4[4,5],xmm5[6,7]
	pshufd	$245, %xmm3, %xmm5              ## xmm5 = xmm3[1,1,3,3]
	pmuludq	%xmm0, %xmm5
	movdqa	%xmm3, %xmm7
	pmaxud	%xmm2, %xmm7
	pcmpeqd	%xmm3, %xmm7
	pmuludq	%xmm0, %xmm3
	pshufd	$245, %xmm3, %xmm3              ## xmm3 = xmm3[1,1,3,3]
	pblendw	$204, %xmm5, %xmm3              ## xmm3 = xmm3[0,1],xmm5[2,3],xmm3[4,5],xmm5[6,7]
	pand	%xmm0, %xmm6
	psubd	%xmm4, %xmm6
	pand	%xmm0, %xmm7
	psubd	%xmm3, %xmm7
	movdqu	%xmm6, (%rdi,%rcx,4)
	movdqu	%xmm7, 16(%rdi,%rcx,4)
	addq	$8, %rcx
	cmpq	$256, %rcx                      ## imm = 0x100
	jne	LBB2_18
LBB2_19:
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
LBB2_20:
                                        ## kill: def $eax killed $eax killed $rax
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__literal16,16byte_literals
	.p2align	4, 0x0                          ## -- Begin function OP_cwm
LCPI3_0:
	.long	2340676097                      ## 0x8b83e201
	.long	2340676097                      ## 0x8b83e201
	.long	2340676097                      ## 0x8b83e201
	.long	2340676097                      ## 0x8b83e201
LCPI3_1:
	.long	7681                            ## 0x1e01
	.long	7681                            ## 0x1e01
	.long	7681                            ## 0x1e01
	.long	7681                            ## 0x1e01
LCPI3_2:
	.long	559168                          ## 0x88840
	.long	559168                          ## 0x88840
	.long	559168                          ## 0x88840
	.long	559168                          ## 0x88840
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_OP_cwm
	.p2align	4
_OP_cwm:                                ## @OP_cwm
	.cfi_startproc
## %bb.0:
	movl	$-1, %eax
	testl	%r9d, %r9d
	jne	LBB3_13
## %bb.1:
	cmpl	$7681, %r8d                     ## imm = 0x1E01
	jne	LBB3_13
## %bb.2:
	cmpl	$256, %ecx                      ## imm = 0x100
	jne	LBB3_13
## %bb.3:
	testq	%rdx, %rdx
	je	LBB3_13
## %bb.4:
	testq	%rsi, %rsi
	je	LBB3_13
## %bb.5:
	testq	%rdi, %rdi
	je	LBB3_13
## %bb.6:
	cmpq	$0, _ntt_omega(%rip)
	je	LBB3_13
## %bb.7:
	movq	%rdi, %rax
	subq	%rsi, %rax
	cmpq	$16, %rax
	setb	%al
	movq	%rdi, %rcx
	subq	%rdx, %rcx
	cmpq	$16, %rcx
	setb	%cl
	orb	%al, %cl
	je	LBB3_8
## %bb.11:
	xorl	%eax, %eax
	xorl	%ecx, %ecx
	.p2align	4
LBB3_12:                                ## =>This Inner Loop Header: Depth=1
	movl	(%rsi,%rcx,4), %r8d
	imull	(%rdx,%rcx,4), %r8d
	imull	$-1954291199, %r8d, %r8d        ## imm = 0x8B83E201
	imulq	$7681, %r8, %r9                 ## imm = 0x1E01
	shrq	$32, %r9
	cmpl	$559168, %r8d                   ## imm = 0x88840
	movl	$7681, %r8d                     ## imm = 0x1E01
	cmovbl	%eax, %r8d
	subl	%r9d, %r8d
	movl	%r8d, (%rdi,%rcx,4)
	movl	4(%rsi,%rcx,4), %r8d
	imull	4(%rdx,%rcx,4), %r8d
	imull	$-1954291199, %r8d, %r8d        ## imm = 0x8B83E201
	imulq	$7681, %r8, %r9                 ## imm = 0x1E01
	shrq	$32, %r9
	cmpl	$559168, %r8d                   ## imm = 0x88840
	movl	$7681, %r8d                     ## imm = 0x1E01
	cmovbl	%eax, %r8d
	subl	%r9d, %r8d
	movl	%r8d, 4(%rdi,%rcx,4)
	addq	$2, %rcx
	cmpq	$256, %rcx                      ## imm = 0x100
	jne	LBB3_12
LBB3_13:
                                        ## kill: def $eax killed $eax killed $rax
	retq
LBB3_8:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	xorl	%eax, %eax
	movdqa	LCPI3_0(%rip), %xmm0            ## xmm0 = [2340676097,2340676097,2340676097,2340676097]
	movdqa	LCPI3_1(%rip), %xmm1            ## xmm1 = [7681,7681,7681,7681]
	movdqa	LCPI3_2(%rip), %xmm2            ## xmm2 = [559168,559168,559168,559168]
	xorl	%ecx, %ecx
	.p2align	4
LBB3_9:                                 ## =>This Inner Loop Header: Depth=1
	movdqu	(%rsi,%rcx,4), %xmm3
	movdqu	(%rdx,%rcx,4), %xmm4
	pmulld	%xmm3, %xmm4
	pmulld	%xmm0, %xmm4
	pshufd	$245, %xmm4, %xmm3              ## xmm3 = xmm4[1,1,3,3]
	pmuludq	%xmm1, %xmm3
	movdqa	%xmm4, %xmm5
	pmaxud	%xmm2, %xmm5
	pcmpeqd	%xmm4, %xmm5
	pmuludq	%xmm1, %xmm4
	pshufd	$245, %xmm4, %xmm4              ## xmm4 = xmm4[1,1,3,3]
	pblendw	$204, %xmm3, %xmm4              ## xmm4 = xmm4[0,1],xmm3[2,3],xmm4[4,5],xmm3[6,7]
	pand	%xmm1, %xmm5
	psubd	%xmm4, %xmm5
	movdqu	%xmm5, (%rdi,%rcx,4)
	addq	$4, %rcx
	cmpq	$256, %rcx                      ## imm = 0x100
	jne	LBB3_9
## %bb.10:
	popq	%rbp
                                        ## kill: def $eax killed $eax killed $rax
	retq
	.cfi_endproc
                                        ## -- End function
.zerofill __DATA,__bss,_ntt_omega,8,3   ## @ntt_omega
.subsections_via_symbols
