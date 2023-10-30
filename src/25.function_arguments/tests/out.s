	.text
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	addq	$-16,%rsp
	movq	$1, %r10
	movl	%r10d, -4(%rbp)
L2:
	movslq	-4(%rbp), %r10
	movq	$10, %r11
	cmpq	%r11, %r10
	jg	L3
	movslq	-4(%rbp), %r10
	movq	%r10, %rdi
	call	printint
	movq	%rax, %r10
	movslq	-4(%rbp), %r10
	movq	$1, %r11
	addq	%r10, %r11
	movl	%r11d, -4(%rbp)
	jmp	L2
L3:
L1:
	addq	$16,%rsp
	popq	%rbp
	ret
	.globl	fred
	.type	fred, @function
fred:
	pushq	%rbp
	movq	%rsp, %rbp
	addq	$-16,%rsp
	movq	$12, %r10
	movslq	-4(%rbp), %r11
	movl	%r10d, -4(%rbp)
	movq	$3, %r10
	movslq	-4(%rbp), %r11
	imulq	%r10, %r11
	movslq	-8(%rbp), %r10
	movl	%r11d, -8(%rbp)
	movslq	-4(%rbp), %r10
	movslq	-8(%rbp), %r11
	cmpq	%r11, %r10
	jl	L5
	movq	$2, %r10
	movslq	-8(%rbp), %r11
	imulq	%r10, %r11
	movslq	-4(%rbp), %r10
	subq	%r10, %r11
	movq	%r11, %rdi
	call	printint
	movq	%rax, %r10
L5:
L4:
	addq	$16,%rsp
	popq	%rbp
	ret
