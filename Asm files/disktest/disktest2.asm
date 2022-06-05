add $t0, $zero, $zero, 0
add $a1, $zero, $zero, 0
add $s1, $zero, $imm, 1000
add $s2, $zero, $imm, 2000
LoopStart:
    add $t1, $zero, $imm, LoopStart
    in $t2, $zero, $imm, 17
    bne $t1, $zero, $t2, 0                      # check if disk is busy
    add $t1, $zero, $imm, LoopEnd
    add $t2, $zero, $imm, 8
    beq $t1, $t0, $t2, 0                        # if (i==8) goto LoopEnd
    out $t0, $zero, $imm, 15                    # disksector = i
    out $s1, $zero, $imm, 16                    # diskbuffer = 1000
    add $t1, $zero, $imm, 1
    out $t1, $zero, $imm, 14                    # diskcmd = read
IsBusy:
    add $t1, $zero, $imm, IsBusy
    in $t2, $zero, $imm, 17
    bne $t1, $zero, $t2, 0                      # check if disk is busy
    add $t2, $zero, $imm, 128
for128:
    lw $a0, $zero, $s1, 0                       # a0 = mem[1000+j]
    add $s1, $s1, $imm, 1
    lw $a1, $zero, $s2, 0                       # a1 = mem[2000+j]
    add $a1, $a1, $a0, 0                        # a1 = a0 + a1
    sw $a1, $s2, $zero, 0                       # mem[2000+j] = a1
    add $s2, $s2, $imm, 1
    sub $t2, $t2, $imm, 1
    add $t1, $zero, $imm, for128
    bne $t1, $zero, $t2, 0
    add $t0, $t0, $imm, 1                       # i++
    add $t1, $zero, $imm, LoopStart             # return to LoopStart
    beq $t1, $zero, $zero, 0
LoopEnd:
    add $t1, $zero, $imm, LoopEnd
    in $t2, $zero, $imm, 17
    bne $t1, $zero, $t2, 0                      # check if disk is busy
    add $t1, $zero, $imm, 8                     # write to sec[8]
    out $t1, $zero, $imm, 15                    # disksector = 8
    add $s2, $zero, $imm, 2000
    out $s2, $zero, $imm, 16                    # diskbuffer = 2000
    add $t1, $zero, $imm, 2
    out $t1, $zero, $imm, 14                    # diskcmd = write
    FinFin:
    add $t1, $zero, $imm, FinFin
    in $t2, $zero, $imm, 17                     # check disk status
    bne $t1, $zero, $t2, 0                      # check if disk is busy
    halt $zero, $zero, $zero, 0