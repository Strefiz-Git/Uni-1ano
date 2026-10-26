.data

.code

    daddi r29, r0, 0x80

    ; 0 = aberto | 1 = fechado

    daddi r2, r0, 8
    daddi r1, r0, 0xDE
    jal Bin; chamada rotina
    halt

Bin: