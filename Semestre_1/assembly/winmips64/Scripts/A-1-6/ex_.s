    .data

Vetor: .word16 3, 15, 100, 7, 0xf5aa, 17, 5, 15, -100, 2137
VLen: .word 10

CR:     .word32 0x10000
DR:     .word32 0x10008

    .code
    lwu   r2, DR(r0)        ; r2 = endereço DATA
    lwu   r1, CR(r0)        ; r1 = endereço CONTROL

ld r10, VLen(r0)        ; r10 = tamanho do vetor

daddi r5, r0, 0         ; i = 0

For: 
    slt r3, r5, r10
    beqz r3, FimFor

    daddi r3, r0, 2
    dmul r8, r5, r3         ; r8 = i * 2 (offset do elemento)
    ld r4, Vetor(r8)        ; r4 = Vetor[i] (mesma cena se usar lh, mas utilizado em 16 bits)
    sd r4, (r2)
    daddi r3, r0, 2         ; 2 = inteiro com sinal
    sd r3, (r1)

    daddi r5, r5, 1
    j For

FimFor: 
    daddi r5, r0, 3
    daddi r3, r0, 2
    dmul r8, r5, r3         ; r8 = i * 2 (offset do elemento)
    lh r4, Vetor(r8)        ; r4 = Vetor[i]
    daddi r4, r4, 1
    sh r4, Vetor(r8)        ; Vetor[i] = Vetor[i] + 1




FIM: 
    halt