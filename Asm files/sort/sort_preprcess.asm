set a0 0x100 #a0 = baseadress
call SORT
halt $zero, $zero, $zero, 0
SORT:
    push s0
    push s1
    push s2 
    mov s2 a0 #making sure the base adress is saved
    call IS_SORTED
    beq ra, v0, imm, 1 #if the array is sorted, return.
    set t1 15 #t1 = length - 1 !!!!!!!!!!!!!!!!!
WHILE_NOT_SORTED:
    mov a0 s2
    set t0 0 #t0 = counter = 0
FOR:
        add t2, a0, t0,0 # t2 = baseAdress+index
        lw s0, t2, zero, 0 #s0=MEM[baseAdress+index]
        lw s1, t2, imm, 1 #s1=MEM[baseAdress+index+1]
        ble imm, s0, s1,  FOR_CONDITION #if s0 <= s1 there is no need to swap
NOT_SORTED:
        push a0
        mov a0 t2 # a0 = baseAdress+index
        call SWAP
        pop a0
FOR_CONDITION:
        add t0, t0, imm, 1 #counter++
        blt imm, t0, t1, FOR # if counter < length, countue the for loop
WHILE_CONDITION:
    mov a0 s2
    call IS_SORTED
    set t2 1 #set t2 to True
    bne imm, v0, t2, WHILE_NOT_SORTED
    ret
IS_SORTED:
push s0
push s1
set t0 0 #t0 = counter = 0
set v0 1 # return value is true by default
set t1 15 #t1 = length - 1 #t1 = length - 1 !!!!!!!!!!!!!!!!!
LOOP:
        add t2, a0, t0,0 # t2 = baseAdress+index
        lw s0, t2, zero, 0 #s0=MEM[baseAdress+index]
        lw s1, t2, imm, 1 #s1=MEM[baseAdress+index+1]
        bgt imm, s0, s1,  RETURN0
        add t0, t0, imm, 1 #counter++
        bge imm, t0, t1, RETURN1
        jmp LOOP
RETURN1:
    pop s1
    pop s0
    ret
RETURN0:
    set v0 0
    pop s1
    pop s0
    ret
SWAP:
    push s0
    push s1
    lw s0, a0, zero, 0 #s0=MEM[baseAdress]
    lw s1, a0, imm, 1 #s1=MEM[baseAdress+1]
    sw s0, a0, imm, 1 #MEM[baseAdress+1] = s1 = old MEM[baseAdress]
    sw s1, a0, zero, 0 #MEM[baseAdress] = s1 = old MEM[baseAdress+1]
    pop s1
    pop s0
    ret
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