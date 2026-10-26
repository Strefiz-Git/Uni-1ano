        .data
CR:             .word32 0x10000
DR:             .word32 0x10008
MsgPedeSegredo: .asciiz "Jogador 1, introduza o numero secreto: "
MsgJogador2:    .asciiz "Jogador 2, tente adivinhar o numero secreto: "
MsgMaior:       .asciiz "O segredo e maior. Tente de novo.\n\n"
MsgMenor:       .asciiz "O segredo e menor. Tente de novo.\n\n"
MsgParabens:    .asciiz "Parabens! Acertou!\n"
MsgFantastico:  .asciiz "Fantastico!\n"
MsgMuitoBom:    .asciiz "Muito Bom!\n"
MsgTarde:       .asciiz "Ja nao era sem tempo...\n"
MsgNTent:       .asciiz "Numero de tentativas: "

; R1: CR   R2: DR   R3: tentativas   R4: segredo
; R5: tentativa   R7: temp   R8: endereco da string   R9: codigo para CR

        .text
        lwu r1, CR(r0)
        lwu r2, DR(r0)
        daddi r3, r0, 0

        daddi r8, r0, MsgPedeSegredo
        jal PRINT
        daddi r9, r0, 8
        sd r9, (r1)
        ld r4, (r2)

        daddi r9, r0, 6
        sd r9, (r1)

CICLO:
        daddi r8, r0, MsgJogador2
        jal PRINT
        daddi r9, r0, 8
        sd r9, (r1)
        ld r5, (r2)
        daddi r3, r3, 1

        slt r7, r5, r4
        bnez r7, MAIOR
        slt r7, r4, r5
        bnez r7, MENOR
        j ACERTOU

MAIOR:
        daddi r8, r0, MsgMaior
        jal PRINT
        j CICLO

MENOR:
        daddi r8, r0, MsgMenor
        jal PRINT
        j CICLO

ACERTOU:
        daddi r8, r0, MsgParabens
        jal PRINT

        slti r7, r3, 3
        bnez r7, FANT
        slti r7, r3, 5
        bnez r7, MBOM
        daddi r8, r0, MsgTarde
        j IMPRIME
FANT:
        daddi r8, r0, MsgFantastico
        j IMPRIME
MBOM:
        daddi r8, r0, MsgMuitoBom
IMPRIME:
        jal PRINT

        daddi r8, r0, MsgNTent
        jal PRINT
        sd r3, (r2)
        daddi r9, r0, 1
        sd r9, (r1)
        halt

PRINT:
        sd r8, (r2)
        daddi r9, r0, 4
        sd r9, (r1)
        jr r31