	.text
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	addq	$-32,%rsp
	leaq	a(%rip), %r8
	movq	%r8, -12(%rbp)
	movq	$19, %r8
	movq	-12(%rbp), %r9
	movb	%r8b, (%r9)
	movzbq	-4(%rbp), %r8
	movq	%r8, %rdi
	call	printint
	movq	%rax, %r9
	leaq	d(%rip), %r8
	movq	%r8, -24(%rbp)
	movq	$12, %r8
	movq	-24(%rbp), %r9
	movq	%r8, (%r9)
	movslq	-16(%rbp), %r8
	movq	%r8, %rdi
	call	printint
	movq	%rax, %r9
	movq	$0, %r8
	movl	%r8d, %eax
	jmp	L1
L1:
	addq	$32,%rsp
	popq	%rbp
	ret
