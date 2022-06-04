add $t2, $zero, $imm, 1          #$t2 = 1
sll $sp, $t2, $imm, 11           #set $sp = 1 << 11 = 2048
add $a0, $imm, $zero, 0x100                      #$a0 = baseadress
add $sp, $imm, $sp, -4                         #allocating memory in the stack
sw $ra, $zero, $sp, 0                          #pushing $ra to stack
jal $ra, $imm, $zero, SORT                     #calling function SORT
lw $ra, $zero, $sp, 0                          #poping $ra from stack
add $sp, $imm, $sp, 4                          #freeing memory in stack
halt $zero, $zero, $zero, 0                    #exit program
SORT:
    add $sp, $imm, $sp, -4                     #allocating memory in the stack
    sw $s0, $zero, $sp, 0                      #pushing $s0 to stack 
    add $sp, $imm, $sp, -4                     #allocating memory in the stack
    sw $s1, $zero, $sp, 0                      #pushing $s1 to stack 
    add $sp, $imm, $sp, -4                     #allocating memory in the stack
    sw $s2, $zero, $sp, 0                      #pushing $s2 to stack 
    add $s2, $a0, $zero, 0                     #making sure the base adress is saved
    add $sp, $imm, $sp, -4                     #allocating memory in the stack
    sw $ra, $zero, $sp, 0                      #pushing $ra to stack
    jal $ra, $imm, $zero, IS_SORTED            #calling function IS_SORTED
    lw $ra, $zero, $sp, 0                      #poping $ra from stack
    add $sp, $imm, $sp, 4                      #freeing memory in stack
    beq $ra, $v0, $imm, 1                      #if the array is sorted, return.
    add $t1, $imm, $zero, 15                   #$t1 = length - 1
WHILE_NOT_SORTED:
    add $a0, $s2, $zero, 0                     #a0 = s2 = base_adress
    add $t0, $imm, $zero, 0                    #$t0 = counter = 0
FOR:
        add $t2, $a0, $t0,0                    #$t2 = baseAdress+index
        lw $s0, $t2, $zero, 0                  #$s0=MEM[baseAdress+index]
        lw $s1, $t2, $imm, 1                   #$s1=MEM[baseAdress+index+1]
        ble $imm, $s0, $s1, FOR_CONDITION      #if $s0 <= $s1 there is no need to swap
NOT_SORTED:
        add $sp, $imm, $sp, -4                 #allocating memory in the stack
        sw $a0, $zero, $sp, 0                  #pushing $a0 to stack ,
        add $a0, $t2, $zero, 0                 #$a0 = baseAdress+index
        add $sp, $imm, $sp, -4                 #allocating memory in the stack
        sw $ra, $zero, $sp, 0                  #pushing $ra to stack
        jal $ra, $imm, $zero, SWAP             #calling function SWAP
        lw $ra, $zero, $sp, 0                  #poping $ra from stack
        add $sp, $imm, $sp, 4                  #freeing memory in stack
        lw $a0, $zero, $sp, 0                  #poping $a0 from stack, 
        add $sp, $imm, $sp, 4                  #freeing memory in stack
FOR_CONDITION:
        add $t0, $t0, $imm, 1                  #counter++
        blt $imm, $t0, $t1, FOR                #if counter < length, countue the for loop
WHILE_CONDITION:
    add $a0, $s2, $zero, 0 
    add $sp, $imm, $sp, -4                     #allocating memory in the stack
    sw $ra, $zero, $sp, 0                      #pushing $ra to stack
    jal $ra, $imm, $zero, IS_SORTED            #calling function IS_SORTED
    lw $ra, $zero, $sp, 0                      #poping $ra from stack
    add $sp, $imm, $sp, 4                      #freeing memory in stack
    add $t2, $imm, $zero, 1                    #set $t2 to True
    bne $imm, $v0, $t2, WHILE_NOT_SORTED       #if note sorted, continure loop
    jal $zero, $ra, $zero, 0                   #exit the function and retuen back
IS_SORTED:
add $sp, $imm, $sp, -4                         #allocating memory in the stack
sw $s0, $zero, $sp, 0                          #pushing $s0 to stack 
add $sp, $imm, $sp, -4                         #allocating memory in the stack
sw $s1, $zero, $sp, 0                          #pushing $s1 to stack 
add $t0, $imm, $zero, 0                        #$t0 = counter = 0
add $v0, $imm, $zero, 1                        #return value is true by default
add $t1, $imm, $zero, 15                       #$t1 = length - 1 #$t1 = length - 1
LOOP:
        add $t2, $a0, $t0,0                    #$t2 = baseAdress+index
        lw $s0, $t2, $zero, 0                  #$s0=MEM[baseAdress+index]
        lw $s1, $t2, $imm, 1                   #$s1=MEM[baseAdress+index+1]
        bgt $imm, $s0, $s1, RETURN0            # if MEM[baseAdress+index+1]> MEM[baseAdress+index] return false
        add $t0, $t0, $imm, 1                  #counter++
        bge $imm, $t0, $t1, RETURN1            #if we got to the end of the array, return True
        jal $zero, $imm, $zero, LOOP           #Otherwise, continue the loop
RETURN1:
    lw $s1, $zero, $sp, 0                      #poping $s1 from stack, 
    add $sp, $imm, $sp, 4                      #freeing memory in stack
    lw $s0, $zero, $sp, 0                      #poping $s0 from stack, 
    add $sp, $imm, $sp, 4                      #freeing memory in stack
    jal $zero, $ra, $zero, 0                   #return
RETURN0:
    add $v0, $imm, $zero, 0 
    lw $s1, $zero, $sp, 0                      #poping $s1 from stack, 
    add $sp, $imm, $sp, 4                      #freeing memory in stack
    lw $s0, $zero, $sp, 0                      #poping $s0 from stack, 
    add $sp, $imm, $sp, 4                      #freeing memory in stack
    jal $zero, $ra, $zero, 0                   #return
SWAP:
    add $sp, $imm, $sp, -4                     #allocating memory in the stack
    sw $s0, $zero, $sp, 0                      #pushing $s0 to stack ,
    add $sp, $imm, $sp, -4                     #allocating memory in the stack
    sw $s1, $zero, $sp, 0                      #pushing $s1 to stack ,
    lw $s0, $a0, $zero, 0                      #$s0=MEM[baseAdress]
    lw $s1, $a0, $imm, 1                       #$s1=MEM[baseAdress+1]
    sw $s0, $a0, $imm, 1                       #MEM[baseAdress+1] = $s1 = old MEM[baseAdress]
    sw $s1, $a0, $zero, 0                      #MEM[baseAdress] = $s1 = old MEM[baseAdress+1]
    lw $s1, $zero, $sp, 0                      #poping $s1 from stack, 
    add $sp, $imm, $sp, 4                      #freeing memory in stack
    lw $s0, $zero, $sp, 0                      #poping $s0 from stack, 
    add $sp, $imm, $sp, 4                      #freeing memory in stack
    jal $zero, $ra, $zero, 0                   #return
.word 0x100 3
.word 0x101 4
.word 0x102 2
.word 0x103 5
.word 0x104 1
.word 0x105 6
.word 0x106 8
.word 0x107 9
.word 0x108 7
.word 0x109 10
.word 0x10A 16
.word 0x10B 15
.word 0x10C 14
.word 0x10D 11
.word 0x10E 13
.word 0x10F 12