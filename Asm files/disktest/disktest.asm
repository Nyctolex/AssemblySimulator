add $s0, $zero, $zero, 0            # i = 0
add $s1, $zero, $zero, 0            # j = 0
WordLoop:
    add $t3, $zero, $zero, WordEnd  # t3 = *WordEnd
    bgt $t3, $s0, $imm, 127         # if (i > 127): goto WordEnd
    