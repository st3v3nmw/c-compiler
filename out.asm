.data
	newline: .asciiz "\n"
	x: .word 0
	y: .word 0
	z: .word 0

.text

.globl main
main:
	li $t0, 14
	li $t1, 48
	mul $t2, $t0, $t1
	li $t0, 4
	div $t1, $t2, $t0
	sw $t1, x
	lw $t0, x
	li $v0, 1
	move $a0, $t0
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	li $t0, -15
	sw $t0, y
	lw $t0, y
	li $v0, 1
	move $a0, $t0
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	lw $t0, x
	lw $t1, y
	add $t2, $t0, $t1
	sub $t2, $t0, $t1
	sw $t2, z
	lw $t0, z
	li $v0, 1
	move $a0, $t0
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	li $v0, 10
	syscall
