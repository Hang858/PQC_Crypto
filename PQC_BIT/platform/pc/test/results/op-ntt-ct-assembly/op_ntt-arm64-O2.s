	.build_version macos, 26, 0	sdk_version 27, 2
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_OP_ntt256_init                 ; -- Begin function OP_ntt256_init
	.p2align	2
_OP_ntt256_init:                        ; @OP_ntt256_init
	.cfi_startproc
; %bb.0:
	cmp	w1, #2
	mov	w8, #7681                       ; =0x1e01
	ccmp	w2, w8, #0, eq
	ccmp	x0, #0, #4, eq
	mov	w8, #57857                      ; =0xe201
	movk	w8, #35715, lsl #16
	ccmp	w3, w8, #0, ne
	mov	w8, #1912                       ; =0x778
	ccmp	w4, w8, #0, eq
	ccmp	w5, #0, #0, eq
	cset	w8, ne
	csel	x9, xzr, x0, ne
	sbfx	w0, w8, #0, #1
	adrp	x8, _ntt_omega@PAGE
	str	x9, [x8, _ntt_omega@PAGEOFF]
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_OP_ntt256                      ; -- Begin function OP_ntt256
	.p2align	2
_OP_ntt256:                             ; @OP_ntt256
	.cfi_startproc
; %bb.0:
	mov	w8, #-1                         ; =0xffffffff
	cbnz	w2, LBB1_20
; %bb.1:
	cbz	x1, LBB1_20
; %bb.2:
	cbz	x0, LBB1_20
; %bb.3:
Lloh0:
	adrp	x9, _ntt_omega@PAGE
Lloh1:
	ldr	x9, [x9, _ntt_omega@PAGEOFF]
	cbz	x9, LBB1_20
; %bb.4:
	sub	x8, x0, x1
	cmp	x8, #63
	b.hi	LBB1_7
; %bb.5:
	mov	x8, #0                          ; =0x0
LBB1_6:                                 ; =>This Inner Loop Header: Depth=1
	ldr	w10, [x1, x8]
	str	w10, [x0, x8]
	add	x8, x8, #4
	cmp	x8, #1024
	b.ne	LBB1_6
	b	LBB1_8
LBB1_7:
	ldp	q0, q1, [x1]
	ldp	q2, q3, [x1, #32]
	stp	q0, q1, [x0]
	stp	q2, q3, [x0, #32]
	ldp	q0, q1, [x1, #64]
	ldp	q2, q3, [x1, #96]
	stp	q0, q1, [x0, #64]
	stp	q2, q3, [x0, #96]
	ldp	q0, q1, [x1, #128]
	ldp	q2, q3, [x1, #160]
	stp	q0, q1, [x0, #128]
	stp	q2, q3, [x0, #160]
	ldp	q0, q1, [x1, #192]
	ldp	q2, q3, [x1, #224]
	stp	q0, q1, [x0, #192]
	stp	q2, q3, [x0, #224]
	ldp	q0, q1, [x1, #256]
	ldp	q2, q3, [x1, #288]
	stp	q0, q1, [x0, #256]
	stp	q2, q3, [x0, #288]
	ldp	q0, q1, [x1, #320]
	ldp	q2, q3, [x1, #352]
	stp	q0, q1, [x0, #320]
	stp	q2, q3, [x0, #352]
	ldp	q0, q1, [x1, #384]
	ldp	q2, q3, [x1, #416]
	stp	q0, q1, [x0, #384]
	stp	q2, q3, [x0, #416]
	ldp	q0, q1, [x1, #448]
	ldp	q2, q3, [x1, #480]
	stp	q0, q1, [x0, #448]
	stp	q2, q3, [x0, #480]
	ldp	q0, q1, [x1, #512]
	ldp	q2, q3, [x1, #544]
	stp	q0, q1, [x0, #512]
	stp	q2, q3, [x0, #544]
	ldp	q0, q1, [x1, #576]
	ldp	q2, q3, [x1, #608]
	stp	q0, q1, [x0, #576]
	stp	q2, q3, [x0, #608]
	ldp	q0, q1, [x1, #640]
	ldp	q2, q3, [x1, #672]
	stp	q0, q1, [x0, #640]
	stp	q2, q3, [x0, #672]
	ldp	q0, q1, [x1, #704]
	ldp	q2, q3, [x1, #736]
	stp	q0, q1, [x0, #704]
	stp	q2, q3, [x0, #736]
	ldp	q0, q1, [x1, #768]
	ldp	q2, q3, [x1, #800]
	stp	q0, q1, [x0, #768]
	stp	q2, q3, [x0, #800]
	ldp	q0, q1, [x1, #832]
	ldp	q2, q3, [x1, #864]
	stp	q0, q1, [x0, #832]
	stp	q2, q3, [x0, #864]
	ldp	q0, q1, [x1, #896]
	ldp	q2, q3, [x1, #928]
	stp	q0, q1, [x0, #896]
	stp	q2, q3, [x0, #928]
	ldp	q0, q1, [x1, #960]
	ldp	q2, q3, [x1, #992]
	stp	q0, q1, [x0, #960]
	stp	q2, q3, [x0, #992]
LBB1_8:
	stp	x24, x23, [sp, #-48]!           ; 16-byte Folded Spill
	stp	x22, x21, [sp, #16]             ; 16-byte Folded Spill
	stp	x20, x19, [sp, #32]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 48
	.cfi_offset w19, -8
	.cfi_offset w20, -16
	.cfi_offset w21, -24
	.cfi_offset w22, -32
	.cfi_offset w23, -40
	.cfi_offset w24, -48
	add	x8, x0, #16
	mov	w19, #1                         ; =0x1
	mov	w13, #256                       ; =0x100
	mov	w10, #57857                     ; =0xe201
	movk	w10, #35715, lsl #16
	mov	w11, #7681                      ; =0x1e01
	mov	w12, #34880                     ; =0x8840
	movk	w12, #8, lsl #16
	dup.4s	v0, w11
	dup.4s	v1, w12
	b	LBB1_10
LBB1_9:                                 ;   in Loop: Header=BB1_10 Depth=1
	lsl	w19, w6, #1
	mov	x13, x14
	cmp	w19, #256
	b.ge	LBB1_17
LBB1_10:                                ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB1_12 Depth 2
                                        ;       Child Loop BB1_14 Depth 3
                                        ;       Child Loop BB1_16 Depth 3
	mov	x15, #0                         ; =0x0
	mov	x16, #0                         ; =0x0
	lsr	w14, w13, #1
	ubfiz	x17, x14, #1, #32
	ubfiz	x4, x14, #2, #32
	add	x1, x0, x4
	ubfiz	x2, x14, #3, #32
	and	x3, x14, #0xf8
	add	x4, x8, x4
	mov	x5, x8
	mov	x7, x14
	mov	w6, w19
	add	x19, x9, w19, uxtw #2
	b	LBB1_12
LBB1_11:                                ;   in Loop: Header=BB1_12 Depth=2
	add	x15, x15, #1
	add	x16, x16, x17
	add	x7, x7, x17
	add	x5, x5, x2
	add	x4, x4, x2
	cmp	x15, x6
	b.eq	LBB1_9
LBB1_12:                                ;   Parent Loop BB1_10 Depth=1
                                        ; =>  This Loop Header: Depth=2
                                        ;       Child Loop BB1_14 Depth 3
                                        ;       Child Loop BB1_16 Depth 3
	ldr	w20, [x19, x15, lsl #2]
	mul	w20, w20, w10
	mov	x21, x16
	cmp	w13, #16
	b.lo	LBB1_16
; %bb.13:                               ;   in Loop: Header=BB1_12 Depth=2
	add	x21, x16, x3
	fmov	s2, w20
	mov	x22, x4
	mov	x23, x5
	mov	x24, x3
LBB1_14:                                ;   Parent Loop BB1_10 Depth=1
                                        ;     Parent Loop BB1_12 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	ldp	q3, q4, [x23, #-16]
	ldp	q5, q6, [x22, #-16]
	mul.4s	v5, v5, v2[0]
	mul.4s	v6, v6, v2[0]
	umull2.2d	v7, v5, v0
	umull.2d	v16, v5, v0
	uzp2.4s	v7, v16, v7
	umull2.2d	v16, v6, v0
	umull.2d	v17, v6, v0
	uzp2.4s	v16, v17, v16
	cmhs.4s	v5, v5, v1
	and.16b	v5, v5, v0
	cmhs.4s	v6, v6, v1
	and.16b	v6, v6, v0
	sub.4s	v5, v5, v7
	sub.4s	v6, v6, v16
	add.4s	v7, v5, v3
	add.4s	v16, v6, v4
	stp	q7, q16, [x23, #-16]
	sub.4s	v3, v3, v5
	add.4s	v3, v3, v0
	sub.4s	v4, v4, v6
	add.4s	v4, v4, v0
	stp	q3, q4, [x22, #-16]
	add	x23, x23, #32
	add	x22, x22, #32
	subs	x24, x24, #8
	b.ne	LBB1_14
; %bb.15:                               ;   in Loop: Header=BB1_12 Depth=2
	cmp	x3, x14
	b.eq	LBB1_11
LBB1_16:                                ;   Parent Loop BB1_10 Depth=1
                                        ;     Parent Loop BB1_12 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	ldr	w22, [x0, x21, lsl #2]
	ldr	w23, [x1, x21, lsl #2]
	mul	w23, w20, w23
	umull	x24, w23, w11
	lsr	x24, x24, #32
	cmp	w23, w12
	csel	w23, wzr, w11, lo
	sub	w23, w23, w24
	add	w24, w23, w22
	str	w24, [x0, x21, lsl #2]
	sub	w22, w22, w23
	add	w22, w22, w11
	str	w22, [x1, x21, lsl #2]
	add	x21, x21, #1
	cmp	x7, x21
	b.ne	LBB1_16
	b	LBB1_11
LBB1_17:
	mov	x8, #0                          ; =0x0
	mov	w9, #34880                      ; =0x8840
	movk	w9, #8, lsl #16
	dup.4s	v0, w9
	mvni.4s	v1, #30, lsl #8
	mov	w9, #7681                       ; =0x1e01
	dup.4s	v2, w9
	ldp	x20, x19, [sp, #32]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]             ; 16-byte Folded Reload
	ldp	x24, x23, [sp], #48             ; 16-byte Folded Reload
LBB1_18:                                ; =>This Inner Loop Header: Depth=1
	ldr	q3, [x0, x8]
	smull2.2d	v4, v3, v0
	smull.2d	v5, v3, v0
	uzp2.4s	v4, v5, v4
	mla.4s	v3, v4, v1
	cmlt.4s	v4, v3, #0
	and.16b	v4, v4, v2
	add.4s	v3, v4, v3
	add.4s	v4, v3, v1
	cmgt.4s	v5, v2, v3
	bif.16b	v3, v4, v5
	str	q3, [x0, x8]
	add	x8, x8, #16
	cmp	x8, #1024
	b.ne	LBB1_18
; %bb.19:
	mov	w8, #0                          ; =0x0
LBB1_20:
	mov	x0, x8
	ret
	.loh AdrpLdr	Lloh0, Lloh1
	.cfi_endproc
                                        ; -- End function
	.globl	_OP_intt256                     ; -- Begin function OP_intt256
	.p2align	2
_OP_intt256:                            ; @OP_intt256
	.cfi_startproc
; %bb.0:
	mov	w8, #-1                         ; =0xffffffff
	cmp	w2, #1
	b.ne	LBB2_20
; %bb.1:
	cbz	x1, LBB2_20
; %bb.2:
	cbz	x0, LBB2_20
; %bb.3:
Lloh2:
	adrp	x9, _ntt_omega@PAGE
Lloh3:
	ldr	x9, [x9, _ntt_omega@PAGEOFF]
	cbz	x9, LBB2_20
; %bb.4:
	sub	x8, x0, x1
	cmp	x8, #63
	b.hi	LBB2_7
; %bb.5:
	mov	x8, #0                          ; =0x0
LBB2_6:                                 ; =>This Inner Loop Header: Depth=1
	ldr	w10, [x1, x8]
	str	w10, [x0, x8]
	add	x8, x8, #4
	cmp	x8, #1024
	b.ne	LBB2_6
	b	LBB2_8
LBB2_7:
	ldp	q0, q1, [x1]
	ldp	q2, q3, [x1, #32]
	stp	q0, q1, [x0]
	stp	q2, q3, [x0, #32]
	ldp	q0, q1, [x1, #64]
	ldp	q2, q3, [x1, #96]
	stp	q0, q1, [x0, #64]
	stp	q2, q3, [x0, #96]
	ldp	q0, q1, [x1, #128]
	ldp	q2, q3, [x1, #160]
	stp	q0, q1, [x0, #128]
	stp	q2, q3, [x0, #160]
	ldp	q0, q1, [x1, #192]
	ldp	q2, q3, [x1, #224]
	stp	q0, q1, [x0, #192]
	stp	q2, q3, [x0, #224]
	ldp	q0, q1, [x1, #256]
	ldp	q2, q3, [x1, #288]
	stp	q0, q1, [x0, #256]
	stp	q2, q3, [x0, #288]
	ldp	q0, q1, [x1, #320]
	ldp	q2, q3, [x1, #352]
	stp	q0, q1, [x0, #320]
	stp	q2, q3, [x0, #352]
	ldp	q0, q1, [x1, #384]
	ldp	q2, q3, [x1, #416]
	stp	q0, q1, [x0, #384]
	stp	q2, q3, [x0, #416]
	ldp	q0, q1, [x1, #448]
	ldp	q2, q3, [x1, #480]
	stp	q0, q1, [x0, #448]
	stp	q2, q3, [x0, #480]
	ldp	q0, q1, [x1, #512]
	ldp	q2, q3, [x1, #544]
	stp	q0, q1, [x0, #512]
	stp	q2, q3, [x0, #544]
	ldp	q0, q1, [x1, #576]
	ldp	q2, q3, [x1, #608]
	stp	q0, q1, [x0, #576]
	stp	q2, q3, [x0, #608]
	ldp	q0, q1, [x1, #640]
	ldp	q2, q3, [x1, #672]
	stp	q0, q1, [x0, #640]
	stp	q2, q3, [x0, #672]
	ldp	q0, q1, [x1, #704]
	ldp	q2, q3, [x1, #736]
	stp	q0, q1, [x0, #704]
	stp	q2, q3, [x0, #736]
	ldp	q0, q1, [x1, #768]
	ldp	q2, q3, [x1, #800]
	stp	q0, q1, [x0, #768]
	stp	q2, q3, [x0, #800]
	ldp	q0, q1, [x1, #832]
	ldp	q2, q3, [x1, #864]
	stp	q0, q1, [x0, #832]
	stp	q2, q3, [x0, #864]
	ldp	q0, q1, [x1, #896]
	ldp	q2, q3, [x1, #928]
	stp	q0, q1, [x0, #896]
	stp	q2, q3, [x0, #928]
	ldp	q0, q1, [x1, #960]
	ldp	q2, q3, [x1, #992]
	stp	q0, q1, [x0, #960]
	stp	q2, q3, [x0, #992]
LBB2_8:
	stp	x26, x25, [sp, #-64]!           ; 16-byte Folded Spill
	stp	x24, x23, [sp, #16]             ; 16-byte Folded Spill
	stp	x22, x21, [sp, #32]             ; 16-byte Folded Spill
	stp	x20, x19, [sp, #48]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 64
	.cfi_offset w19, -8
	.cfi_offset w20, -16
	.cfi_offset w21, -24
	.cfi_offset w22, -32
	.cfi_offset w23, -40
	.cfi_offset w24, -48
	.cfi_offset w25, -56
	.cfi_offset w26, -64
	add	x8, x9, #1024
	mov	w9, #256                        ; =0x100
	mov	w3, #1                          ; =0x1
	mov	w10, #57857                     ; =0xe201
	movk	w10, #35715, lsl #16
	mov	w11, #7681                      ; =0x1e01
	dup.4s	v0, w11
	b	LBB2_10
LBB2_9:                                 ;   in Loop: Header=BB2_10 Depth=1
	lsl	w3, w16, #1
	cmp	w9, #3
	mov	x9, x12
	b.ls	LBB2_17
LBB2_10:                                ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB2_12 Depth 2
                                        ;       Child Loop BB2_14 Depth 3
                                        ;       Child Loop BB2_16 Depth 3
	mov	x13, #0                         ; =0x0
	mov	x14, #0                         ; =0x0
	lsr	w12, w9, #1
	ubfiz	x15, x3, #1, #32
	ubfiz	x17, x3, #2, #32
	mov	w16, w3
	add	x1, x0, x17
	and	x2, x16, #0xfffffffc
	ubfiz	x3, x3, #3, #32
	mov	x4, x0
	mov	x5, x16
	add	x6, x8, w12, uxtw #2
	b	LBB2_12
LBB2_11:                                ;   in Loop: Header=BB2_12 Depth=2
	add	x13, x13, #1
	add	x14, x14, x15
	add	x5, x5, x15
	add	x4, x4, x3
	cmp	x13, x12
	b.eq	LBB2_9
LBB2_12:                                ;   Parent Loop BB2_10 Depth=1
                                        ; =>  This Loop Header: Depth=2
                                        ;       Child Loop BB2_14 Depth 3
                                        ;       Child Loop BB2_16 Depth 3
	ldrsw	x7, [x6, x13, lsl #2]
	mul	x19, x7, x10
	mov	x20, x14
	cmp	w16, #4
	b.lo	LBB2_16
; %bb.13:                               ;   in Loop: Header=BB2_12 Depth=2
	add	x20, x14, x2
	dup.2d	v1, x19
	dup.4s	v2, w7
	mov	x21, x4
	mov	x22, x2
LBB2_14:                                ;   Parent Loop BB2_10 Depth=1
                                        ;     Parent Loop BB2_12 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	ldr	q3, [x21]
	ldr	q4, [x21, x17]
	add.4s	v5, v4, v3
	str	q5, [x21]
	sub.4s	v3, v3, v4
	sshll.2d	v4, v3, #0
	sshll2.2d	v5, v3, #0
	smull2.2d	v6, v2, v3
	mov.s	w23, v5[2]
	mov.s	w24, v1[2]
	mul	w23, w24, w23
	fmov	w25, s5
	fmov	w26, s1
	mul	w25, w26, w25
	fmov	d5, x25
	mov.d	v5[1], x23
	mov.s	w23, v4[2]
	mul	w23, w24, w23
	fmov	w24, s4
	fmov	w25, s1
	mul	w24, w25, w24
	fmov	d4, x24
	mov.d	v4[1], x23
	smull.2d	v3, v2, v3
	uzp2.4s	v3, v3, v6
	xtn.2s	v4, v4
	xtn.2s	v5, v5
	umull.2d	v5, v5, v0
	umull.2d	v4, v4, v0
	uzp2.4s	v4, v4, v5
	sub.4s	v3, v3, v4
	cmlt.4s	v4, v3, #0
	and.16b	v4, v4, v0
	add.4s	v3, v4, v3
	cmlt.4s	v4, v3, #0
	and.16b	v4, v4, v0
	add.4s	v3, v4, v3
	str	q3, [x21, x17]
	add	x21, x21, #16
	subs	x22, x22, #4
	b.ne	LBB2_14
; %bb.15:                               ;   in Loop: Header=BB2_12 Depth=2
	cmp	x2, x16
	b.eq	LBB2_11
LBB2_16:                                ;   Parent Loop BB2_10 Depth=1
                                        ;     Parent Loop BB2_12 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	ldr	w21, [x0, x20, lsl #2]
	ldr	w22, [x1, x20, lsl #2]
	add	w23, w22, w21
	str	w23, [x0, x20, lsl #2]
	sub	w21, w21, w22
	smull	x22, w7, w21
	mul	w21, w19, w21
	lsr	x22, x22, #32
	umull	x21, w21, w11
	lsr	x21, x21, #32
	sub	w21, w22, w21
	and	w22, w11, w21, asr #31
	add	w21, w22, w21
	and	w22, w11, w21, asr #31
	add	w21, w22, w21
	str	w21, [x1, x20, lsl #2]
	add	x20, x20, #1
	cmp	x5, x20
	b.ne	LBB2_16
	b	LBB2_11
LBB2_17:
	add	x8, x0, #32
	mov	w9, #256                        ; =0x100
	mov	w10, #63352                     ; =0xf778
	movk	w10, #255, lsl #16
	dup.4s	v0, w10
	mov	w10, #7681                      ; =0x1e01
	dup.4s	v1, w10
	mov	w10, #34880                     ; =0x8840
	movk	w10, #8, lsl #16
	dup.4s	v2, w10
LBB2_18:                                ; =>This Inner Loop Header: Depth=1
	ldp	q3, q4, [x8, #-32]
	ldp	q5, q6, [x8]
	mul.4s	v3, v3, v0
	mul.4s	v4, v4, v0
	mul.4s	v5, v5, v0
	mul.4s	v6, v6, v0
	umull2.2d	v7, v3, v1
	umull.2d	v16, v3, v1
	uzp2.4s	v7, v16, v7
	umull2.2d	v16, v4, v1
	umull.2d	v17, v4, v1
	uzp2.4s	v16, v17, v16
	umull2.2d	v17, v5, v1
	umull.2d	v18, v5, v1
	uzp2.4s	v17, v18, v17
	umull2.2d	v18, v6, v1
	umull.2d	v19, v6, v1
	uzp2.4s	v18, v19, v18
	cmhs.4s	v3, v3, v2
	and.16b	v3, v3, v1
	cmhs.4s	v4, v4, v2
	and.16b	v4, v4, v1
	cmhs.4s	v5, v5, v2
	and.16b	v5, v5, v1
	cmhs.4s	v6, v6, v2
	and.16b	v6, v6, v1
	sub.4s	v3, v3, v7
	sub.4s	v4, v4, v16
	sub.4s	v5, v5, v17
	stp	q3, q4, [x8, #-32]
	sub.4s	v3, v6, v18
	stp	q5, q3, [x8], #64
	subs	x9, x9, #16
	b.ne	LBB2_18
; %bb.19:
	mov	w8, #0                          ; =0x0
	ldp	x20, x19, [sp, #48]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #32]             ; 16-byte Folded Reload
	ldp	x24, x23, [sp, #16]             ; 16-byte Folded Reload
	ldp	x26, x25, [sp], #64             ; 16-byte Folded Reload
LBB2_20:
	mov	x0, x8
	ret
	.loh AdrpLdr	Lloh2, Lloh3
	.cfi_endproc
                                        ; -- End function
	.globl	_OP_cwm                         ; -- Begin function OP_cwm
	.p2align	2
_OP_cwm:                                ; @OP_cwm
	.cfi_startproc
; %bb.0:
	mov	w8, #-1                         ; =0xffffffff
	cbnz	w5, LBB3_14
; %bb.1:
	mov	w9, #7681                       ; =0x1e01
	cmp	w4, w9
	b.ne	LBB3_14
; %bb.2:
	cmp	w3, #256
	b.ne	LBB3_14
; %bb.3:
	cbz	x2, LBB3_14
; %bb.4:
	cbz	x1, LBB3_14
; %bb.5:
	cbz	x0, LBB3_14
; %bb.6:
Lloh4:
	adrp	x9, _ntt_omega@PAGE
Lloh5:
	ldr	x9, [x9, _ntt_omega@PAGEOFF]
	cbz	x9, LBB3_14
; %bb.7:
	sub	x8, x0, x1
	cmp	x8, #64
	b.lo	LBB3_11
; %bb.8:
	sub	x8, x0, x2
	cmp	x8, #64
	b.lo	LBB3_11
; %bb.9:
	add	x8, x1, #32
	add	x9, x2, #32
	add	x10, x0, #32
	mov	w11, #256                       ; =0x100
	mov	w12, #57857                     ; =0xe201
	movk	w12, #35715, lsl #16
	dup.4s	v0, w12
	mov	w12, #7681                      ; =0x1e01
	dup.4s	v1, w12
	mov	w12, #34880                     ; =0x8840
	movk	w12, #8, lsl #16
	dup.4s	v2, w12
LBB3_10:                                ; =>This Inner Loop Header: Depth=1
	ldp	q3, q4, [x8, #-32]
	ldp	q5, q6, [x8], #64
	ldp	q7, q16, [x9, #-32]
	ldp	q17, q18, [x9], #64
	mul.4s	v3, v3, v7
	mul.4s	v4, v4, v16
	mul.4s	v5, v5, v17
	mul.4s	v6, v6, v18
	mul.4s	v3, v3, v0
	mul.4s	v4, v4, v0
	mul.4s	v5, v5, v0
	mul.4s	v6, v6, v0
	umull2.2d	v7, v3, v1
	umull.2d	v16, v3, v1
	uzp2.4s	v7, v16, v7
	umull2.2d	v16, v4, v1
	umull.2d	v17, v4, v1
	uzp2.4s	v16, v17, v16
	umull2.2d	v17, v5, v1
	umull.2d	v18, v5, v1
	uzp2.4s	v17, v18, v17
	umull2.2d	v18, v6, v1
	umull.2d	v19, v6, v1
	uzp2.4s	v18, v19, v18
	cmhs.4s	v3, v3, v2
	and.16b	v3, v3, v1
	cmhs.4s	v4, v4, v2
	and.16b	v4, v4, v1
	cmhs.4s	v5, v5, v2
	and.16b	v5, v5, v1
	cmhs.4s	v6, v6, v2
	and.16b	v6, v6, v1
	sub.4s	v3, v3, v7
	sub.4s	v4, v4, v16
	sub.4s	v5, v5, v17
	sub.4s	v6, v6, v18
	stp	q3, q4, [x10, #-32]
	stp	q5, q6, [x10], #64
	subs	x11, x11, #16
	b.ne	LBB3_10
	b	LBB3_13
LBB3_11:
	mov	x8, #0                          ; =0x0
	mov	w9, #57857                      ; =0xe201
	movk	w9, #35715, lsl #16
	mov	w10, #7681                      ; =0x1e01
	mov	w11, #34880                     ; =0x8840
	movk	w11, #8, lsl #16
LBB3_12:                                ; =>This Inner Loop Header: Depth=1
	ldr	w12, [x1, x8]
	ldr	w13, [x2, x8]
	mul	w12, w12, w13
	mul	w12, w12, w9
	umull	x13, w12, w10
	lsr	x13, x13, #32
	cmp	w12, w11
	csel	w12, wzr, w10, lo
	sub	w12, w12, w13
	str	w12, [x0, x8]
	add	x8, x8, #4
	cmp	x8, #1024
	b.ne	LBB3_12
LBB3_13:
	mov	w8, #0                          ; =0x0
LBB3_14:
	mov	x0, x8
	ret
	.loh AdrpLdr	Lloh4, Lloh5
	.cfi_endproc
                                        ; -- End function
.zerofill __DATA,__bss,_ntt_omega,8,3   ; @ntt_omega
.subsections_via_symbols
