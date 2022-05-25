set a0 4870

call CALC_AB
sw v0, imm, zero, 0x105 #mem[105] = AB
call CALK_BC
sw v0, imm, zero, 0x106 #mem[105] = BC
call IN_X_RANGE
halt $zero, $zero, $zero, 0
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

.word 0x100 1797
.word 0x101 5125
.word 0x102 5130