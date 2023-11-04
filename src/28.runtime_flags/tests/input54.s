L3:
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
	.byte	44
	.byte	32
	.byte	37
	.byte	100
	.byte	10
	.byte	0
	.text
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	addq	$-16,%rsp
	movq	$0, %r10
	movl	%r10d, -4(%rbp)
L4:
	movslq	-4(%rbp), %r10
	movq	$20, %r11
	cmpq	%r11, %r10
	jge	L5
	movslq	-4(%rbp), %r10
	movq	%r10, %rsi
	leaq	L3(%rip), %r10
	movq	%r10, %rdi
	call	printf
	movq	%rax, %r10
	movslq	i(%rip), %r10
	incl	i(%rip)
	jmp	L4
L5:
	movq	$0, %r10
	movl	%r10d, %eax
	jmp	L2
L2:
	addq	$16,%rsp
	popq	%rbp
	ret
