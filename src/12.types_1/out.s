	.text
.LC0:
	.string	"%d\n"
printint:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
	leave
	ret

	.comm	i,8,8
	.comm	j,1,1
	.text
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	movq	$20, %r8
	movb	%r8b, j(%rip)
	movzbq	j(%rip), %r8
	movq	%r8, %rdi
	call	printint
	movq	$10, %r8
	movq	%r8, i(%rip)
	movq	i(%rip), %r8
	movq	%r8, %rdi
	call	printint
	movq	$1, %r8
	movq	%r8, i(%rip)
L0:
	movq	i(%rip), %r8
	movq	$5, %r9
	cmpq	%r9, %r8
	setle	%r9b
	movzbq	%r9b, %r9
	movq	i(%rip), %r8
	movq	%r8, %rdi
	call	printint
	movq	i(%rip), %r8
	movq	$1, %r9
	addq	%r8, %r9
	movq	%r9, i(%rip)
	jmp	L0
L1:
	movq	$253, %r8
	movb	%r8b, j(%rip)
L2:
	movzbq	j(%rip), %r8
	movq	$2, %r9
	cmpq	%r9, %r8
	setl	%r9b
	movzbq	%r9b, %r9
	movzbq	j(%rip), %r8
	movq	%r8, %rdi
	call	printint
	movzbq	j(%rip), %r8
	movq	$1, %r9
	addq	%r8, %r9
	movb	%r9b, j(%rip)
	jmp	L2
L3:
	movl	$0, %eax
	popq	%rbp
	ret
