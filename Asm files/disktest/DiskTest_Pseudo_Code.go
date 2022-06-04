i = 0
j = 0
WordLoop:
    if i == 128: goto WordEnd
    // * temp0 = sector[0][i]
        disksector = 0 // 15
        diskbuffer = i // 16
        diskcmd = 1 // 14
        temp0 = in
    // * temp1 = sector[1][i]
        disksector = 1 // 15
        diskbuffer = i // 16
        diskcmd = 1 // 14
        temp1 = in
    SectorLoop:
        if j == 8: goto SectorEnd
        temp2 = temp0+temp1
        // * temp0 = sector[j][i]
            disksector = j // 15
            diskbuffer = i // 16
            diskcmd = 1 // 14
            temp0 = in
        temp2 += temp0
        j++
        goto SectorLoop
        // ! Done
    SectorEnd:
    // * sector[8][i] = temp2
        disksector = 8 // 15
        diskbuffer = i // 16
        diskcmd = 2 // 14
        out = temp2
    i++
    goto WordLoop
    WordEnd:
HALT