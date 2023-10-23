	.text
	.data
	.globl	str
str:	.quad	0
	.data
	.globl	x
x:	.long	0
L2:
	.byte	72
	.byte	101
	.byte	108
	.byte	108
	.byte	111
	.byte	32
	.byte	119
	.byte	111
	.byte	114
	.byte	108
	.byte	100
	.byte	10
	.byte	0
	.text
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	movq	$23, %r8
	negq	%r8
	movl	%r8d, x(%rip)
	movslq	x(%rip), %r8
	movq	%r8, %rdi
	call	printint
	movq	%rax, %r9
	movq	$10, %r8
	negq	%r8
	movq	$10, %r9
	negq	%r9
	imulq	%r8, %r9
	movq	%r9, %rdi
	call	printint
	movq	%rax, %r8
	movq	$1, %r8
	movl	%r8d, x(%rip)
	movslq	x(%rip), %r8
	notq	%r8
	movl	%r8d, x(%rip)
	movslq	x(%rip), %r8
	movq	%r8, %rdi
	call	printint
	movq	%rax, %r9
	movq	$2, %r8
	movq	$5, %r9
	cmpq	%r9, %r8
	setg	%r9b
	movzbq	%r9b, %r9
	movl	%r9d, x(%rip)
	movslq	x(%rip), %r8
	movq	%r8, %rdi
	call	printint
	movq	%rax, %r9
	movslq	x(%rip), %r8
	test	%r8, %r8
	sete	%r8b
	movzbq	%r8b, %r8
	movl	%r8d, x(%rip)
	movslq	x(%rip), %r8
	movq	%r8, %rdi
	call	printint
	movq	%rax, %r9
	movslq	x(%rip), %r8
	test	%r8, %r8
	sete	%r8b
	movzbq	%r8b, %r8
	movl	%r8d, x(%rip)
	movslq	x(%rip), %r8
	movq	%r8, %rdi
	call	printint
	movq	%rax, %r9
	movq	$13, %r8
	movl	%r8d, x(%rip)
	movslq	x(%rip), %r8
	test	%r8, %r8
	je	L3
	movq	$13, %r8
	movq	%r8, %rdi
	call	printint
	movq	%rax, %r9
L3:
	movq	$0, %r8
	movl	%r8d, x(%rip)
	movslq	x(%rip), %r8
	test	%r8, %r8
	sete	%r8b
	movzbq	%r8b, %r8
	test	%r8, %r8
	je	L4
	movq	$14, %r8
	movq	%r8, %rdi
	call	printint
	movq	%rax, %r9
L4:
	leaq	L2(%rip), %r8
	movq	%r8, str(%rip)
L5:
	movq	str(%rip), %r8
	movzbq	(%r8), %r8
	test	%r8, %r8
	je	L6
	movq	str(%rip), %r8
	movzbq	(%r8), %r8
	movq	%r8, %rdi
	call	printchar
	movq	%rax, %r9
	movq	str(%rip), %r8
	incq	str(%rip)
	jmp	L5
L6:
	movq	$0, %r8
	movl	%r8d, %eax
	jmp	L1
L1:
	popq	%rbp
	ret
