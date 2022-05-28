call CALC_AB #mem[0x105] = AB
call CALK_BC #mem[0x106] = BC
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
    lw s2, imm, zero, 0x100 #s2 = point A
    and s2, s2, imm, 0x00FF #s2 = Ax -> s2 is the x counter
    sw s2, imm, zero, 0x108 # save in mem[0x108] the max value of x counter from previouse line
LOOP_Y:
    lw s2, imm, zero, 0x100 #s2 = point A
    and s2, s2, imm, 0x00FF #s2 = Ax -> s2 is the x counter
    call CALC_AK_MUL_BC
LOOP_X:
    mov a0 s0 #a0 = y counter
    sll a0, a0, imm, 8 #a0<<8
    add a0, a0, s2, 0 #a0+= x counter. a0= current point
    lw t0, imm, zero, 0x108 # load from mem[0x108] the max value of x counter from previouse line
    ble imm, s2, t0, PRINT_DOT #if current x <= max know x -> print the pixel
    push a0 #save current point
    call IN_X_RANGE
    pop a0 #a0 = current point
    beq imm, v0, zero, NEXT_ROW #if we are out of the tringle, go to the next row
    sw s2, imm, zero, 0x108 # update in mem[0x108] the max value of x counter from previouse line
PRINT_DOT:
    set t0 255
    out t0, imm, zero, 21 #set monitordata to 255
    out a0, imm, zero, 20 #set monitoraddr to a0
    set t0 1
    out t0, imm, zero, 22 #set monitorcmd to 1
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
    set v0 0 #set v0 to false
CALC_KP:
    lw t0, imm, zero, 0x100 #t0 = Point A
    mov t1 a0 #set t1 to the given Point
    and t0, t0, imm, 0x00FF #t0 = Ax
    and t1, t1, imm, 0x00FF #t1 = Px
    sub t0, t1, t0, 0 #t0=Px-Ax = KP
SAVE_KP_MUL_AB:
    lw t1, imm, zero, 0x105 #t1=AB
    mul t0, t0, t1, 0 #t0 = AB*KP
LOAD_AK_MUL_BC:
    lw t1, imm, zero, 0x107 #t1 = mem[105] = Ak*BC
    bge imm, t1, t0, RETURN_TRUE #if Ak*BC>= AB*KP return True
RETUN_FALSE:
    ret #return False
RETURN_TRUE:
    set v0 1 #set v0 to true
    ret #return true
CALK_BC:
    lw t0, imm, zero, 0x102 #t0 = point c
    lw t1, imm, zero, 0x101 #t1 = point B
    and t0, t0, imm, 0x00FF #t0 = Cx
    and t1, t1, imm, 0x00FF #t1 = Bx
    sub v0, t0, t1, 0 #v0=Cx-Bx = BC
    sw v0, imm, zero, 0x106 #mem[105] = BC
    ret #return
CALC_AB:
    lw t0, imm, zero, 0x100 #t0 = Point A
    lw t1, imm, zero, 0x101 #t1 = Point B
    sra t0, t0, imm, 8 #t0 = Ay
    sra t1, t1, imm, 8 #t1 = By
    sub v0, t1, t0, 0 #v0 = By-Ay = AB 
    sw v0, imm, zero, 0x105 #mem[105] = AB
    ret #return
CALC_AK_MUL_BC:
CALC_AK:
    lw t0, imm, zero, 0x100 #t0 = Point A
    sra t0, t0, imm, 8 #t0 = Ay
    mov t1 a0 #set t1 to the given Point
    sra t1, t1, imm, 8 #t1 = Py
    sub t0, t1, t0, 0 #t0 = Py-Ay = Ak
    lw t1, imm, zero, 0x106 #t1=BC
SAVE_AK_MUL_BC:
    mul v0, t0, t1, 0 #v0 = Ak*BC
    sw v0, imm, zero, 0x107 #mem[105] = Ak*BC
    ret #return
.word 0x100 1797
.word 0x101 5125
.word 0x102 5130 