	.text
	.globl	param8
	.type	param8, @function
param8:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	%edx, -12(%rbp)
	movl	%ecx, -16(%rbp)
	movl	%r8d, -20(%rbp)
	movl	%r9d, -24(%rbp)
	addq	$-32,%rsp
	movslq	-4(%rbp), %r10
	movq	%r10, %rdi
	call	printint
	movq	%rax, %r10
	movslq	-8(%rbp), %r10
	movq	%r10, %rdi
	call	printint
	movq	%rax, %r10
	movslq	-12(%rbp), %r10
	movq	%r10, %rdi
	call	printint
	movq	%rax, %r10
	movslq	-16(%rbp), %r10
	movq	%r10, %rdi
	call	printint
	movq	%rax, %r10
	movslq	-20(%rbp), %r10
	movq	%r10, %rdi
	call	printint
	movq	%rax, %r10
	movslq	-24(%rbp), %r10
	movq	%r10, %rdi
	call	printint
	movq	%rax, %r10
	movslq	16(%rbp), %r10
	movq	%r10, %rdi
	call	printint
	movq	%rax, %r10
	movslq	24(%rbp), %r10
	movq	%r10, %rdi
	call	printint
	movq	%rax, %r10
	movq	$0, %r10
	movl	%r10d, %eax
	jmp	L1
L1:
	addq	$32,%rsp
	popq	%rbp
	ret
	.globl	fred
	.type	fred, @function
fred:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	%edx, -12(%rbp)
	addq	$-16,%rsp
	movslq	-4(%rbp), %r10
	movslq	-8(%rbp), %r11
	addq	%r10, %r11
	movslq	-12(%rbp), %r10
	addq	%r11, %r10
	movl	%r10d, %eax
	jmp	L2
L2:
	addq	$16,%rsp
	popq	%rbp
	ret
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	addq	$-16,%rsp
	movq	$34, %r11
	pushq	%r11
	movq	$21, %r10
	pushq	%r10
	movq	$13, %r10
	movq	%r10, %r9
	movq	$8, %r10
	movq	%r10, %r8
	movq	$5, %r10
	movq	%r10, %rcx
	movq	$3, %r10
	movq	%r10, %rdx
	movq	$2, %r10
	movq	%r10, %rsi
	movq	$1, %r10
	movq	%r10, %rdi
	call	param8
	addq	$16, %rsp
	movq	%rax, %r10
	movq	$4, %r10
	movq	%r10, %rdx
	movq	$3, %r10
	movq	%r10, %rsi
	movq	$2, %r10
	movq	%r10, %rdi
	call	fred
	movq	%rax, %r10
	movslq	-4(%rbp), %r11
	movl	%r10d, -4(%rbp)
	movslq	-4(%rbp), %r10
	movq	%r10, %rdi
	call	printint
	movq	%rax, %r10
	movq	$0, %r10
	movl	%r10d, %eax
	jmp	L3
L3:
	addq	$16,%rsp
	popq	%rbp
	ret
