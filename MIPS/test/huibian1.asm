#BaseAddre:00001000;
main:
add $s0, $s1, $zero;
addi $s2, $s3, 32767;
sub $s4, $s5, $s6;
xor $s7, $t0, $t1;
xori $t2, $t3, -1;
nor $t5, $t6, $t7;
or $t8, $t9, $v0;
ori $v1, $a0, 0;
Step1:
lui $t4, 128;
slt $at, $a1, $zero;
slti $gp, $sp, 248;
j Begin;
lw $s0, -20($t0);
#lw $s0, $t0;
sw $s1, 20($t1);
#sw $s1, $t1;
sll $s2, $s3, 15;
srl $s4, $s5, 7;
beq $s6, $s7, 10;
beq $s6, $s7, main;
bne $t6, $t7, -10;
bne $t6, $t7, End;
End:
jal Step1;

DataAddre:00002000;

buffer: resb 8;
dw 1024, 0x12ef, 'ab';
wordvar: resw 3;
dd 2147483647, 0x1234cdef, 'abcd';
doublevar: resd 2;

DataAddre:00003000;
