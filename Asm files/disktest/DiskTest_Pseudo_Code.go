i = 0
j = 0
WordLoop:
    if i > 127: goto WordEnd
    // * temp0 = sector[0][i]
        diskcmd = 1 // 14
        disksector = 0 // 15
        diskbuffer = i // 16
        // ! Done
        temp0 = in
    // * temp1 = sector[1][i]
        diskcmd = 1 // 14
        disksector = 1 // 15
        diskbuffer = i // 16
        temp1 = in
    SectorLoop:
        if j > 7: goto SectorEnd
        temp2 = temp0+temp1
        // * temp0 = sector[j][i]
            diskcmd = 1 // 14
            disksector = j // 15
            diskbuffer = i // 16
            temp0 = in
        temp2 += temp0
        j++
        goto SectorLoop
    SectorEnd:
    // * sector[8][i] = temp2
        diskcmd = 2 // 14
        disksector = 8 // 15
        diskbuffer = i // 16
        out = temp2
    i++
    goto WordLoop
    WordEnd:
HALT