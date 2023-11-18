	.data
	.globl	var2
var2:	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.globl	varptr
varptr:	.quad	0
L3:
	.byte	37
	.byte	100
	.byte	10
	.byte	0
L4:
	.byte	37
	.byte	100
	.byte	10
	.byte	0
L5:
	.byte	37
	.byte	100
	.byte	10
	.byte	0
L6:
	.byte	37
	.byte	100
	.byte	10
	.byte	0
L7:
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
	movq	$12, %r10
	leaq	var2(%rip), %r11
	movq	$0, %r12
	addq	%r11, %r12
	movq	%r10, (%r12)
	leaq	var2(%rip), %r10
	movq	$0, %r11
	addq	%r10, %r11
	movq	(%r11), %r11
	movq	%r11, %rsi
	leaq	L3(%rip), %r10
	movq	%r10, %rdi
	call	printf@PLT
	movq	%rax, %r10
	movq	$99, %r10
	leaq	var2(%rip), %r11
	movq	$4, %r12
	addq	%r11, %r12
	movb	%r10b, (%r12)
	leaq	var2(%rip), %r10
	movq	$4, %r11
	addq	%r10, %r11
	movzbq	(%r11), %r11
	movq	%r11, %rsi
	leaq	L4(%rip), %r10
	movq	%r10, %rdi
	call	printf@PLT
	movq	%rax, %r10
	movq	$4005, %r10
	leaq	var2(%rip), %r11
	movq	$8, %r12
	addq	%r11, %r12
	movq	%r10, (%r12)
	leaq	var2(%rip), %r10
	movq	$8, %r11
	addq	%r10, %r11
	movq	(%r11), %r11
	movq	%r11, %rsi
	leaq	L5(%rip), %r10
	movq	%r10, %rdi
	call	printf@PLT
	movq	%rax, %r10
	leaq	var2(%rip), %r10
	movq	$0, %r11
	addq	%r10, %r11
	movq	(%r11), %r11
	leaq	var2(%rip), %r10
	movq	$4, %r12
	addq	%r10, %r12
	movzbq	(%r12), %r12
	addq	%r11, %r12
	leaq	var2(%rip), %r10
	movq	$8, %r11
	addq	%r10, %r11
	movq	(%r11), %r11
	addq	%r12, %r11
	movq	%r11, -8(%rbp)
	movq	-8(%rbp), %r10
	movq	%r10, %rsi
	leaq	L6(%rip), %r10
	movq	%r10, %rdi
	call	printf@PLT
	movq	%rax, %r10
	leaq	var2(%rip), %r10
	movq	%r10, varptr(%rip)
	movq	varptr(%rip), %r10
	movq	$0, %r11
	addq	%r10, %r11
	movq	(%r11), %r11
	movq	varptr(%rip), %r10
	movq	$4, %r12
	addq	%r10, %r12
	movzbq	(%r12), %r12
	addq	%r11, %r12
	movq	varptr(%rip), %r10
	movq	$8, %r11
	addq	%r10, %r11
	movq	(%r11), %r11
	addq	%r12, %r11
	movq	%r11, -8(%rbp)
	movq	-8(%rbp), %r10
	movq	%r10, %rsi
	leaq	L7(%rip), %r10
	movq	%r10, %rdi
	call	printf@PLT
	movq	%rax, %r10
	movq	$0, %r10
	movl	%r10d, %eax
	jmp	L2
L2:
	addq	$16,%rsp
	popq	%rbp
	ret
