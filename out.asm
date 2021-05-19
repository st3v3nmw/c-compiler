.data
	newline: .asciiz "\n"
	q: .float 0.0
	UFJ8Pcjn1844XajR1: .asciiz "Hello world!"
	qq: .float 0.0

.text

.globl main
main:
	jal U3quItLgAXaPHLuf
	la $t0, UFJ8Pcjn1844XajR1
	li $v0, 4
	move $a0, $t0
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	li $t0, 3
	li.s $f1, 7.5
	mtc1 $t0, $f0
	cvt.s.w $f0, $f0
	add.s $f2, $f0, $f1
	lw $t0, twice_that
	add.s $f1, $f2, $f0
	s.s $f1, qq
	l.s $f0, qq
	li $v0, 2
	mov.s $f12, $f0
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	li $v0, 10
	syscall

.globl U3quItLgAXaPHLuf
.ent U3quItLgAXaPHLuf
U3quItLgAXaPHLuf:
	li.s $f0, 2.5
	li $t1, 16
	mtc1 $t1, $f1
	cvt.s.w $f1, $f1
	mul.s $f2, $f0, $f1
	s.s $f2, q
	jr $ra
.end U3quItLgAXaPHLuf

.globl print_sth
.ent print_sth
print_sth:

.end print_sth

.globl twice_that
.ent twice_that
twice_that:

.end twice_that
