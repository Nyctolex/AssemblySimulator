lw a0, imm, zero, 0x100 #set a0 to n
lw a1, imm, zero, 0x101 #set a1 to k
call BINOM #call binom
sw v0, imm, zero, 0x102 #save output value to 0x102
halt $zero, $zero, $zero, 0
BINOM:
    beq imm, a1, zero, RETURN1 #if k== 0 -> return 1
    beq imm, a0, a1, RETURN1 #if k== n -> return 1
    push s0
    push s1
    mov s0 a0 #s0 = n
    mov s1 a1 #s1 = k
    sub s0, s0, imm, 1 #s0 = n-1
    mov a0 s0 #a0 = s0 = n-1
    call BINOM
    push v0 #save the value of binom(n-1, k)
    mov a0, s0 #a0 = n-1
    sub s1, s1, imm, 1 #s1=k-1
    mov a1 s1 #a1 = s1 = k-1
    call BINOM
    pop s0 #s0 = binom(n-1, k)
    add v0, v0, s0, 0 #v0 = binom(n-1, k) =binom(n-1, k-1)
    pop s1
    pop s0
    ret #return
RETURN1:
    set v0 1
    ret #return
.word 0x100 2
.word 0x101 1