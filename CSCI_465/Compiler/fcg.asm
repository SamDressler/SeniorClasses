#Beginning of data section
.data
PL1 : .asciiz "Hello Dr. Reza\n"
PL2 : .asciiz "Enter your favorite integer: \n"
PL3 : .asciiz "\nGood Bye and \n"
PL4 : .asciiz "Happy Holidays!\n"
arr1 : .word 0
arr2 : .word 0
arr3 : .word 0
arr4 : .word 0
arr5 : .word 0
y : .word 0
x : .word 0
result : .word 0
c : .byte 0
sum : .word 0
tmp0 : .word 0
tmp1 : .word 0
tmp2 : .word 0
tmp3 : .word 0
#Beginning of Code section
.text
start:
li $v0, 4
la $a0, PL1
syscall

lw $s0, x
li $t0, 9
sw $t0, x

lw $s1, y
li $t1, 6
sw $t1, y

lb $s2, c
li $t2, 'L'
sb $t2, c

lw $t3, x
lw $t4, y 
bgt $t3, $t4, L0

j L1

L0:
lw $t0, x
lw $t1, y
add $t2, $t0, $t1
sw $t2, tmp0

lw $t3, tmp0
li $t4, 6
mult $t3, $t4
mflo $t5
sw $t5, tmp1

lw $s0, result
lw $t6, tmp2
sw $t6, result

j L2

L1:
lw $t7, x
li $t8, 5
mult $t7, $t8
mflo $t9
sw $t9, tmp2

lw $s1, result
lw $t0, tmp2
sw $t0, result

j L2

L2:
li $v0, 1
lw $t0, result
move $a0, $t0
syscall

lw $s0, arr1
li $t1, 10
sw $t1, arr1

lw $s1, arr2
li $t2, 5
sw $t2, arr2

lw $t3, arr1
lw $t4, arr2
add $t5, $t3, $t4
sw $t5, tmp3

lw $s2, sum
lw $t6, tmp3
sw $t6, sum

li $v0, 4
la $a0, PL2
syscall

li $v0, 5
syscall 

sw $v0, x

li $v0, 1
lw $t7, x
move $a0, $t7
syscall

li $v0, 4
la $a0, PL3
syscall

li $v0, 4
la $a0, PL4
syscall

li $v0, 10
syscall

