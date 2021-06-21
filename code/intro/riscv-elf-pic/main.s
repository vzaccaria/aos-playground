	.file	"main.c"
	.option pic
	.attribute arch, "rv64i2p0_m2p0_a2p0_f2p0_d2p0_c2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align	3
.LC0:
	.string	"Hello world! %p\n"
	.section	.text.startup,"ax",@progbits
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-16
	la	a1,x
	lla	a0,.LC0
	sd	ra,8(sp)
	call	printf@plt
	ld	ra,8(sp)
	li	a0,0
	addi	sp,sp,16
	jr	ra
	.size	main, .-main
	.globl	x
	.data
	.align	2
	.type	x, @object
	.size	x, 4
x:
	.word	1
	.ident	"GCC: (GNU) 10.2.0"
