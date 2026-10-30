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
	cbnz	w2, LBB1_39
; %bb.1:
	cbz	x1, LBB1_39
; %bb.2:
	cbz	x0, LBB1_39
; %bb.3:
Lloh0:
	adrp	x9, _ntt_omega@PAGE
Lloh1:
	ldr	x9, [x9, _ntt_omega@PAGEOFF]
	cbz	x9, LBB1_39
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
	stp	d9, d8, [sp, #-48]!             ; 16-byte Folded Spill
	stp	x22, x21, [sp, #16]             ; 16-byte Folded Spill
	stp	x20, x19, [sp, #32]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 48
	.cfi_offset w19, -8
	.cfi_offset w20, -16
	.cfi_offset w21, -24
	.cfi_offset w22, -32
	.cfi_offset b8, -40
	.cfi_offset b9, -48
	mov	w8, #57857                      ; =0xe201
	movk	w8, #35715, lsl #16
	ldr	w10, [x9, #4]
	mul	w10, w10, w8
	fmov	s0, w10
	mov	w10, #16                        ; =0x10
	mov	w11, #7681                      ; =0x1e01
	dup.4s	v1, w11
	mov	w11, #34880                     ; =0x8840
	movk	w11, #8, lsl #16
	dup.4s	v2, w11
LBB1_9:                                 ; =>This Inner Loop Header: Depth=1
	add	x11, x0, x10
	ldp	q3, q4, [x11, #-16]
	ldp	q5, q6, [x11, #496]
	mul.4s	v5, v5, v0[0]
	mul.4s	v6, v6, v0[0]
	umull2.2d	v7, v5, v1
	umull.2d	v16, v5, v1
	uzp2.4s	v7, v16, v7
	umull2.2d	v16, v6, v1
	umull.2d	v17, v6, v1
	uzp2.4s	v16, v17, v16
	cmhs.4s	v5, v5, v2
	and.16b	v5, v5, v1
	cmhs.4s	v6, v6, v2
	and.16b	v6, v6, v1
	sub.4s	v5, v5, v7
	sub.4s	v6, v6, v16
	add.4s	v7, v5, v3
	add.4s	v16, v6, v4
	stp	q7, q16, [x11, #-16]
	sub.4s	v3, v3, v5
	add.4s	v3, v3, v1
	sub.4s	v4, v4, v6
	add.4s	v4, v4, v1
	stp	q3, q4, [x11, #496]
	add	x10, x10, #32
	cmp	x10, #528
	b.ne	LBB1_9
; %bb.10:
	mov	x12, #0                         ; =0x0
	ldr	w10, [x9, #8]
	mul	w10, w10, w8
	dup.4s	v2, w10
	ldp	q4, q0, [x0, #240]
	mul.4s	v3, v2, v0
	ldp	q0, q5, [x0, #272]
	mul.4s	v6, v2, v0
	mov	w14, #7681                      ; =0x1e01
	dup.4s	v0, w14
	umull2.2d	v1, v3, v0
	umull.2d	v7, v3, v0
	uzp2.4s	v7, v7, v1
	umull2.2d	v16, v6, v0
	umull.2d	v17, v6, v0
	mov	w10, #34880                     ; =0x8840
	movk	w10, #8, lsl #16
	dup.4s	v1, w10
	uzp2.4s	v16, v17, v16
	cmhs.4s	v3, v3, v1
	and.16b	v3, v3, v0
	cmhs.4s	v6, v6, v1
	and.16b	v6, v6, v0
	ldp	q17, q18, [x0]
	sub.4s	v3, v3, v7
	sub.4s	v6, v6, v16
	add.4s	v7, v3, v17
	add.4s	v16, v6, v18
	stp	q7, q16, [x0]
	sub.4s	v3, v17, v3
	sub.4s	v6, v18, v6
	mov	x13, x0
	ldr	q7, [x13, #32]!
	add.4s	v6, v6, v0
	mul.4s	v5, v2, v5
	ldp	q16, q17, [x0, #304]
	mul.4s	v16, v2, v16
	ldr	q18, [x0, #48]
	umull2.2d	v19, v5, v0
	umull.2d	v20, v5, v0
	uzp2.4s	v19, v20, v19
	umull2.2d	v20, v16, v0
	umull.2d	v21, v16, v0
	uzp2.4s	v20, v21, v20
	cmhs.4s	v5, v5, v1
	and.16b	v5, v5, v0
	cmhs.4s	v16, v16, v1
	and.16b	v16, v16, v0
	sub.4s	v5, v5, v19
	sub.4s	v16, v16, v20
	add.4s	v19, v5, v7
	add.4s	v20, v16, v18
	str	q19, [x13]
	str	q20, [x0, #48]
	sub.4s	v5, v7, v5
	add.4s	v5, v5, v0
	sub.4s	v7, v18, v16
	add.4s	v7, v7, v0
	mov	x11, x0
	ldr	q16, [x11, #64]!
	stp	q6, q5, [x0, #272]
	mul.4s	v5, v2, v17
	ldp	q6, q17, [x0, #336]
	mul.4s	v6, v2, v6
	umull2.2d	v18, v5, v0
	umull.2d	v19, v5, v0
	uzp2.4s	v18, v19, v18
	umull2.2d	v19, v6, v0
	umull.2d	v20, v6, v0
	uzp2.4s	v19, v20, v19
	cmhs.4s	v5, v5, v1
	and.16b	v5, v5, v0
	cmhs.4s	v6, v6, v1
	and.16b	v6, v6, v0
	sub.4s	v5, v5, v18
	sub.4s	v6, v6, v19
	add.4s	v18, v5, v16
	ldp	q19, q20, [x0, #80]
	add.4s	v21, v6, v19
	str	q18, [x11]
	sub.4s	v5, v16, v5
	add.4s	v5, v5, v0
	sub.4s	v6, v19, v6
	add.4s	v6, v6, v0
	stp	q7, q5, [x0, #304]
	mul.4s	v5, v2, v17
	ldp	q7, q16, [x0, #368]
	mul.4s	v7, v2, v7
	umull2.2d	v17, v5, v0
	umull.2d	v18, v5, v0
	uzp2.4s	v17, v18, v17
	umull2.2d	v18, v7, v0
	umull.2d	v19, v7, v0
	uzp2.4s	v18, v19, v18
	cmhs.4s	v5, v5, v1
	and.16b	v5, v5, v0
	cmhs.4s	v7, v7, v1
	and.16b	v7, v7, v0
	sub.4s	v5, v5, v17
	sub.4s	v7, v7, v18
	add.4s	v17, v5, v20
	ldp	q18, q19, [x0, #112]
	add.4s	v22, v7, v18
	stp	q21, q17, [x0, #80]
	sub.4s	v5, v20, v5
	add.4s	v5, v5, v0
	sub.4s	v7, v18, v7
	add.4s	v7, v7, v0
	stp	q6, q5, [x0, #336]
	mul.4s	v5, v2, v16
	ldp	q6, q16, [x0, #400]
	mul.4s	v6, v2, v6
	umull2.2d	v17, v5, v0
	umull.2d	v18, v5, v0
	uzp2.4s	v17, v18, v17
	umull2.2d	v18, v6, v0
	umull.2d	v20, v6, v0
	uzp2.4s	v18, v20, v18
	cmhs.4s	v5, v5, v1
	and.16b	v5, v5, v0
	cmhs.4s	v6, v6, v1
	and.16b	v6, v6, v0
	sub.4s	v5, v5, v17
	sub.4s	v6, v6, v18
	add.4s	v17, v5, v19
	ldp	q18, q20, [x0, #144]
	add.4s	v21, v6, v18
	stp	q22, q17, [x0, #112]
	sub.4s	v5, v19, v5
	add.4s	v5, v5, v0
	sub.4s	v6, v18, v6
	add.4s	v6, v6, v0
	stp	q7, q5, [x0, #368]
	mul.4s	v5, v2, v16
	ldp	q7, q16, [x0, #432]
	mul.4s	v7, v2, v7
	umull2.2d	v17, v5, v0
	umull.2d	v18, v5, v0
	uzp2.4s	v17, v18, v17
	umull2.2d	v18, v7, v0
	umull.2d	v19, v7, v0
	uzp2.4s	v18, v19, v18
	cmhs.4s	v5, v5, v1
	and.16b	v5, v5, v0
	cmhs.4s	v7, v7, v1
	and.16b	v7, v7, v0
	sub.4s	v5, v5, v17
	sub.4s	v7, v7, v18
	add.4s	v17, v5, v20
	ldp	q18, q19, [x0, #176]
	add.4s	v22, v7, v18
	stp	q21, q17, [x0, #144]
	sub.4s	v5, v20, v5
	add.4s	v5, v5, v0
	sub.4s	v7, v18, v7
	stp	q6, q5, [x0, #400]
	mul.4s	v6, v2, v16
	ldp	q5, q16, [x0, #464]
	mul.4s	v17, v2, v5
	umull2.2d	v5, v6, v0
	umull.2d	v18, v6, v0
	uzp2.4s	v18, v18, v5
	umull2.2d	v5, v17, v0
	umull.2d	v20, v17, v0
	uzp2.4s	v20, v20, v5
	add.4s	v5, v3, v0
	add.4s	v3, v7, v0
	cmhs.4s	v6, v6, v1
	and.16b	v6, v6, v0
	cmhs.4s	v7, v17, v1
	and.16b	v7, v7, v0
	sub.4s	v6, v6, v18
	sub.4s	v7, v7, v20
	add.4s	v17, v6, v19
	stp	q22, q17, [x0, #176]
	ldp	q17, q18, [x0, #208]
	add.4s	v20, v7, v17
	sub.4s	v6, v19, v6
	add.4s	v6, v6, v0
	sub.4s	v7, v17, v7
	add.4s	v7, v7, v0
	stp	q3, q6, [x0, #432]
	mul.4s	v6, v2, v16
	ldp	q16, q3, [x0, #496]
	mul.4s	v2, v2, v16
	umull2.2d	v16, v6, v0
	umull.2d	v17, v6, v0
	uzp2.4s	v16, v17, v16
	umull2.2d	v17, v2, v0
	umull.2d	v19, v2, v0
	uzp2.4s	v17, v19, v17
	cmhs.4s	v6, v6, v1
	and.16b	v6, v6, v0
	cmhs.4s	v2, v2, v1
	and.16b	v2, v2, v0
	sub.4s	v6, v6, v16
	sub.4s	v2, v2, v17
	add.4s	v16, v6, v18
	add.4s	v17, v2, v4
	stp	q20, q16, [x0, #208]
	stp	q17, q5, [x0, #240]
	sub.4s	v5, v18, v6
	add.4s	v5, v5, v0
	sub.4s	v2, v4, v2
	add.4s	v2, v2, v0
	stp	q7, q5, [x0, #464]
	str	q2, [x0, #496]
	ldr	w15, [x9, #12]
	mul	w15, w15, w8
	dup.4s	v2, w15
	ldp	q5, q4, [x0, #752]
	mul.4s	v4, v2, v4
	ldp	q6, q7, [x0, #784]
	mul.4s	v6, v2, v6
	umull2.2d	v16, v4, v0
	umull.2d	v17, v4, v0
	uzp2.4s	v16, v17, v16
	umull2.2d	v17, v6, v0
	umull.2d	v18, v6, v0
	uzp2.4s	v17, v18, v17
	cmhs.4s	v4, v4, v1
	and.16b	v4, v4, v0
	cmhs.4s	v6, v6, v1
	and.16b	v6, v6, v0
	sub.4s	v4, v4, v16
	sub.4s	v6, v6, v17
	add.4s	v16, v4, v3
	ldp	q17, q18, [x0, #528]
	add.4s	v19, v6, v17
	stp	q16, q19, [x0, #512]
	sub.4s	v3, v3, v4
	add.4s	v3, v3, v0
	sub.4s	v4, v17, v6
	add.4s	v4, v4, v0
	stp	q3, q4, [x0, #768]
	mul.4s	v3, v2, v7
	ldp	q4, q6, [x0, #816]
	mul.4s	v4, v2, v4
	umull2.2d	v7, v3, v0
	umull.2d	v16, v3, v0
	uzp2.4s	v7, v16, v7
	umull2.2d	v16, v4, v0
	umull.2d	v17, v4, v0
	uzp2.4s	v16, v17, v16
	cmhs.4s	v3, v3, v1
	and.16b	v3, v3, v0
	cmhs.4s	v4, v4, v1
	and.16b	v4, v4, v0
	sub.4s	v3, v3, v7
	sub.4s	v4, v4, v16
	add.4s	v7, v3, v18
	ldp	q16, q17, [x0, #560]
	add.4s	v19, v4, v16
	stp	q7, q19, [x0, #544]
	sub.4s	v3, v18, v3
	add.4s	v3, v3, v0
	sub.4s	v4, v16, v4
	add.4s	v4, v4, v0
	stp	q3, q4, [x0, #800]
	mul.4s	v3, v2, v6
	ldp	q4, q6, [x0, #848]
	mul.4s	v4, v2, v4
	umull2.2d	v7, v3, v0
	umull.2d	v16, v3, v0
	uzp2.4s	v7, v16, v7
	umull2.2d	v16, v4, v0
	umull.2d	v18, v4, v0
	uzp2.4s	v16, v18, v16
	cmhs.4s	v3, v3, v1
	and.16b	v3, v3, v0
	cmhs.4s	v4, v4, v1
	and.16b	v4, v4, v0
	sub.4s	v3, v3, v7
	sub.4s	v4, v4, v16
	add.4s	v7, v3, v17
	ldp	q16, q18, [x0, #592]
	add.4s	v19, v4, v16
	stp	q7, q19, [x0, #576]
	sub.4s	v3, v17, v3
	add.4s	v3, v3, v0
	sub.4s	v4, v16, v4
	add.4s	v4, v4, v0
	stp	q3, q4, [x0, #832]
	mul.4s	v3, v2, v6
	ldp	q4, q6, [x0, #880]
	mul.4s	v4, v2, v4
	umull2.2d	v7, v3, v0
	umull.2d	v16, v3, v0
	uzp2.4s	v7, v16, v7
	umull2.2d	v16, v4, v0
	umull.2d	v17, v4, v0
	uzp2.4s	v16, v17, v16
	cmhs.4s	v3, v3, v1
	and.16b	v3, v3, v0
	cmhs.4s	v4, v4, v1
	and.16b	v4, v4, v0
	sub.4s	v3, v3, v7
	sub.4s	v4, v4, v16
	add.4s	v7, v3, v18
	ldp	q16, q17, [x0, #624]
	add.4s	v19, v4, v16
	stp	q7, q19, [x0, #608]
	sub.4s	v3, v18, v3
	add.4s	v3, v3, v0
	sub.4s	v4, v16, v4
	add.4s	v4, v4, v0
	stp	q3, q4, [x0, #864]
	mul.4s	v3, v2, v6
	ldp	q4, q6, [x0, #912]
	mul.4s	v4, v2, v4
	umull2.2d	v7, v3, v0
	umull.2d	v16, v3, v0
	uzp2.4s	v7, v16, v7
	umull2.2d	v16, v4, v0
	umull.2d	v18, v4, v0
	uzp2.4s	v16, v18, v16
	cmhs.4s	v3, v3, v1
	and.16b	v3, v3, v0
	cmhs.4s	v4, v4, v1
	and.16b	v4, v4, v0
	sub.4s	v3, v3, v7
	sub.4s	v4, v4, v16
	add.4s	v7, v3, v17
	ldp	q16, q18, [x0, #656]
	add.4s	v19, v4, v16
	stp	q7, q19, [x0, #640]
	sub.4s	v3, v17, v3
	add.4s	v3, v3, v0
	sub.4s	v4, v16, v4
	add.4s	v4, v4, v0
	stp	q3, q4, [x0, #896]
	mul.4s	v3, v2, v6
	ldp	q4, q6, [x0, #944]
	mul.4s	v4, v2, v4
	umull2.2d	v7, v3, v0
	umull.2d	v16, v3, v0
	uzp2.4s	v7, v16, v7
	umull2.2d	v16, v4, v0
	umull.2d	v17, v4, v0
	uzp2.4s	v16, v17, v16
	cmhs.4s	v3, v3, v1
	and.16b	v3, v3, v0
	cmhs.4s	v4, v4, v1
	and.16b	v4, v4, v0
	sub.4s	v3, v3, v7
	sub.4s	v4, v4, v16
	add.4s	v7, v3, v18
	ldp	q16, q17, [x0, #688]
	add.4s	v19, v4, v16
	stp	q7, q19, [x0, #672]
	sub.4s	v3, v18, v3
	add.4s	v3, v3, v0
	sub.4s	v4, v16, v4
	add.4s	v4, v4, v0
	stp	q3, q4, [x0, #928]
	mul.4s	v3, v2, v6
	ldp	q4, q6, [x0, #976]
	mul.4s	v4, v2, v4
	umull2.2d	v7, v3, v0
	umull.2d	v16, v3, v0
	uzp2.4s	v7, v16, v7
	umull2.2d	v16, v4, v0
	umull.2d	v18, v4, v0
	uzp2.4s	v16, v18, v16
	cmhs.4s	v3, v3, v1
	and.16b	v3, v3, v0
	cmhs.4s	v4, v4, v1
	and.16b	v4, v4, v0
	sub.4s	v3, v3, v7
	sub.4s	v4, v4, v16
	add.4s	v7, v3, v17
	ldp	q16, q18, [x0, #720]
	add.4s	v19, v4, v16
	stp	q7, q19, [x0, #704]
	sub.4s	v3, v17, v3
	add.4s	v3, v3, v0
	sub.4s	v4, v16, v4
	add.4s	v4, v4, v0
	stp	q3, q4, [x0, #960]
	ldr	q3, [x0, #1008]
	mul.4s	v4, v2, v6
	mul.4s	v2, v2, v3
	umull2.2d	v3, v4, v0
	umull.2d	v6, v4, v0
	uzp2.4s	v3, v6, v3
	umull2.2d	v6, v2, v0
	umull.2d	v7, v2, v0
	uzp2.4s	v6, v7, v6
	cmhs.4s	v4, v4, v1
	and.16b	v4, v4, v0
	cmhs.4s	v2, v2, v1
	and.16b	v2, v2, v0
	sub.4s	v3, v4, v3
	sub.4s	v2, v2, v6
	add.4s	v4, v3, v18
	add.4s	v6, v2, v5
	stp	q4, q6, [x0, #736]
	sub.4s	v3, v18, v3
	add.4s	v3, v3, v0
	sub.4s	v2, v5, v2
	add.4s	v2, v2, v0
	stp	q3, q2, [x0, #992]
	ldr	w15, [x9, #16]
	mul	w15, w15, w8
	dup.4s	v2, w15
	ldp	q3, q5, [x0]
	ldp	q4, q6, [x0, #112]
	mul.4s	v6, v2, v6
	ldp	q7, q16, [x0, #144]
	mul.4s	v7, v2, v7
	umull2.2d	v17, v6, v0
	umull.2d	v18, v6, v0
	uzp2.4s	v17, v18, v17
	umull2.2d	v18, v7, v0
	umull.2d	v19, v7, v0
	uzp2.4s	v18, v19, v18
	cmhs.4s	v6, v6, v1
	and.16b	v6, v6, v0
	cmhs.4s	v7, v7, v1
	and.16b	v7, v7, v0
	sub.4s	v6, v6, v17
	sub.4s	v7, v7, v18
	add.4s	v17, v6, v3
	add.4s	v18, v7, v5
	stp	q17, q18, [x0]
	sub.4s	v6, v3, v6
	sub.4s	v3, v5, v7
	add.4s	v3, v3, v0
	ldr	q5, [x13]
	mul.4s	v7, v2, v16
	ldp	q16, q17, [x0, #176]
	mul.4s	v16, v2, v16
	umull2.2d	v18, v7, v0
	umull.2d	v19, v7, v0
	uzp2.4s	v18, v19, v18
	umull2.2d	v19, v16, v0
	umull.2d	v20, v16, v0
	uzp2.4s	v19, v20, v19
	ldr	q20, [x0, #48]
	cmhs.4s	v7, v7, v1
	and.16b	v7, v7, v0
	cmhs.4s	v16, v16, v1
	and.16b	v16, v16, v0
	sub.4s	v7, v7, v18
	sub.4s	v16, v16, v19
	add.4s	v18, v7, v5
	add.4s	v19, v16, v20
	str	q18, [x13]
	str	q19, [x0, #48]
	sub.4s	v5, v5, v7
	add.4s	v5, v5, v0
	sub.4s	v7, v20, v16
	add.4s	v7, v7, v0
	stp	q3, q5, [x0, #144]
	ldr	q3, [x11]
	mul.4s	v5, v2, v17
	ldp	q16, q17, [x0, #208]
	mul.4s	v16, v2, v16
	umull2.2d	v18, v5, v0
	umull.2d	v19, v5, v0
	uzp2.4s	v18, v19, v18
	umull2.2d	v19, v16, v0
	umull.2d	v20, v16, v0
	uzp2.4s	v19, v20, v19
	cmhs.4s	v5, v5, v1
	and.16b	v5, v5, v0
	cmhs.4s	v16, v16, v1
	and.16b	v16, v16, v0
	sub.4s	v5, v5, v18
	sub.4s	v16, v16, v19
	add.4s	v18, v5, v3
	str	q18, [x11]
	ldp	q18, q19, [x0, #80]
	add.4s	v20, v16, v18
	sub.4s	v3, v3, v5
	add.4s	v3, v3, v0
	sub.4s	v5, v18, v16
	stp	q7, q3, [x0, #176]
	mul.4s	v7, v2, v17
	ldp	q16, q3, [x0, #240]
	mul.4s	v2, v2, v16
	umull2.2d	v16, v7, v0
	umull.2d	v17, v7, v0
	uzp2.4s	v16, v17, v16
	umull2.2d	v17, v2, v0
	umull.2d	v18, v2, v0
	uzp2.4s	v17, v18, v17
	add.4s	v6, v6, v0
	add.4s	v5, v5, v0
	cmhs.4s	v7, v7, v1
	and.16b	v7, v7, v0
	cmhs.4s	v2, v2, v1
	and.16b	v2, v2, v0
	sub.4s	v7, v7, v16
	sub.4s	v2, v2, v17
	add.4s	v16, v7, v19
	add.4s	v17, v2, v4
	stp	q20, q16, [x0, #80]
	stp	q17, q6, [x0, #112]
	sub.4s	v6, v19, v7
	add.4s	v6, v6, v0
	sub.4s	v2, v4, v2
	add.4s	v2, v2, v0
	stp	q5, q6, [x0, #208]
	str	q2, [x0, #240]
	ldr	w15, [x9, #20]
	mul	w15, w15, w8
	dup.4s	v2, w15
	ldp	q4, q5, [x0, #368]
	mul.4s	v5, v2, v5
	ldp	q6, q7, [x0, #400]
	mul.4s	v6, v2, v6
	umull2.2d	v16, v5, v0
	umull.2d	v17, v5, v0
	uzp2.4s	v16, v17, v16
	umull2.2d	v17, v6, v0
	umull.2d	v18, v6, v0
	uzp2.4s	v17, v18, v17
	cmhs.4s	v5, v5, v1
	and.16b	v5, v5, v0
	cmhs.4s	v6, v6, v1
	and.16b	v6, v6, v0
	sub.4s	v5, v5, v16
	sub.4s	v6, v6, v17
	add.4s	v16, v5, v3
	ldp	q17, q18, [x0, #272]
	add.4s	v19, v6, v17
	stp	q16, q19, [x0, #256]
	sub.4s	v3, v3, v5
	add.4s	v3, v3, v0
	sub.4s	v5, v17, v6
	add.4s	v5, v5, v0
	stp	q3, q5, [x0, #384]
	mul.4s	v3, v2, v7
	ldp	q5, q6, [x0, #432]
	mul.4s	v5, v2, v5
	umull2.2d	v7, v3, v0
	umull.2d	v16, v3, v0
	uzp2.4s	v7, v16, v7
	umull2.2d	v16, v5, v0
	umull.2d	v17, v5, v0
	uzp2.4s	v16, v17, v16
	cmhs.4s	v3, v3, v1
	and.16b	v3, v3, v0
	cmhs.4s	v5, v5, v1
	and.16b	v5, v5, v0
	sub.4s	v3, v3, v7
	sub.4s	v5, v5, v16
	add.4s	v7, v3, v18
	ldp	q16, q17, [x0, #304]
	add.4s	v19, v5, v16
	stp	q7, q19, [x0, #288]
	sub.4s	v3, v18, v3
	add.4s	v3, v3, v0
	sub.4s	v5, v16, v5
	add.4s	v5, v5, v0
	stp	q3, q5, [x0, #416]
	mul.4s	v3, v2, v6
	ldp	q5, q6, [x0, #464]
	mul.4s	v5, v2, v5
	umull2.2d	v7, v3, v0
	umull.2d	v16, v3, v0
	uzp2.4s	v7, v16, v7
	umull2.2d	v16, v5, v0
	umull.2d	v18, v5, v0
	uzp2.4s	v16, v18, v16
	cmhs.4s	v3, v3, v1
	and.16b	v3, v3, v0
	cmhs.4s	v5, v5, v1
	and.16b	v5, v5, v0
	sub.4s	v3, v3, v7
	sub.4s	v5, v5, v16
	add.4s	v7, v3, v17
	ldp	q16, q18, [x0, #336]
	add.4s	v19, v5, v16
	stp	q7, q19, [x0, #320]
	sub.4s	v3, v17, v3
	add.4s	v3, v3, v0
	sub.4s	v5, v16, v5
	add.4s	v5, v5, v0
	stp	q3, q5, [x0, #448]
	mul.4s	v5, v2, v6
	ldp	q6, q3, [x0, #496]
	mul.4s	v2, v2, v6
	umull2.2d	v6, v5, v0
	umull.2d	v7, v5, v0
	uzp2.4s	v6, v7, v6
	umull2.2d	v7, v2, v0
	umull.2d	v16, v2, v0
	uzp2.4s	v7, v16, v7
	cmhs.4s	v5, v5, v1
	and.16b	v5, v5, v0
	cmhs.4s	v2, v2, v1
	and.16b	v2, v2, v0
	sub.4s	v5, v5, v6
	sub.4s	v2, v2, v7
	add.4s	v6, v5, v18
	add.4s	v7, v2, v4
	stp	q6, q7, [x0, #352]
	sub.4s	v5, v18, v5
	add.4s	v5, v5, v0
	sub.4s	v2, v4, v2
	add.4s	v2, v2, v0
	stp	q5, q2, [x0, #480]
	ldr	w15, [x9, #24]
	mul	w15, w15, w8
	dup.4s	v2, w15
	ldp	q4, q5, [x0, #624]
	mul.4s	v5, v2, v5
	ldp	q6, q7, [x0, #656]
	mul.4s	v6, v2, v6
	umull2.2d	v16, v5, v0
	umull.2d	v17, v5, v0
	uzp2.4s	v16, v17, v16
	umull2.2d	v17, v6, v0
	umull.2d	v18, v6, v0
	uzp2.4s	v17, v18, v17
	cmhs.4s	v5, v5, v1
	and.16b	v5, v5, v0
	cmhs.4s	v6, v6, v1
	and.16b	v6, v6, v0
	sub.4s	v5, v5, v16
	sub.4s	v6, v6, v17
	add.4s	v16, v5, v3
	ldp	q17, q18, [x0, #528]
	add.4s	v19, v6, v17
	stp	q16, q19, [x0, #512]
	sub.4s	v3, v3, v5
	add.4s	v3, v3, v0
	sub.4s	v5, v17, v6
	add.4s	v5, v5, v0
	stp	q3, q5, [x0, #640]
	mul.4s	v3, v2, v7
	ldp	q5, q6, [x0, #688]
	mul.4s	v5, v2, v5
	umull2.2d	v7, v3, v0
	umull.2d	v16, v3, v0
	uzp2.4s	v7, v16, v7
	umull2.2d	v16, v5, v0
	umull.2d	v17, v5, v0
	uzp2.4s	v16, v17, v16
	cmhs.4s	v3, v3, v1
	and.16b	v3, v3, v0
	cmhs.4s	v5, v5, v1
	and.16b	v5, v5, v0
	sub.4s	v3, v3, v7
	sub.4s	v5, v5, v16
	add.4s	v7, v3, v18
	ldp	q16, q17, [x0, #560]
	add.4s	v19, v5, v16
	stp	q7, q19, [x0, #544]
	sub.4s	v3, v18, v3
	add.4s	v3, v3, v0
	sub.4s	v5, v16, v5
	add.4s	v5, v5, v0
	stp	q3, q5, [x0, #672]
	mul.4s	v3, v2, v6
	ldp	q5, q6, [x0, #720]
	mul.4s	v5, v2, v5
	umull2.2d	v7, v3, v0
	umull.2d	v16, v3, v0
	uzp2.4s	v7, v16, v7
	umull2.2d	v16, v5, v0
	umull.2d	v18, v5, v0
	uzp2.4s	v16, v18, v16
	cmhs.4s	v3, v3, v1
	and.16b	v3, v3, v0
	cmhs.4s	v5, v5, v1
	and.16b	v5, v5, v0
	sub.4s	v3, v3, v7
	sub.4s	v5, v5, v16
	add.4s	v7, v3, v17
	ldp	q16, q18, [x0, #592]
	add.4s	v19, v5, v16
	stp	q7, q19, [x0, #576]
	sub.4s	v3, v17, v3
	add.4s	v3, v3, v0
	sub.4s	v5, v16, v5
	add.4s	v5, v5, v0
	stp	q3, q5, [x0, #704]
	mul.4s	v5, v2, v6
	ldp	q6, q3, [x0, #752]
	mul.4s	v2, v2, v6
	umull2.2d	v6, v5, v0
	umull.2d	v7, v5, v0
	uzp2.4s	v6, v7, v6
	umull2.2d	v7, v2, v0
	umull.2d	v16, v2, v0
	uzp2.4s	v7, v16, v7
	cmhs.4s	v5, v5, v1
	and.16b	v5, v5, v0
	cmhs.4s	v2, v2, v1
	and.16b	v2, v2, v0
	sub.4s	v5, v5, v6
	sub.4s	v2, v2, v7
	add.4s	v6, v5, v18
	add.4s	v7, v2, v4
	stp	q6, q7, [x0, #608]
	sub.4s	v5, v18, v5
	add.4s	v5, v5, v0
	sub.4s	v2, v4, v2
	add.4s	v2, v2, v0
	stp	q5, q2, [x0, #736]
	ldr	w15, [x9, #28]
	mul	w15, w15, w8
	dup.4s	v2, w15
	ldp	q4, q5, [x0, #880]
	mul.4s	v5, v2, v5
	ldp	q6, q7, [x0, #912]
	mul.4s	v6, v2, v6
	umull2.2d	v16, v5, v0
	umull.2d	v17, v5, v0
	uzp2.4s	v16, v17, v16
	umull2.2d	v17, v6, v0
	umull.2d	v18, v6, v0
	uzp2.4s	v17, v18, v17
	cmhs.4s	v5, v5, v1
	and.16b	v5, v5, v0
	cmhs.4s	v6, v6, v1
	and.16b	v6, v6, v0
	sub.4s	v5, v5, v16
	sub.4s	v6, v6, v17
	add.4s	v16, v5, v3
	ldp	q17, q18, [x0, #784]
	add.4s	v19, v6, v17
	stp	q16, q19, [x0, #768]
	sub.4s	v3, v3, v5
	add.4s	v3, v3, v0
	sub.4s	v5, v17, v6
	add.4s	v5, v5, v0
	stp	q3, q5, [x0, #896]
	mul.4s	v3, v2, v7
	ldp	q5, q6, [x0, #944]
	mul.4s	v5, v2, v5
	umull2.2d	v7, v3, v0
	umull.2d	v16, v3, v0
	uzp2.4s	v7, v16, v7
	umull2.2d	v16, v5, v0
	umull.2d	v17, v5, v0
	uzp2.4s	v16, v17, v16
	cmhs.4s	v3, v3, v1
	and.16b	v3, v3, v0
	cmhs.4s	v5, v5, v1
	and.16b	v5, v5, v0
	sub.4s	v3, v3, v7
	sub.4s	v5, v5, v16
	add.4s	v7, v3, v18
	ldp	q16, q17, [x0, #816]
	add.4s	v19, v5, v16
	stp	q7, q19, [x0, #800]
	sub.4s	v3, v18, v3
	add.4s	v3, v3, v0
	sub.4s	v5, v16, v5
	add.4s	v5, v5, v0
	stp	q3, q5, [x0, #928]
	mul.4s	v3, v2, v6
	ldp	q5, q6, [x0, #976]
	mul.4s	v5, v2, v5
	umull2.2d	v7, v3, v0
	umull.2d	v16, v3, v0
	uzp2.4s	v7, v16, v7
	umull2.2d	v16, v5, v0
	umull.2d	v18, v5, v0
	uzp2.4s	v16, v18, v16
	cmhs.4s	v3, v3, v1
	and.16b	v3, v3, v0
	cmhs.4s	v5, v5, v1
	and.16b	v5, v5, v0
	sub.4s	v3, v3, v7
	sub.4s	v5, v5, v16
	add.4s	v7, v3, v17
	ldp	q16, q18, [x0, #848]
	add.4s	v19, v5, v16
	stp	q7, q19, [x0, #832]
	sub.4s	v3, v17, v3
	add.4s	v3, v3, v0
	sub.4s	v5, v16, v5
	add.4s	v5, v5, v0
	stp	q3, q5, [x0, #960]
	ldr	q3, [x0, #1008]
	mul.4s	v5, v2, v6
	mul.4s	v2, v2, v3
	umull2.2d	v3, v5, v0
	umull.2d	v6, v5, v0
	uzp2.4s	v3, v6, v3
	umull2.2d	v6, v2, v0
	umull.2d	v7, v2, v0
	uzp2.4s	v6, v7, v6
	cmhs.4s	v5, v5, v1
	and.16b	v5, v5, v0
	cmhs.4s	v1, v2, v1
	and.16b	v1, v1, v0
	sub.4s	v2, v5, v3
	sub.4s	v1, v1, v6
	add.4s	v3, v2, v18
	add.4s	v5, v1, v4
	stp	q3, q5, [x0, #864]
	sub.4s	v2, v18, v2
	add.4s	v2, v2, v0
	sub.4s	v1, v4, v1
	add.4s	v0, v1, v0
	stp	q2, q0, [x0, #992]
	add	x17, x9, #32
	add	x15, x0, #4
	add	x16, x0, #8
LBB1_11:                                ; =>This Inner Loop Header: Depth=1
	ldr	w1, [x17], #4
	mul	w2, w1, w8
	add	x1, x0, x12
	ldp	w3, w4, [x1, #64]
	mul	w3, w2, w3
	umull	x5, w3, w14
	lsr	x5, x5, #32
	cmp	w3, w10
	csel	w3, wzr, w14, lo
	ldp	w6, w7, [x1]
	sub	w3, w3, w5
	add	w5, w3, w6
	sub	w3, w6, w3
	add	w3, w3, w14
	mul	w4, w2, w4
	umull	x6, w4, w14
	lsr	x6, x6, #32
	cmp	w4, w10
	csel	w4, wzr, w14, lo
	sub	w4, w4, w6
	add	w6, w4, w7
	sub	w4, w7, w4
	add	w4, w4, w14
	ldp	w7, w19, [x1, #72]
	mul	w7, w2, w7
	umull	x20, w7, w14
	lsr	x20, x20, #32
	cmp	w7, w10
	csel	w7, wzr, w14, lo
	ldp	w21, w22, [x1, #8]
	sub	w7, w7, w20
	add	w20, w7, w21
	sub	w7, w21, w7
	add	w7, w7, w14
	mul	w19, w2, w19
	stp	w5, w6, [x1]
	umull	x5, w19, w14
	lsr	x5, x5, #32
	cmp	w19, w10
	csel	w6, wzr, w14, lo
	stp	w3, w4, [x1, #64]
	sub	w3, w6, w5
	add	w4, w3, w22
	sub	w3, w22, w3
	add	w3, w3, w14
	ldp	w5, w6, [x1, #80]
	mul	w5, w2, w5
	stp	w20, w4, [x1, #8]
	umull	x4, w5, w14
	lsr	x4, x4, #32
	cmp	w5, w10
	csel	w5, wzr, w14, lo
	stp	w7, w3, [x1, #72]
	ldp	w3, w7, [x1, #16]
	sub	w4, w5, w4
	add	w5, w4, w3
	sub	w3, w3, w4
	add	w3, w3, w14
	mul	w4, w2, w6
	umull	x6, w4, w14
	lsr	x6, x6, #32
	cmp	w4, w10
	csel	w4, wzr, w14, lo
	sub	w4, w4, w6
	add	w6, w4, w7
	stp	w5, w6, [x1, #16]
	sub	w4, w7, w4
	add	w4, w4, w14
	ldp	w5, w6, [x1, #88]
	mul	w5, w2, w5
	umull	x7, w5, w14
	lsr	x7, x7, #32
	stp	w3, w4, [x1, #80]
	cmp	w5, w10
	csel	w3, wzr, w14, lo
	ldp	w4, w5, [x1, #24]
	sub	w3, w3, w7
	add	w7, w3, w4
	sub	w3, w4, w3
	add	w3, w3, w14
	mul	w4, w2, w6
	umull	x6, w4, w14
	lsr	x6, x6, #32
	cmp	w4, w10
	csel	w4, wzr, w14, lo
	sub	w4, w4, w6
	add	w6, w4, w5
	stp	w7, w6, [x1, #24]
	sub	w4, w5, w4
	add	w4, w4, w14
	stp	w3, w4, [x1, #88]
	ldp	w3, w4, [x1, #96]
	mul	w3, w2, w3
	umull	x5, w3, w14
	lsr	x5, x5, #32
	cmp	w3, w10
	csel	w3, wzr, w14, lo
	ldp	w6, w7, [x1, #32]
	sub	w3, w3, w5
	add	w5, w3, w6
	sub	w3, w6, w3
	add	w3, w3, w14
	mul	w4, w2, w4
	umull	x6, w4, w14
	lsr	x6, x6, #32
	cmp	w4, w10
	csel	w4, wzr, w14, lo
	sub	w4, w4, w6
	add	w6, w4, w7
	stp	w5, w6, [x1, #32]
	sub	w4, w7, w4
	add	w4, w4, w14
	stp	w3, w4, [x1, #96]
	ldp	w3, w4, [x1, #104]
	mul	w3, w2, w3
	umull	x5, w3, w14
	lsr	x5, x5, #32
	cmp	w3, w10
	csel	w3, wzr, w14, lo
	ldp	w6, w7, [x1, #40]
	sub	w3, w3, w5
	add	w5, w3, w6
	sub	w3, w6, w3
	add	w3, w3, w14
	mul	w4, w2, w4
	umull	x6, w4, w14
	lsr	x6, x6, #32
	cmp	w4, w10
	csel	w4, wzr, w14, lo
	sub	w4, w4, w6
	add	w6, w4, w7
	stp	w5, w6, [x1, #40]
	sub	w4, w7, w4
	add	w4, w4, w14
	stp	w3, w4, [x1, #104]
	ldp	w3, w4, [x1, #112]
	mul	w3, w2, w3
	umull	x5, w3, w14
	lsr	x5, x5, #32
	cmp	w3, w10
	csel	w3, wzr, w14, lo
	ldp	w6, w7, [x1, #48]
	sub	w3, w3, w5
	add	w5, w3, w6
	sub	w3, w6, w3
	add	w3, w3, w14
	mul	w4, w2, w4
	umull	x6, w4, w14
	lsr	x6, x6, #32
	cmp	w4, w10
	csel	w4, wzr, w14, lo
	sub	w4, w4, w6
	add	w6, w4, w7
	stp	w5, w6, [x1, #48]
	sub	w4, w7, w4
	add	w4, w4, w14
	stp	w3, w4, [x1, #112]
	ldp	w3, w4, [x1, #120]
	mul	w3, w2, w3
	umull	x5, w3, w14
	lsr	x5, x5, #32
	cmp	w3, w10
	csel	w3, wzr, w14, lo
	ldp	w6, w7, [x1, #56]
	sub	w3, w3, w5
	add	w5, w3, w6
	mul	w2, w2, w4
	umull	x4, w2, w14
	lsr	x4, x4, #32
	cmp	w2, w10
	csel	w2, wzr, w14, lo
	sub	w2, w2, w4
	add	w4, w2, w7
	stp	w5, w4, [x1, #56]
	sub	w3, w6, w3
	add	w3, w3, w14
	sub	w2, w7, w2
	add	w2, w2, w14
	stp	w3, w2, [x1, #120]
	add	x12, x12, #128
	cmp	x12, #1024
	b.ne	LBB1_11
; %bb.12:
	add	x17, x9, #64
	add	x12, x0, #1024
	add	x14, x9, #128
	cmp	x0, x14
	b.hs	LBB1_16
; %bb.13:
	cmp	x17, x12
	b.hs	LBB1_16
; %bb.14:
	mov	x1, #0                          ; =0x0
	mov	w2, #7681                       ; =0x1e01
LBB1_15:                                ; =>This Inner Loop Header: Depth=1
	ldr	w3, [x17, x1]
	mul	w3, w3, w8
	ldp	w4, w5, [x13]
	mul	w4, w3, w4
	umull	x6, w4, w2
	lsr	x6, x6, #32
	cmp	w4, w10
	csel	w4, wzr, w2, lo
	ldp	w7, w19, [x13, #-32]
	sub	w4, w4, w6
	add	w6, w4, w7
	sub	w4, w7, w4
	add	w4, w4, w2
	mul	w5, w3, w5
	umull	x7, w5, w2
	lsr	x7, x7, #32
	cmp	w5, w10
	csel	w5, wzr, w2, lo
	sub	w5, w5, w7
	add	w7, w5, w19
	stp	w6, w7, [x13, #-32]
	sub	w5, w19, w5
	add	w5, w5, w2
	stp	w4, w5, [x13]
	ldp	w4, w5, [x13, #8]
	mul	w4, w3, w4
	umull	x6, w4, w2
	lsr	x6, x6, #32
	cmp	w4, w10
	csel	w4, wzr, w2, lo
	ldp	w7, w19, [x13, #-24]
	sub	w4, w4, w6
	add	w6, w4, w7
	sub	w4, w7, w4
	add	w4, w4, w2
	mul	w5, w3, w5
	umull	x7, w5, w2
	lsr	x7, x7, #32
	cmp	w5, w10
	csel	w5, wzr, w2, lo
	sub	w5, w5, w7
	add	w7, w5, w19
	stp	w6, w7, [x13, #-24]
	sub	w5, w19, w5
	add	w5, w5, w2
	stp	w4, w5, [x13, #8]
	ldp	w4, w5, [x13, #16]
	mul	w4, w3, w4
	umull	x6, w4, w2
	lsr	x6, x6, #32
	cmp	w4, w10
	csel	w4, wzr, w2, lo
	ldp	w7, w19, [x13, #-16]
	sub	w4, w4, w6
	add	w6, w4, w7
	sub	w4, w7, w4
	add	w4, w4, w2
	mul	w5, w3, w5
	umull	x7, w5, w2
	lsr	x7, x7, #32
	cmp	w5, w10
	csel	w5, wzr, w2, lo
	sub	w5, w5, w7
	add	w7, w5, w19
	stp	w6, w7, [x13, #-16]
	sub	w5, w19, w5
	add	w5, w5, w2
	stp	w4, w5, [x13, #16]
	ldp	w4, w5, [x13, #24]
	mul	w4, w3, w4
	umull	x6, w4, w2
	lsr	x6, x6, #32
	cmp	w4, w10
	csel	w4, wzr, w2, lo
	ldp	w7, w19, [x13, #-8]
	sub	w4, w4, w6
	add	w6, w4, w7
	sub	w4, w7, w4
	add	w4, w4, w2
	mul	w3, w3, w5
	umull	x5, w3, w2
	lsr	x5, x5, #32
	cmp	w3, w10
	csel	w3, wzr, w2, lo
	sub	w3, w3, w5
	add	w5, w3, w19
	stp	w6, w5, [x13, #-8]
	sub	w3, w19, w3
	add	w3, w3, w2
	stp	w4, w3, [x13, #24]
	add	x1, x1, #4
	add	x13, x13, #64
	cmp	x1, #64
	b.ne	LBB1_15
	b	LBB1_18
LBB1_16:
	mov	x13, #0                         ; =0x0
	add	x1, x0, #128
	mov	w2, #57857                      ; =0xe201
	movk	w2, #35715, lsl #16
	dup.4s	v0, w2
	mov	w2, #7681                       ; =0x1e01
	dup.4s	v1, w2
	mov	w2, #34880                      ; =0x8840
	movk	w2, #8, lsl #16
	dup.4s	v2, w2
LBB1_17:                                ; =>This Inner Loop Header: Depth=1
	sub	x2, x1, #64
	ldur	s4, [x1, #-128]
	ld1.s	{ v4 }[1], [x2]
	ldr	q3, [x17, x13]
	mov	x2, x1
	ld1.s	{ v4 }[2], [x2], #4
	mul.4s	v3, v3, v0
	add	x3, x1, #64
	ld1.s	{ v4 }[3], [x3]
	sub	x3, x1, #32
	ldp	s6, s5, [x1, #-100]
	ld1.s	{ v5 }[1], [x3]
	add	x3, x1, #32
	ld1.s	{ v5 }[2], [x3]
	add	x3, x1, #96
	ld1.s	{ v5 }[3], [x3]
	mul.4s	v5, v3, v5
	umull2.2d	v7, v5, v1
	umull.2d	v16, v5, v1
	uzp2.4s	v7, v16, v7
	cmhs.4s	v5, v5, v2
	and.16b	v5, v5, v1
	sub.4s	v5, v5, v7
	add.4s	v19, v5, v4
	sub	x3, x1, #60
	ldp	s16, s17, [x1, #-124]
	ld1.s	{ v16 }[1], [x3]
	ld1.s	{ v16 }[2], [x2]
	sub.4s	v4, v4, v5
	add	x2, x1, #68
	ld1.s	{ v16 }[3], [x2]
	sub	x2, x1, #28
	ldp	s5, s7, [x1, #-92]
	ld1.s	{ v5 }[1], [x2]
	add	x2, x1, #36
	ld1.s	{ v5 }[2], [x2]
	add	x2, x1, #100
	ld1.s	{ v5 }[3], [x2]
	mul.4s	v5, v3, v5
	umull2.2d	v18, v5, v1
	umull.2d	v20, v5, v1
	uzp2.4s	v18, v20, v18
	cmhs.4s	v5, v5, v2
	and.16b	v5, v5, v1
	sub.4s	v20, v5, v18
	add.4s	v23, v20, v16
	sub	x2, x1, #56
	ld1.s	{ v17 }[1], [x2]
	add	x2, x1, #8
	ld1.s	{ v17 }[2], [x2]
	add	x2, x1, #72
	ld1.s	{ v17 }[3], [x2]
	sub	x2, x1, #24
	ld1.s	{ v7 }[1], [x2]
	add	x2, x1, #40
	ld1.s	{ v7 }[2], [x2]
	add	x2, x1, #104
	ld1.s	{ v7 }[3], [x2]
	mul.4s	v5, v3, v7
	umull2.2d	v7, v5, v1
	umull.2d	v18, v5, v1
	uzp2.4s	v7, v18, v7
	cmhs.4s	v5, v5, v2
	and.16b	v5, v5, v1
	sub.4s	v24, v5, v7
	add.4s	v25, v24, v17
	sub	x2, x1, #52
	add	x3, x1, #12
	add	x4, x1, #76
	ldp	s5, s7, [x1, #-116]
	ld1.s	{ v5 }[1], [x2]
	ld1.s	{ v5 }[2], [x3]
	ld1.s	{ v5 }[3], [x4]
	sub	x2, x1, #20
	add	x3, x1, #44
	ldp	s18, s26, [x1, #-84]
	ld1.s	{ v18 }[1], [x2]
	add	x2, x1, #108
	ld1.s	{ v18 }[2], [x3]
	ld1.s	{ v18 }[3], [x2]
	mul.4s	v18, v3, v18
	umull2.2d	v21, v18, v1
	umull.2d	v22, v18, v1
	uzp2.4s	v21, v22, v21
	cmhs.4s	v18, v18, v2
	and.16b	v18, v18, v1
	sub.4s	v18, v18, v21
	add.4s	v27, v18, v5
	zip1.4s	v21, v19, v23
	mov.s	v21[2], v25[0]
	mov.s	v21[3], v27[0]
	sub.4s	v16, v16, v20
	trn2.4s	v22, v19, v23
	mov.s	v22[2], v25[1]
	mov.s	v22[3], v27[1]
	sub.4s	v17, v17, v24
	zip2.4s	v20, v19, v23
	mov.s	v20[2], v25[2]
	mov.s	v20[3], v27[2]
	uzp2.4s	v23, v19, v23
	uzp2.4s	v19, v23, v19
	mov.s	v19[2], v25[3]
	mov.s	v19[3], v27[3]
	sub	x2, x1, #48
	add	x3, x1, #16
	ld1.s	{ v7 }[1], [x2]
	ld1.s	{ v7 }[2], [x3]
	add	x2, x1, #80
	ld1.s	{ v7 }[3], [x2]
	sub	x2, x1, #16
	add	x3, x1, #48
	add	x4, x1, #112
	ld1.s	{ v26 }[1], [x2]
	ld1.s	{ v26 }[2], [x3]
	ld1.s	{ v26 }[3], [x4]
	mul.4s	v23, v3, v26
	umull2.2d	v24, v23, v1
	umull.2d	v25, v23, v1
	uzp2.4s	v24, v25, v24
	cmhs.4s	v23, v23, v2
	and.16b	v23, v23, v1
	sub.4s	v25, v23, v24
	sub	x2, x1, #44
	add	x3, x1, #20
	add	x4, x1, #84
	ldp	s24, s23, [x1, #-108]
	ld1.s	{ v24 }[1], [x2]
	ld1.s	{ v24 }[2], [x3]
	ld1.s	{ v24 }[3], [x4]
	add.4s	v26, v25, v7
	sub	x2, x1, #12
	add	x3, x1, #52
	add	x4, x1, #116
	ldp	s27, s29, [x1, #-76]
	ld1.s	{ v27 }[1], [x2]
	ld1.s	{ v27 }[2], [x3]
	ld1.s	{ v27 }[3], [x4]
	mul.4s	v27, v3, v27
	umull2.2d	v28, v27, v1
	umull.2d	v30, v27, v1
	uzp2.4s	v28, v30, v28
	cmhs.4s	v27, v27, v2
	and.16b	v27, v27, v1
	sub.4s	v27, v27, v28
	add.4s	v28, v27, v24
	sub	x2, x1, #40
	add	x3, x1, #24
	ld1.s	{ v23 }[1], [x2]
	add	x2, x1, #88
	ld1.s	{ v23 }[2], [x3]
	ld1.s	{ v23 }[3], [x2]
	sub	x2, x1, #8
	add	x3, x1, #56
	add	x4, x1, #120
	ld1.s	{ v29 }[1], [x2]
	ld1.s	{ v29 }[2], [x3]
	ld1.s	{ v29 }[3], [x4]
	mul.4s	v29, v3, v29
	umull2.2d	v30, v29, v1
	umull.2d	v31, v29, v1
	uzp2.4s	v30, v31, v30
	cmhs.4s	v29, v29, v2
	and.16b	v29, v29, v1
	sub.4s	v29, v29, v30
	sub	x2, x1, #36
	add	x3, x1, #28
	add	x4, x1, #92
	ld1.s	{ v6 }[1], [x2]
	ld1.s	{ v6 }[2], [x3]
	ld1.s	{ v6 }[3], [x4]
	add.4s	v30, v29, v23
	sub	x2, x1, #4
	add	x3, x1, #60
	ldur	s31, [x1, #-68]
	ld1.s	{ v31 }[1], [x2]
	add	x2, x1, #124
	ld1.s	{ v31 }[2], [x3]
	ld1.s	{ v31 }[3], [x2]
	mul.4s	v3, v3, v31
	umull2.2d	v31, v3, v1
	umull.2d	v8, v3, v1
	uzp2.4s	v31, v8, v31
	cmhs.4s	v3, v3, v2
	and.16b	v3, v3, v1
	sub.4s	v3, v3, v31
	add.4s	v31, v3, v6
	zip1.4s	v8, v26, v28
	mov.s	v8[2], v30[0]
	mov.s	v8[3], v31[0]
	trn2.4s	v9, v26, v28
	mov.s	v9[2], v30[1]
	mov.s	v9[3], v31[1]
	stp	q21, q8, [x1, #-128]
	stp	q22, q9, [x1, #-64]
	zip2.4s	v21, v26, v28
	mov.s	v21[2], v30[2]
	mov.s	v21[3], v31[2]
	stp	q20, q21, [x1]
	sub.4s	v5, v5, v18
	sub.4s	v7, v7, v25
	uzp2.4s	v18, v26, v28
	uzp2.4s	v18, v18, v26
	mov.s	v18[2], v30[3]
	mov.s	v18[3], v31[3]
	stp	q19, q18, [x1, #64]
	add.4s	v4, v4, v1
	add.4s	v16, v16, v1
	add.4s	v17, v17, v1
	add.4s	v5, v5, v1
	sub.4s	v18, v24, v27
	zip1.4s	v19, v4, v16
	mov.s	v19[2], v17[0]
	mov.s	v19[3], v5[0]
	add.4s	v7, v7, v1
	add.4s	v18, v18, v1
	sub.4s	v20, v23, v29
	add.4s	v20, v20, v1
	sub.4s	v3, v6, v3
	add.4s	v3, v3, v1
	zip1.4s	v6, v7, v18
	mov.s	v6[2], v20[0]
	mov.s	v6[3], v3[0]
	stp	q19, q6, [x1, #-96]
	trn2.4s	v6, v4, v16
	mov.s	v6[2], v17[1]
	mov.s	v6[3], v5[1]
	trn2.4s	v19, v7, v18
	mov.s	v19[2], v20[1]
	mov.s	v19[3], v3[1]
	stp	q6, q19, [x1, #-32]
	zip2.4s	v6, v4, v16
	mov.s	v6[2], v17[2]
	mov.s	v6[3], v5[2]
	uzp2.4s	v16, v4, v16
	uzp2.4s	v4, v16, v4
	mov.s	v4[2], v17[3]
	mov.s	v4[3], v5[3]
	zip2.4s	v5, v7, v18
	mov.s	v5[2], v20[2]
	mov.s	v5[3], v3[2]
	uzp2.4s	v16, v7, v18
	uzp2.4s	v7, v16, v7
	mov.s	v7[2], v20[3]
	mov.s	v7[3], v3[3]
	stp	q6, q5, [x1, #32]
	stp	q4, q7, [x1, #96]
	add	x13, x13, #16
	add	x1, x1, #256
	cmp	x13, #64
	b.ne	LBB1_17
LBB1_18:
	add	x13, x9, #256
	cmp	x0, x13
	b.hs	LBB1_22
; %bb.19:
	cmp	x14, x12
	b.hs	LBB1_22
; %bb.20:
	mov	x11, #0                         ; =0x0
	mov	w17, #7681                      ; =0x1e01
	dup.4s	v0, w17
	mov	w17, #34880                     ; =0x8840
	movk	w17, #8, lsl #16
	dup.4s	v1, w17
	mvni.4s	v2, #30, lsl #8
	mov	x17, x0
LBB1_21:                                ; =>This Inner Loop Header: Depth=1
	ldr	w1, [x14, x11]
	mul	w1, w1, w8
	ldp	q3, q4, [x17]
	dup.4s	v5, w1
	mul.4s	v4, v5, v4
	umull2.2d	v5, v4, v0
	umull.2d	v6, v4, v0
	uzp2.4s	v5, v6, v5
	cmhs.4s	v4, v4, v1
	and.16b	v4, v4, v0
	sub.4s	v4, v4, v5
	add.4s	v5, v4, v3
	sub.4s	v3, v3, v4
	sub.4s	v3, v3, v2
	stp	q5, q3, [x17], #32
	add	x11, x11, #4
	cmp	x11, #128
	b.ne	LBB1_21
	b	LBB1_24
LBB1_22:
	mov	x17, #0                         ; =0x0
	mov	w1, #57857                      ; =0xe201
	movk	w1, #35715, lsl #16
	dup.4s	v0, w1
	mov	w1, #7681                       ; =0x1e01
	dup.4s	v1, w1
	mov	w1, #34880                      ; =0x8840
	movk	w1, #8, lsl #16
	dup.4s	v2, w1
LBB1_23:                                ; =>This Inner Loop Header: Depth=1
	ldr	q3, [x14, x17]
	mul.4s	v3, v3, v0
	sub	x1, x11, #32
	add	x2, x11, #32
	ldur	s5, [x11, #-64]
	ld1.s	{ v5 }[1], [x1]
	mov	x1, x11
	ld1.s	{ v5 }[2], [x1], #4
	ld1.s	{ v5 }[3], [x2]
	sub	x3, x11, #16
	add	x2, x11, #16
	ldp	s4, s6, [x11, #-52]
	ld1.s	{ v6 }[1], [x3]
	add	x3, x11, #48
	ld1.s	{ v6 }[2], [x2]
	ld1.s	{ v6 }[3], [x3]
	mul.4s	v6, v3, v6
	umull2.2d	v7, v6, v1
	umull.2d	v16, v6, v1
	uzp2.4s	v7, v16, v7
	cmhs.4s	v6, v6, v2
	and.16b	v6, v6, v1
	sub.4s	v7, v6, v7
	add.4s	v6, v7, v5
	sub.4s	v5, v5, v7
	sub	x2, x11, #28
	add	x3, x11, #36
	ldp	s7, s17, [x11, #-60]
	ld1.s	{ v7 }[1], [x2]
	ld1.s	{ v7 }[2], [x1]
	ld1.s	{ v7 }[3], [x3]
	sub	x1, x11, #12
	add	x2, x11, #20
	ldp	s16, s18, [x11, #-44]
	ld1.s	{ v16 }[1], [x1]
	add	x1, x11, #52
	ld1.s	{ v16 }[2], [x2]
	ld1.s	{ v16 }[3], [x1]
	mul.4s	v16, v3, v16
	umull2.2d	v19, v16, v1
	umull.2d	v20, v16, v1
	uzp2.4s	v19, v20, v19
	cmhs.4s	v16, v16, v2
	and.16b	v16, v16, v1
	sub.4s	v19, v16, v19
	add.4s	v16, v19, v7
	sub.4s	v7, v7, v19
	sub	x1, x11, #24
	add	x2, x11, #8
	ld1.s	{ v17 }[1], [x1]
	add	x1, x11, #40
	ld1.s	{ v17 }[2], [x2]
	ld1.s	{ v17 }[3], [x1]
	sub	x1, x11, #8
	add	x2, x11, #24
	add	x3, x11, #56
	ld1.s	{ v18 }[1], [x1]
	ld1.s	{ v18 }[2], [x2]
	ld1.s	{ v18 }[3], [x3]
	mul.4s	v18, v3, v18
	umull2.2d	v19, v18, v1
	umull.2d	v20, v18, v1
	uzp2.4s	v19, v20, v19
	cmhs.4s	v18, v18, v2
	and.16b	v18, v18, v1
	sub.4s	v18, v18, v19
	add.4s	v19, v18, v17
	sub.4s	v17, v17, v18
	sub	x1, x11, #4
	add	x2, x11, #28
	ldur	s18, [x11, #-36]
	ld1.s	{ v18 }[1], [x1]
	ld1.s	{ v18 }[2], [x2]
	add	x1, x11, #60
	ld1.s	{ v18 }[3], [x1]
	mul.4s	v3, v3, v18
	umull2.2d	v18, v3, v1
	umull.2d	v20, v3, v1
	uzp2.4s	v18, v20, v18
	cmhs.4s	v3, v3, v2
	and.16b	v3, v3, v1
	sub.4s	v3, v3, v18
	zip1.4s	v18, v6, v16
	trn2.4s	v20, v6, v16
	zip2.4s	v21, v6, v16
	uzp2.4s	v16, v6, v16
	uzp2.4s	v6, v16, v6
	add.4s	v5, v5, v1
	add.4s	v7, v7, v1
	sub	x1, x11, #20
	add	x2, x11, #12
	add	x3, x11, #44
	ld1.s	{ v4 }[1], [x1]
	ld1.s	{ v4 }[2], [x2]
	ld1.s	{ v4 }[3], [x3]
	mov.s	v18[2], v19[0]
	mov.s	v20[2], v19[1]
	add.4s	v16, v17, v1
	mov.s	v21[2], v19[2]
	mov.s	v6[2], v19[3]
	add.4s	v17, v3, v4
	mov.s	v18[3], v17[0]
	mov.s	v20[3], v17[1]
	mov.s	v21[3], v17[2]
	mov.s	v6[3], v17[3]
	sub.4s	v3, v4, v3
	zip1.4s	v4, v5, v7
	mov.s	v4[2], v16[0]
	add.4s	v3, v3, v1
	mov.s	v4[3], v3[0]
	stp	q18, q4, [x11, #-64]
	trn2.4s	v4, v5, v7
	mov.s	v4[2], v16[1]
	mov.s	v4[3], v3[1]
	stp	q20, q4, [x11, #-32]
	zip2.4s	v4, v5, v7
	mov.s	v4[2], v16[2]
	mov.s	v4[3], v3[2]
	uzp2.4s	v7, v5, v7
	uzp2.4s	v5, v7, v5
	mov.s	v5[2], v16[3]
	mov.s	v5[3], v3[3]
	stp	q21, q4, [x11]
	stp	q6, q5, [x11, #32]
	add	x17, x17, #16
	add	x11, x11, #128
	cmp	x17, #128
	b.ne	LBB1_23
LBB1_24:
	add	x11, x9, #512
	cmp	x0, x11
	b.hs	LBB1_28
; %bb.25:
	cmp	x13, x12
	b.hs	LBB1_28
; %bb.26:
	mov	x14, #0                         ; =0x0
	mov	w17, #7681                      ; =0x1e01
LBB1_27:                                ; =>This Inner Loop Header: Depth=1
	ldr	w1, [x13, x14]
	mul	w1, w1, w8
	ldp	w2, w3, [x16]
	mul	w2, w1, w2
	umull	x4, w2, w17
	lsr	x4, x4, #32
	cmp	w2, w10
	csel	w2, wzr, w17, lo
	ldp	w5, w6, [x16, #-8]
	sub	w2, w2, w4
	add	w4, w2, w5
	sub	w2, w5, w2
	add	w2, w2, w17
	mul	w1, w1, w3
	umull	x3, w1, w17
	lsr	x3, x3, #32
	cmp	w1, w10
	csel	w1, wzr, w17, lo
	sub	w1, w1, w3
	add	w3, w1, w6
	stp	w4, w3, [x16, #-8]
	sub	w1, w6, w1
	add	w1, w1, w17
	stp	w2, w1, [x16], #16
	add	x14, x14, #4
	cmp	x14, #256
	b.ne	LBB1_27
	b	LBB1_30
LBB1_28:
	mov	x14, #0                         ; =0x0
	mov	w16, #57857                     ; =0xe201
	movk	w16, #35715, lsl #16
	dup.4s	v0, w16
	mov	w16, #7681                      ; =0x1e01
	dup.4s	v1, w16
	mov	w16, #34880                     ; =0x8840
	movk	w16, #8, lsl #16
	dup.4s	v2, w16
	mov	x16, x0
LBB1_29:                                ; =>This Inner Loop Header: Depth=1
	ld4.4s	{ v3, v4, v5, v6 }, [x16]
	ldr	q7, [x13, x14]
	mul.4s	v7, v7, v0
	mul.4s	v16, v7, v5
	umull2.2d	v17, v16, v1
	umull.2d	v18, v16, v1
	uzp2.4s	v17, v18, v17
	cmhs.4s	v16, v16, v2
	and.16b	v16, v16, v1
	sub.4s	v16, v16, v17
	add.4s	v17, v16, v3
	sub.4s	v16, v3, v16
	add.4s	v19, v16, v1
	mul.4s	v7, v7, v6
	umull2.2d	v16, v7, v1
	umull.2d	v21, v7, v1
	uzp2.4s	v16, v21, v16
	cmhs.4s	v7, v7, v2
	and.16b	v7, v7, v1
	sub.4s	v7, v7, v16
	add.4s	v18, v7, v4
	sub.4s	v3, v4, v7
	add.4s	v20, v3, v1
	st4.4s	{ v17, v18, v19, v20 }, [x16], #64
	add	x14, x14, #16
	cmp	x14, #256
	b.ne	LBB1_29
LBB1_30:
	add	x9, x9, #1024
	cmp	x0, x9
	b.hs	LBB1_34
; %bb.31:
	cmp	x11, x12
	b.hs	LBB1_34
; %bb.32:
	mov	x9, #0                          ; =0x0
	mov	w12, #7681                      ; =0x1e01
LBB1_33:                                ; =>This Inner Loop Header: Depth=1
	ldr	w13, [x11, x9]
	mul	w13, w13, w8
	ldp	w14, w16, [x15, #-4]
	mul	w13, w13, w16
	umull	x16, w13, w12
	lsr	x16, x16, #32
	cmp	w13, w10
	csel	w13, wzr, w12, lo
	sub	w13, w13, w16
	add	w16, w13, w14
	sub	w13, w14, w13
	add	w13, w13, w12
	stp	w16, w13, [x15, #-4]
	add	x9, x9, #4
	add	x15, x15, #8
	cmp	x9, #512
	b.ne	LBB1_33
	b	LBB1_36
LBB1_34:
	mov	x8, #0                          ; =0x0
	mov	w9, #57857                      ; =0xe201
	movk	w9, #35715, lsl #16
	dup.4s	v0, w9
	mov	w9, #7681                       ; =0x1e01
	dup.4s	v1, w9
	mov	w9, #34880                      ; =0x8840
	movk	w9, #8, lsl #16
	dup.4s	v2, w9
	mov	x9, x0
LBB1_35:                                ; =>This Inner Loop Header: Depth=1
	ld2.4s	{ v3, v4 }, [x9]
	ldr	q5, [x11, x8]
	mul.4s	v5, v5, v4
	mul.4s	v5, v5, v0
	umull2.2d	v6, v5, v1
	umull.2d	v7, v5, v1
	uzp2.4s	v6, v7, v6
	cmhs.4s	v5, v5, v2
	and.16b	v5, v5, v1
	sub.4s	v5, v5, v6
	add.4s	v6, v5, v3
	sub.4s	v3, v3, v5
	add.4s	v7, v3, v1
	st2.4s	{ v6, v7 }, [x9], #32
	add	x8, x8, #16
	cmp	x8, #512
	b.ne	LBB1_35
LBB1_36:
	mov	x8, #0                          ; =0x0
	mov	w9, #34880                      ; =0x8840
	movk	w9, #8, lsl #16
	dup.4s	v0, w9
	mvni.4s	v1, #30, lsl #8
	mov	w9, #7681                       ; =0x1e01
	dup.4s	v2, w9
	ldp	x20, x19, [sp, #32]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]             ; 16-byte Folded Reload
	ldp	d9, d8, [sp], #48               ; 16-byte Folded Reload
LBB1_37:                                ; =>This Inner Loop Header: Depth=1
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
	b.ne	LBB1_37
; %bb.38:
	mov	w8, #0                          ; =0x0
LBB1_39:
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
	b.ne	LBB2_25
; %bb.1:
	cbz	x1, LBB2_25
; %bb.2:
	cbz	x0, LBB2_25
; %bb.3:
Lloh2:
	adrp	x9, _ntt_omega@PAGE
Lloh3:
	ldr	x9, [x9, _ntt_omega@PAGEOFF]
	cbz	x9, LBB2_25
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
	stp	x22, x21, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x20, x19, [sp, #16]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 32
	.cfi_offset w19, -8
	.cfi_offset w20, -16
	.cfi_offset w21, -24
	.cfi_offset w22, -32
	mov	w8, #57857                      ; =0xe201
	movk	w8, #35715, lsl #16
	add	x9, x9, #1024
	mov	w10, #256                       ; =0x100
	mov	w1, #1                          ; =0x1
	mov	w11, #7681                      ; =0x1e01
	dup.4s	v0, w11
	b	LBB2_10
LBB2_9:                                 ;   in Loop: Header=BB2_10 Depth=1
	lsl	w1, w13, #1
	cmp	w10, #3
	mov	x10, x12
	b.ls	LBB2_22
LBB2_10:                                ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB2_18 Depth 2
                                        ;       Child Loop BB2_19 Depth 3
                                        ;       Child Loop BB2_21 Depth 3
                                        ;     Child Loop BB2_13 Depth 2
	lsr	w12, w10, #1
	mov	w13, w1
	add	x14, x9, w12, uxtw #2
	lsl	x15, x13, #3
	add	x16, x0, w1, uxtw #2
	cmp	w1, #4
	b.hs	LBB2_16
; %bb.11:                               ;   in Loop: Header=BB2_10 Depth=1
	mov	x17, #0                         ; =0x0
	ubfiz	x1, x1, #1, #32
	mov	w2, #8                          ; =0x8
	mov	x3, x12
	b	LBB2_13
LBB2_12:                                ;   in Loop: Header=BB2_13 Depth=2
	add	x17, x17, x1
	add	x2, x2, x15
	subs	x3, x3, #1
	b.eq	LBB2_9
LBB2_13:                                ;   Parent Loop BB2_10 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	ldrsw	x5, [x14], #4
	mul	x4, x5, x8
	add	x6, x0, x2
	ldur	w7, [x6, #-8]
	add	x19, x16, x2
	ldur	w20, [x19, #-8]
	add	w21, w20, w7
	stur	w21, [x6, #-8]
	sub	w7, w7, w20
	smull	x20, w5, w7
	mul	w7, w4, w7
	lsr	x20, x20, #32
	umull	x7, w7, w11
	lsr	x7, x7, #32
	sub	w7, w20, w7
	and	w20, w11, w7, asr #31
	add	w7, w20, w7
	and	w20, w11, w7, asr #31
	add	w7, w20, w7
	stur	w7, [x19, #-8]
	cmp	x13, #1
	b.eq	LBB2_12
; %bb.14:                               ;   in Loop: Header=BB2_13 Depth=2
	ldur	w7, [x6, #-4]
	add	x19, x16, x17, lsl #2
	ldr	w20, [x19, #4]
	add	w21, w20, w7
	stur	w21, [x6, #-4]
	sub	w6, w7, w20
	smull	x7, w5, w6
	mul	w6, w4, w6
	lsr	x7, x7, #32
	umull	x6, w6, w11
	lsr	x6, x6, #32
	sub	w6, w7, w6
	and	w7, w11, w6, asr #31
	add	w6, w7, w6
	and	w7, w11, w6, asr #31
	add	w6, w7, w6
	str	w6, [x19, #4]
	add	x6, x17, #2
	add	x7, x13, x17
	cmp	x6, x7
	b.eq	LBB2_12
; %bb.15:                               ;   in Loop: Header=BB2_13 Depth=2
	ldr	w6, [x0, x2]
	ldr	w7, [x16, x2]
	add	w19, w7, w6
	str	w19, [x0, x2]
	sub	w6, w6, w7
	smull	x5, w5, w6
	mul	w4, w4, w6
	lsr	x5, x5, #32
	umull	x4, w4, w11
	lsr	x4, x4, #32
	sub	w4, w5, w4
	and	w5, w11, w4, asr #31
	add	w4, w5, w4
	and	w5, w11, w4, asr #31
	add	w4, w5, w4
	str	w4, [x16, x2]
	b	LBB2_12
LBB2_16:                                ;   in Loop: Header=BB2_10 Depth=1
	mov	x17, #0                         ; =0x0
	and	x1, x13, #0xfffffffc
	lsl	x2, x13, #2
	mov	x3, x0
	b	LBB2_18
LBB2_17:                                ;   in Loop: Header=BB2_18 Depth=2
	add	x17, x17, #1
	add	x3, x3, x15
	add	x16, x16, x15
	cmp	x17, x12
	b.eq	LBB2_9
LBB2_18:                                ;   Parent Loop BB2_10 Depth=1
                                        ; =>  This Loop Header: Depth=2
                                        ;       Child Loop BB2_19 Depth 3
                                        ;       Child Loop BB2_21 Depth 3
	ldrsw	x4, [x14, x17, lsl #2]
	mul	x5, x4, x8
	dup.2d	v1, x5
	dup.4s	v2, w4
	mov	x6, x3
	mov	x7, x1
LBB2_19:                                ;   Parent Loop BB2_10 Depth=1
                                        ;     Parent Loop BB2_18 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	ldr	q3, [x6]
	ldr	q4, [x6, x2]
	add.4s	v5, v4, v3
	str	q5, [x6]
	sub.4s	v3, v3, v4
	sshll.2d	v4, v3, #0
	sshll2.2d	v5, v3, #0
	smull2.2d	v6, v2, v3
	mov.s	w19, v5[2]
	mov.s	w20, v1[2]
	mul	w19, w20, w19
	fmov	w21, s5
	fmov	w22, s1
	mul	w21, w22, w21
	fmov	d5, x21
	mov.d	v5[1], x19
	mov.s	w19, v4[2]
	mul	w19, w20, w19
	fmov	w20, s4
	fmov	w21, s1
	mul	w20, w21, w20
	fmov	d4, x20
	mov.d	v4[1], x19
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
	str	q3, [x6, x2]
	add	x6, x6, #16
	subs	x7, x7, #4
	b.ne	LBB2_19
; %bb.20:                               ;   in Loop: Header=BB2_18 Depth=2
	mov	x6, x1
	cmp	x1, x13
	b.eq	LBB2_17
LBB2_21:                                ;   Parent Loop BB2_10 Depth=1
                                        ;     Parent Loop BB2_18 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	ldr	w7, [x3, x6, lsl #2]
	ldr	w19, [x16, x6, lsl #2]
	add	w20, w19, w7
	str	w20, [x3, x6, lsl #2]
	sub	w7, w7, w19
	smull	x19, w4, w7
	mul	w7, w5, w7
	lsr	x19, x19, #32
	umull	x7, w7, w11
	lsr	x7, x7, #32
	sub	w7, w19, w7
	and	w19, w11, w7, asr #31
	add	w7, w19, w7
	and	w19, w11, w7, asr #31
	add	w7, w19, w7
	str	w7, [x16, x6, lsl #2]
	add	x6, x6, #1
	cmp	x13, x6
	b.ne	LBB2_21
	b	LBB2_17
LBB2_22:
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
LBB2_23:                                ; =>This Inner Loop Header: Depth=1
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
	b.ne	LBB2_23
; %bb.24:
	mov	w8, #0                          ; =0x0
	ldp	x20, x19, [sp, #16]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp], #32             ; 16-byte Folded Reload
LBB2_25:
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
