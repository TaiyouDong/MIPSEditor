main:
	addi	$a0, $zero, 0x01
	addi	$a1, $zero, 0x01
	addi	$a2, $zero, 0x03
	jal		multiply
	nop	
	move $s0, $v0
	move	$s1, $v1
	li $v0, 10		# exits program
	syscall

	exit:
	lw		$s0, 0($sp)		
	lw		$s1, 4($sp)		
	lw		$s2,	8($sp)
	jr $ra
	
# calculate x1 * x2
multiply:
	addi $sp, $sp, -12
	sw		$s0, 0($sp)		#被乘数高位
	sw		$s1, 4($sp)		#被乘数低位
	sw		$s2,	8($sp)		#乘数
	
	move $v0, $zero
	move $v1, $zero
	move $s0, $a0			
	move $s1, $a1			
	move $s2, $a2			#s2 = x2
	move $t0, $zero		#i = 0
loop:
	andi $t1, $s2, 1
	beq	$t1, $zero, continue
	addu $v1, $v1, $s1
	sltu $t2, $v1, $s1
	addu $v0, $v0, $t2
	addu $v0, $v0, $s0
	
continue:
	srl $s2, $s2, 1
	sll $s0, $s0, 1
	srl $t3, $s1, 31
	or	  $s0, $s0, $t3
	sll $s1, $s1, 1
	addi $t0, $t0, 1
	slti $t4, $t0, 32		#t4 == t0 < 32
	beq	$t4, $zero, exit
	j loop





























	