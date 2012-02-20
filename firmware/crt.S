/* Copyright (c) 2002, Marek Michalkiewicz <marekm@amelek.gda.pl>
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.

   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE. */

/* $Id: crt.S,v 1.2 2007/08/10 03:17:48 iruka Exp $ */

#if (__GNUC__ < 3) || (__GNUC__ == 3 && __GNUC_MINOR__ < 3)
#error "GCC version >= 3.3 required"
#endif

// *******************************************************
// 		attiny2313専用	int0のみ使用.
// *******************************************************
#define	VECSIZE			6	// reset,int0,int1 の３個のみ.

// *******************************************************
// 		.dataセクションにstaticデータをコピーする.
// *******************************************************
#define	DO_COPY_DATA	1	// .dataサイズが0のときは0に.

// *******************************************************
// 		.bss セクションをゼロクリアする.
// *******************************************************
#define	DO_CLEAR_BSS	1	// .bssサイズが 0のときは0に.


#include "macros.inc"

	.macro	vector name
	.if (. - __vectors < VECSIZE)
	.weak	\name
	.set	\name, __bad_interrupt
	XJMP	\name
	.endif
	.endm

	.section .vectors,"ax",@progbits
	.global	__vectors
	.func	__vectors
__vectors:
	XJMP	__init
	vector	__vector_1
	vector	__vector_2
	vector	__vector_3
	vector	__vector_4
	vector	__vector_5
	vector	__vector_6
	vector	__vector_7
	vector	__vector_8
	vector	__vector_9
	vector	__vector_10
	vector	__vector_11
	vector	__vector_12
	vector	__vector_13
	vector	__vector_14
	vector	__vector_15
	vector	__vector_16
	vector	__vector_17
	vector	__vector_18
	vector	__vector_19
	vector	__vector_20
	vector	__vector_21
	vector	__vector_22
	vector	__vector_23
	vector	__vector_24
	.endfunc

	/* Handle unexpected interrupts (enabled and no handler), which
	   usually indicate a bug.  Jump to the __vector_default function
	   if defined by the user, otherwise jump to the reset address.

	   This must be in a different section, otherwise the assembler
	   will resolve "rjmp" offsets and there will be no relocs.  */

	.section .init0,"ax",@progbits
	.weak	__init
;	.func	__init
__init:
__bad_interrupt:

#ifndef __AVR_ASM_ONLY__
	.weak	__stack

	/* By default, malloc() uses the current value of the stack pointer
	   minus __malloc_margin as the highest available address.

	   In some applications with external SRAM, the stack can be below
	   the data section (in the internal SRAM - faster), and __heap_end
	   should be set to the highest address available for malloc().  */
	.weak	__heap_end
	.set	__heap_end, 0

	.section .init2,"ax",@progbits
	clr	__zero_reg__
	out	_SFR_IO_ADDR(SREG), __zero_reg__
	ldi	r28,lo8(__stack)

	out	_SFR_IO_ADDR(SPL), r28

	.section .init4,"ax",@progbits
	.global __do_copy_data
	.global __do_clear_bss

// *******************************************************
// attiny2313専用:
// .data , .bss の上位アドレスは常に０であると仮定.
// *******************************************************

__do_copy_data:
//	ldi	r17, hi8(__data_end)

	// Y r26:27 = 初期化データのコピー先SRAMアドレス.
	ldi	r26, lo8(__data_start)
	ldi	r27, hi8(__data_start)

	// Z r30:31 = staticデータのコピー元ROMアドレス.
	ldi	r30, lo8(__data_load_start)
	ldi	r31, hi8(__data_load_start)


// *******************************************************
// 		.dataセクションにstaticデータをコピーする.
// *******************************************************
#if	DO_COPY_DATA
// *******************************************************
// .data  のサイズは非ゼロを仮定.
//	rjmp	.do_copy_data_start
// *******************************************************

.do_copy_data_loop:
	lpm r0, Z+
	st	X+, r0

.do_copy_data_start:
	cpi	r26, lo8(__data_end)

// *******************************************************
// attiny2313専用:
// .data , .bss の上位アドレスは常に０であると仮定.
//	cpc	r27, r17
// *******************************************************

	brne	.do_copy_data_loop

#endif	// DO_COPY_DATA

// *******************************************************
// 		.bss セクションをゼロクリアする.
// *******************************************************
#if	DO_CLEAR_BSS	

__do_clear_bss:

// *******************************************************
// .data の直後に.bssが来ると仮定.
// .bss  のサイズは非ゼロを仮定.
// *******************************************************
//	ldi	r17, hi8(__bss_end)
//	ldi	r26, lo8(__bss_start)
//	ldi	r27, hi8(__bss_start)
//	rjmp	.do_clear_bss_start


.do_clear_bss_loop:
	st	X+, r1
.do_clear_bss_start:
	cpi	r26, lo8(__bss_end)
//	cpc	r27, r17
	brne	.do_clear_bss_loop

#endif	//	DO_CLEAR_BSS	

	.set	__stack, RAMEND

#endif /* !__AVR_ASM_ONLY__ */

	.section .init9,"ax",@progbits
	XJMP	main

;	.endfunc

