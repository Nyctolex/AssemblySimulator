set a0 100 #a0 = baseadress
call IS_SORTED
halt $zero, $zero, $zero, 0
IS_SORTED:
push s0
push s1
set t0 0 #t0 = counter = 0
set v0 1 # return value is true by default
set t1 2 #t1 = length - 1
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
.word 100 10
.word 101 12
.word 102 14