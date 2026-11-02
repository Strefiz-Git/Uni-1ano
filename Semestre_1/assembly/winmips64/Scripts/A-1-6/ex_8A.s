        .data

Vetor:  .word 3, 15, 100, 7, -466292695414463062, 17, 5, 15, -100, 2023

VLen:   .word 10

CR:     .word32 0x10000
DR:     .word32 0x10008

        .code
        lwu   r2, DR(r0)
        lwu   r1, CR(r0)

        ld    r10, VLen(r0)
        daddi r5, r0, 0         ; i = 0

For:
        slt   r3, r5, r10
        beqz  r3, FimFor

        dsll  r8, r5, 3
        ld    r4, Vetor(r8)

        sd    r4, 0(r2)
        daddi r3, r0, 2         ; 2 = inteiro com sinal
        sd    r3, 0(r1)

        daddi r5, r5, 1
        j     For

FimFor:
        daddi r5, r0, 3         ; 4.º elemento
        dsll  r8, r5, 3
        ld    r4, Vetor(r8)
        daddi r4, r4, 1
        sd    r4, Vetor(r8)

        halt