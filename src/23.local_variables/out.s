	.data
	.globl	printint
printint:	.data
	.globl	printchar
printchar:	.text
	.data
	.globl	a
a:	.long	0
	.data
	.globl	b
b:	.long	0
	.data
	.globl	c
c:	.long	0
	.data
	.globl	main
main:	.text
	.text
	.globl		addq	$%d,%%rsp

	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	movq	$10, %r8
	movl	%r8d, x(%rip)
	movq	$20, %r8
	movl	%r8d, y(%rip)
	movq	$30, %r8
	movb	%r8b, z(%rip)
	movslq	-12(%rbp), %r8
	movq	%r8, %rdi
	call	printint
	movq	%rax, %r9
	movslq	-8(%rbp), %r8
	movq	%r8, %rdi
	call	printint
	movq	%rax, %r9
	movzbq	-4(%rbp), %r8
	movq	%r8, %rdi
	call	printint
	movq	%rax, %r9
	movq	$5, %r8
	movl	%r8d, a(%rip)
	movq	$15, %r8
	movl	%r8d, b(%rip)
	movq	$25, %r8
	movl	%r8d, c(%rip)
	movslq	a(%rip), %r8
	movq	%r8, %rdi
	call	printint
	movq	%rax, %r9
	movslq	b(%rip), %r8
	movq	%r8, %rdi
	call	printint
	movq	%rax, %r9
	movslq	c(%rip), %r8
	movq	%r8, %rdi
	call	printint
	movq	%rax, %r9
	movq	$0, %r8
	movl	%r8d, %eax
	jmp	L1
L1:
	addq	$16,%rsp
	popq	%rbp
	ret
