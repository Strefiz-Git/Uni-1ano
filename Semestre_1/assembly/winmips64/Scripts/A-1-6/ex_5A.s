    .data
CR:         .word32 0x10000
DR:         .word32 0x10008
MsgHello:   .asciiz "Hello World!\n"
MsgIguais:  .asciiz "Iguais!\n"
MsgOMaior:  .asciiz "O maior e: "

    .text
    lwu r1, CR(r0)          ; r1 = endereço do CONTROL
    lwu r2, DR(r0)          ; r2 = endereço do DATA

    ; ---------- a) Hello World ----------
    daddi r3, r0, 4         ; 4 = escrever string
    daddi r4, r0, MsgHello
    sd r4, (r2)
    sd r3, (r1)

    ; ---------- ler a e b ----------
    daddi r3, r0, 8         ; 8 = ler inteiro
    sd r3, (r1)
    ld r5, (r2)             ; r5 = a

    sd r3, (r1)
    ld r6, (r2)             ; r6 = b

    ; ---------- b) / c) comparar ----------
    beq r5, r6, Iguais

Diferentes:
    daddi r3, r0, 4
    daddi r4, r0, MsgOMaior
    sd r4, (r2)
    sd r3, (r1)             ; escreve "O maior e: "

    dadd r7, r5, r0         ; r7 = a
    slt r3, r5, r6          ; r3 = (a < b)
    beq r3, r0, MaxA        ; se a >= b, o maior é a
    dadd r7, r6, r0         ; senão r7 = b

MaxA:
    daddi r3, r0, 2         ; 2 = escrever inteiro com sinal
    sd r7, (r2)
    sd r3, (r1)
    j END

Iguais:
    daddi r3, r0, 4
    daddi r4, r0, MsgIguais
    sd r4, (r2)
    sd r3, (r1)

END:
    halt