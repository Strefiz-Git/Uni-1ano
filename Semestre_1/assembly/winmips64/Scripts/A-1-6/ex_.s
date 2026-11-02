    .data

CR:     .word32 0x10000
DR:     .word32 0x10008

    .code
    lwu   r2, DR(r0)        ; r2 = endereço DATA
    lwu   r1, CR(r0)        ; r1 = endereço CONTROL






FIM: 
    halt