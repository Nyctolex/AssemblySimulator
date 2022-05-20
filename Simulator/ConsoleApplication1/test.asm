	add $t1, $zero, $imm, -1				# $t1 = 1
	add $t2, $zero, $imm, 2				# 
	add $t0, $t1, $t2, 0 
	sub $t0, $t0, $imm, 2 
	beq $imm, $t0, $zero, L1				# jump to L1 if x > 1
	bne $imm, $t0, $zero, L2
	halt $zero, $zero, $zero, 0 
L1:
	add $t0, $zero, $imm , 6 #
	halt $zero, $zero, $zero, 0 
L2:
	add $t0, $zero, $imm , 10 #
	halt $zero, $zero, $zero, 0 
