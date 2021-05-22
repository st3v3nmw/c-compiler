.data
	newline: .asciiz "\n"
	q: .float 0.0
	temp_variable_UFJ8Pcjn11: .asciiz "\nHello world!"
	temp_variable_UFJ8Pcjn12: .asciiz "\nIf output:"
	temp_variable_UFJ8Pcjn13: .asciiz "q > 10 and 2 / 3 < 1"
	temp_variable_UFJ8Pcjn14: .asciiz "q < 10 or 2 / 3 > 1"
	temp_variable_UFJ8Pcjn15: .asciiz "\nWhile loop output:"
	i: .word 0
	temp_variable_UFJ8Pcjn16: .asciiz "\nFor loop output:"
	foo: .word 0

.text

.globl main
main:
	jal compute_globals_U3quItLgAX
	la $t0, temp_variable_UFJ8Pcjn11
	li $v0, 4
	move $a0, $t0
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	li $t0, 1
	l.s $f1, q
	mtc1 $t0, $f0
	cvt.s.w $f0, $f0
	add.s $f2, $f0, $f1
	li $v0, 2
	mov.s $f12, $f2
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	la $t0, temp_variable_UFJ8Pcjn12
	li $v0, 4
	move $a0, $t0
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	li $t0, 78
	li $t1, 10
	sgt $t2, $t0, $t1
	beq $t2, $0, L1
	la $t0, temp_variable_UFJ8Pcjn13
	li $v0, 4
	move $a0, $t0
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	j L2
L1:
	la $t0, temp_variable_UFJ8Pcjn14
	li $v0, 4
	move $a0, $t0
	syscall
	li $v0, 4
	la $a0, newline
	syscall
L2:
	la $t0, temp_variable_UFJ8Pcjn15
	li $v0, 4
	move $a0, $t0
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	li $t0, 0
	sw $t0, i
L3:
	lw $t0, i
	li $t1, 3
	sle $t2, $t0, $t1
	beq $t2, $0, L4
	lw $t0, i
	li $v0, 1
	move $a0, $t0
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	lw $t0, i
	li $t1, 1
	add $t3, $t0, $t1
	sw $t3, i
	j L3
L4:
	la $t0, temp_variable_UFJ8Pcjn16
	li $v0, 4
	move $a0, $t0
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	li $t0, 14
	sw $t0, foo
L5:
	lw $t0, foo
	li $t1, 10
	sgt $t2, $t0, $t1
	beq $t2, $0, L6
	lw $t0, foo
	li $v0, 1
	move $a0, $t0
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	lw $t0, foo
	li $t1, 1
	sub $t3, $t0, $t1
	sw $t3, foo
	j L5
L6:
	li $v0, 10
	syscall

.globl compute_globals_U3quItLgAX
.ent compute_globals_U3quItLgAX
compute_globals_U3quItLgAX:
	li $t0, 37
	li.s $f1, -100.0
	mtc1 $t0, $f0
	cvt.s.w $f0, $f0
	div.s $f2, $f0, $f1
	s.s $f2, q
	jr $ra
.end compute_globals_U3quItLgAX
