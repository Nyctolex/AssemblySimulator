set a0 100 #a0 = baseadress
call SWAP
halt $zero, $zero, $zero, 0
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
.word 100 10
.word 101 12
.word 102 14