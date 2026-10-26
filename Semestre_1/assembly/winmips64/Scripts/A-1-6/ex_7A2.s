.data

.code

    daddi r29, r0, 0x80

    daddi r2, r0, 8
    daddi r1, r0, 0xDE ; 1101 1110 ; portas 0 e 5 abertas
    jal Bin; chamada rotina
    ;ex 7 A2 = abrir a porta 2
    ; 0 = aberto | 1 = fechado
    ; 8 portas (0 a 7)

    andi r1, r1, 0xFB

    jal Bin; chamada rotina

    ;ex 7 A3 = abrir a porta 2
    ; 0 = aberto | 1 = fechado
    ; 8 portas (0 a 7)

    ori    

    jal Bin
    halt

    Bin: 
    ;(...)
    jr r31