    .data
CR:     .word32 0x10000
DR:     .word32 0x10008


    .code


    lwu r1, CR(r0)        ; r1 = endereço CONTROL
    lwu r2, DR(r0)        ; r2 = endereço DATA

    daddi r5, r0, 0

    While:
        lb r4, String(r5)
        beqz r4, FimWhile
        bne r4, r6, no_space ; if string [i] != ' ' (0x20) then no_space

        daddi r5, r5, 1

        j While



    halt
