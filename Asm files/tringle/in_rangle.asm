set a0 4870
call IN_X_RANGE
halt $zero, $zero, $zero, 0
IN_X_RANGE:
    push s0
    push s1
    push s2
    set v0 0 #set v0 to false
CALC_AK:
    lw s0, imm, zero, 0x100 #s0 = Point A
    sra s0, s0, imm, 8 #s0 = Ay
    mov s1 a0 #set s1 to the given Point
    sra s1, s1, imm, 8 #s1 = Py
    sub s0, s1, s0, 0 #s0 = Py-Ay = Ak
    push s0 #save AK in stack
CALK_BC:
    lw s0, imm, zero, 0x102 #s0 = point c
    lw s1, imm, zero, 0x101 #s1 = point B
    and s0, s0, imm, 0x00FF #s0 = Cx
    and s1, s1, imm, 0x00FF #s1 = Bx
    sub s1, s0, s1, 0 #s1=Cx-Bx = BC
    pop s0 #s0 = Py-Ay = Ak
SAVE_AK_MUL_BC:
    mul s0, s0, s1, 0 #s0 = Ak*BC
    push s0 #save s0 = Ak*BC
CALC_KP:
    lw s0, imm, zero, 0x100 #s0 = Point A
    mov s1 a0 #set s1 to the given Point
    and s0, s0, imm, 0x00FF #s0 = Ax
    and s1, s1, imm, 0x00FF #s1 = Px
    sub s0, s1, s0, 0 #s0=Px-Ax = KP
CALC_AB:
    lw s0, imm, zero, 0x100 #s0 = Point A
    lw s1, imm, zero, 0x101 #s1 = Point B
    sra s0, s0, imm, 8 #s0 = Ay
    sra s1, s1, imm, 8 #s1 = By
    sub s0, s1, s0, 0 #s0 = By-Ay = AB 
    push s0 #save KP
SAVE_KP_MUL_AB:
    pop s1 #s1 = KP
    mul s0, s0, s1, 0 #s0 = AB*KP
LOAD_AK_MUL_BC:
    pop s1 #s1 = Ak*BC
    bge imm, s1, s0, RETURN_TRUE #if Ak*BC>= AB*KP return True
RETUN_FALSE:
    pop s2
    pop s1
    pop s0
    ret #return False
RETURN_TRUE:
    pop s2
    pop s1
    pop s0
    set v0 1 #set v0 to true
    ret #return true
.word 0x100 1797
.word 0x101 5125
.word 0x102 5130