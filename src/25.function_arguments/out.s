	.text
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, -4(%rbp)
	movb	%sil, -8(%rbp)
	movq	%rdx, -16(%rbp)
	movl	%ecx, -20(%rbp)
	movl	%r8d, -24(%rbp)
	movl	%r9d, -28(%rbp)
	addq	$-48,%rsp
	movq	$13, %r10
	movl	%r10d, -4(%rbp)
	movslq	-4(%rbp), %r10
	movq	%r10, %rdi
	call	printint
	movq	%rax, %r10
	movq	$23, %r10
	movb	%r10b, -8(%rbp)
	movzbq	-8(%rbp), %r10
	movq	%r10, %rdi
	call	printint
	movq	%rax, %r10
	movq	$34, %r10
	movq	%r10, -16(%rbp)
	movq	-16(%rbp), %r10
	movq	%r10, %rdi
	call	printint
	movq	%rax, %r10
	movq	$44, %r10
	movl	%r10d, -20(%rbp)
	movslq	-20(%rbp), %r10
	movq	%r10, %rdi
	call	printint
	movq	%rax, %r10
	movq	$54, %r10
	movl	%r10d, -24(%rbp)
	movslq	-24(%rbp), %r10
	movq	%r10, %rdi
	call	printint
	movq	%rax, %r10
	movq	$64, %r10
	movl	%r10d, -28(%rbp)
	movslq	-28(%rbp), %r10
	movq	%r10, %rdi
	call	printint
	movq	%rax, %r10
	movq	$74, %r10
	movl	%r10d, 16(%rbp)
	movslq	16(%rbp), %r10
	movq	%r10, %rdi
	call	printint
	movq	%rax, %r10
	movq	$84, %r10
	movl	%r10d, 24(%rbp)
	movslq	24(%rbp), %r10
	movq	%r10, %rdi
	call	printint
	movq	%rax, %r10
	movq	$94, %r10
	movl	%r10d, -32(%rbp)
	movslq	-32(%rbp), %r10
	movq	%r10, %rdi
	call	printint
	movq	%rax, %r10
	movq	$95, %r10
	movl	%r10d, -36(%rbp)
	movslq	-36(%rbp), %r10
	movq	%r10, %rdi
	call	printint
	movq	%rax, %r10
	movq	$96, %r10
	movl	%r10d, -40(%rbp)
	movslq	-40(%rbp), %r10
	movq	%r10, %rdi
	call	printint
	movq	%rax, %r10
	movq	$0, %r10
	movl	%r10d, %eax
	jmp	L1
L1:
	addq	$48,%rsp
	popq	%rbp
	ret
