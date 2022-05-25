call PRINT_TRINGLE
halt $zero, $zero, $zero, 0
PRINT_TRINGLE:
    push s0
    push s1
    push s2
    lw s0, imm, zero, 0x100 #s0 = point A
    lw s1, imm, zero, 0x101 #s1 = point B
    sra s0, s0, imm, 8 #s0 = Ay
    sra s1, s1, imm, 8 #s1 = By
LOOP_Y:
    lw s2, imm, zero, 0x100 #s2 = point A
    and s2, s2, imm, 0x00FF #s2 = Ax -> s2 is the x counter
LOOP_X:
    mov a0 s0 #a0 = y counter
    sll a0, a0, imm, 8 #a0<<8
    add a0, a0, s2, 0 #a0+= x counter. a0= current point
    push a0 #save current point
    call IN_X_RANGE
    pop a0 #a0 = current point
    beq imm, v0, zero, NEXT_ROW #if we are out of the tringle, go to the next row
    call PRINT_DOT #call print point function
    add s2, s2, imm, 1 #inc x counter
    jmp LOOP_X #jump to LOOP_X
NEXT_ROW:
    bgt imm, s0, s1, RETURN #if we are out of the y range values, return
    add s0, s0, imm, 1 #s0+=1
    jmp LOOP_Y #jump to the LOOP_Y
RETURN:
    pop s2
    pop s1
    pop s0
    ret #return
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
    push s0 #save KP
CALC_AB:
    lw s0, imm, zero, 0x100 #s0 = Point A
    lw s1, imm, zero, 0x101 #s1 = Point B
    sra s0, s0, imm, 8 #s0 = Ay
    sra s1, s1, imm, 8 #s1 = By
    sub s0, s1, s0, 0 #s0 = By-Ay = AB 
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
PRINT_DOT:
    push s0
    set s0 255
    out s0, imm, zero, 21 #set monitordata to 255
    out a0, imm, zero, 20 #set monitoraddr to a0
    set s0 1
    out s0, imm, zero, 22 #set monitorcmd to 1
    pop s0
    ret #return
.word 0x100 1797
.word 0x101 5125
.word 0x102 5130