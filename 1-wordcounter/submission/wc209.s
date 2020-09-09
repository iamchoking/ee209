	.file	"wc209.c"
	.text
	.comm	curC,4,4
	.comm	nxtC,4,4
	.comm	befC,4,4
	.comm	state,2,2
	.comm	cmtl,4,4
	.comm	cn,4,4
	.comm	wn,4,4
	.comm	ln,4,4
	.globl	progress
	.type	progress, @function
progress:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	curC(%rip), %eax
	movl	%eax, befC(%rip)
	movl	nxtC(%rip), %eax
	movl	%eax, curC(%rip)
	movl	curC(%rip), %eax
	cmpl	$-1, %eax
	je	.L4
	movq	stdin(%rip), %rax
	movq	%rax, %rdi
	call	_IO_getc@PLT
	movl	%eax, nxtC(%rip)
	nop
	jmp	.L1
.L4:
	nop
.L1:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	progress, .-progress
	.globl	inscript
	.type	inscript, @function
inscript:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	cn(%rip), %eax
	addl	$1, %eax
	movl	%eax, cn(%rip)
	call	__ctype_b_loc@PLT
	movq	(%rax), %rax
	movl	befC(%rip), %edx
	movslq	%edx, %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	je	.L6
	call	__ctype_b_loc@PLT
	movq	(%rax), %rax
	movl	curC(%rip), %edx
	movslq	%edx, %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	jne	.L6
	movl	curC(%rip), %eax
	cmpl	$-1, %eax
	je	.L6
	movl	wn(%rip), %eax
	addl	$1, %eax
	movl	%eax, wn(%rip)
.L6:
	movl	curC(%rip), %eax
	cmpl	$10, %eax
	jne	.L8
	movl	ln(%rip), %eax
	addl	$1, %eax
	movl	%eax, ln(%rip)
.L8:
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	inscript, .-inscript
	.globl	incomment
	.type	incomment, @function
incomment:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	curC(%rip), %eax
	cmpl	$10, %eax
	jne	.L10
	movl	ln(%rip), %eax
	addl	$1, %eax
	movl	%eax, ln(%rip)
	movl	cn(%rip), %eax
	addl	$1, %eax
	movl	%eax, cn(%rip)
.L10:
	movzwl	state(%rip), %eax
	cmpw	$3, %ax
	jne	.L12
	movw	$2, state(%rip)
.L12:
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	incomment, .-incomment
	.section	.rodata
.LC0:
	.string	"0 0 0"
	.align 8
.LC1:
	.string	"Error: line %d: unterminated comment\n"
.LC2:
	.string	"%d %d %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movw	$1, state(%rip)
	movq	stdin(%rip), %rax
	movq	%rax, %rdi
	call	_IO_getc@PLT
	movl	%eax, nxtC(%rip)
	movl	$10, curC(%rip)
	movl	$-1, cn(%rip)
	movl	$0, wn(%rip)
	movl	$0, ln(%rip)
	movl	$0, cmtl(%rip)
	movl	curC(%rip), %eax
	cmpl	$10, %eax
	jne	.L14
	movl	nxtC(%rip), %eax
	cmpl	$-1, %eax
	je	.L15
.L14:
	movl	curC(%rip), %eax
	cmpl	$-1, %eax
	jne	.L18
.L15:
	leaq	.LC0(%rip), %rdi
	call	puts@PLT
	movl	$0, %eax
	jmp	.L17
.L24:
	movzwl	state(%rip), %eax
	cmpw	$1, %ax
	jne	.L19
	movl	curC(%rip), %eax
	cmpl	$47, %eax
	jne	.L20
	movl	nxtC(%rip), %eax
	cmpl	$42, %eax
	jne	.L20
	movw	$3, state(%rip)
	movl	ln(%rip), %eax
	movl	%eax, cmtl(%rip)
	jmp	.L22
.L20:
	movl	$0, %eax
	call	inscript
	jmp	.L22
.L19:
	movl	curC(%rip), %eax
	cmpl	$42, %eax
	jne	.L23
	movl	nxtC(%rip), %eax
	cmpl	$47, %eax
	jne	.L23
	movzwl	state(%rip), %eax
	cmpw	$2, %ax
	jne	.L23
	movw	$1, state(%rip)
	movl	$32, nxtC(%rip)
	jmp	.L22
.L23:
	movl	$0, %eax
	call	incomment
.L22:
	movl	$0, %eax
	call	progress
.L18:
	movl	curC(%rip), %eax
	cmpl	$-1, %eax
	jne	.L24
	movzwl	state(%rip), %eax
	cmpw	$2, %ax
	jne	.L25
	movl	cmtl(%rip), %edx
	movq	stderr(%rip), %rax
	leaq	.LC1(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$0, %eax
	jmp	.L17
.L25:
	movl	cn(%rip), %ecx
	movl	wn(%rip), %edx
	movl	ln(%rip), %eax
	movl	%eax, %esi
	leaq	.LC2(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
.L17:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
