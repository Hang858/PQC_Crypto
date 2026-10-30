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
	.long	2340676097                      ## 0x8b83e201
	.long	2340676097                      ## 0x8b83e201
	.long	2340676097                      ## 0x8b83e201
	.long	2340676097                      ## 0x8b83e201
LCPI1_3:
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
	jne	LBB1_31
## %bb.1:
	testq	%rsi, %rsi
	je	LBB1_31
## %bb.2:
	testq	%rdi, %rdi
	je	LBB1_31
## %bb.3:
	movq	_ntt_omega(%rip), %rcx
	testq	%rcx, %rcx
	je	LBB1_31
## %bb.4:
	movq	%rdi, %rax
	subq	%rsi, %rax
	cmpq	$31, %rax
	ja	LBB1_32
## %bb.5:
	xorl	%eax, %eax
	.p2align	4
LBB1_6:                                 ## =>This Inner Loop Header: Depth=1
	movl	(%rsi,%rax,4), %edx
	movl	%edx, (%rdi,%rax,4)
	movl	4(%rsi,%rax,4), %edx
	movl	%edx, 4(%rdi,%rax,4)
	movl	8(%rsi,%rax,4), %edx
	movl	%edx, 8(%rdi,%rax,4)
	movl	12(%rsi,%rax,4), %edx
	movl	%edx, 12(%rdi,%rax,4)
	addq	$4, %rax
	cmpq	$256, %rax                      ## imm = 0x100
	jne	LBB1_6
	jmp	LBB1_33
LBB1_32:
	movups	(%rsi), %xmm0
	movups	16(%rsi), %xmm1
	movups	%xmm0, (%rdi)
	movups	%xmm1, 16(%rdi)
	movups	32(%rsi), %xmm0
	movups	48(%rsi), %xmm1
	movups	%xmm0, 32(%rdi)
	movups	%xmm1, 48(%rdi)
	movups	64(%rsi), %xmm0
	movups	80(%rsi), %xmm1
	movups	%xmm0, 64(%rdi)
	movups	%xmm1, 80(%rdi)
	movups	96(%rsi), %xmm0
	movups	112(%rsi), %xmm1
	movups	%xmm0, 96(%rdi)
	movups	%xmm1, 112(%rdi)
	movups	128(%rsi), %xmm0
	movups	144(%rsi), %xmm1
	movups	%xmm0, 128(%rdi)
	movups	%xmm1, 144(%rdi)
	movups	160(%rsi), %xmm0
	movups	176(%rsi), %xmm1
	movups	%xmm0, 160(%rdi)
	movups	%xmm1, 176(%rdi)
	movups	192(%rsi), %xmm0
	movups	208(%rsi), %xmm1
	movups	%xmm0, 192(%rdi)
	movups	%xmm1, 208(%rdi)
	movups	224(%rsi), %xmm0
	movups	240(%rsi), %xmm1
	movups	%xmm0, 224(%rdi)
	movups	%xmm1, 240(%rdi)
	movups	256(%rsi), %xmm0
	movups	272(%rsi), %xmm1
	movups	%xmm0, 256(%rdi)
	movups	%xmm1, 272(%rdi)
	movups	288(%rsi), %xmm0
	movups	304(%rsi), %xmm1
	movups	%xmm0, 288(%rdi)
	movups	%xmm1, 304(%rdi)
	movups	320(%rsi), %xmm0
	movups	336(%rsi), %xmm1
	movups	%xmm0, 320(%rdi)
	movups	%xmm1, 336(%rdi)
	movups	352(%rsi), %xmm0
	movups	368(%rsi), %xmm1
	movups	%xmm0, 352(%rdi)
	movups	%xmm1, 368(%rdi)
	movups	384(%rsi), %xmm0
	movups	400(%rsi), %xmm1
	movups	%xmm0, 384(%rdi)
	movups	%xmm1, 400(%rdi)
	movups	416(%rsi), %xmm0
	movups	432(%rsi), %xmm1
	movups	%xmm0, 416(%rdi)
	movups	%xmm1, 432(%rdi)
	movups	448(%rsi), %xmm0
	movups	464(%rsi), %xmm1
	movups	%xmm0, 448(%rdi)
	movups	%xmm1, 464(%rdi)
	movups	480(%rsi), %xmm0
	movups	496(%rsi), %xmm1
	movups	%xmm0, 480(%rdi)
	movups	%xmm1, 496(%rdi)
	movups	512(%rsi), %xmm0
	movups	528(%rsi), %xmm1
	movups	%xmm0, 512(%rdi)
	movups	%xmm1, 528(%rdi)
	movups	544(%rsi), %xmm0
	movups	560(%rsi), %xmm1
	movups	%xmm0, 544(%rdi)
	movups	%xmm1, 560(%rdi)
	movups	576(%rsi), %xmm0
	movups	592(%rsi), %xmm1
	movups	%xmm0, 576(%rdi)
	movups	%xmm1, 592(%rdi)
	movups	608(%rsi), %xmm0
	movups	624(%rsi), %xmm1
	movups	%xmm0, 608(%rdi)
	movups	%xmm1, 624(%rdi)
	movups	640(%rsi), %xmm0
	movups	656(%rsi), %xmm1
	movups	%xmm0, 640(%rdi)
	movups	%xmm1, 656(%rdi)
	movups	672(%rsi), %xmm0
	movups	688(%rsi), %xmm1
	movups	%xmm0, 672(%rdi)
	movups	%xmm1, 688(%rdi)
	movups	704(%rsi), %xmm0
	movups	720(%rsi), %xmm1
	movups	%xmm0, 704(%rdi)
	movups	%xmm1, 720(%rdi)
	movups	736(%rsi), %xmm0
	movups	752(%rsi), %xmm1
	movups	%xmm0, 736(%rdi)
	movups	%xmm1, 752(%rdi)
	movups	768(%rsi), %xmm0
	movups	784(%rsi), %xmm1
	movups	%xmm0, 768(%rdi)
	movups	%xmm1, 784(%rdi)
	movups	800(%rsi), %xmm0
	movups	816(%rsi), %xmm1
	movups	%xmm0, 800(%rdi)
	movups	%xmm1, 816(%rdi)
	movups	832(%rsi), %xmm0
	movups	848(%rsi), %xmm1
	movups	%xmm0, 832(%rdi)
	movups	%xmm1, 848(%rdi)
	movups	864(%rsi), %xmm0
	movups	880(%rsi), %xmm1
	movups	%xmm0, 864(%rdi)
	movups	%xmm1, 880(%rdi)
	movups	896(%rsi), %xmm0
	movups	912(%rsi), %xmm1
	movups	%xmm0, 896(%rdi)
	movups	%xmm1, 912(%rdi)
	movups	928(%rsi), %xmm0
	movups	944(%rsi), %xmm1
	movups	%xmm0, 928(%rdi)
	movups	%xmm1, 944(%rdi)
	movups	960(%rsi), %xmm0
	movups	976(%rsi), %xmm1
	movups	%xmm0, 960(%rdi)
	movups	%xmm1, 976(%rdi)
	movups	992(%rsi), %xmm0
	movups	1008(%rsi), %xmm1
	movups	%xmm0, 992(%rdi)
	movups	%xmm1, 1008(%rdi)
LBB1_33:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r15
	pushq	%r14
	pushq	%rbx
	.cfi_offset %rbx, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	imull	$-1954291199, 4(%rcx), %eax     ## imm = 0x8B83E201
	movd	%eax, %xmm0
	pshufd	$0, %xmm0, %xmm0                ## xmm0 = xmm0[0,0,0,0]
	movl	$128, %eax
	movdqa	LCPI1_0(%rip), %xmm1            ## xmm1 = [7681,7681,7681,7681]
	movdqa	LCPI1_1(%rip), %xmm2            ## xmm2 = [559168,559168,559168,559168]
	.p2align	4
LBB1_34:                                ## =>This Inner Loop Header: Depth=1
	movdqu	-512(%rdi,%rax,4), %xmm3
	movdqu	(%rdi,%rax,4), %xmm4
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
	movdqu	%xmm4, -512(%rdi,%rax,4)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, (%rdi,%rax,4)
	addq	$4, %rax
	cmpq	$256, %rax                      ## imm = 0x100
	jne	LBB1_34
## %bb.35:
	imull	$-1954291199, 8(%rcx), %eax     ## imm = 0x8B83E201
	movd	%eax, %xmm0
	pshufd	$0, %xmm0, %xmm0                ## xmm0 = xmm0[0,0,0,0]
	movl	$64, %eax
	.p2align	4
LBB1_7:                                 ## =>This Inner Loop Header: Depth=1
	movdqu	-256(%rdi,%rax,4), %xmm3
	movdqu	(%rdi,%rax,4), %xmm4
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
	movdqu	%xmm4, -256(%rdi,%rax,4)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, (%rdi,%rax,4)
	addq	$4, %rax
	cmpq	$128, %rax
	jne	LBB1_7
## %bb.8:
	imull	$-1954291199, 12(%rcx), %eax    ## imm = 0x8B83E201
	movd	%eax, %xmm0
	pshufd	$0, %xmm0, %xmm0                ## xmm0 = xmm0[0,0,0,0]
	movl	$192, %eax
	.p2align	4
LBB1_9:                                 ## =>This Inner Loop Header: Depth=1
	movdqu	-256(%rdi,%rax,4), %xmm3
	movdqu	(%rdi,%rax,4), %xmm4
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
	movdqu	%xmm4, -256(%rdi,%rax,4)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, (%rdi,%rax,4)
	addq	$4, %rax
	cmpq	$256, %rax                      ## imm = 0x100
	jne	LBB1_9
## %bb.10:
	imull	$-1954291199, 16(%rcx), %eax    ## imm = 0x8B83E201
	movd	%eax, %xmm0
	pshufd	$0, %xmm0, %xmm0                ## xmm0 = xmm0[0,0,0,0]
	movdqu	128(%rdi), %xmm3
	pmulld	%xmm0, %xmm3
	pshufd	$245, %xmm3, %xmm4              ## xmm4 = xmm3[1,1,3,3]
	pmuludq	%xmm1, %xmm4
	movdqa	%xmm3, %xmm6
	pmaxud	%xmm2, %xmm6
	pcmpeqd	%xmm3, %xmm6
	pmuludq	%xmm1, %xmm3
	pshufd	$245, %xmm3, %xmm3              ## xmm3 = xmm3[1,1,3,3]
	pblendw	$204, %xmm4, %xmm3              ## xmm3 = xmm3[0,1],xmm4[2,3],xmm3[4,5],xmm4[6,7]
	pand	%xmm1, %xmm6
	psubd	%xmm3, %xmm6
	movdqu	(%rdi), %xmm7
	movdqu	16(%rdi), %xmm5
	movdqu	32(%rdi), %xmm4
	movdqu	48(%rdi), %xmm3
	movdqa	%xmm6, %xmm8
	paddd	%xmm7, %xmm8
	movdqu	%xmm8, (%rdi)
	psubd	%xmm6, %xmm7
	paddd	%xmm1, %xmm7
	movdqu	%xmm7, 128(%rdi)
	movdqu	144(%rdi), %xmm6
	pmulld	%xmm0, %xmm6
	pshufd	$245, %xmm6, %xmm7              ## xmm7 = xmm6[1,1,3,3]
	pmuludq	%xmm1, %xmm7
	movdqa	%xmm6, %xmm8
	pmaxud	%xmm2, %xmm8
	pcmpeqd	%xmm6, %xmm8
	pmuludq	%xmm1, %xmm6
	pshufd	$245, %xmm6, %xmm6              ## xmm6 = xmm6[1,1,3,3]
	pblendw	$204, %xmm7, %xmm6              ## xmm6 = xmm6[0,1],xmm7[2,3],xmm6[4,5],xmm7[6,7]
	pand	%xmm1, %xmm8
	psubd	%xmm6, %xmm8
	movdqa	%xmm8, %xmm6
	paddd	%xmm5, %xmm6
	movdqu	%xmm6, 16(%rdi)
	psubd	%xmm8, %xmm5
	paddd	%xmm1, %xmm5
	movdqu	%xmm5, 144(%rdi)
	movdqu	160(%rdi), %xmm5
	pmulld	%xmm0, %xmm5
	pshufd	$245, %xmm5, %xmm6              ## xmm6 = xmm5[1,1,3,3]
	pmuludq	%xmm1, %xmm6
	movdqa	%xmm5, %xmm7
	pmaxud	%xmm2, %xmm7
	pcmpeqd	%xmm5, %xmm7
	pmuludq	%xmm1, %xmm5
	pshufd	$245, %xmm5, %xmm5              ## xmm5 = xmm5[1,1,3,3]
	pblendw	$204, %xmm6, %xmm5              ## xmm5 = xmm5[0,1],xmm6[2,3],xmm5[4,5],xmm6[6,7]
	pand	%xmm1, %xmm7
	psubd	%xmm5, %xmm7
	movdqa	%xmm7, %xmm5
	paddd	%xmm4, %xmm5
	movdqu	%xmm5, 32(%rdi)
	psubd	%xmm7, %xmm4
	paddd	%xmm1, %xmm4
	movdqu	%xmm4, 160(%rdi)
	movdqu	176(%rdi), %xmm4
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
	movdqu	%xmm4, 48(%rdi)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 176(%rdi)
	movdqu	64(%rdi), %xmm3
	movdqu	192(%rdi), %xmm4
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
	movdqu	%xmm4, 64(%rdi)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 192(%rdi)
	movdqu	80(%rdi), %xmm3
	movdqu	208(%rdi), %xmm4
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
	movdqu	%xmm4, 80(%rdi)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 208(%rdi)
	movdqu	96(%rdi), %xmm3
	movdqu	224(%rdi), %xmm4
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
	movdqu	%xmm4, 96(%rdi)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 224(%rdi)
	movdqu	112(%rdi), %xmm3
	movdqu	240(%rdi), %xmm4
	pmulld	%xmm0, %xmm4
	pshufd	$245, %xmm4, %xmm0              ## xmm0 = xmm4[1,1,3,3]
	pmuludq	%xmm1, %xmm0
	movdqa	%xmm4, %xmm5
	pmaxud	%xmm2, %xmm5
	pcmpeqd	%xmm4, %xmm5
	pmuludq	%xmm1, %xmm4
	pshufd	$245, %xmm4, %xmm4              ## xmm4 = xmm4[1,1,3,3]
	pblendw	$204, %xmm0, %xmm4              ## xmm4 = xmm4[0,1],xmm0[2,3],xmm4[4,5],xmm0[6,7]
	pand	%xmm1, %xmm5
	psubd	%xmm4, %xmm5
	movdqa	%xmm5, %xmm0
	paddd	%xmm3, %xmm0
	movdqu	%xmm0, 112(%rdi)
	psubd	%xmm5, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 240(%rdi)
	imull	$-1954291199, 20(%rcx), %eax    ## imm = 0x8B83E201
	movd	%eax, %xmm0
	pshufd	$0, %xmm0, %xmm0                ## xmm0 = xmm0[0,0,0,0]
	movdqu	256(%rdi), %xmm3
	movdqu	384(%rdi), %xmm4
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
	movdqu	%xmm4, 256(%rdi)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 384(%rdi)
	movdqu	272(%rdi), %xmm3
	movdqu	400(%rdi), %xmm4
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
	movdqu	%xmm4, 272(%rdi)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 400(%rdi)
	movdqu	288(%rdi), %xmm3
	movdqu	416(%rdi), %xmm4
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
	movdqu	%xmm4, 288(%rdi)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 416(%rdi)
	movdqu	304(%rdi), %xmm3
	movdqu	432(%rdi), %xmm4
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
	movdqu	%xmm4, 304(%rdi)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 432(%rdi)
	movdqu	320(%rdi), %xmm3
	movdqu	448(%rdi), %xmm4
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
	movdqu	%xmm4, 320(%rdi)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 448(%rdi)
	movdqu	336(%rdi), %xmm3
	movdqu	464(%rdi), %xmm4
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
	movdqu	%xmm4, 336(%rdi)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 464(%rdi)
	movdqu	352(%rdi), %xmm3
	movdqu	480(%rdi), %xmm4
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
	movdqu	%xmm4, 352(%rdi)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 480(%rdi)
	movdqu	368(%rdi), %xmm3
	movdqu	496(%rdi), %xmm4
	pmulld	%xmm0, %xmm4
	pshufd	$245, %xmm4, %xmm0              ## xmm0 = xmm4[1,1,3,3]
	pmuludq	%xmm1, %xmm0
	movdqa	%xmm4, %xmm5
	pmaxud	%xmm2, %xmm5
	pcmpeqd	%xmm4, %xmm5
	pmuludq	%xmm1, %xmm4
	pshufd	$245, %xmm4, %xmm4              ## xmm4 = xmm4[1,1,3,3]
	pblendw	$204, %xmm0, %xmm4              ## xmm4 = xmm4[0,1],xmm0[2,3],xmm4[4,5],xmm0[6,7]
	pand	%xmm1, %xmm5
	psubd	%xmm4, %xmm5
	movdqa	%xmm5, %xmm0
	paddd	%xmm3, %xmm0
	movdqu	%xmm0, 368(%rdi)
	psubd	%xmm5, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 496(%rdi)
	imull	$-1954291199, 24(%rcx), %eax    ## imm = 0x8B83E201
	movd	%eax, %xmm0
	pshufd	$0, %xmm0, %xmm0                ## xmm0 = xmm0[0,0,0,0]
	movdqu	512(%rdi), %xmm3
	movdqu	640(%rdi), %xmm4
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
	movdqu	%xmm4, 512(%rdi)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 640(%rdi)
	movdqu	528(%rdi), %xmm3
	movdqu	656(%rdi), %xmm4
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
	movdqu	%xmm4, 528(%rdi)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 656(%rdi)
	movdqu	544(%rdi), %xmm3
	movdqu	672(%rdi), %xmm4
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
	movdqu	%xmm4, 544(%rdi)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 672(%rdi)
	movdqu	560(%rdi), %xmm3
	movdqu	688(%rdi), %xmm4
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
	movdqu	%xmm4, 560(%rdi)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 688(%rdi)
	movdqu	576(%rdi), %xmm3
	movdqu	704(%rdi), %xmm4
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
	movdqu	%xmm4, 576(%rdi)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 704(%rdi)
	movdqu	592(%rdi), %xmm3
	movdqu	720(%rdi), %xmm4
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
	movdqu	%xmm4, 592(%rdi)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 720(%rdi)
	movdqu	608(%rdi), %xmm3
	movdqu	736(%rdi), %xmm4
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
	movdqu	%xmm4, 608(%rdi)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 736(%rdi)
	movdqu	624(%rdi), %xmm3
	movdqu	752(%rdi), %xmm4
	pmulld	%xmm0, %xmm4
	pshufd	$245, %xmm4, %xmm0              ## xmm0 = xmm4[1,1,3,3]
	pmuludq	%xmm1, %xmm0
	movdqa	%xmm4, %xmm5
	pmaxud	%xmm2, %xmm5
	pcmpeqd	%xmm4, %xmm5
	pmuludq	%xmm1, %xmm4
	pshufd	$245, %xmm4, %xmm4              ## xmm4 = xmm4[1,1,3,3]
	pblendw	$204, %xmm0, %xmm4              ## xmm4 = xmm4[0,1],xmm0[2,3],xmm4[4,5],xmm0[6,7]
	pand	%xmm1, %xmm5
	psubd	%xmm4, %xmm5
	movdqa	%xmm5, %xmm0
	paddd	%xmm3, %xmm0
	movdqu	%xmm0, 624(%rdi)
	psubd	%xmm5, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 752(%rdi)
	imull	$-1954291199, 28(%rcx), %eax    ## imm = 0x8B83E201
	movd	%eax, %xmm0
	pshufd	$0, %xmm0, %xmm0                ## xmm0 = xmm0[0,0,0,0]
	movdqu	768(%rdi), %xmm3
	movdqu	896(%rdi), %xmm4
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
	movdqu	%xmm4, 768(%rdi)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 896(%rdi)
	movdqu	784(%rdi), %xmm3
	movdqu	912(%rdi), %xmm4
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
	movdqu	%xmm4, 784(%rdi)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 912(%rdi)
	movdqu	800(%rdi), %xmm3
	movdqu	928(%rdi), %xmm4
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
	movdqu	%xmm4, 800(%rdi)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 928(%rdi)
	movdqu	816(%rdi), %xmm3
	movdqu	944(%rdi), %xmm4
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
	movdqu	%xmm4, 816(%rdi)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 944(%rdi)
	movdqu	832(%rdi), %xmm3
	movdqu	960(%rdi), %xmm4
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
	movdqu	%xmm4, 832(%rdi)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 960(%rdi)
	movdqu	848(%rdi), %xmm3
	movdqu	976(%rdi), %xmm4
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
	movdqu	%xmm4, 848(%rdi)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 976(%rdi)
	movdqu	864(%rdi), %xmm3
	movdqu	992(%rdi), %xmm4
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
	movdqu	%xmm4, 864(%rdi)
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 992(%rdi)
	movdqu	880(%rdi), %xmm3
	movdqu	1008(%rdi), %xmm4
	pmulld	%xmm0, %xmm4
	pshufd	$245, %xmm4, %xmm0              ## xmm0 = xmm4[1,1,3,3]
	pmuludq	%xmm1, %xmm0
	movdqa	%xmm4, %xmm5
	pmaxud	%xmm2, %xmm5
	pcmpeqd	%xmm4, %xmm5
	pmuludq	%xmm1, %xmm4
	pshufd	$245, %xmm4, %xmm4              ## xmm4 = xmm4[1,1,3,3]
	pblendw	$204, %xmm0, %xmm4              ## xmm4 = xmm4[0,1],xmm0[2,3],xmm4[4,5],xmm0[6,7]
	pand	%xmm1, %xmm5
	psubd	%xmm4, %xmm5
	movdqa	%xmm5, %xmm0
	paddd	%xmm3, %xmm0
	movdqu	%xmm0, 880(%rdi)
	psubd	%xmm5, %xmm3
	paddd	%xmm1, %xmm3
	movdqu	%xmm3, 1008(%rdi)
	leaq	32(%rcx), %rax
	movl	$28, %edx
	.p2align	4
LBB1_11:                                ## =>This Inner Loop Header: Depth=1
	imull	$-1954291199, (%rax), %esi      ## imm = 0x8B83E201
	movdqu	-48(%rdi,%rdx,4), %xmm0
	movd	%esi, %xmm3
	pshufd	$0, %xmm3, %xmm3                ## xmm3 = xmm3[0,0,0,0]
	pmulld	%xmm3, %xmm0
	pshufd	$245, %xmm0, %xmm4              ## xmm4 = xmm0[1,1,3,3]
	pmuludq	%xmm1, %xmm4
	movdqa	%xmm0, %xmm6
	pmaxud	%xmm2, %xmm6
	pcmpeqd	%xmm0, %xmm6
	pmuludq	%xmm1, %xmm0
	pshufd	$245, %xmm0, %xmm0              ## xmm0 = xmm0[1,1,3,3]
	pblendw	$204, %xmm4, %xmm0              ## xmm0 = xmm0[0,1],xmm4[2,3],xmm0[4,5],xmm4[6,7]
	pand	%xmm1, %xmm6
	psubd	%xmm0, %xmm6
	movdqu	-112(%rdi,%rdx,4), %xmm7
	movdqu	-96(%rdi,%rdx,4), %xmm5
	movdqu	-80(%rdi,%rdx,4), %xmm4
	movdqu	-64(%rdi,%rdx,4), %xmm0
	movdqa	%xmm6, %xmm8
	paddd	%xmm7, %xmm8
	movdqu	%xmm8, -112(%rdi,%rdx,4)
	psubd	%xmm6, %xmm7
	paddd	%xmm1, %xmm7
	movdqu	%xmm7, -48(%rdi,%rdx,4)
	movdqu	-32(%rdi,%rdx,4), %xmm6
	pmulld	%xmm3, %xmm6
	pshufd	$245, %xmm6, %xmm7              ## xmm7 = xmm6[1,1,3,3]
	pmuludq	%xmm1, %xmm7
	movdqa	%xmm6, %xmm8
	pmaxud	%xmm2, %xmm8
	pcmpeqd	%xmm6, %xmm8
	pmuludq	%xmm1, %xmm6
	pshufd	$245, %xmm6, %xmm6              ## xmm6 = xmm6[1,1,3,3]
	pblendw	$204, %xmm7, %xmm6              ## xmm6 = xmm6[0,1],xmm7[2,3],xmm6[4,5],xmm7[6,7]
	pand	%xmm1, %xmm8
	psubd	%xmm6, %xmm8
	movdqa	%xmm8, %xmm6
	paddd	%xmm5, %xmm6
	movdqu	%xmm6, -96(%rdi,%rdx,4)
	psubd	%xmm8, %xmm5
	paddd	%xmm1, %xmm5
	movdqu	%xmm5, -32(%rdi,%rdx,4)
	movdqu	-16(%rdi,%rdx,4), %xmm5
	pmulld	%xmm3, %xmm5
	pshufd	$245, %xmm5, %xmm6              ## xmm6 = xmm5[1,1,3,3]
	pmuludq	%xmm1, %xmm6
	movdqa	%xmm5, %xmm7
	pmaxud	%xmm2, %xmm7
	pcmpeqd	%xmm5, %xmm7
	pmuludq	%xmm1, %xmm5
	pshufd	$245, %xmm5, %xmm5              ## xmm5 = xmm5[1,1,3,3]
	pblendw	$204, %xmm6, %xmm5              ## xmm5 = xmm5[0,1],xmm6[2,3],xmm5[4,5],xmm6[6,7]
	pand	%xmm1, %xmm7
	psubd	%xmm5, %xmm7
	movdqa	%xmm7, %xmm5
	paddd	%xmm4, %xmm5
	movdqu	%xmm5, -80(%rdi,%rdx,4)
	psubd	%xmm7, %xmm4
	paddd	%xmm1, %xmm4
	movdqu	%xmm4, -16(%rdi,%rdx,4)
	movdqu	(%rdi,%rdx,4), %xmm4
	pmulld	%xmm3, %xmm4
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
	movdqa	%xmm5, %xmm3
	paddd	%xmm0, %xmm3
	movdqu	%xmm3, -64(%rdi,%rdx,4)
	psubd	%xmm5, %xmm0
	paddd	%xmm1, %xmm0
	movdqu	%xmm0, (%rdi,%rdx,4)
	addq	$4, %rax
	addq	$32, %rdx
	cmpq	$284, %rdx                      ## imm = 0x11C
	jne	LBB1_11
## %bb.12:
	leaq	64(%rcx), %rax
	movl	$12, %edx
	.p2align	4
LBB1_13:                                ## =>This Inner Loop Header: Depth=1
	imull	$-1954291199, (%rax), %esi      ## imm = 0x8B83E201
	movd	%esi, %xmm0
	pshufd	$0, %xmm0, %xmm4                ## xmm4 = xmm0[0,0,0,0]
	movdqu	-48(%rdi,%rdx,4), %xmm5
	movdqu	-32(%rdi,%rdx,4), %xmm0
	movdqu	-16(%rdi,%rdx,4), %xmm6
	movdqu	(%rdi,%rdx,4), %xmm3
	pmulld	%xmm4, %xmm6
	pshufd	$245, %xmm6, %xmm7              ## xmm7 = xmm6[1,1,3,3]
	pmuludq	%xmm1, %xmm7
	movdqa	%xmm6, %xmm8
	pmaxud	%xmm2, %xmm8
	pcmpeqd	%xmm6, %xmm8
	pmuludq	%xmm1, %xmm6
	pshufd	$245, %xmm6, %xmm6              ## xmm6 = xmm6[1,1,3,3]
	pblendw	$204, %xmm7, %xmm6              ## xmm6 = xmm6[0,1],xmm7[2,3],xmm6[4,5],xmm7[6,7]
	pand	%xmm1, %xmm8
	psubd	%xmm6, %xmm8
	movdqa	%xmm8, %xmm6
	paddd	%xmm5, %xmm6
	movdqu	%xmm6, -48(%rdi,%rdx,4)
	psubd	%xmm8, %xmm5
	paddd	%xmm1, %xmm5
	movdqu	%xmm5, -16(%rdi,%rdx,4)
	pmulld	%xmm4, %xmm3
	pshufd	$245, %xmm3, %xmm4              ## xmm4 = xmm3[1,1,3,3]
	pmuludq	%xmm1, %xmm4
	movdqa	%xmm3, %xmm5
	pmaxud	%xmm2, %xmm5
	pcmpeqd	%xmm3, %xmm5
	pmuludq	%xmm1, %xmm3
	pshufd	$245, %xmm3, %xmm3              ## xmm3 = xmm3[1,1,3,3]
	pblendw	$204, %xmm4, %xmm3              ## xmm3 = xmm3[0,1],xmm4[2,3],xmm3[4,5],xmm4[6,7]
	pand	%xmm1, %xmm5
	psubd	%xmm3, %xmm5
	movdqa	%xmm5, %xmm3
	paddd	%xmm0, %xmm3
	movdqu	%xmm3, -32(%rdi,%rdx,4)
	psubd	%xmm5, %xmm0
	paddd	%xmm1, %xmm0
	movdqu	%xmm0, (%rdi,%rdx,4)
	addq	$4, %rax
	addq	$16, %rdx
	cmpq	$268, %rdx                      ## imm = 0x10C
	jne	LBB1_13
## %bb.14:
	leaq	128(%rcx), %rsi
	leaq	1024(%rdi), %rax
	leaq	256(%rcx), %rdx
	cmpq	%rdx, %rdi
	setb	%r8b
	cmpq	%rax, %rsi
	setb	%r9b
	testb	%r9b, %r8b
	je	LBB1_15
## %bb.17:
	xorl	%r8d, %r8d
	.p2align	4
LBB1_18:                                ## =>This Inner Loop Header: Depth=1
	imull	$-1954291199, (%rsi,%r8), %r9d  ## imm = 0x8B83E201
	movdqu	(%rdi,%r8,8), %xmm0
	movdqu	16(%rdi,%r8,8), %xmm3
	movd	%r9d, %xmm4
	pshufd	$0, %xmm4, %xmm4                ## xmm4 = xmm4[0,0,0,0]
	pmulld	%xmm3, %xmm4
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
	movdqa	%xmm5, %xmm3
	paddd	%xmm0, %xmm3
	movdqu	%xmm3, (%rdi,%r8,8)
	psubd	%xmm5, %xmm0
	paddd	%xmm1, %xmm0
	movdqu	%xmm0, 16(%rdi,%r8,8)
	addq	$4, %r8
	cmpq	$128, %r8
	jne	LBB1_18
	jmp	LBB1_19
LBB1_15:
	xorl	%r8d, %r8d
	movdqa	LCPI1_2(%rip), %xmm0            ## xmm0 = [2340676097,2340676097,2340676097,2340676097]
	.p2align	4
LBB1_16:                                ## =>This Inner Loop Header: Depth=1
	movdqu	(%rsi,%r8), %xmm7
	movd	(%rdi,%r8,8), %xmm3             ## xmm3 = mem[0],zero,zero,zero
	pinsrd	$1, 32(%rdi,%r8,8), %xmm3
	pinsrd	$2, 64(%rdi,%r8,8), %xmm3
	pinsrd	$3, 96(%rdi,%r8,8), %xmm3
	pmulld	%xmm0, %xmm7
	movd	16(%rdi,%r8,8), %xmm4           ## xmm4 = mem[0],zero,zero,zero
	pinsrd	$1, 48(%rdi,%r8,8), %xmm4
	pinsrd	$2, 80(%rdi,%r8,8), %xmm4
	pinsrd	$3, 112(%rdi,%r8,8), %xmm4
	pmulld	%xmm7, %xmm4
	pshufd	$245, %xmm4, %xmm5              ## xmm5 = xmm4[1,1,3,3]
	pmuludq	%xmm1, %xmm5
	movdqa	%xmm4, %xmm6
	pmaxud	%xmm2, %xmm6
	pcmpeqd	%xmm4, %xmm6
	pmuludq	%xmm1, %xmm4
	pshufd	$245, %xmm4, %xmm8              ## xmm8 = xmm4[1,1,3,3]
	pblendw	$204, %xmm5, %xmm8              ## xmm8 = xmm8[0,1],xmm5[2,3],xmm8[4,5],xmm5[6,7]
	pand	%xmm1, %xmm6
	movd	4(%rdi,%r8,8), %xmm4            ## xmm4 = mem[0],zero,zero,zero
	pinsrd	$1, 36(%rdi,%r8,8), %xmm4
	pinsrd	$2, 68(%rdi,%r8,8), %xmm4
	psubd	%xmm8, %xmm6
	pinsrd	$3, 100(%rdi,%r8,8), %xmm4
	movd	20(%rdi,%r8,8), %xmm5           ## xmm5 = mem[0],zero,zero,zero
	pinsrd	$1, 52(%rdi,%r8,8), %xmm5
	pinsrd	$2, 84(%rdi,%r8,8), %xmm5
	pinsrd	$3, 116(%rdi,%r8,8), %xmm5
	pmulld	%xmm7, %xmm5
	pshufd	$245, %xmm5, %xmm8              ## xmm8 = xmm5[1,1,3,3]
	pmuludq	%xmm1, %xmm8
	movdqa	%xmm5, %xmm9
	pmaxud	%xmm2, %xmm9
	pcmpeqd	%xmm5, %xmm9
	pmuludq	%xmm1, %xmm5
	pshufd	$245, %xmm5, %xmm10             ## xmm10 = xmm5[1,1,3,3]
	pblendw	$204, %xmm8, %xmm10             ## xmm10 = xmm10[0,1],xmm8[2,3],xmm10[4,5],xmm8[6,7]
	pand	%xmm1, %xmm9
	movd	8(%rdi,%r8,8), %xmm5            ## xmm5 = mem[0],zero,zero,zero
	pinsrd	$1, 40(%rdi,%r8,8), %xmm5
	pinsrd	$2, 72(%rdi,%r8,8), %xmm5
	pinsrd	$3, 104(%rdi,%r8,8), %xmm5
	psubd	%xmm10, %xmm9
	movd	24(%rdi,%r8,8), %xmm8           ## xmm8 = mem[0],zero,zero,zero
	pinsrd	$1, 56(%rdi,%r8,8), %xmm8
	pinsrd	$2, 88(%rdi,%r8,8), %xmm8
	pinsrd	$3, 120(%rdi,%r8,8), %xmm8
	pmulld	%xmm7, %xmm8
	pshufd	$245, %xmm8, %xmm11             ## xmm11 = xmm8[1,1,3,3]
	pmuludq	%xmm1, %xmm11
	movdqa	%xmm8, %xmm10
	pmaxud	%xmm2, %xmm10
	pcmpeqd	%xmm8, %xmm10
	pmuludq	%xmm1, %xmm8
	pshufd	$245, %xmm8, %xmm12             ## xmm12 = xmm8[1,1,3,3]
	pblendw	$204, %xmm11, %xmm12            ## xmm12 = xmm12[0,1],xmm11[2,3],xmm12[4,5],xmm11[6,7]
	pand	%xmm1, %xmm10
	movd	12(%rdi,%r8,8), %xmm8           ## xmm8 = mem[0],zero,zero,zero
	pinsrd	$1, 44(%rdi,%r8,8), %xmm8
	pinsrd	$2, 76(%rdi,%r8,8), %xmm8
	psubd	%xmm12, %xmm10
	pinsrd	$3, 108(%rdi,%r8,8), %xmm8
	movd	28(%rdi,%r8,8), %xmm11          ## xmm11 = mem[0],zero,zero,zero
	pinsrd	$1, 60(%rdi,%r8,8), %xmm11
	pinsrd	$2, 92(%rdi,%r8,8), %xmm11
	pinsrd	$3, 124(%rdi,%r8,8), %xmm11
	pmulld	%xmm7, %xmm11
	pshufd	$245, %xmm11, %xmm7             ## xmm7 = xmm11[1,1,3,3]
	pmuludq	%xmm1, %xmm7
	movdqa	%xmm11, %xmm12
	pmaxud	%xmm2, %xmm12
	pcmpeqd	%xmm11, %xmm12
	pmuludq	%xmm1, %xmm11
	pshufd	$245, %xmm11, %xmm11            ## xmm11 = xmm11[1,1,3,3]
	pblendw	$204, %xmm7, %xmm11             ## xmm11 = xmm11[0,1],xmm7[2,3],xmm11[4,5],xmm7[6,7]
	pand	%xmm1, %xmm12
	psubd	%xmm11, %xmm12
	movdqa	%xmm6, %xmm7
	paddd	%xmm3, %xmm7
	movdqa	%xmm9, %xmm11
	paddd	%xmm4, %xmm11
	psubd	%xmm9, %xmm4
	movdqa	%xmm10, %xmm9
	paddd	%xmm5, %xmm9
	psubd	%xmm6, %xmm3
	movdqa	%xmm12, %xmm6
	paddd	%xmm8, %xmm6
	paddd	%xmm1, %xmm4
	paddd	%xmm1, %xmm3
	psubd	%xmm12, %xmm8
	paddd	%xmm1, %xmm8
	psubd	%xmm10, %xmm5
	paddd	%xmm1, %xmm5
	pshufd	$85, %xmm3, %xmm10              ## xmm10 = xmm3[1,1,1,1]
	pblendw	$12, %xmm4, %xmm10              ## xmm10 = xmm10[0,1],xmm4[2,3],xmm10[4,5,6,7]
	movdqa	%xmm3, %xmm12
	punpckhdq	%xmm4, %xmm12           ## xmm12 = xmm12[2],xmm4[2],xmm12[3],xmm4[3]
	punpckldq	%xmm4, %xmm3            ## xmm3 = xmm3[0],xmm4[0],xmm3[1],xmm4[1]
	movdqa	%xmm5, %xmm4
	punpckldq	%xmm8, %xmm4            ## xmm4 = xmm4[0],xmm8[0],xmm4[1],xmm8[1]
	pblendw	$240, %xmm4, %xmm10             ## xmm10 = xmm10[0,1,2,3],xmm4[4,5,6,7]
	punpcklqdq	%xmm4, %xmm3            ## xmm3 = xmm3[0],xmm4[0]
	pshufd	$170, %xmm8, %xmm4              ## xmm4 = xmm8[2,2,2,2]
	pblendw	$63, %xmm5, %xmm4               ## xmm4 = xmm5[0,1,2,3,4,5],xmm4[6,7]
	pblendw	$15, %xmm12, %xmm4              ## xmm4 = xmm12[0,1,2,3],xmm4[4,5,6,7]
	punpckhdq	%xmm8, %xmm5            ## xmm5 = xmm5[2],xmm8[2],xmm5[3],xmm8[3]
	pshufd	$85, %xmm7, %xmm8               ## xmm8 = xmm7[1,1,1,1]
	pblendw	$12, %xmm11, %xmm8              ## xmm8 = xmm8[0,1],xmm11[2,3],xmm8[4,5,6,7]
	punpckhqdq	%xmm5, %xmm12           ## xmm12 = xmm12[1],xmm5[1]
	movdqa	%xmm7, %xmm5
	punpckhdq	%xmm11, %xmm5           ## xmm5 = xmm5[2],xmm11[2],xmm5[3],xmm11[3]
	punpckldq	%xmm11, %xmm7           ## xmm7 = xmm7[0],xmm11[0],xmm7[1],xmm11[1]
	movdqa	%xmm9, %xmm11
	punpckldq	%xmm6, %xmm11           ## xmm11 = xmm11[0],xmm6[0],xmm11[1],xmm6[1]
	pblendw	$240, %xmm11, %xmm8             ## xmm8 = xmm8[0,1,2,3],xmm11[4,5,6,7]
	punpcklqdq	%xmm11, %xmm7           ## xmm7 = xmm7[0],xmm11[0]
	pshufd	$170, %xmm6, %xmm11             ## xmm11 = xmm6[2,2,2,2]
	pblendw	$63, %xmm9, %xmm11              ## xmm11 = xmm9[0,1,2,3,4,5],xmm11[6,7]
	punpckhdq	%xmm6, %xmm9            ## xmm9 = xmm9[2],xmm6[2],xmm9[3],xmm6[3]
	pblendw	$15, %xmm5, %xmm11              ## xmm11 = xmm5[0,1,2,3],xmm11[4,5,6,7]
	punpckhqdq	%xmm9, %xmm5            ## xmm5 = xmm5[1],xmm9[1]
	movdqu	%xmm5, 96(%rdi,%r8,8)
	movdqu	%xmm7, (%rdi,%r8,8)
	movdqu	%xmm12, 112(%rdi,%r8,8)
	movdqu	%xmm11, 64(%rdi,%r8,8)
	movdqu	%xmm8, 32(%rdi,%r8,8)
	movdqu	%xmm3, 16(%rdi,%r8,8)
	movdqu	%xmm4, 80(%rdi,%r8,8)
	movdqu	%xmm10, 48(%rdi,%r8,8)
	addq	$16, %r8
	cmpq	$128, %r8
	jne	LBB1_16
LBB1_19:
	leaq	512(%rcx), %rsi
	cmpq	%rsi, %rdi
	setb	%r8b
	cmpq	%rax, %rdx
	setb	%r9b
	testb	%r9b, %r8b
	je	LBB1_20
## %bb.22:
	xorl	%r8d, %r8d
	xorl	%r9d, %r9d
	.p2align	4
LBB1_23:                                ## =>This Inner Loop Header: Depth=1
	imull	$-1954291199, (%rdx,%r9), %r10d ## imm = 0x8B83E201
	movl	8(%rdi,%r9,4), %r11d
	imull	%r10d, %r11d
	imulq	$7681, %r11, %rbx               ## imm = 0x1E01
	shrq	$32, %rbx
	cmpl	$559168, %r11d                  ## imm = 0x88840
	movl	$7681, %r14d                    ## imm = 0x1E01
	cmovbl	%r8d, %r14d
	subl	%ebx, %r14d
	movl	(%rdi,%r9,4), %ebx
	movl	4(%rdi,%r9,4), %r11d
	leal	(%r14,%rbx), %r15d
	movl	%r15d, (%rdi,%r9,4)
	subl	%r14d, %ebx
	addl	$7681, %ebx                     ## imm = 0x1E01
	movl	%ebx, 8(%rdi,%r9,4)
	imull	12(%rdi,%r9,4), %r10d
	imulq	$7681, %r10, %rbx               ## imm = 0x1E01
	shrq	$32, %rbx
	cmpl	$559168, %r10d                  ## imm = 0x88840
	movl	$7681, %r10d                    ## imm = 0x1E01
	cmovbl	%r8d, %r10d
	subl	%ebx, %r10d
	leal	(%r10,%r11), %ebx
	movl	%ebx, 4(%rdi,%r9,4)
	subl	%r10d, %r11d
	addl	$7681, %r11d                    ## imm = 0x1E01
	movl	%r11d, 12(%rdi,%r9,4)
	addq	$4, %r9
	cmpq	$256, %r9                       ## imm = 0x100
	jne	LBB1_23
	jmp	LBB1_24
LBB1_20:
	xorl	%r8d, %r8d
	movdqa	LCPI1_2(%rip), %xmm0            ## xmm0 = [2340676097,2340676097,2340676097,2340676097]
	.p2align	4
LBB1_21:                                ## =>This Inner Loop Header: Depth=1
	movdqu	(%rdx,%r8), %xmm5
	movd	(%rdi,%r8,4), %xmm3             ## xmm3 = mem[0],zero,zero,zero
	pinsrd	$1, 16(%rdi,%r8,4), %xmm3
	pinsrd	$2, 32(%rdi,%r8,4), %xmm3
	pmulld	%xmm0, %xmm5
	pinsrd	$3, 48(%rdi,%r8,4), %xmm3
	movd	8(%rdi,%r8,4), %xmm4            ## xmm4 = mem[0],zero,zero,zero
	pinsrd	$1, 24(%rdi,%r8,4), %xmm4
	pinsrd	$2, 40(%rdi,%r8,4), %xmm4
	pinsrd	$3, 56(%rdi,%r8,4), %xmm4
	pmulld	%xmm5, %xmm4
	pshufd	$245, %xmm4, %xmm7              ## xmm7 = xmm4[1,1,3,3]
	pmuludq	%xmm1, %xmm7
	movdqa	%xmm4, %xmm6
	pmaxud	%xmm2, %xmm6
	pcmpeqd	%xmm4, %xmm6
	pmuludq	%xmm1, %xmm4
	pshufd	$245, %xmm4, %xmm8              ## xmm8 = xmm4[1,1,3,3]
	pblendw	$204, %xmm7, %xmm8              ## xmm8 = xmm8[0,1],xmm7[2,3],xmm8[4,5],xmm7[6,7]
	pand	%xmm1, %xmm6
	movd	4(%rdi,%r8,4), %xmm4            ## xmm4 = mem[0],zero,zero,zero
	pinsrd	$1, 20(%rdi,%r8,4), %xmm4
	pinsrd	$2, 36(%rdi,%r8,4), %xmm4
	pinsrd	$3, 52(%rdi,%r8,4), %xmm4
	psubd	%xmm8, %xmm6
	movd	12(%rdi,%r8,4), %xmm7           ## xmm7 = mem[0],zero,zero,zero
	pinsrd	$1, 28(%rdi,%r8,4), %xmm7
	pinsrd	$2, 44(%rdi,%r8,4), %xmm7
	pinsrd	$3, 60(%rdi,%r8,4), %xmm7
	pmulld	%xmm5, %xmm7
	pshufd	$245, %xmm7, %xmm5              ## xmm5 = xmm7[1,1,3,3]
	pmuludq	%xmm1, %xmm5
	movdqa	%xmm7, %xmm8
	pmaxud	%xmm2, %xmm8
	pcmpeqd	%xmm7, %xmm8
	pmuludq	%xmm1, %xmm7
	pshufd	$245, %xmm7, %xmm7              ## xmm7 = xmm7[1,1,3,3]
	pblendw	$204, %xmm5, %xmm7              ## xmm7 = xmm7[0,1],xmm5[2,3],xmm7[4,5],xmm5[6,7]
	pand	%xmm1, %xmm8
	psubd	%xmm7, %xmm8
	movdqa	%xmm6, %xmm5
	paddd	%xmm3, %xmm5
	movdqa	%xmm8, %xmm7
	paddd	%xmm4, %xmm7
	psubd	%xmm6, %xmm3
	paddd	%xmm1, %xmm3
	psubd	%xmm8, %xmm4
	paddd	%xmm1, %xmm4
	movdqa	%xmm5, %xmm6
	punpckhdq	%xmm7, %xmm6            ## xmm6 = xmm6[2],xmm7[2],xmm6[3],xmm7[3]
	pshufd	$170, %xmm4, %xmm8              ## xmm8 = xmm4[2,2,2,2]
	pblendw	$63, %xmm3, %xmm8               ## xmm8 = xmm3[0,1,2,3,4,5],xmm8[6,7]
	pblendw	$15, %xmm6, %xmm8               ## xmm8 = xmm6[0,1,2,3],xmm8[4,5,6,7]
	pshufd	$85, %xmm5, %xmm9               ## xmm9 = xmm5[1,1,1,1]
	punpckldq	%xmm7, %xmm5            ## xmm5 = xmm5[0],xmm7[0],xmm5[1],xmm7[1]
	movdqa	%xmm3, %xmm10
	punpckldq	%xmm4, %xmm10           ## xmm10 = xmm10[0],xmm4[0],xmm10[1],xmm4[1]
	punpcklqdq	%xmm10, %xmm5           ## xmm5 = xmm5[0],xmm10[0]
	pblendw	$12, %xmm7, %xmm9               ## xmm9 = xmm9[0,1],xmm7[2,3],xmm9[4,5,6,7]
	pblendw	$240, %xmm10, %xmm9             ## xmm9 = xmm9[0,1,2,3],xmm10[4,5,6,7]
	punpckhdq	%xmm4, %xmm3            ## xmm3 = xmm3[2],xmm4[2],xmm3[3],xmm4[3]
	punpckhqdq	%xmm3, %xmm6            ## xmm6 = xmm6[1],xmm3[1]
	movdqu	%xmm6, 48(%rdi,%r8,4)
	movdqu	%xmm9, 16(%rdi,%r8,4)
	movdqu	%xmm5, (%rdi,%r8,4)
	movdqu	%xmm8, 32(%rdi,%r8,4)
	addq	$16, %r8
	cmpq	$256, %r8                       ## imm = 0x100
	jne	LBB1_21
LBB1_24:
	addq	$1024, %rcx                     ## imm = 0x400
	cmpq	%rcx, %rdi
	setb	%cl
	cmpq	%rax, %rsi
	setb	%al
	testb	%al, %cl
	je	LBB1_25
## %bb.27:
	xorl	%eax, %eax
	xorl	%ecx, %ecx
	.p2align	4
LBB1_28:                                ## =>This Inner Loop Header: Depth=1
	movl	(%rsi,%rcx,4), %edx
	imull	4(%rdi,%rcx,8), %edx
	movl	(%rdi,%rcx,8), %r8d
	imull	$-1954291199, %edx, %edx        ## imm = 0x8B83E201
	imulq	$7681, %rdx, %r9                ## imm = 0x1E01
	shrq	$32, %r9
	cmpl	$559168, %edx                   ## imm = 0x88840
	movl	$7681, %edx                     ## imm = 0x1E01
	cmovbl	%eax, %edx
	subl	%r9d, %edx
	leal	(%rdx,%r8), %r9d
	movl	%r9d, (%rdi,%rcx,8)
	subl	%edx, %r8d
	addl	$7681, %r8d                     ## imm = 0x1E01
	movl	%r8d, 4(%rdi,%rcx,8)
	incq	%rcx
	cmpq	$128, %rcx
	jne	LBB1_28
	jmp	LBB1_29
LBB1_25:
	xorl	%eax, %eax
	movdqa	LCPI1_2(%rip), %xmm0            ## xmm0 = [2340676097,2340676097,2340676097,2340676097]
	.p2align	4
LBB1_26:                                ## =>This Inner Loop Header: Depth=1
	movdqu	(%rsi,%rax,4), %xmm4
	movups	(%rdi,%rax,8), %xmm3
	movups	16(%rdi,%rax,8), %xmm5
	movaps	%xmm3, %xmm6
	shufps	$221, %xmm5, %xmm6              ## xmm6 = xmm6[1,3],xmm5[1,3]
	pmulld	%xmm4, %xmm6
	pmulld	%xmm0, %xmm6
	shufps	$136, %xmm5, %xmm3              ## xmm3 = xmm3[0,2],xmm5[0,2]
	pshufd	$245, %xmm6, %xmm4              ## xmm4 = xmm6[1,1,3,3]
	pmuludq	%xmm1, %xmm4
	movdqa	%xmm6, %xmm5
	pmaxud	%xmm2, %xmm5
	pcmpeqd	%xmm6, %xmm5
	pmuludq	%xmm1, %xmm6
	pshufd	$245, %xmm6, %xmm6              ## xmm6 = xmm6[1,1,3,3]
	pblendw	$204, %xmm4, %xmm6              ## xmm6 = xmm6[0,1],xmm4[2,3],xmm6[4,5],xmm4[6,7]
	pand	%xmm1, %xmm5
	psubd	%xmm6, %xmm5
	movdqa	%xmm5, %xmm4
	paddd	%xmm3, %xmm4
	psubd	%xmm5, %xmm3
	paddd	%xmm1, %xmm3
	movdqa	%xmm4, %xmm5
	punpckhdq	%xmm3, %xmm5            ## xmm5 = xmm5[2],xmm3[2],xmm5[3],xmm3[3]
	punpckldq	%xmm3, %xmm4            ## xmm4 = xmm4[0],xmm3[0],xmm4[1],xmm3[1]
	movdqu	%xmm4, (%rdi,%rax,8)
	movdqu	%xmm5, 16(%rdi,%rax,8)
	addq	$4, %rax
	cmpq	$128, %rax
	jne	LBB1_26
LBB1_29:
	xorl	%eax, %eax
	movdqa	LCPI1_3(%rip), %xmm3            ## xmm3 = [4294959615,4294959615,4294959615,4294959615]
	xorl	%ecx, %ecx
	popq	%rbx
	popq	%r14
	popq	%r15
	popq	%rbp
	.p2align	4
LBB1_30:                                ## =>This Inner Loop Header: Depth=1
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
	jne	LBB1_30
LBB1_31:
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
	jne	LBB2_23
## %bb.1:
	testq	%rsi, %rsi
	je	LBB2_23
## %bb.2:
	testq	%rdi, %rdi
	je	LBB2_23
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
	je	LBB2_22
## %bb.4:
	movq	%rdi, %rax
	subq	%rsi, %rax
	cmpq	$31, %rax
	ja	LBB2_25
## %bb.5:
	xorl	%eax, %eax
	.p2align	4
LBB2_6:                                 ## =>This Inner Loop Header: Depth=1
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
	jne	LBB2_6
	jmp	LBB2_7
LBB2_25:
	movups	(%rsi), %xmm0
	movups	16(%rsi), %xmm1
	movups	%xmm0, (%rdi)
	movups	%xmm1, 16(%rdi)
	movups	32(%rsi), %xmm0
	movups	48(%rsi), %xmm1
	movups	%xmm0, 32(%rdi)
	movups	%xmm1, 48(%rdi)
	movups	64(%rsi), %xmm0
	movups	80(%rsi), %xmm1
	movups	%xmm0, 64(%rdi)
	movups	%xmm1, 80(%rdi)
	movups	96(%rsi), %xmm0
	movups	112(%rsi), %xmm1
	movups	%xmm0, 96(%rdi)
	movups	%xmm1, 112(%rdi)
	movups	128(%rsi), %xmm0
	movups	144(%rsi), %xmm1
	movups	%xmm0, 128(%rdi)
	movups	%xmm1, 144(%rdi)
	movups	160(%rsi), %xmm0
	movups	176(%rsi), %xmm1
	movups	%xmm0, 160(%rdi)
	movups	%xmm1, 176(%rdi)
	movups	192(%rsi), %xmm0
	movups	208(%rsi), %xmm1
	movups	%xmm0, 192(%rdi)
	movups	%xmm1, 208(%rdi)
	movups	224(%rsi), %xmm0
	movups	240(%rsi), %xmm1
	movups	%xmm0, 224(%rdi)
	movups	%xmm1, 240(%rdi)
	movups	256(%rsi), %xmm0
	movups	272(%rsi), %xmm1
	movups	%xmm0, 256(%rdi)
	movups	%xmm1, 272(%rdi)
	movups	288(%rsi), %xmm0
	movups	304(%rsi), %xmm1
	movups	%xmm0, 288(%rdi)
	movups	%xmm1, 304(%rdi)
	movups	320(%rsi), %xmm0
	movups	336(%rsi), %xmm1
	movups	%xmm0, 320(%rdi)
	movups	%xmm1, 336(%rdi)
	movups	352(%rsi), %xmm0
	movups	368(%rsi), %xmm1
	movups	%xmm0, 352(%rdi)
	movups	%xmm1, 368(%rdi)
	movups	384(%rsi), %xmm0
	movups	400(%rsi), %xmm1
	movups	%xmm0, 384(%rdi)
	movups	%xmm1, 400(%rdi)
	movups	416(%rsi), %xmm0
	movups	432(%rsi), %xmm1
	movups	%xmm0, 416(%rdi)
	movups	%xmm1, 432(%rdi)
	movups	448(%rsi), %xmm0
	movups	464(%rsi), %xmm1
	movups	%xmm0, 448(%rdi)
	movups	%xmm1, 464(%rdi)
	movups	480(%rsi), %xmm0
	movups	496(%rsi), %xmm1
	movups	%xmm0, 480(%rdi)
	movups	%xmm1, 496(%rdi)
	movups	512(%rsi), %xmm0
	movups	528(%rsi), %xmm1
	movups	%xmm0, 512(%rdi)
	movups	%xmm1, 528(%rdi)
	movups	544(%rsi), %xmm0
	movups	560(%rsi), %xmm1
	movups	%xmm0, 544(%rdi)
	movups	%xmm1, 560(%rdi)
	movups	576(%rsi), %xmm0
	movups	592(%rsi), %xmm1
	movups	%xmm0, 576(%rdi)
	movups	%xmm1, 592(%rdi)
	movups	608(%rsi), %xmm0
	movups	624(%rsi), %xmm1
	movups	%xmm0, 608(%rdi)
	movups	%xmm1, 624(%rdi)
	movups	640(%rsi), %xmm0
	movups	656(%rsi), %xmm1
	movups	%xmm0, 640(%rdi)
	movups	%xmm1, 656(%rdi)
	movups	672(%rsi), %xmm0
	movups	688(%rsi), %xmm1
	movups	%xmm0, 672(%rdi)
	movups	%xmm1, 688(%rdi)
	movups	704(%rsi), %xmm0
	movups	720(%rsi), %xmm1
	movups	%xmm0, 704(%rdi)
	movups	%xmm1, 720(%rdi)
	movups	736(%rsi), %xmm0
	movups	752(%rsi), %xmm1
	movups	%xmm0, 736(%rdi)
	movups	%xmm1, 752(%rdi)
	movups	768(%rsi), %xmm0
	movups	784(%rsi), %xmm1
	movups	%xmm0, 768(%rdi)
	movups	%xmm1, 784(%rdi)
	movups	800(%rsi), %xmm0
	movups	816(%rsi), %xmm1
	movups	%xmm0, 800(%rdi)
	movups	%xmm1, 816(%rdi)
	movups	832(%rsi), %xmm0
	movups	848(%rsi), %xmm1
	movups	%xmm0, 832(%rdi)
	movups	%xmm1, 848(%rdi)
	movups	864(%rsi), %xmm0
	movups	880(%rsi), %xmm1
	movups	%xmm0, 864(%rdi)
	movups	%xmm1, 880(%rdi)
	movups	896(%rsi), %xmm0
	movups	912(%rsi), %xmm1
	movups	%xmm0, 896(%rdi)
	movups	%xmm1, 912(%rdi)
	movups	928(%rsi), %xmm0
	movups	944(%rsi), %xmm1
	movups	%xmm0, 928(%rdi)
	movups	%xmm1, 944(%rdi)
	movups	960(%rsi), %xmm0
	movups	976(%rsi), %xmm1
	movups	%xmm0, 960(%rdi)
	movups	%xmm1, 976(%rdi)
	movups	992(%rsi), %xmm0
	movups	1008(%rsi), %xmm1
	movups	%xmm0, 992(%rdi)
	movups	%xmm1, 1008(%rdi)
LBB2_7:
	movl	$2340676097, %esi               ## imm = 0x8B83E201
	addq	$1024, -56(%rbp)                ## 8-byte Folded Spill
                                        ## imm = 0x400
	movl	$256, %eax                      ## imm = 0x100
	movl	$1, %ecx
	movdqa	LCPI2_0(%rip), %xmm1            ## xmm1 = [7681,7681]
	movdqa	LCPI2_1(%rip), %xmm0            ## xmm0 = [7681,7681,7681,7681]
	movl	$256, %r9d                      ## imm = 0x100
	movq	%rdi, -64(%rbp)                 ## 8-byte Spill
	jmp	LBB2_8
	.p2align	4
LBB2_19:                                ##   in Loop: Header=BB2_8 Depth=1
	addl	%r8d, %r8d
	movl	%r8d, %ecx
	cmpl	$3, -44(%rbp)                   ## 4-byte Folded Reload
	movl	%r9d, %eax
	movq	-64(%rbp), %rdi                 ## 8-byte Reload
	jbe	LBB2_20
LBB2_8:                                 ## =>This Loop Header: Depth=1
                                        ##     Child Loop BB2_10 Depth 2
                                        ##       Child Loop BB2_11 Depth 3
                                        ##       Child Loop BB2_24 Depth 3
                                        ##     Child Loop BB2_15 Depth 2
	movl	%eax, -44(%rbp)                 ## 4-byte Spill
	shrl	%r9d
	movl	%ecx, %r8d
	movq	-56(%rbp), %rax                 ## 8-byte Reload
	leaq	(%rax,%r9,4), %rax
	movq	%rax, -72(%rbp)                 ## 8-byte Spill
	leaq	(%rdi,%r8,4), %r10
	cmpl	$4, %ecx
	jae	LBB2_9
## %bb.14:                              ##   in Loop: Header=BB2_8 Depth=1
	leaq	(%r8,%r8), %r11
	movl	$1, %ebx
	xorl	%r14d, %r14d
	jmp	LBB2_15
	.p2align	4
LBB2_18:                                ##   in Loop: Header=BB2_15 Depth=2
	incq	%r14
	addq	%r11, %rbx
	cmpq	%r14, %r9
	je	LBB2_19
LBB2_15:                                ##   Parent Loop BB2_8 Depth=1
                                        ## =>  This Inner Loop Header: Depth=2
	leaq	(%r8,%rbx), %r12
	decq	%r12
	movq	-72(%rbp), %rax                 ## 8-byte Reload
	movslq	(%rax,%r14,4), %rcx
	movq	%rcx, %r15
	imulq	%rsi, %r15
	movq	-64(%rbp), %rdi                 ## 8-byte Reload
	movl	-4(%rdi,%rbx,4), %edx
	movl	-4(%r10,%rbx,4), %r13d
	leal	(%rdx,%r13), %eax
	movl	%eax, -4(%rdi,%rbx,4)
	subl	%r13d, %edx
	movslq	%edx, %rax
	movq	%rax, %rdx
	imulq	%rcx, %rdx
	imull	%r15d, %eax
	shrq	$32, %rdx
	imulq	$7681, %rax, %rax               ## imm = 0x1E01
	shrq	$32, %rax
	subl	%eax, %edx
	movl	%edx, %eax
	sarl	$31, %eax
	andl	$7681, %eax                     ## imm = 0x1E01
	addl	%edx, %eax
	movl	%eax, %edx
	sarl	$31, %edx
	andl	$7681, %edx                     ## imm = 0x1E01
	addl	%eax, %edx
	movl	%edx, -4(%r10,%rbx,4)
	cmpq	%r12, %rbx
	je	LBB2_18
## %bb.16:                              ##   in Loop: Header=BB2_15 Depth=2
	movl	(%rdi,%rbx,4), %eax
	movl	(%r10,%rbx,4), %edx
	leal	(%rdx,%rax), %r13d
	movl	%r13d, (%rdi,%rbx,4)
	subl	%edx, %eax
	cltq
	movq	%rax, %rdx
	imulq	%rcx, %rdx
	imull	%r15d, %eax
	shrq	$32, %rdx
	imulq	$7681, %rax, %rax               ## imm = 0x1E01
	shrq	$32, %rax
	subl	%eax, %edx
	movl	%edx, %eax
	sarl	$31, %eax
	andl	$7681, %eax                     ## imm = 0x1E01
	addl	%edx, %eax
	movl	%eax, %edx
	sarl	$31, %edx
	andl	$7681, %edx                     ## imm = 0x1E01
	addl	%eax, %edx
	movl	%edx, (%r10,%rbx,4)
	leaq	1(%rbx), %rax
	cmpq	%r12, %rax
	je	LBB2_18
## %bb.17:                              ##   in Loop: Header=BB2_15 Depth=2
	movl	4(%rdi,%rbx,4), %eax
	movl	4(%r10,%rbx,4), %edx
	leal	(%rdx,%rax), %r12d
	movl	%r12d, 4(%rdi,%rbx,4)
	subl	%edx, %eax
	cltq
	imulq	%rax, %rcx
	imull	%r15d, %eax
	shrq	$32, %rcx
	imulq	$7681, %rax, %rax               ## imm = 0x1E01
	shrq	$32, %rax
	subl	%eax, %ecx
	movl	%ecx, %eax
	sarl	$31, %eax
	andl	$7681, %eax                     ## imm = 0x1E01
	addl	%ecx, %eax
	movl	%eax, %ecx
	sarl	$31, %ecx
	andl	$7681, %ecx                     ## imm = 0x1E01
	addl	%eax, %ecx
	movl	%ecx, 4(%r10,%rbx,4)
	jmp	LBB2_18
	.p2align	4
LBB2_9:                                 ##   in Loop: Header=BB2_8 Depth=1
	movq	%rdi, %r15
	movl	%r8d, %r11d
	andl	$-4, %r11d
	leaq	(,%r8,4), %rbx
	andq	$-16, %rbx
	leaq	(,%r8,8), %rdi
	xorl	%r12d, %r12d
	jmp	LBB2_10
	.p2align	4
LBB2_13:                                ##   in Loop: Header=BB2_10 Depth=2
	incq	%r12
	addq	%rdi, %r10
	addq	%rdi, %r15
	movq	%r14, %r9
	cmpq	%r14, %r12
	movl	$2340676097, %esi               ## imm = 0x8B83E201
	je	LBB2_19
LBB2_10:                                ##   Parent Loop BB2_8 Depth=1
                                        ## =>  This Loop Header: Depth=2
                                        ##       Child Loop BB2_11 Depth 3
                                        ##       Child Loop BB2_24 Depth 3
	movq	%r9, %r14
	movq	-72(%rbp), %rax                 ## 8-byte Reload
	movslq	(%rax,%r12,4), %rcx
	movq	%rcx, %r13
	imulq	%rsi, %r13
	movq	%rcx, %xmm2
	pshufd	$68, %xmm2, %xmm2               ## xmm2 = xmm2[0,1,0,1]
	movq	%r13, %xmm3
	pshufd	$68, %xmm3, %xmm3               ## xmm3 = xmm3[0,1,0,1]
	movdqa	%xmm2, %xmm4
	psrlq	$32, %xmm4
	xorl	%edx, %edx
	.p2align	4
LBB2_11:                                ##   Parent Loop BB2_8 Depth=1
                                        ##     Parent Loop BB2_10 Depth=2
                                        ## =>    This Inner Loop Header: Depth=3
	movdqu	(%r15,%rdx), %xmm5
	movdqu	(%r10,%rdx), %xmm6
	movdqa	%xmm6, %xmm7
	paddd	%xmm5, %xmm7
	movdqu	%xmm7, (%r15,%rdx)
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
	movdqu	%xmm6, (%r10,%rdx)
	addq	$16, %rdx
	cmpq	%rdx, %rbx
	jne	LBB2_11
## %bb.12:                              ##   in Loop: Header=BB2_10 Depth=2
	movq	%r11, %rdx
	cmpl	%r8d, %r11d
	je	LBB2_13
	.p2align	4
LBB2_24:                                ##   Parent Loop BB2_8 Depth=1
                                        ##     Parent Loop BB2_10 Depth=2
                                        ## =>    This Inner Loop Header: Depth=3
	movl	(%r15,%rdx,4), %eax
	movl	(%r10,%rdx,4), %r9d
	leal	(%r9,%rax), %esi
	movl	%esi, (%r15,%rdx,4)
	subl	%r9d, %eax
	cltq
	movq	%rax, %rsi
	imulq	%rcx, %rsi
	imull	%r13d, %eax
	shrq	$32, %rsi
	imulq	$7681, %rax, %rax               ## imm = 0x1E01
	shrq	$32, %rax
	subl	%eax, %esi
	movl	%esi, %eax
	sarl	$31, %eax
	andl	$7681, %eax                     ## imm = 0x1E01
	addl	%esi, %eax
	movl	%eax, %esi
	sarl	$31, %esi
	andl	$7681, %esi                     ## imm = 0x1E01
	addl	%eax, %esi
	movl	%esi, (%r10,%rdx,4)
	incq	%rdx
	cmpq	%rdx, %r8
	jne	LBB2_24
	jmp	LBB2_13
LBB2_20:
	xorl	%eax, %eax
	movdqa	LCPI2_2(%rip), %xmm1            ## xmm1 = [16775032,16775032,16775032,16775032]
	movdqa	LCPI2_3(%rip), %xmm2            ## xmm2 = [559168,559168,559168,559168]
	xorl	%ecx, %ecx
	.p2align	4
LBB2_21:                                ## =>This Inner Loop Header: Depth=1
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
	jne	LBB2_21
LBB2_22:
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
LBB2_23:
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
