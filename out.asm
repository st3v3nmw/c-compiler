.data
	newline: .asciiz "\n"
	q: .word 0
	foo: .word 0
	x: .word 0
	y: .word 0
	z: .word 0
	i: .word 0

.text
	li $t0, 2
	sw $t0, q

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
	li $t0, 4
	lw $t1, q
	mul $t3, $t0, $t1
	sub $t0, $t2, $t3
	sw $t0, z
	lw $t0, z
	li $v0, 1
	move $a0, $t0
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	lw $t0, z
	li $t1, 10
	slt $t2, $t0, $t1
	beq $t2, $0, L1
	lw $t0, x
	lw $t1, y
	sub $t2, $t0, $t1
	li $v0, 1
	move $a0, $t2
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	j L2
L1:
	lw $t0, z
	li $t1, 100
	sgt $t2, $t0, $t1
	beq $t2, $0, L3
	li $t0, 1232
	li $v0, 1
	move $a0, $t0
	syscall
	li $v0, 4
	la $a0, newline
	syscall
L3:
L2:
	li $t0, 0
	sw $t0, i
L4:
	lw $t0, i
	li $t1, 5
	sle $t2, $t0, $t1
	beq $t2, $0, L5
	lw $t0, i
	li $t1, 4
	sge $t3, $t0, $t1
	beq $t3, $0, L6
	lw $t0, i
	li $v0, 1
	move $a0, $t0
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	j L7
L6:
	li $t0, 2
	li $v0, 1
	move $a0, $t0
	syscall
	li $v0, 4
	la $a0, newline
	syscall
L7:
	lw $t0, i
	li $t1, 1
	add $t3, $t0, $t1
	sw $t3, i
	j L4
L5:
	li $v0, 10
	syscall
