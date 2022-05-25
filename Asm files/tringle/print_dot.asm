set a0 32896
call PRINT_DOT
halt $zero, $zero, $zero, 0
PRINT_DOT:
    push t0
    set s0 255
    out s0, imm, zero, 21 #set monitordata to 255
    out a0, imm, zero, 20 #set monitoraddr to a0
    set s0 1
    out s0, imm, zero, 22 #set monitorcmd to 1
    pop s0
    ret