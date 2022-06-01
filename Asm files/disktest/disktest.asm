add $a0, $zero, $zero, 0            # i = 0
add $a1, $zero, $zero, 0            # j = 0
WordLoop:
    add $t3, $zero, $zero, WordEnd  # t3 = *WordEnd
    bgt $t3, $a0, $imm, 127         # if (i > 127): goto WordEnd
    add $a3, $zero, $imm, 15        # a3 = *disksector
    out $a3, $zero, $imm, 0         # disksector = 0
    add $a3, $zero, $imm, 16        # a3 = *diskbuffer
    out $a3, $zero, $a0, 0          # diskbuffer = i
    add $a3, $zero, $imm, 14        # a3 = *diskcmd
    out $a3, $zero, $imm, 1         # diskcmd = 1
    in $a0, $zero, $imm, $