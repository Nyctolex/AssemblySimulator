set a0 32896
call PRINT_DOT
halt $zero, $zero, $zero, 0
PRINT_DOT:
    set t0 255
    out t0, imm, zero, 21 #set monitordata to 255
    out a0, imm, zero, 20 #set monitoraddr to a0
    set t0 1
    out t0, imm, zero, 22 #set monitorcmd to 1
    ret