	.text
	.text
	.globl	test
	.type	test, @function
test:
	pushq	%rbp
	movq	%rsp, %rbp
	movq	$22, %r8
	movq	%r8, %rdi
	call	printint
	movq	%rax, %r9
L1:
	popq	%rbp
	ret
	.comm	a,1,1
	.comm	b,1,1
	.comm	c,1,1
	.comm	d,8,8
	.comm	e,1,1
	.comm	f,8,8
	.text
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	movq	$18, %r8
	movb	%r8b, a(%rip)
	movzbq	a(%rip), %r8
	movq	%r8, %rdi
	call	printint
	movq	%rax, %r9
	leaq	a(%rip), %r8
	movq	%r8, b(%rip)
	movq	b(%rip), %r8
	movzbq	(%r8), %r8
	movb	%r8b, c(%rip)
	movzbq	c(%rip), %r8
	movq	%r8, %rdi
	call	printint
	movq	%rax, %r9
	movq	$12, %r8
	movl	%r8d, d(%rip)
	movzbl	d(%rip), %r8
	movq	%r8, %rdi
	call	printint
	movq	%rax, %r9
	leaq	d(%rip), %r8
	movq	%r8, e(%rip)
	movq	e(%rip), %r8
	movq	(%r8), %r8
	movl	%r8d, f(%rip)
	movzbl	f(%rip), %r8
	movq	%r8, %rdi
	call	printint
	movq	%rax, %r9
	movq	$1, %r8
	movq	%r8, %rdi
	call	test
	movq	%rax, %r9
	movq	$0, %r8
	movl	%r8d, %eax
	jmp	L2
L2:
	popq	%rbp
	ret
