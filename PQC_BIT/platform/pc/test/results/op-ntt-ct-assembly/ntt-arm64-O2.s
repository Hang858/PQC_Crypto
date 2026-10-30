	.build_version macos, 26, 0	sdk_version 27, 2
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_bit_ntt_init                   ; -- Begin function bit_ntt_init
	.p2align	2
_bit_ntt_init:                          ; @bit_ntt_init
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
Lloh0:
	adrp	x0, _bit_ntt_roots32@PAGE
Lloh1:
	add	x0, x0, _bit_ntt_roots32@PAGEOFF
	mov	w1, #2                          ; =0x2
	mov	w2, #7681                       ; =0x1e01
	mov	w3, #57857                      ; =0xe201
	movk	w3, #35715, lsl #16
	mov	w4, #1912                       ; =0x778
	mov	w5, #0                          ; =0x0
	bl	_OP_ntt256_init
	cmp	w0, #0
	csetm	w0, ne
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh0, Lloh1
	.cfi_endproc
                                        ; -- End function
	.globl	_ntt_forward_ws                 ; -- Begin function ntt_forward_ws
	.p2align	2
_ntt_forward_ws:                        ; @ntt_forward_ws
	.cfi_startproc
; %bb.0:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x20, x2
	mov	x19, x0
	ldp	q0, q1, [x1]
	ldp	q2, q3, [x1, #32]
	sshll.4s	v4, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v5, v1, #0
	sshll2.4s	v1, v1, #0
	sshll.4s	v6, v2, #0
	sshll2.4s	v2, v2, #0
	sshll.4s	v7, v3, #0
	sshll2.4s	v3, v3, #0
	stp	q4, q0, [x2]
	stp	q5, q1, [x2, #32]
	stp	q6, q2, [x2, #64]
	stp	q7, q3, [x2, #96]
	ldp	q0, q1, [x1, #64]
	ldp	q2, q3, [x1, #96]
	sshll.4s	v4, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v5, v1, #0
	sshll2.4s	v1, v1, #0
	sshll.4s	v6, v2, #0
	sshll2.4s	v2, v2, #0
	sshll.4s	v7, v3, #0
	sshll2.4s	v3, v3, #0
	stp	q4, q0, [x2, #128]
	stp	q5, q1, [x2, #160]
	stp	q6, q2, [x2, #192]
	stp	q7, q3, [x2, #224]
	ldp	q0, q1, [x1, #128]
	ldp	q2, q3, [x1, #160]
	sshll.4s	v4, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v5, v1, #0
	sshll2.4s	v1, v1, #0
	sshll.4s	v6, v2, #0
	sshll2.4s	v2, v2, #0
	sshll.4s	v7, v3, #0
	sshll2.4s	v3, v3, #0
	stp	q4, q0, [x2, #256]
	stp	q5, q1, [x2, #288]
	stp	q6, q2, [x2, #320]
	stp	q7, q3, [x2, #352]
	ldp	q0, q1, [x1, #192]
	ldp	q2, q3, [x1, #224]
	sshll.4s	v4, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v5, v1, #0
	sshll2.4s	v1, v1, #0
	sshll.4s	v6, v2, #0
	sshll2.4s	v2, v2, #0
	sshll.4s	v7, v3, #0
	sshll2.4s	v3, v3, #0
	stp	q4, q0, [x2, #384]
	stp	q5, q1, [x2, #416]
	stp	q6, q2, [x2, #448]
	stp	q7, q3, [x2, #480]
	ldp	q0, q1, [x1, #256]
	ldp	q2, q3, [x1, #288]
	sshll.4s	v4, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v5, v1, #0
	sshll2.4s	v1, v1, #0
	sshll.4s	v6, v2, #0
	sshll2.4s	v2, v2, #0
	sshll.4s	v7, v3, #0
	sshll2.4s	v3, v3, #0
	stp	q4, q0, [x2, #512]
	stp	q5, q1, [x2, #544]
	stp	q6, q2, [x2, #576]
	stp	q7, q3, [x2, #608]
	ldp	q0, q1, [x1, #320]
	ldp	q2, q3, [x1, #352]
	sshll.4s	v4, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v5, v1, #0
	sshll2.4s	v1, v1, #0
	sshll.4s	v6, v2, #0
	sshll2.4s	v2, v2, #0
	sshll.4s	v7, v3, #0
	sshll2.4s	v3, v3, #0
	stp	q4, q0, [x2, #640]
	stp	q5, q1, [x2, #672]
	stp	q6, q2, [x2, #704]
	stp	q7, q3, [x2, #736]
	ldp	q0, q1, [x1, #384]
	ldp	q2, q3, [x1, #416]
	sshll.4s	v4, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v5, v1, #0
	sshll2.4s	v1, v1, #0
	sshll.4s	v6, v2, #0
	sshll2.4s	v2, v2, #0
	sshll.4s	v7, v3, #0
	sshll2.4s	v3, v3, #0
	stp	q4, q0, [x2, #768]
	stp	q5, q1, [x2, #800]
	stp	q6, q2, [x2, #832]
	stp	q7, q3, [x2, #864]
	ldp	q0, q1, [x1, #448]
	ldp	q2, q3, [x1, #480]
	sshll.4s	v4, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v5, v1, #0
	sshll2.4s	v1, v1, #0
	sshll.4s	v6, v2, #0
	sshll2.4s	v2, v2, #0
	sshll.4s	v7, v3, #0
	sshll2.4s	v3, v3, #0
	stp	q4, q0, [x2, #896]
	stp	q5, q1, [x2, #928]
	stp	q6, q2, [x2, #960]
	stp	q7, q3, [x2, #992]
	add	x0, x2, #2048
	mov	x1, x2
	mov	w2, #0                          ; =0x0
	bl	_OP_ntt256
	cbz	w0, LBB1_2
; %bb.1:
	mov	w0, #-1                         ; =0xffffffff
	b	LBB1_3
LBB1_2:
	ldr	q0, [x20, #2064]
	ldr	q1, [x20, #2048]
	ldr	q2, [x20, #2096]
	ldr	q3, [x20, #2080]
	ldr	q4, [x20, #2128]
	ldr	q5, [x20, #2112]
	ldr	q6, [x20, #2160]
	ldr	q7, [x20, #2144]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x19]
	stp	q2, q3, [x19, #32]
	ldr	q0, [x20, #2192]
	ldr	q1, [x20, #2176]
	ldr	q2, [x20, #2224]
	ldr	q3, [x20, #2208]
	ldr	q4, [x20, #2256]
	ldr	q5, [x20, #2240]
	ldr	q6, [x20, #2288]
	ldr	q7, [x20, #2272]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x19, #64]
	stp	q2, q3, [x19, #96]
	ldr	q0, [x20, #2320]
	ldr	q1, [x20, #2304]
	ldr	q2, [x20, #2352]
	ldr	q3, [x20, #2336]
	ldr	q4, [x20, #2384]
	ldr	q5, [x20, #2368]
	ldr	q6, [x20, #2416]
	ldr	q7, [x20, #2400]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x19, #128]
	stp	q2, q3, [x19, #160]
	ldr	q0, [x20, #2448]
	ldr	q1, [x20, #2432]
	ldr	q2, [x20, #2480]
	ldr	q3, [x20, #2464]
	ldr	q4, [x20, #2512]
	ldr	q5, [x20, #2496]
	ldr	q6, [x20, #2544]
	ldr	q7, [x20, #2528]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x19, #192]
	stp	q2, q3, [x19, #224]
	ldr	q0, [x20, #2576]
	ldr	q1, [x20, #2560]
	ldr	q2, [x20, #2608]
	ldr	q3, [x20, #2592]
	ldr	q4, [x20, #2640]
	ldr	q5, [x20, #2624]
	ldr	q6, [x20, #2672]
	ldr	q7, [x20, #2656]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x19, #256]
	stp	q2, q3, [x19, #288]
	ldr	q0, [x20, #2704]
	ldr	q1, [x20, #2688]
	ldr	q2, [x20, #2736]
	ldr	q3, [x20, #2720]
	ldr	q4, [x20, #2768]
	ldr	q5, [x20, #2752]
	ldr	q6, [x20, #2800]
	ldr	q7, [x20, #2784]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x19, #320]
	stp	q2, q3, [x19, #352]
	ldr	q0, [x20, #2832]
	ldr	q1, [x20, #2816]
	ldr	q2, [x20, #2864]
	ldr	q3, [x20, #2848]
	ldr	q4, [x20, #2896]
	ldr	q5, [x20, #2880]
	ldr	q6, [x20, #2928]
	ldr	q7, [x20, #2912]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x19, #384]
	stp	q2, q3, [x19, #416]
	ldr	q0, [x20, #2960]
	ldr	q1, [x20, #2944]
	ldr	q2, [x20, #2992]
	ldr	q3, [x20, #2976]
	ldr	q4, [x20, #3024]
	ldr	q5, [x20, #3008]
	ldr	q6, [x20, #3056]
	ldr	q7, [x20, #3040]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x19, #448]
	stp	q2, q3, [x19, #480]
LBB1_3:
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_ntt_inverse_ws                 ; -- Begin function ntt_inverse_ws
	.p2align	2
_ntt_inverse_ws:                        ; @ntt_inverse_ws
	.cfi_startproc
; %bb.0:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x20, x2
	mov	x19, x0
	ldp	q0, q1, [x1]
	ldp	q2, q3, [x1, #32]
	sshll.4s	v4, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v5, v1, #0
	sshll2.4s	v1, v1, #0
	sshll.4s	v6, v2, #0
	sshll2.4s	v2, v2, #0
	sshll.4s	v7, v3, #0
	sshll2.4s	v3, v3, #0
	stp	q4, q0, [x2]
	stp	q5, q1, [x2, #32]
	stp	q6, q2, [x2, #64]
	stp	q7, q3, [x2, #96]
	ldp	q0, q1, [x1, #64]
	ldp	q2, q3, [x1, #96]
	sshll.4s	v4, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v5, v1, #0
	sshll2.4s	v1, v1, #0
	sshll.4s	v6, v2, #0
	sshll2.4s	v2, v2, #0
	sshll.4s	v7, v3, #0
	sshll2.4s	v3, v3, #0
	stp	q4, q0, [x2, #128]
	stp	q5, q1, [x2, #160]
	stp	q6, q2, [x2, #192]
	stp	q7, q3, [x2, #224]
	ldp	q0, q1, [x1, #128]
	ldp	q2, q3, [x1, #160]
	sshll.4s	v4, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v5, v1, #0
	sshll2.4s	v1, v1, #0
	sshll.4s	v6, v2, #0
	sshll2.4s	v2, v2, #0
	sshll.4s	v7, v3, #0
	sshll2.4s	v3, v3, #0
	stp	q4, q0, [x2, #256]
	stp	q5, q1, [x2, #288]
	stp	q6, q2, [x2, #320]
	stp	q7, q3, [x2, #352]
	ldp	q0, q1, [x1, #192]
	ldp	q2, q3, [x1, #224]
	sshll.4s	v4, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v5, v1, #0
	sshll2.4s	v1, v1, #0
	sshll.4s	v6, v2, #0
	sshll2.4s	v2, v2, #0
	sshll.4s	v7, v3, #0
	sshll2.4s	v3, v3, #0
	stp	q4, q0, [x2, #384]
	stp	q5, q1, [x2, #416]
	stp	q6, q2, [x2, #448]
	stp	q7, q3, [x2, #480]
	ldp	q0, q1, [x1, #256]
	ldp	q2, q3, [x1, #288]
	sshll.4s	v4, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v5, v1, #0
	sshll2.4s	v1, v1, #0
	sshll.4s	v6, v2, #0
	sshll2.4s	v2, v2, #0
	sshll.4s	v7, v3, #0
	sshll2.4s	v3, v3, #0
	stp	q4, q0, [x2, #512]
	stp	q5, q1, [x2, #544]
	stp	q6, q2, [x2, #576]
	stp	q7, q3, [x2, #608]
	ldp	q0, q1, [x1, #320]
	ldp	q2, q3, [x1, #352]
	sshll.4s	v4, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v5, v1, #0
	sshll2.4s	v1, v1, #0
	sshll.4s	v6, v2, #0
	sshll2.4s	v2, v2, #0
	sshll.4s	v7, v3, #0
	sshll2.4s	v3, v3, #0
	stp	q4, q0, [x2, #640]
	stp	q5, q1, [x2, #672]
	stp	q6, q2, [x2, #704]
	stp	q7, q3, [x2, #736]
	ldp	q0, q1, [x1, #384]
	ldp	q2, q3, [x1, #416]
	sshll.4s	v4, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v5, v1, #0
	sshll2.4s	v1, v1, #0
	sshll.4s	v6, v2, #0
	sshll2.4s	v2, v2, #0
	sshll.4s	v7, v3, #0
	sshll2.4s	v3, v3, #0
	stp	q4, q0, [x2, #768]
	stp	q5, q1, [x2, #800]
	stp	q6, q2, [x2, #832]
	stp	q7, q3, [x2, #864]
	ldp	q0, q1, [x1, #448]
	ldp	q2, q3, [x1, #480]
	sshll.4s	v4, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v5, v1, #0
	sshll2.4s	v1, v1, #0
	sshll.4s	v6, v2, #0
	sshll2.4s	v2, v2, #0
	sshll.4s	v7, v3, #0
	sshll2.4s	v3, v3, #0
	stp	q4, q0, [x2, #896]
	stp	q5, q1, [x2, #928]
	stp	q6, q2, [x2, #960]
	stp	q7, q3, [x2, #992]
	add	x0, x2, #2048
	mov	x1, x2
	mov	w2, #1                          ; =0x1
	bl	_OP_intt256
	cbz	w0, LBB2_2
; %bb.1:
	mov	w0, #-1                         ; =0xffffffff
	b	LBB2_3
LBB2_2:
	ldr	q0, [x20, #2064]
	ldr	q1, [x20, #2048]
	ldr	q2, [x20, #2096]
	ldr	q3, [x20, #2080]
	ldr	q4, [x20, #2128]
	ldr	q5, [x20, #2112]
	ldr	q6, [x20, #2160]
	ldr	q7, [x20, #2144]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x19]
	stp	q2, q3, [x19, #32]
	ldr	q0, [x20, #2192]
	ldr	q1, [x20, #2176]
	ldr	q2, [x20, #2224]
	ldr	q3, [x20, #2208]
	ldr	q4, [x20, #2256]
	ldr	q5, [x20, #2240]
	ldr	q6, [x20, #2288]
	ldr	q7, [x20, #2272]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x19, #64]
	stp	q2, q3, [x19, #96]
	ldr	q0, [x20, #2320]
	ldr	q1, [x20, #2304]
	ldr	q2, [x20, #2352]
	ldr	q3, [x20, #2336]
	ldr	q4, [x20, #2384]
	ldr	q5, [x20, #2368]
	ldr	q6, [x20, #2416]
	ldr	q7, [x20, #2400]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x19, #128]
	stp	q2, q3, [x19, #160]
	ldr	q0, [x20, #2448]
	ldr	q1, [x20, #2432]
	ldr	q2, [x20, #2480]
	ldr	q3, [x20, #2464]
	ldr	q4, [x20, #2512]
	ldr	q5, [x20, #2496]
	ldr	q6, [x20, #2544]
	ldr	q7, [x20, #2528]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x19, #192]
	stp	q2, q3, [x19, #224]
	ldr	q0, [x20, #2576]
	ldr	q1, [x20, #2560]
	ldr	q2, [x20, #2608]
	ldr	q3, [x20, #2592]
	ldr	q4, [x20, #2640]
	ldr	q5, [x20, #2624]
	ldr	q6, [x20, #2672]
	ldr	q7, [x20, #2656]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x19, #256]
	stp	q2, q3, [x19, #288]
	ldr	q0, [x20, #2704]
	ldr	q1, [x20, #2688]
	ldr	q2, [x20, #2736]
	ldr	q3, [x20, #2720]
	ldr	q4, [x20, #2768]
	ldr	q5, [x20, #2752]
	ldr	q6, [x20, #2800]
	ldr	q7, [x20, #2784]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x19, #320]
	stp	q2, q3, [x19, #352]
	ldr	q0, [x20, #2832]
	ldr	q1, [x20, #2816]
	ldr	q2, [x20, #2864]
	ldr	q3, [x20, #2848]
	ldr	q4, [x20, #2896]
	ldr	q5, [x20, #2880]
	ldr	q6, [x20, #2928]
	ldr	q7, [x20, #2912]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x19, #384]
	stp	q2, q3, [x19, #416]
	ldr	q0, [x20, #2960]
	ldr	q1, [x20, #2944]
	ldr	q2, [x20, #2992]
	ldr	q3, [x20, #2976]
	ldr	q4, [x20, #3024]
	ldr	q5, [x20, #3008]
	ldr	q6, [x20, #3056]
	ldr	q7, [x20, #3040]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x19, #448]
	stp	q2, q3, [x19, #480]
LBB2_3:
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_ntt_basemul_raw_ws             ; -- Begin function ntt_basemul_raw_ws
	.p2align	2
_ntt_basemul_raw_ws:                    ; @ntt_basemul_raw_ws
	.cfi_startproc
; %bb.0:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x20, x3
	mov	x19, x0
	add	x8, x1, #16
	add	x9, x2, #16
	mov	w10, #32                        ; =0x20
LBB3_1:                                 ; =>This Inner Loop Header: Depth=1
	ldp	q0, q1, [x8, #-16]
	sshll.4s	v2, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v3, v1, #0
	sshll2.4s	v1, v1, #0
	add	x11, x20, x10
	stp	q2, q0, [x11, #-32]
	stp	q3, q1, [x11]
	ldp	q0, q1, [x9, #-16]
	sshll.4s	v2, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v3, v1, #0
	sshll2.4s	v1, v1, #0
	stp	q2, q0, [x11, #992]
	str	q1, [x11, #1040]
	str	q3, [x11, #1024]
	add	x10, x10, #64
	add	x8, x8, #32
	add	x9, x9, #32
	cmp	x10, #1056
	b.ne	LBB3_1
; %bb.2:
	add	x0, x20, #2048
	add	x2, x20, #1024
	mov	x1, x20
	mov	w3, #256                        ; =0x100
	mov	w4, #7681                       ; =0x1e01
	mov	w5, #0                          ; =0x0
	bl	_OP_cwm
	cbz	w0, LBB3_4
; %bb.3:
	mov	w0, #-1                         ; =0xffffffff
	b	LBB3_5
LBB3_4:
	ldr	q0, [x20, #2064]
	ldr	q1, [x20, #2048]
	ldr	q2, [x20, #2096]
	ldr	q3, [x20, #2080]
	ldr	q4, [x20, #2128]
	ldr	q5, [x20, #2112]
	ldr	q6, [x20, #2160]
	ldr	q7, [x20, #2144]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x19]
	stp	q2, q3, [x19, #32]
	ldr	q0, [x20, #2192]
	ldr	q1, [x20, #2176]
	ldr	q2, [x20, #2224]
	ldr	q3, [x20, #2208]
	ldr	q4, [x20, #2256]
	ldr	q5, [x20, #2240]
	ldr	q6, [x20, #2288]
	ldr	q7, [x20, #2272]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x19, #64]
	stp	q2, q3, [x19, #96]
	ldr	q0, [x20, #2320]
	ldr	q1, [x20, #2304]
	ldr	q2, [x20, #2352]
	ldr	q3, [x20, #2336]
	ldr	q4, [x20, #2384]
	ldr	q5, [x20, #2368]
	ldr	q6, [x20, #2416]
	ldr	q7, [x20, #2400]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x19, #128]
	stp	q2, q3, [x19, #160]
	ldr	q0, [x20, #2448]
	ldr	q1, [x20, #2432]
	ldr	q2, [x20, #2480]
	ldr	q3, [x20, #2464]
	ldr	q4, [x20, #2512]
	ldr	q5, [x20, #2496]
	ldr	q6, [x20, #2544]
	ldr	q7, [x20, #2528]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x19, #192]
	stp	q2, q3, [x19, #224]
	ldr	q0, [x20, #2576]
	ldr	q1, [x20, #2560]
	ldr	q2, [x20, #2608]
	ldr	q3, [x20, #2592]
	ldr	q4, [x20, #2640]
	ldr	q5, [x20, #2624]
	ldr	q6, [x20, #2672]
	ldr	q7, [x20, #2656]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x19, #256]
	stp	q2, q3, [x19, #288]
	ldr	q0, [x20, #2704]
	ldr	q1, [x20, #2688]
	ldr	q2, [x20, #2736]
	ldr	q3, [x20, #2720]
	ldr	q4, [x20, #2768]
	ldr	q5, [x20, #2752]
	ldr	q6, [x20, #2800]
	ldr	q7, [x20, #2784]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x19, #320]
	stp	q2, q3, [x19, #352]
	ldr	q0, [x20, #2832]
	ldr	q1, [x20, #2816]
	ldr	q2, [x20, #2864]
	ldr	q3, [x20, #2848]
	ldr	q4, [x20, #2896]
	ldr	q5, [x20, #2880]
	ldr	q6, [x20, #2928]
	ldr	q7, [x20, #2912]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x19, #384]
	stp	q2, q3, [x19, #416]
	ldr	q0, [x20, #2960]
	ldr	q1, [x20, #2944]
	ldr	q2, [x20, #2992]
	ldr	q3, [x20, #2976]
	ldr	q4, [x20, #3024]
	ldr	q5, [x20, #3008]
	ldr	q6, [x20, #3056]
	ldr	q7, [x20, #3040]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x19, #448]
	stp	q2, q3, [x19, #480]
LBB3_5:
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_ntt_montgomery_lift_ws         ; -- Begin function ntt_montgomery_lift_ws
	.p2align	2
_ntt_montgomery_lift_ws:                ; @ntt_montgomery_lift_ws
	.cfi_startproc
; %bb.0:
	mov	x8, #0                          ; =0x0
	mov	w9, #5569                       ; =0x15c1
	dup.8h	v0, w9
	mov	w9, #34880                      ; =0x8840
	movk	w9, #8, lsl #16
	dup.4s	v1, w9
	mvni.4s	v2, #30, lsl #8
	mov	w9, #7681                       ; =0x1e01
	dup.4s	v3, w9
	movi.4s	v4, #225, msl #8
LBB4_1:                                 ; =>This Inner Loop Header: Depth=1
	ldr	q5, [x0, x8]
	smull.4s	v6, v5, v0
	smull2.4s	v7, v5, v0
	smull2.2d	v16, v7, v1
	smull.2d	v7, v7, v1
	uzp2.4s	v7, v7, v16
	smull2.2d	v16, v6, v1
	smull.2d	v6, v6, v1
	uzp2.4s	v6, v6, v16
	mul.4s	v6, v6, v2
	mul.4s	v7, v7, v2
	smlal2.4s	v7, v5, v0
	smlal.4s	v6, v5, v0
	cmlt.4s	v5, v6, #0
	and.16b	v5, v5, v3
	cmlt.4s	v16, v7, #0
	and.16b	v16, v16, v3
	add.4s	v7, v16, v7
	add.4s	v5, v5, v6
	add.4s	v6, v5, v4
	add.4s	v16, v7, v4
	cmgt.4s	v17, v3, v5
	cmgt.4s	v18, v3, v7
	bif.16b	v7, v16, v18
	bif.16b	v5, v6, v17
	uzp1.8h	v5, v5, v7
	str	q5, [x0, x8]
	add	x8, x8, #16
	cmp	x8, #512
	b.ne	LBB4_1
; %bb.2:
	mov	w0, #0                          ; =0x0
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_ntt_forward                    ; -- Begin function ntt_forward
	.p2align	2
_ntt_forward:                           ; @ntt_forward
	.cfi_startproc
; %bb.0:
	mov	x1, x0
	mov	w0, #0                          ; =0x0
	mov	x2, x1
	mov	x3, #0                          ; =0x0
	b	_standalone
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function standalone
_standalone:                            ; @standalone
	.cfi_startproc
; %bb.0:
	stp	d9, d8, [sp, #-80]!             ; 16-byte Folded Spill
	stp	x24, x23, [sp, #16]             ; 16-byte Folded Spill
	stp	x22, x21, [sp, #32]             ; 16-byte Folded Spill
	stp	x20, x19, [sp, #48]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #64]             ; 16-byte Folded Spill
	add	x29, sp, #64
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -56
	.cfi_offset w24, -64
	.cfi_offset b8, -72
	.cfi_offset b9, -80
	mov	x23, x3
	mov	x22, x2
	mov	x20, x1
	mov	x21, x0
	mov	w0, #3072                       ; =0xc00
	bl	_malloc
	cbz	x0, LBB6_4
; %bb.1:
	mov	x19, x0
Lloh2:
	adrp	x0, _bit_ntt_roots32@PAGE
Lloh3:
	add	x0, x0, _bit_ntt_roots32@PAGEOFF
	mov	w1, #2                          ; =0x2
	mov	w2, #7681                       ; =0x1e01
	mov	w3, #57857                      ; =0xe201
	movk	w3, #35715, lsl #16
	mov	w4, #1912                       ; =0x778
	mov	w5, #0                          ; =0x0
	bl	_OP_ntt256_init
	cbz	w0, LBB6_6
LBB6_2:
	mov	w21, #-1                        ; =0xffffffff
LBB6_3:
	mov	x0, x19
	mov	w1, #3072                       ; =0xc00
	bl	_bit_secure_zero
	mov	x0, x19
	bl	_free
	b	LBB6_5
LBB6_4:
	mov	w21, #-1                        ; =0xffffffff
LBB6_5:
	mov	x0, x21
	ldp	x29, x30, [sp, #64]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #48]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #32]             ; 16-byte Folded Reload
	ldp	x24, x23, [sp, #16]             ; 16-byte Folded Reload
	ldp	d9, d8, [sp], #80               ; 16-byte Folded Reload
	ret
LBB6_6:
	cmp	w21, #1
	b.hi	LBB6_9
; %bb.7:
	ldp	q0, q1, [x22]
	ldp	q2, q3, [x22, #32]
	sshll.4s	v4, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v5, v1, #0
	sshll2.4s	v1, v1, #0
	sshll.4s	v6, v2, #0
	sshll2.4s	v2, v2, #0
	sshll.4s	v7, v3, #0
	sshll2.4s	v3, v3, #0
	stp	q4, q0, [x19]
	stp	q5, q1, [x19, #32]
	stp	q6, q2, [x19, #64]
	stp	q7, q3, [x19, #96]
	ldp	q0, q1, [x22, #64]
	ldp	q2, q3, [x22, #96]
	sshll.4s	v4, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v5, v1, #0
	sshll2.4s	v1, v1, #0
	sshll.4s	v6, v2, #0
	sshll2.4s	v2, v2, #0
	sshll.4s	v7, v3, #0
	sshll2.4s	v3, v3, #0
	stp	q4, q0, [x19, #128]
	stp	q5, q1, [x19, #160]
	stp	q6, q2, [x19, #192]
	stp	q7, q3, [x19, #224]
	ldp	q0, q1, [x22, #128]
	ldp	q2, q3, [x22, #160]
	sshll.4s	v4, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v5, v1, #0
	sshll2.4s	v1, v1, #0
	sshll.4s	v6, v2, #0
	sshll2.4s	v2, v2, #0
	sshll.4s	v7, v3, #0
	sshll2.4s	v3, v3, #0
	stp	q4, q0, [x19, #256]
	stp	q5, q1, [x19, #288]
	stp	q6, q2, [x19, #320]
	stp	q7, q3, [x19, #352]
	ldp	q0, q1, [x22, #192]
	ldp	q2, q3, [x22, #224]
	sshll.4s	v4, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v5, v1, #0
	sshll2.4s	v1, v1, #0
	sshll.4s	v6, v2, #0
	sshll2.4s	v2, v2, #0
	sshll.4s	v7, v3, #0
	sshll2.4s	v3, v3, #0
	stp	q4, q0, [x19, #384]
	stp	q5, q1, [x19, #416]
	stp	q6, q2, [x19, #448]
	stp	q7, q3, [x19, #480]
	ldp	q0, q1, [x22, #256]
	ldp	q2, q3, [x22, #288]
	sshll.4s	v4, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v5, v1, #0
	sshll2.4s	v1, v1, #0
	sshll.4s	v6, v2, #0
	sshll2.4s	v2, v2, #0
	sshll.4s	v7, v3, #0
	sshll2.4s	v3, v3, #0
	stp	q4, q0, [x19, #512]
	stp	q5, q1, [x19, #544]
	stp	q6, q2, [x19, #576]
	stp	q7, q3, [x19, #608]
	ldp	q0, q1, [x22, #320]
	ldp	q2, q3, [x22, #352]
	sshll.4s	v4, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v5, v1, #0
	sshll2.4s	v1, v1, #0
	sshll.4s	v6, v2, #0
	sshll2.4s	v2, v2, #0
	sshll.4s	v7, v3, #0
	sshll2.4s	v3, v3, #0
	stp	q4, q0, [x19, #640]
	stp	q5, q1, [x19, #672]
	stp	q6, q2, [x19, #704]
	stp	q7, q3, [x19, #736]
	ldp	q0, q1, [x22, #384]
	ldp	q2, q3, [x22, #416]
	sshll.4s	v4, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v5, v1, #0
	sshll2.4s	v1, v1, #0
	sshll.4s	v6, v2, #0
	sshll2.4s	v2, v2, #0
	sshll.4s	v7, v3, #0
	sshll2.4s	v3, v3, #0
	stp	q4, q0, [x19, #768]
	stp	q5, q1, [x19, #800]
	stp	q6, q2, [x19, #832]
	stp	q7, q3, [x19, #864]
	ldp	q0, q1, [x22, #448]
	ldp	q2, q3, [x22, #480]
	sshll.4s	v4, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v5, v1, #0
	sshll2.4s	v1, v1, #0
	sshll.4s	v6, v2, #0
	sshll2.4s	v2, v2, #0
	sshll.4s	v7, v3, #0
	sshll2.4s	v3, v3, #0
	stp	q4, q0, [x19, #896]
	stp	q5, q1, [x19, #928]
	stp	q6, q2, [x19, #960]
	stp	q7, q3, [x19, #992]
	add	x0, x19, #2048
	mov	x1, x19
	cbz	w21, LBB6_16
; %bb.8:
	mov	w2, #1                          ; =0x1
	bl	_OP_intt256
	cbnz	w0, LBB6_2
	b	LBB6_20
LBB6_9:
	cmp	w21, #2
	b.eq	LBB6_17
; %bb.10:
	cmp	w21, #3
	b.ne	LBB6_21
; %bb.11:
	add	x8, x22, #16
	add	x9, x23, #16
	mov	w10, #32                        ; =0x20
LBB6_12:                                ; =>This Inner Loop Header: Depth=1
	ldp	q0, q1, [x8, #-16]
	sshll.4s	v2, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v3, v1, #0
	sshll2.4s	v1, v1, #0
	add	x11, x19, x10
	stp	q2, q0, [x11, #-32]
	stp	q3, q1, [x11]
	ldp	q0, q1, [x9, #-16]
	sshll.4s	v2, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v3, v1, #0
	sshll2.4s	v1, v1, #0
	stp	q2, q0, [x11, #992]
	str	q1, [x11, #1040]
	str	q3, [x11, #1024]
	add	x10, x10, #64
	add	x8, x8, #32
	add	x9, x9, #32
	cmp	x10, #1056
	b.ne	LBB6_12
; %bb.13:
	mov	w21, #7681                      ; =0x1e01
	add	x0, x19, #2048
	add	x2, x19, #1024
	mov	x1, x19
	mov	w3, #256                        ; =0x100
	mov	w5, #0                          ; =0x0
	mov	w4, #7681                       ; =0x1e01
	bl	_OP_cwm
	cbnz	w0, LBB6_2
; %bb.14:
	add	x8, x20, #32
	add	x9, x19, #2144
	mov	w10, #256                       ; =0x100
	movi.4s	v0, #225, msl #8
	dup.4s	v1, w21
	mov	w21, #0                         ; =0x0
LBB6_15:                                ; =>This Inner Loop Header: Depth=1
	ldp	q2, q3, [x8, #-32]
	ldp	q4, q5, [x8]
	ldp	q6, q7, [x9, #-96]
	ldp	q16, q17, [x9, #-64]
	ldp	q18, q19, [x9, #-32]
	saddw2.4s	v7, v7, v2
	saddw.4s	v2, v6, v2
	saddw2.4s	v6, v17, v3
	saddw.4s	v3, v16, v3
	saddw2.4s	v16, v19, v4
	saddw.4s	v4, v18, v4
	ldp	q17, q18, [x9], #128
	saddw2.4s	v18, v18, v5
	saddw.4s	v5, v17, v5
	add.4s	v17, v2, v0
	add.4s	v19, v7, v0
	add.4s	v20, v3, v0
	add.4s	v21, v6, v0
	add.4s	v22, v4, v0
	add.4s	v23, v16, v0
	add.4s	v24, v5, v0
	add.4s	v25, v18, v0
	cmgt.4s	v26, v1, v2
	cmgt.4s	v27, v1, v7
	cmgt.4s	v28, v1, v3
	cmgt.4s	v29, v1, v6
	cmgt.4s	v30, v1, v4
	cmgt.4s	v31, v1, v16
	cmgt.4s	v8, v1, v5
	cmgt.4s	v9, v1, v18
	bif.16b	v7, v19, v27
	bif.16b	v2, v17, v26
	bif.16b	v6, v21, v29
	bif.16b	v3, v20, v28
	bif.16b	v16, v23, v31
	bif.16b	v4, v22, v30
	mov.16b	v17, v9
	bsl.16b	v17, v18, v25
	bif.16b	v5, v24, v8
	uzp1.8h	v2, v2, v7
	uzp1.8h	v3, v3, v6
	uzp1.8h	v4, v4, v16
	stp	q2, q3, [x8, #-32]
	uzp1.8h	v2, v5, v17
	stp	q4, q2, [x8], #64
	subs	x10, x10, #32
	b.ne	LBB6_15
	b	LBB6_3
LBB6_16:
	mov	w2, #0                          ; =0x0
	bl	_OP_ntt256
	cbnz	w0, LBB6_2
	b	LBB6_20
LBB6_17:
	add	x8, x22, #16
	add	x9, x23, #16
	mov	w10, #32                        ; =0x20
LBB6_18:                                ; =>This Inner Loop Header: Depth=1
	ldp	q0, q1, [x8, #-16]
	sshll.4s	v2, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v3, v1, #0
	sshll2.4s	v1, v1, #0
	add	x11, x19, x10
	stp	q2, q0, [x11, #-32]
	stp	q3, q1, [x11]
	ldp	q0, q1, [x9, #-16]
	sshll.4s	v2, v0, #0
	sshll2.4s	v0, v0, #0
	sshll.4s	v3, v1, #0
	sshll2.4s	v1, v1, #0
	stp	q2, q0, [x11, #992]
	str	q1, [x11, #1040]
	str	q3, [x11, #1024]
	add	x10, x10, #64
	add	x8, x8, #32
	add	x9, x9, #32
	cmp	x10, #1056
	b.ne	LBB6_18
; %bb.19:
	add	x0, x19, #2048
	add	x2, x19, #1024
	mov	x1, x19
	mov	w3, #256                        ; =0x100
	mov	w4, #7681                       ; =0x1e01
	mov	w5, #0                          ; =0x0
	bl	_OP_cwm
	cbnz	w0, LBB6_2
LBB6_20:
	ldr	q0, [x19, #2064]
	ldr	q1, [x19, #2048]
	ldr	q3, [x19, #2096]
	ldr	q5, [x19, #2080]
	ldr	q2, [x19, #2128]
	ldr	q4, [x19, #2112]
	ldr	q6, [x19, #2160]
	ldr	q7, [x19, #2144]
	mov	w21, #0                         ; =0x0
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v5, v3
	stp	q0, q1, [x20]
	uzp1.8h	v0, v4, v2
	uzp1.8h	v1, v7, v6
	stp	q0, q1, [x20, #32]
	ldr	q0, [x19, #2192]
	ldr	q1, [x19, #2176]
	ldr	q2, [x19, #2224]
	ldr	q3, [x19, #2208]
	ldr	q4, [x19, #2256]
	ldr	q5, [x19, #2240]
	ldr	q6, [x19, #2288]
	ldr	q7, [x19, #2272]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x20, #64]
	stp	q2, q3, [x20, #96]
	ldr	q0, [x19, #2320]
	ldr	q1, [x19, #2304]
	ldr	q2, [x19, #2352]
	ldr	q3, [x19, #2336]
	ldr	q4, [x19, #2384]
	ldr	q5, [x19, #2368]
	ldr	q6, [x19, #2416]
	ldr	q7, [x19, #2400]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x20, #128]
	stp	q2, q3, [x20, #160]
	ldr	q0, [x19, #2448]
	ldr	q1, [x19, #2432]
	ldr	q2, [x19, #2480]
	ldr	q3, [x19, #2464]
	ldr	q4, [x19, #2512]
	ldr	q5, [x19, #2496]
	ldr	q6, [x19, #2544]
	ldr	q7, [x19, #2528]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x20, #192]
	stp	q2, q3, [x20, #224]
	ldr	q0, [x19, #2576]
	ldr	q1, [x19, #2560]
	ldr	q2, [x19, #2608]
	ldr	q3, [x19, #2592]
	ldr	q4, [x19, #2640]
	ldr	q5, [x19, #2624]
	ldr	q6, [x19, #2672]
	ldr	q7, [x19, #2656]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x20, #256]
	stp	q2, q3, [x20, #288]
	ldr	q0, [x19, #2704]
	ldr	q1, [x19, #2688]
	ldr	q2, [x19, #2736]
	ldr	q3, [x19, #2720]
	ldr	q4, [x19, #2768]
	ldr	q5, [x19, #2752]
	ldr	q6, [x19, #2800]
	ldr	q7, [x19, #2784]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x20, #320]
	stp	q2, q3, [x20, #352]
	ldr	q0, [x19, #2832]
	ldr	q1, [x19, #2816]
	ldr	q2, [x19, #2864]
	ldr	q3, [x19, #2848]
	ldr	q4, [x19, #2896]
	ldr	q5, [x19, #2880]
	ldr	q6, [x19, #2928]
	ldr	q7, [x19, #2912]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x20, #384]
	stp	q2, q3, [x20, #416]
	ldr	q0, [x19, #2960]
	ldr	q1, [x19, #2944]
	ldr	q2, [x19, #2992]
	ldr	q3, [x19, #2976]
	ldr	q4, [x19, #3024]
	ldr	q5, [x19, #3008]
	ldr	q6, [x19, #3056]
	ldr	q7, [x19, #3040]
	uzp1.8h	v0, v1, v0
	uzp1.8h	v1, v3, v2
	uzp1.8h	v2, v5, v4
	uzp1.8h	v3, v7, v6
	stp	q0, q1, [x20, #448]
	stp	q2, q3, [x20, #480]
	b	LBB6_3
LBB6_21:
	mov	x8, #0                          ; =0x0
	mov	w9, #5569                       ; =0x15c1
	dup.8h	v0, w9
	mov	w9, #34880                      ; =0x8840
	movk	w9, #8, lsl #16
	dup.4s	v1, w9
	mvni.4s	v2, #30, lsl #8
	mov	w9, #7681                       ; =0x1e01
	dup.4s	v3, w9
	movi.4s	v4, #225, msl #8
LBB6_22:                                ; =>This Inner Loop Header: Depth=1
	ldr	q5, [x20, x8]
	smull.4s	v6, v5, v0
	smull2.4s	v7, v5, v0
	smull2.2d	v16, v7, v1
	smull.2d	v7, v7, v1
	uzp2.4s	v7, v7, v16
	smull2.2d	v16, v6, v1
	smull.2d	v6, v6, v1
	uzp2.4s	v6, v6, v16
	mul.4s	v6, v6, v2
	mul.4s	v7, v7, v2
	smlal2.4s	v7, v5, v0
	smlal.4s	v6, v5, v0
	cmlt.4s	v5, v6, #0
	and.16b	v5, v5, v3
	cmlt.4s	v16, v7, #0
	and.16b	v16, v16, v3
	add.4s	v7, v16, v7
	add.4s	v5, v5, v6
	add.4s	v6, v5, v4
	add.4s	v16, v7, v4
	cmgt.4s	v17, v3, v5
	cmgt.4s	v18, v3, v7
	bif.16b	v7, v16, v18
	bif.16b	v5, v6, v17
	uzp1.8h	v5, v5, v7
	str	q5, [x20, x8]
	add	x8, x8, #16
	cmp	x8, #512
	b.ne	LBB6_22
; %bb.23:
	mov	w21, #0                         ; =0x0
	b	LBB6_3
	.loh AdrpAdd	Lloh2, Lloh3
	.cfi_endproc
                                        ; -- End function
	.globl	_ntt_inverse                    ; -- Begin function ntt_inverse
	.p2align	2
_ntt_inverse:                           ; @ntt_inverse
	.cfi_startproc
; %bb.0:
	mov	x1, x0
	mov	w0, #1                          ; =0x1
	mov	x2, x1
	mov	x3, #0                          ; =0x0
	b	_standalone
	.cfi_endproc
                                        ; -- End function
	.globl	_ntt_basemul_raw                ; -- Begin function ntt_basemul_raw
	.p2align	2
_ntt_basemul_raw:                       ; @ntt_basemul_raw
	.cfi_startproc
; %bb.0:
	mov	x3, x2
	mov	x2, x1
	mov	x1, x0
	mov	w0, #2                          ; =0x2
	b	_standalone
	.cfi_endproc
                                        ; -- End function
	.globl	_ntt_basemul_acc_raw            ; -- Begin function ntt_basemul_acc_raw
	.p2align	2
_ntt_basemul_acc_raw:                   ; @ntt_basemul_acc_raw
	.cfi_startproc
; %bb.0:
	mov	x3, x2
	mov	x2, x1
	mov	x1, x0
	mov	w0, #3                          ; =0x3
	b	_standalone
	.cfi_endproc
                                        ; -- End function
	.globl	_ntt_montgomery_lift            ; -- Begin function ntt_montgomery_lift
	.p2align	2
_ntt_montgomery_lift:                   ; @ntt_montgomery_lift
	.cfi_startproc
; %bb.0:
	mov	x1, x0
	mov	w0, #4                          ; =0x4
	mov	x2, #0                          ; =0x0
	mov	x3, #0                          ; =0x0
	b	_standalone
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__const
	.p2align	2, 0x0                          ; @bit_ntt_roots32
_bit_ntt_roots32:
	.long	5569                            ; 0x15c1
	.long	6115                            ; 0x17e3
	.long	5330                            ; 0x14d2
	.long	4083                            ; 0xff3
	.long	2336                            ; 0x920
	.long	6620                            ; 0x19dc
	.long	3415                            ; 0xd57
	.long	721                             ; 0x2d1
	.long	4320                            ; 0x10e0
	.long	5298                            ; 0x14b2
	.long	5158                            ; 0x1426
	.long	5963                            ; 0x174b
	.long	808                             ; 0x328
	.long	6709                            ; 0x1a35
	.long	3838                            ; 0xefe
	.long	3064                            ; 0xbf8
	.long	4753                            ; 0x1291
	.long	3066                            ; 0xbfa
	.long	1454                            ; 0x5ae
	.long	3042                            ; 0xbe2
	.long	1842                            ; 0x732
	.long	2195                            ; 0x893
	.long	4909                            ; 0x132d
	.long	825                             ; 0x339
	.long	3196                            ; 0xc7c
	.long	4901                            ; 0x1325
	.long	7500                            ; 0x1d4c
	.long	2157                            ; 0x86d
	.long	3215                            ; 0xc8f
	.long	49                              ; 0x31
	.long	5670                            ; 0x1626
	.long	2153                            ; 0x869
	.long	4981                            ; 0x1375
	.long	6290                            ; 0x1892
	.long	2537                            ; 0x9e9
	.long	2994                            ; 0xbb2
	.long	7176                            ; 0x1c08
	.long	4448                            ; 0x1160
	.long	3362                            ; 0xd22
	.long	5766                            ; 0x1686
	.long	6221                            ; 0x184d
	.long	7384                            ; 0x1cd8
	.long	746                             ; 0x2ea
	.long	4350                            ; 0x10fe
	.long	6270                            ; 0x187e
	.long	4169                            ; 0x1049
	.long	2899                            ; 0xb53
	.long	6361                            ; 0x18d9
	.long	1843                            ; 0x733
	.long	5578                            ; 0x15ca
	.long	6834                            ; 0x1ab2
	.long	7293                            ; 0x1c7d
	.long	871                             ; 0x367
	.long	4770                            ; 0x12a2
	.long	2217                            ; 0x8a9
	.long	3455                            ; 0xd7f
	.long	769                             ; 0x301
	.long	5349                            ; 0x14e5
	.long	5573                            ; 0x15c5
	.long	4285                            ; 0x10bd
	.long	4852                            ; 0x12f4
	.long	19                              ; 0x13
	.long	4                               ; 0x4
	.long	5851                            ; 0x16db
	.long	2862                            ; 0xb2e
	.long	4086                            ; 0xff6
	.long	2073                            ; 0x819
	.long	206                             ; 0xce
	.long	5912                            ; 0x1718
	.long	6653                            ; 0x19fd
	.long	5039                            ; 0x13af
	.long	2798                            ; 0xaee
	.long	4620                            ; 0x120c
	.long	6306                            ; 0x18a2
	.long	6583                            ; 0x19b7
	.long	3070                            ; 0xbfe
	.long	2571                            ; 0xa0b
	.long	2801                            ; 0xaf1
	.long	2611                            ; 0xa33
	.long	7544                            ; 0x1d78
	.long	7110                            ; 0x1bc6
	.long	3919                            ; 0xf4f
	.long	6889                            ; 0x1ae9
	.long	1333                            ; 0x535
	.long	2610                            ; 0xa32
	.long	4161                            ; 0x1041
	.long	876                             ; 0x36c
	.long	6323                            ; 0x18b3
	.long	6405                            ; 0x1905
	.long	14                              ; 0xe
	.long	1620                            ; 0x654
	.long	3907                            ; 0xf43
	.long	6532                            ; 0x1984
	.long	7200                            ; 0x1c20
	.long	303                             ; 0x12f
	.long	3476                            ; 0xd94
	.long	953                             ; 0x3b9
	.long	5660                            ; 0x161c
	.long	6447                            ; 0x192f
	.long	3842                            ; 0xf02
	.long	5114                            ; 0x13fa
	.long	3050                            ; 0xbea
	.long	5089                            ; 0x13e1
	.long	2966                            ; 0xb96
	.long	3986                            ; 0xf92
	.long	4483                            ; 0x1183
	.long	7412                            ; 0x1cf4
	.long	4012                            ; 0xfac
	.long	3505                            ; 0xdb1
	.long	5632                            ; 0x1600
	.long	3207                            ; 0xc87
	.long	3709                            ; 0xe7d
	.long	4638                            ; 0x121e
	.long	5752                            ; 0x1678
	.long	2828                            ; 0xb0c
	.long	4279                            ; 0x10b7
	.long	7439                            ; 0x1d0f
	.long	3181                            ; 0xc6d
	.long	2691                            ; 0xa83
	.long	1668                            ; 0x684
	.long	289                             ; 0x121
	.long	2200                            ; 0x898
	.long	3293                            ; 0xcdd
	.long	2769                            ; 0xad1
	.long	495                             ; 0x1ef
	.long	127                             ; 0x7f
	.long	431                             ; 0x1af
	.long	6364                            ; 0x18dc
	.long	813                             ; 0x32d
	.long	581                             ; 0x245
	.long	5782                            ; 0x1696
	.long	4680                            ; 0x1248
	.long	2243                            ; 0x8c3
	.long	6922                            ; 0x1b0a
	.long	1053                            ; 0x41d
	.long	5996                            ; 0x176c
	.long	781                             ; 0x30d
	.long	7540                            ; 0x1d74
	.long	5630                            ; 0x15fe
	.long	5091                            ; 0x13e3
	.long	5537                            ; 0x15a1
	.long	5393                            ; 0x1511
	.long	5178                            ; 0x143a
	.long	4494                            ; 0x118e
	.long	7237                            ; 0x1c45
	.long	3424                            ; 0xd60
	.long	5572                            ; 0x15c4
	.long	902                             ; 0x386
	.long	5379                            ; 0x1503
	.long	868                             ; 0x364
	.long	587                             ; 0x24b
	.long	4123                            ; 0x101b
	.long	3003                            ; 0xbbb
	.long	4867                            ; 0x1303
	.long	4663                            ; 0x1237
	.long	5836                            ; 0x16cc
	.long	519                             ; 0x207
	.long	4509                            ; 0x119d
	.long	545                             ; 0x221
	.long	295                             ; 0x127
	.long	472                             ; 0x1d8
	.long	6809                            ; 0x1a99
	.long	2242                            ; 0x8c2
	.long	3539                            ; 0xdd3
	.long	1340                            ; 0x53c
	.long	1430                            ; 0x596
	.long	6365                            ; 0x18dd
	.long	2952                            ; 0xb88
	.long	5319                            ; 0x14c7
	.long	5275                            ; 0x149b
	.long	302                             ; 0x12e
	.long	93                              ; 0x5d
	.long	2147                            ; 0x863
	.long	4756                            ; 0x1294
	.long	597                             ; 0x255
	.long	7229                            ; 0x1c3d
	.long	4844                            ; 0x12ec
	.long	3679                            ; 0xe5f
	.long	7647                            ; 0x1ddf
	.long	193                             ; 0xc1
	.long	2556                            ; 0x9fc
	.long	5823                            ; 0x16bf
	.long	4460                            ; 0x116c
	.long	2696                            ; 0xa88
	.long	3359                            ; 0xd1f
	.long	3298                            ; 0xce2
	.long	6354                            ; 0x18d2
	.long	4144                            ; 0x1030
	.long	358                             ; 0x166
	.long	5197                            ; 0x144d
	.long	5541                            ; 0x15a5
	.long	3563                            ; 0xdeb
	.long	5030                            ; 0x13a6
	.long	3075                            ; 0xc03
	.long	4690                            ; 0x1252
	.long	5005                            ; 0x138d
	.long	1652                            ; 0x674
	.long	4629                            ; 0x1215
	.long	166                             ; 0xa6
	.long	865                             ; 0x361
	.long	1582                            ; 0x62e
	.long	5930                            ; 0x172a
	.long	3674                            ; 0xe5a
	.long	1284                            ; 0x504
	.long	7095                            ; 0x1bb7
	.long	6941                            ; 0x1b1d
	.long	1057                            ; 0x421
	.long	4166                            ; 0x1046
	.long	5926                            ; 0x1726
	.long	248                             ; 0xf8
	.long	1265                            ; 0x4f1
	.long	1178                            ; 0x49a
	.long	1592                            ; 0x638
	.long	1355                            ; 0x54b
	.long	7562                            ; 0x1d8a
	.long	4516                            ; 0x11a4
	.long	6732                            ; 0x1a4c
	.long	191                             ; 0xbf
	.long	1253                            ; 0x4e5
	.long	6668                            ; 0x1a0c
	.long	235                             ; 0xeb
	.long	3862                            ; 0xf16
	.long	6877                            ; 0x1add
	.long	6823                            ; 0x1aa7
	.long	4772                            ; 0x12a4
	.long	5895                            ; 0x1707
	.long	7305                            ; 0x1c89
	.long	3038                            ; 0xbde
	.long	6127                            ; 0x17ef
	.long	4303                            ; 0x10cf
	.long	4140                            ; 0x102c
	.long	3157                            ; 0xc55
	.long	2808                            ; 0xaf8
	.long	5748                            ; 0x1674
	.long	5657                            ; 0x1619
	.long	4260                            ; 0x10a4
	.long	6670                            ; 0x1a0e
	.long	5513                            ; 0x1589
	.long	4799                            ; 0x12bf
	.long	5064                            ; 0x13c8
	.long	7314                            ; 0x1c92
	.long	2761                            ; 0xac9
	.long	177                             ; 0xb1
	.long	7354                            ; 0x1cba
	.long	6574                            ; 0x19ae
	.long	3347                            ; 0xd13
	.long	4343                            ; 0x10f7
	.long	6297                            ; 0x1899
	.long	6686                            ; 0x1a1e
	.long	5874                            ; 0x16f2
	.long	4875                            ; 0x130b
	.long	1018                            ; 0x3fa
	.long	4010                            ; 0xfaa
	.long	1184                            ; 0x4a0
	.long	7526                            ; 0x1d66
	.long	5624                            ; 0x15f8
	.long	5569                            ; 0x15c1
	.long	1566                            ; 0x61e
	.long	3598                            ; 0xe0e
	.long	2351                            ; 0x92f
	.long	6960                            ; 0x1b30
	.long	4266                            ; 0x10aa
	.long	1061                            ; 0x425
	.long	5345                            ; 0x14e1
	.long	4617                            ; 0x1209
	.long	3843                            ; 0xf03
	.long	972                             ; 0x3cc
	.long	6873                            ; 0x1ad9
	.long	1718                            ; 0x6b6
	.long	2523                            ; 0x9db
	.long	2383                            ; 0x94f
	.long	3361                            ; 0xd21
	.long	5528                            ; 0x1598
	.long	2011                            ; 0x7db
	.long	7632                            ; 0x1dd0
	.long	4466                            ; 0x1172
	.long	5524                            ; 0x1594
	.long	181                             ; 0xb5
	.long	2780                            ; 0xadc
	.long	4485                            ; 0x1185
	.long	6856                            ; 0x1ac8
	.long	2772                            ; 0xad4
	.long	5486                            ; 0x156e
	.long	5839                            ; 0x16cf
	.long	4639                            ; 0x121f
	.long	6227                            ; 0x1853
	.long	4615                            ; 0x1207
	.long	2928                            ; 0xb70
	.long	1830                            ; 0x726
	.long	7677                            ; 0x1dfd
	.long	7662                            ; 0x1dee
	.long	2829                            ; 0xb0d
	.long	3396                            ; 0xd44
	.long	2108                            ; 0x83c
	.long	2332                            ; 0x91c
	.long	6912                            ; 0x1b00
	.long	4226                            ; 0x1082
	.long	5464                            ; 0x1558
	.long	2911                            ; 0xb5f
	.long	6810                            ; 0x1a9a
	.long	388                             ; 0x184
	.long	847                             ; 0x34f
	.long	2103                            ; 0x837
	.long	5838                            ; 0x16ce
	.long	1320                            ; 0x528
	.long	4782                            ; 0x12ae
	.long	3512                            ; 0xdb8
	.long	1411                            ; 0x583
	.long	3331                            ; 0xd03
	.long	6935                            ; 0x1b17
	.long	297                             ; 0x129
	.long	1460                            ; 0x5b4
	.long	1915                            ; 0x77b
	.long	4319                            ; 0x10df
	.long	3233                            ; 0xca1
	.long	505                             ; 0x1f9
	.long	4687                            ; 0x124f
	.long	5144                            ; 0x1418
	.long	1391                            ; 0x56f
	.long	2700                            ; 0xa8c
	.long	1317                            ; 0x525
	.long	7250                            ; 0x1c52
	.long	7554                            ; 0x1d82
	.long	7186                            ; 0x1c12
	.long	4912                            ; 0x1330
	.long	4388                            ; 0x1124
	.long	5481                            ; 0x1569
	.long	7392                            ; 0x1ce0
	.long	6013                            ; 0x177d
	.long	4990                            ; 0x137e
	.long	4500                            ; 0x1194
	.long	242                             ; 0xf2
	.long	3402                            ; 0xd4a
	.long	4853                            ; 0x12f5
	.long	1929                            ; 0x789
	.long	3043                            ; 0xbe3
	.long	3972                            ; 0xf84
	.long	4474                            ; 0x117a
	.long	2049                            ; 0x801
	.long	4176                            ; 0x1050
	.long	3669                            ; 0xe55
	.long	269                             ; 0x10d
	.long	3198                            ; 0xc7e
	.long	3695                            ; 0xe6f
	.long	4715                            ; 0x126b
	.long	2592                            ; 0xa20
	.long	4631                            ; 0x1217
	.long	2567                            ; 0xa07
	.long	3839                            ; 0xeff
	.long	1234                            ; 0x4d2
	.long	2021                            ; 0x7e5
	.long	6728                            ; 0x1a48
	.long	4205                            ; 0x106d
	.long	7378                            ; 0x1cd2
	.long	481                             ; 0x1e1
	.long	1149                            ; 0x47d
	.long	3774                            ; 0xebe
	.long	6061                            ; 0x17ad
	.long	7667                            ; 0x1df3
	.long	1276                            ; 0x4fc
	.long	1358                            ; 0x54e
	.long	6805                            ; 0x1a95
	.long	3520                            ; 0xdc0
	.long	5071                            ; 0x13cf
	.long	6348                            ; 0x18cc
	.long	792                             ; 0x318
	.long	3762                            ; 0xeb2
	.long	571                             ; 0x23b
	.long	137                             ; 0x89
	.long	5070                            ; 0x13ce
	.long	4880                            ; 0x1310
	.long	5110                            ; 0x13f6
	.long	4611                            ; 0x1203
	.long	1098                            ; 0x44a
	.long	1375                            ; 0x55f
	.long	3061                            ; 0xbf5
	.long	4883                            ; 0x1313
	.long	2642                            ; 0xa52
	.long	1028                            ; 0x404
	.long	1769                            ; 0x6e9
	.long	7475                            ; 0x1d33
	.long	5608                            ; 0x15e8
	.long	3595                            ; 0xe0b
	.long	4819                            ; 0x12d3
	.long	2057                            ; 0x809
	.long	155                             ; 0x9b
	.long	6497                            ; 0x1961
	.long	3671                            ; 0xe57
	.long	6663                            ; 0x1a07
	.long	2806                            ; 0xaf6
	.long	1807                            ; 0x70f
	.long	995                             ; 0x3e3
	.long	1384                            ; 0x568
	.long	3338                            ; 0xd0a
	.long	4334                            ; 0x10ee
	.long	1107                            ; 0x453
	.long	327                             ; 0x147
	.long	7504                            ; 0x1d50
	.long	4920                            ; 0x1338
	.long	367                             ; 0x16f
	.long	2617                            ; 0xa39
	.long	2882                            ; 0xb42
	.long	2168                            ; 0x878
	.long	1011                            ; 0x3f3
	.long	3421                            ; 0xd5d
	.long	2024                            ; 0x7e8
	.long	1933                            ; 0x78d
	.long	4873                            ; 0x1309
	.long	4524                            ; 0x11ac
	.long	3541                            ; 0xdd5
	.long	3378                            ; 0xd32
	.long	1554                            ; 0x612
	.long	4643                            ; 0x1223
	.long	376                             ; 0x178
	.long	1786                            ; 0x6fa
	.long	2909                            ; 0xb5d
	.long	858                             ; 0x35a
	.long	804                             ; 0x324
	.long	3819                            ; 0xeeb
	.long	7446                            ; 0x1d16
	.long	1013                            ; 0x3f5
	.long	6428                            ; 0x191c
	.long	7490                            ; 0x1d42
	.long	949                             ; 0x3b5
	.long	3165                            ; 0xc5d
	.long	119                             ; 0x77
	.long	6326                            ; 0x18b6
	.long	6089                            ; 0x17c9
	.long	6503                            ; 0x1967
	.long	6416                            ; 0x1910
	.long	7433                            ; 0x1d09
	.long	1755                            ; 0x6db
	.long	3515                            ; 0xdbb
	.long	6624                            ; 0x19e0
	.long	740                             ; 0x2e4
	.long	586                             ; 0x24a
	.long	6397                            ; 0x18fd
	.long	4007                            ; 0xfa7
	.long	1751                            ; 0x6d7
	.long	6099                            ; 0x17d3
	.long	6816                            ; 0x1aa0
	.long	7515                            ; 0x1d5b
	.long	3052                            ; 0xbec
	.long	6029                            ; 0x178d
	.long	2676                            ; 0xa74
	.long	2991                            ; 0xbaf
	.long	4606                            ; 0x11fe
	.long	2651                            ; 0xa5b
	.long	4118                            ; 0x1016
	.long	2140                            ; 0x85c
	.long	2484                            ; 0x9b4
	.long	7323                            ; 0x1c9b
	.long	3537                            ; 0xdd1
	.long	1327                            ; 0x52f
	.long	4383                            ; 0x111f
	.long	4322                            ; 0x10e2
	.long	4985                            ; 0x1379
	.long	3221                            ; 0xc95
	.long	1858                            ; 0x742
	.long	5125                            ; 0x1405
	.long	7488                            ; 0x1d40
	.long	34                              ; 0x22
	.long	4002                            ; 0xfa2
	.long	2837                            ; 0xb15
	.long	452                             ; 0x1c4
	.long	7084                            ; 0x1bac
	.long	2925                            ; 0xb6d
	.long	5534                            ; 0x159e
	.long	7588                            ; 0x1da4
	.long	7379                            ; 0x1cd3
	.long	2406                            ; 0x966
	.long	2362                            ; 0x93a
	.long	4729                            ; 0x1279
	.long	1316                            ; 0x524
	.long	6251                            ; 0x186b
	.long	6341                            ; 0x18c5
	.long	4142                            ; 0x102e
	.long	5439                            ; 0x153f
	.long	872                             ; 0x368
	.long	7209                            ; 0x1c29
	.long	7386                            ; 0x1cda
	.long	7136                            ; 0x1be0
	.long	3172                            ; 0xc64
	.long	7162                            ; 0x1bfa
	.long	1845                            ; 0x735
	.long	3018                            ; 0xbca
	.long	2814                            ; 0xafe
	.long	4678                            ; 0x1246
	.long	3558                            ; 0xde6
	.long	7094                            ; 0x1bb6
	.long	6813                            ; 0x1a9d
	.long	2302                            ; 0x8fe
	.long	6779                            ; 0x1a7b
	.long	2109                            ; 0x83d
	.long	4257                            ; 0x10a1
	.long	444                             ; 0x1bc
	.long	3187                            ; 0xc73
	.long	2503                            ; 0x9c7
	.long	2288                            ; 0x8f0
	.long	2144                            ; 0x860
	.long	2590                            ; 0xa1e
	.long	2051                            ; 0x803
	.long	141                             ; 0x8d
	.long	6900                            ; 0x1af4
	.long	1685                            ; 0x695
	.long	6628                            ; 0x19e4
	.long	759                             ; 0x2f7
	.long	5438                            ; 0x153e
	.long	3001                            ; 0xbb9
	.long	1899                            ; 0x76b
	.long	7100                            ; 0x1bbc
	.long	6868                            ; 0x1ad4

.subsections_via_symbols
