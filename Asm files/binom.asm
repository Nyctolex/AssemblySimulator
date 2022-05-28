lw $a0, $imm, $zero, 0x100             #set $a0 to n
lw $a1, $imm, $zero, 0x101             #set $a1 to k
add $sp, $imm, $sp, -4                 #allocating memory in the stack
sw $ra, $zero, $sp, 0                  #pushing $ra to stack
jal $ra, $imm, $zero, BINOM            #calling function BINOM
lw $ra, $zero, $sp, 0                  #poping $ra from stack
add $sp, $imm, $sp, 4                  #freeing memory in stack # call binom
sw $v0, $imm, $zero, 0x102             #save output value to 0x102
halt $zero, $zero, $zero, 0
BINOM:
    beq $imm, $a1, $zero, RETUEN1      #if k== 0 -> return 1
    beq $imm, $a0, $a1, RETUEN1        #if k== n -> return 1
    add $sp, $imm, $sp, -4             #allocating memory in the stack
    sw $s0, $zero, $sp, 0              #pushing $s0 to stack ,
    add $sp, $imm, $sp, -4             #allocating memory in the stack
    sw $s1, $zero, $sp, 0              #pushing $s1 to stack ,
    add $s0, $a0, $zero, 0             #$s0 = n
    add $s1, $a1, $zero, 0             #$s1 = k
    sub $s0, $s0, $imm, 1              #$s0 = n-1
    add $a0, $s0, $zero, 0             #$a0 = $s0 = n-1
    add $sp, $imm, $sp, -4             #allocating memory in the stack
    sw $ra, $zero, $sp, 0              #pushing $ra to stack
    jal $ra, $imm, $zero, BINOM        #calling function BINOM
    lw $ra, $zero, $sp, 0              #poping $ra from stack
    add $sp, $imm, $sp, 4              #freeing memory in stack
    add $sp, $imm, $sp, -4             #allocating memory in the stack
    sw $v0, $zero, $sp, 0              #pushing $v0 to stack ,save the value of binom(n-1, k)
    add $a0, $s0, $zero, 0            #$a0 = n-1
    sub $s1, $s1, $imm, 1              #$s1=k-1
    add $a1, $s1, $zero, 0             #$a1 = $s1 = k-1
    add $sp, $imm, $sp, -4             #allocating memory in the stack
    sw $ra, $zero, $sp, 0              #pushing $ra to stack
    jal $ra, $imm, $zero, BINOM        #calling function BINOM
    lw $ra, $zero, $sp, 0              #poping $ra from stack
    add $sp, $imm, $sp, 4              #freeing memory in stack
    lw $s0, $zero, $sp, 0              #poping $s0 from stack, $s0 = binom(n-1, k)
    add $sp, $imm, $sp, 4              #freeing memory in stack
    add $v0, $v0, $s0, 0               #$v0 = binom(n-1, k) =binom(n-1, k-1)
    lw $s1, $zero, $sp, 0              #poping $s1 from stack, 
    add $sp, $imm, $sp, 4              #freeing memory in stack
    lw $s0, $zero, $sp, 0              #poping $s0 from stack, 
    add $sp, $imm, $sp, 4              #freeing memory in stack
    jal $zero, $ra, $zero, 0           #return
RETUEN1:
    add $v0, $imm, $zero, 1 
    jal $zero, $ra, $zero, 0           #return
.word 0x100 8
.word 0x101 5

