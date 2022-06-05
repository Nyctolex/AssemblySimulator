add $t0, $zero, $zero, 0
add $a1, $zero, $zero, 0
LoopStart:
    # check if disk is busy
    add $t1, $zero, $imm, LoopStart
    in $t2, $zero, $imm, 17
    bne $t1, $zero, $t2, 0
    # if (i==8) goto LoopEnd
    add $t1, $zero, $imm, LoopEnd
    add $t2, $zero, $imm, 8
    beq $t1, $t0, $t2, 0
    # init disk
    out $t0, $zero, $imm, 15                    # disksector = i
    add $s1, $t0, $imm, 1000
    out $s1, $zero, $imm, 16                    # diskbuffer = i
    add $t1, $zero, $imm, 1
    out $t1, $zero, $imm, 14                    # diskcmd = read
    # read from mem and sum
    # ! read all 128 sectors
    lw $a0, $zero, $t1, 0
    add $a1, $a1, $a0, 0
    add $t0, $t0, $imm, 1                       # i++
    # return to LoopStart
    add $t1, $zero, $imm, LoopStart
    beq $t1, $zero, $zero, 0
LoopEnd:
    # check if disk is busy
    add $t1, $zero, $imm, LoopEnd
    in $t2, $zero, $imm, 17
    bne $t1, $zero, $t2, 0
    # write to sec[8]
        # init disk
        add $t1, $zero, $imm, 8 # !!!!!!!!!!!!!!!!!!!!!!
        out $t1, $zero, $imm, 15                # disksector = 8
        # add $s1, $t0, $imm, 1000
        add $s1, $t0, $imm, 2000
        out $s1, $zero, $imm, 16                # diskbuffer = i
        add $t1, $zero, $imm, 2
        out $t1, $zero, $imm, 14                # diskcmd = write
        # ! wait for end
        halt $zero, $zero, $zero, 0