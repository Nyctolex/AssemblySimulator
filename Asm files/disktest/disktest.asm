add $t0, $zero, $zero, 0                    # i = 0
add $t1, $zero, $zero, 0                    # j = 0
add $a2, $zero, $zero, 0                    # a2 = 0
IndexLoop:
    # if (i == 128) goto IndexEnd
    add $t2, $zero, $imm, 128               # t2 = 128
    add $t3, $zero, $imm, IndexEnd
    beq $t3, $t0, $t2, 0                    # if (i == 128) goto IndexEnd
    # a0 = disk[0][i]
    out $zero, $zero, $imm, 15              # disksector = 0
    out $t0, $zero, $imm, 16                # diskbuffer = i
    add $t3, $zero, $imm, 1
    out $t3, $zero, $imm, 14                # diskcmd = read
    lw $a0, $zero, $t0, 0                # a0 = disk[0][i]
    add $a2, $a2, $a0, 0
    # a1 = disk[1][i]
    out $zero, $zero, $imm, 15              # disksector = 0
    out $t0, $zero, $imm, 16                # diskbuffer = i
    add $t3, $zero, $imm, 1
    out $t3, $zero, $imm, 14                # diskcmd = read
    add $t3, $t0, $imm, 128
    lw $a1, $zero, $t3, 0                # a1 = disk[1][i]
    SectorLoop:
        # if (j == 8) goto SectorEnd
        add $t2, $zero, $imm, 8             # t2 = 8
        add $t3, $zero, $imm, SectorEnd
        beq $t3, $t1, $t2, 0                # if (j == 8) goto SectorEnd
        # a2 = a0 + a1
        add $a2, $a2, $a1, 0
        # a0 = disk[j][i]
        out $t1, $zero, $imm, 15              # disksector = j
        out $t0, $zero, $imm, 16                # diskbuffer = i
        add $t3, $zero, $imm, 1
        out $t3, $zero, $imm, 14                # diskcmd = read
        # find space in mem
        add $t3, $zero, $t1, 0
        mul $t3, $t3, $imm, 128
        add $t3, $t3, $t0, $0

        lw $a0, $zero, $t3, 0                    # a0 = disk[j][i]
        # a2 = a2 + a0
        add $a2, $a2, $a0, 0
        # j++
        add $t1, $t1, $imm, 1
        beq $imm, $zero, $zero, SectorLoop
SectorEnd:
    # disk[8][i] = a2
    out $t1, $zero, $imm, 15              # disksector = j
    out $t0, $zero, $imm, 16                # diskbuffer = i
    # find space in mem
    add $t3, $zero, $t1, 0
    mul $t3, $t3, $imm, 128
    add $t3, $t3, $t0, $0
    sw $a2, $t3, $zero, 0
    # write to disk
    add $t3, $zero, $imm, 2
    out $t3, $zero, $imm, 14                # diskcmd = write
    # i++
    add $t0, $t0, $imm, 1
    beq $imm, $zero, $zero, IndexLoop
IndexEnd:
Halt $zero, $zero, $zero, 0