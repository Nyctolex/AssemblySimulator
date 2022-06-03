add $sp, $imm, $sp, -4                        #allocating memory in the stack
sw $ra, $zero, $sp, 0                         #pushing $ra to stack
jal $ra, $imm, $zero, CALC_AB                 #calling function CALC_AB
lw $ra, $zero, $sp, 0                         #poping $ra from stack
add $sp, $imm, $sp, 4                         #freeing memory in stack # mem[0x105] = AB
add $sp, $imm, $sp, -4                        #allocating memory in the stack
sw $ra, $zero, $sp, 0                         #pushing $ra to stack
jal $ra, $imm, $zero, CALK_BC                 #calling function CALK_BC
lw $ra, $zero, $sp, 0                         #poping $ra from stack
add $sp, $imm, $sp, 4                         #freeing memory in stack # mem[0x106] = BC
add $sp, $imm, $sp, -4                        #allocating memory in the stack
sw $ra, $zero, $sp, 0                         #pushing $ra to stack
jal $ra, $imm, $zero, PRINT_TRINGLE           #calling function PRINT_TRINGLE
lw $ra, $zero, $sp, 0                         #poping $ra from stack
add $sp, $imm, $sp, 4                         #freeing memory in stack
halt $zero, $zero, $zero, 0
PRINT_TRINGLE:
    add $sp, $imm, $sp, -4                    #allocating memory in the stack
    sw $s0, $zero, $sp, 0                     #pushing $s0 to stack ,
    add $sp, $imm, $sp, -4                    #allocating memory in the stack
    sw $s1, $zero, $sp, 0                     #pushing $s1 to stack ,
    add $sp, $imm, $sp, -4                    #allocating memory in the stack
    sw $s2, $zero, $sp, 0                     #pushing $s2 to stack ,
    lw $s0, $imm, $zero, 0x100                #$s0 = point A
    lw $s1, $imm, $zero, 0x101                #$s1 = point B
    sra $s0, $s0, $imm, 8                     #$s0 = Ay
    sra $s1, $s1, $imm, 8                     #$s1 = By
    lw $s2, $imm, $zero, 0x100                #$s2 = point A
    and $s2, $s2, $imm, 0x00FF                #$s2 = Ax -> $s2 is the x counter
    sw $s2, $imm, $zero, 0x108                #save in mem[0x108] the max value of x counter from previouse line
LOOP_Y:
    lw $s2, $imm, $zero, 0x100                #$s2 = point A
    and $s2, $s2, $imm, 0x00FF                #$s2 = Ax -> $s2 is the x counter
    add $sp, $imm, $sp, -4                    #allocating memory in the stack
    sw $ra, $zero, $sp, 0                     #pushing $ra to stack
    jal $ra, $imm, $zero, CALC_AK_MUL_BC      #calling function CALC_AK_MUL_BC
    lw $ra, $zero, $sp, 0                     #poping $ra from stack
    add $sp, $imm, $sp, 4                     #freeing memory in stack
LOOP_X:
    add $a0, $s0, $zero, 0                    #$a0 = y counter
    sll $a0, $a0, $imm, 8                     #$a0<<8
    add $a0, $a0, $s2, 0                      #$a0+= x counter. $a0= current point
    lw $t0, $imm, $zero, 0x108                #load from mem[0x108] the max value of x counter from previouse line
    ble $imm, $s2, $t0, PRINT_DOT             #if current x <= max know x -> print the pixel
    add $sp, $imm, $sp, -4                    #allocating memory in the stack
    sw $a0, $zero, $sp, 0                     #pushing $a0 to stack ,save current point
    add $sp, $imm, $sp, -4                    #allocating memory in the stack
    sw $ra, $zero, $sp, 0                     #pushing $ra to stack
    jal $ra, $imm, $zero, IN_X_RANGE          #calling function IN_X_RANGE
    lw $ra, $zero, $sp, 0                     #poping $ra from stack
    add $sp, $imm, $sp, 4                     #freeing memory in stack
    lw $a0, $zero, $sp, 0                     #poping $a0 from stack, $a0 = current point
    add $sp, $imm, $sp, 4                     #freeing memory in stack
    beq $imm, $v0, $zero, NEXT_ROW            #if we are out of the tringle, go to the next row
    sw $s2, $imm, $zero, 0x108                #update in mem[0x108] the max value of x counter from previouse line
PRINT_DOT:
    add $t0, $imm, $zero, 255
    out $t0, $imm, $zero, 21                  #set monitordata to 255
    out $a0, $imm, $zero, 20                  #set monitoraddr to $a0
    add $t0, $imm, $zero, 1
    out $t0, $imm, $zero, 22                  #set monitorcmd to 1
    add $s2, $s2, $imm, 1                     #inc x counter
    add $t0, $imm, $zero, 255
    bgt $imm, $s2, $t0, NEXT_ROW              #if the x value is more than 255
    jal $zero, $imm, $zero, LOOP_X            #jump to LOOP_X
NEXT_ROW:
    bgt $imm, $s0, $s1, RETURN                #if we are out of the y $range values, return
    add $s0, $s0, $imm, 1                     #$s0+=1
    add $t0, $imm, $zero, 255
    bgt $imm, $s0, $t0, RETURN                #if we are out of the y $range values, return
    jal $zero, $imm, $zero, LOOP_Y            #jump to the LOOP_Y
RETURN:
    lw $s2, $zero, $sp, 0                     #poping $s2 from stack, 
    add $sp, $imm, $sp, 4                     #freeing memory in stack
    lw $s1, $zero, $sp, 0                     #poping $s1 from stack, 
    add $sp, $imm, $sp, 4                     #freeing memory in stack
    lw $s0, $zero, $sp, 0                     #poping $s0 from stack, 
    add $sp, $imm, $sp, 4                     #freeing memory in stack
    jal $zero, $ra, $zero, 0                  #return
IN_X_RANGE:
    add $v0, $imm, $zero, 0                   #set $v0 to false
CALC_KP:
    lw $t0, $imm, $zero, 0x100                #$t0 = Point A
    add $t1, $a0, $zero, 0                    #set $t1 to the given Point
    and $t0, $t0, $imm, 0x00FF                #$t0 = Ax
    and $t1, $t1, $imm, 0x00FF                #$t1 = Px
    sub $t0, $t1, $t0, 0                      #$t0=Px-Ax = KP
SAVE_KP_MUL_AB:
    lw $t1, $imm, $zero, 0x105                #$t1=AB
    mul $t0, $t0, $t1, 0                      #$t0 = AB*KP
LOAD_AK_MUL_BC:
    lw $t1, $imm, $zero, 0x107                #$t1 = mem[105] = Ak*BC
    bge $imm, $t1, $t0, RETURN_TRUE           #if Ak*BC>= AB*KP return True
RETUN_FALSE:
    jal $zero, $ra, $zero, 0                  #return False
RETURN_TRUE:
    add $v0, $imm, $zero, 1                   #set $v0 to true
    jal $zero, $ra, $zero, 0                  #return true
CALK_BC:
    lw $t0, $imm, $zero, 0x102                #$t0 = point c
    lw $t1, $imm, $zero, 0x101                #$t1 = point B
    and $t0, $t0, $imm, 0x00FF                #$t0 = Cx
    and $t1, $t1, $imm, 0x00FF                #$t1 = Bx
    sub $v0, $t0, $t1, 0                      #$v0=Cx-Bx = BC
    sw $v0, $imm, $zero, 0x106                #mem[105] = BC
    jal $zero, $ra, $zero, 0                  #return
CALC_AB:
    lw $t0, $imm, $zero, 0x100                #$t0 = Point A
    lw $t1, $imm, $zero, 0x101                #$t1 = Point B
    sra $t0, $t0, $imm, 8                     #$t0 = Ay
    sra $t1, $t1, $imm, 8                     #$t1 = By
    sub $v0, $t1, $t0, 0                      #$v0 = By-Ay = AB 
    sw $v0, $imm, $zero, 0x105                #mem[105] = AB
    jal $zero, $ra, $zero, 0                  #return
CALC_AK_MUL_BC:
CALC_AK:
    lw $t0, $imm, $zero, 0x100                #$t0 = Point A
    sra $t0, $t0, $imm, 8                     #$t0 = Ay
    add $t1, $a0, $zero, 0                    #set $t1 to the given Point
    sra $t1, $t1, $imm, 8                     #$t1 = Py
    sub $t0, $t1, $t0, 0                      #$t0 = Py-Ay = Ak
    lw $t1, $imm, $zero, 0x106                #$t1=BC
SAVE_AK_MUL_BC:
    mul $v0, $t0, $t1, 0                      #$v0 = Ak*BC
    sw $v0, $imm, $zero, 0x107                #mem[105] = Ak*BC
    jal $zero, $ra, $zero, 0                  #return
.word 0x100 1797
.word 0x101 5125
.word 0x102 5130