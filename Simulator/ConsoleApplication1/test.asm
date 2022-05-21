
lw $a0, $imm, $zero, 100 #  $a0 = n
lw $a1, $imm, $zero, 101 #  $a1 = k
jal $ra, $imm, $zero, BINOM #  goto BINOM
sw $v0, $imm, $zero, 102
halt $zero, $imm, $zero, 0

BINOM:
beq $imm, $a1, $zero, RETURN1 #  if k==0 return 1;
beq $imm, $a0, $a1, RETURN1 #  if n==k return 1;

add $sp, $imm, $sp, -4 #  allocating memory in the stack
sw $ra, $zero, $sp, 0 #  pushing $ra to stack
add $sp, $imm, $sp, -4 #  allocating memory in the stack
sw $a0, $zero, $sp, 0 #  pushing $a0 to stack
add $sp, $imm, $sp, -4 #  allocating memory in the stack
sw $a1, $zero, $sp, 0 #  pushing $a1 to stack
sub $a0, $a0, $imm, 1 #  $a0 = n-1
sub $a1, $a1, $imm, 1 #  $a1 = k-1
jal $ra, $imm, $zero, BINOM
add $a2, $v0, $zero, 0 #  $a2 = binom(n-1, k-1)
lw $a1, $zero, $sp, 0 #  poping $a1 from stack, $a0 = n again
add $sp, $imm, $sp, 4 #  freeing memory in stack
lw $a0, $zero, $sp, 0 #  poping $a0 from stack, $a1 = k again
add $sp, $imm, $sp, 4 #  freeing memory in stack

add $sp, $imm, $sp, -4 #  allocating memory in the stack
sw $a0, $zero, $sp, 0 #  pushing $a0 to stack ,$a1 = k again
add $sp, $imm, $sp, -4 #  allocating memory in the stack
sw $a1, $zero, $sp, 0 #  pushing $a1 to stack ,$a1 = k again

sub $a0, $a0, $imm, 1 #  $a0 = n-1
jal $ra, $imm, $zero, BINOM
add $v0, $v0, $a2, 0 #  $v0 = binom(n-1, k-1)+binom(n-1, k)
lw $a1, $zero, $sp, 0 #  poping $a1 from stack, $v0 = binom(n-1, k-1)+binom(n-1, k)
add $sp, $imm, $sp, 4 #  freeing memory in stack
lw $a0, $zero, $sp, 0 #  poping $a0 from stack, $v0 = binom(n-1, k-1)+binom(n-1, k)
add $sp, $imm, $sp, 4 #  freeing memory in stack
lw $ra, $zero, $sp, 0 #  poping $ra from stack, $v0 = binom(n-1, k-1)+binom(n-1, k)
add $sp, $imm, $sp, 4 #  freeing memory in stack
jal $zero, $ra, $zero, 0 #  return

RETURN1:
 add $v0, $zero, $imm, 1
 jal $zero, $ra, $zero, 0

.word 100 2
.word 101 1



