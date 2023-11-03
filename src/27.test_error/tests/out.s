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
