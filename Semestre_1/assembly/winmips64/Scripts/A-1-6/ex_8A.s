    .data

Vetor: 
    .word 3, 15, 100, 7, 0xF9876543987625aa, 17, 5, 15, -100, 2023

VLen: .word32 10


CR: .word32 0x10000
DR: .word32 0x10008

    .code
lwu r2, DR(r0)
lwu r1, CR(r0)

ld r10, VLen(r0)

daddi r5, r0, 0 ; r5 = 0 (soma)

For:
    slt r3, r5, r10
    beqz r3, FimFor

    daddi r3, r0, 8
    dmul r8, r5, r3
    ld r4, Vetor(r8)

    sd r4, (r2)
    daddi r3, r0, 2
    sd r3, (r1)
    
    daddi r5, r5, 1
    j For
FimFor:

FIM:

    hall