	.text
	.data
	.globl	i
i:	.long	0
	.data
	.globl	j
j:	.long	0
	.text
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	movq	$6, %r8
	movl	%r8d, i(%rip)
	movq	$12, %r8
	movl	%r8d, j(%rip)
	movzbl	i(%rip), %r8
	movzbl	j(%rip), %r9
	cmpq	%r9, %r8
	jge	L2
	movzbl	i(%rip), %r8
	movq	%r8, %rdi
	call	printint
	movq	%rax, %r9
	movzbl	j(%rip), %r8
	movq	%r8, %rdi
	call	printint
	movq	%rax, %r9
L3:
L2:
L1:
	popq	%rbp
	ret
