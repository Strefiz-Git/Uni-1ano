
    .data

CR:     .word32 0x10000
DR:     .word32 0x10008

    .code

        lwu r1, CR(r0)        ; r1 = endereço CONTROL
        lwu r2, DR(r0)        ; r2 = endereço DATA


        daddi r4, r0, 0
        daddi r5, r0, 0
        daddi r6, r0, 0x20

        While:
            lb r3, String(r4)
            beqz r3, FimWhile
            bne r3, r6, no_space ; if string [i] != ' ' (0x20) then no_space

            daddi r4, r4, 1

            j While

        FimWhile:

            sd r4, (r2)
            daddi r3, r0, 1
            sd r3, (r1)
            lb r4, (r2)

            daddi r4, r4, -32
            sb r4, String(r0)
            daddi r3, r0, String
            sd r3, (r2)
            daddi r3, r0, 4
            sd r3, (r1)

            halt
