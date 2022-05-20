	add $t0, $zero, $imm, 42
	add $t1, $zero, $imm, 55
	sw $t0, $zero, $t1, 0
	add $zero, $zero, $zero, 0
	lw $t2, $zero, $t1, 0
	halt $zero, $zero, $zero, 0 
	LABEL1: