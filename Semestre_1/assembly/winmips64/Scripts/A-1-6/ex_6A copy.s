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

; R1: CR   R2: DR   R3: n.º tentativas   R4: segredo
; R5: tentativa   R7: temp   R8: endereco da string   R9: codigo p/ CR

        .text
        lwu r1, CR(r0)
        lwu r2, DR(r0)
        daddi r3, r0, 0

        FIM:
                halt