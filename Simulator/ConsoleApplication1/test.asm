add $a0, $imm, $zero, 4616
add $sp, $imm, $sp, -4                   #allocating memory in the stack
sw $ra, $zero, $sp, 0                    #pushing $ra to stack
jal $ra, $imm, $zero, IN_X_RANGE         #calling function IN_X_RANGE
lw $ra, $zero, $sp, 0                    #poping $ra from stack
add $sp, $imm, $sp, 4                    #freeing memory in stack
halt $zero, $zero, $zero, 0
IN_X_RANGE:
    add $sp, $imm, $sp, -4               #allocating memory in the stack
    sw $s0, $zero, $sp, 0                #pushing $s0 to stack ,
    add $sp, $imm, $sp, -4               #allocating memory in the stack
    sw $s1, $zero, $sp, 0                #pushing $s1 to stack ,
    add $sp, $imm, $sp, -4               #allocating memory in the stack
    sw $s2, $zero, $sp, 0                #pushing $s2 to stack ,
    add $v0, $imm, $zero, 0              #set $v0 to false
CALC_AK:
    lw $s0, $imm, $zero, 0x100           #$s0 = Point A
    sra $s0, $s0, $imm, 8                #$s0 = Ay
    add $s1, $a0, $zero, 0               #set $s1 to the given Point
    sra $s1, $s1, $imm, 8                #$s1 = Py
    sub $s0, $s1, $s0, 0                 #$s0 = Py-Ay = Ak
    add $sp, $imm, $sp, -4               #allocating memory in the stack
    sw $s0, $zero, $sp, 0                #pushing $s0 to stack ,save AK in stack
CALK_BC:
    lw $s0, $imm, $zero, 0x102           #$s0 = point c
    lw $s1, $imm, $zero, 0x101           #$s1 = point B
    and $s0, $s0, $imm, 0x00FF           #$s0 = Cx
    and $s1, $s1, $imm, 0x00FF           #$s1 = Bx
    sub $s1, $s0, $s1, 0                 #$s1=Cx-Bx = BC
    lw $s0, $zero, $sp, 0                #poping $s0 from stack, $s0 = Py-Ay = Ak
    add $sp, $imm, $sp, 4                #freeing memory in stack
SAVE_AK_MUL_BC:
    mul $s0, $s0, $s1, 0                 #$s0 = Ak*BC
    add $sp, $imm, $sp, -4               #allocating memory in the stack
    sw $s0, $zero, $sp, 0                #pushing $s0 to stack ,save $s0 = Ak*BC
CALC_KP:
    lw $s0, $imm, $zero, 0x100           #$s0 = Point A
    add $s1, $a0, $zero, 0               #set $s1 to the given Point
    and $s0, $s0, $imm, 0x00FF           #$s0 = Ax
    and $s1, $s1, $imm, 0x00FF           #$s1 = Px
    sub $s0, $s1, $s0, 0                 #$s0=Px-Ax = KP
    add $sp, $imm, $sp, -4               #allocating memory in the stack
    sw $s0, $zero, $sp, 0                #pushing $s0 to stack ,save KP
CALC_AB:
    lw $s0, $imm, $zero, 0x100           #$s0 = Point A
    lw $s1, $imm, $zero, 0x101           #$s1 = Point B
    sra $s0, $s0, $imm, 8                #$s0 = Ay
    sra $s1, $s1, $imm, 8                #$s1 = By
    sub $s0, $s1, $s0, 0                 #$s0 = By-Ay = AB 
SAVE_KP_MUL_AB:
    lw $s1, $zero, $sp, 0                #poping $s1 from stack, $s1 = KP
    add $sp, $imm, $sp, 4                #freeing memory in stack
    mul $s0, $s0, $s1, 0                 #$s0 = AB*KP
LOAD_AK_MUL_BC:
    lw $s1, $zero, $sp, 0                #poping $s1 from stack, $s1 = Ak*BC
    add $sp, $imm, $sp, 4                #freeing memory in stack
    bge $imm, $s1, $s0, RETURN_TRUE      #if Ak*BC>= AB*KP return True
RETUN_FALSE:
    lw $s0, $zero, $sp, 0                #poping $s0 from stack, 
    add $sp, $imm, $sp, 4                #freeing memory in stack
    lw $s1, $zero, $sp, 0                #poping $s1 from stack, 
    add $sp, $imm, $sp, 4                #freeing memory in stack
    lw $s2, $zero, $sp, 0                #poping $s2 from stack, 
    add $sp, $imm, $sp, 4                #freeing memory in stack
    jal $zero, $ra, $zero, 0             #return False
RETURN_TRUE:
    lw $s0, $zero, $sp, 0                #poping $s0 from stack, 
    add $sp, $imm, $sp, 4                #freeing memory in stack
    lw $s1, $zero, $sp, 0                #poping $s1 from stack, 
    add $sp, $imm, $sp, 4                #freeing memory in stack
    lw $s2, $zero, $sp, 0                #poping $s2 from stack, 
    add $sp, $imm, $sp, 4                #freeing memory in stack
    add $v0, $imm, $zero, 1              #set $v0 to true
    jal $zero, $ra, $zero, 0             #return true
.word 0x100 4102
.word 0x101 5126
.word 0x102 65546





















