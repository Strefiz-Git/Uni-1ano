; =============================================================================
; ex_10.s - rotina Max (maior de 2 inteiros), com pilha
;
; Este ficheiro junta as alineas a) a f) numa so versao final:
;   a) programa de teste da rotina                -> Rotina1 + Max
;   b) parametros de entrada passados pela pilha  -> A e B empilhados
;   c) variavel local na pilha (i = 6; i++)       -> dentro de Max
;   d) guardar/restaurar $s0 (r16) na pilha       -> prologo/epilogo de Max
;   e) resultado devolvido pela pilha             -> espaco reservado pelo caller
;   f) Rotina1 pede valores, chama Max, mostra    -> guarda/restaura r31 ($ra)
;
; Convencao CR/DR: ver o topo do ex_test4.s
; Nomes MIPS -> WinMIPS64:  $sp = r29   $ra = r31   $s0 = r16
; =============================================================================

        .data
CR:         .word32 0x10000
DR:         .word32 0x10008
MsgPedeA:   .asciiz "Introduza o 1o inteiro: "
MsgPedeB:   .asciiz "Introduza o 2o inteiro: "
MsgMaior:   .asciiz "O maior e: "

; R1: CR   R2: DR   R4: A   R5: B   R6: resultado   R8: endereco da string
; R9: codigo para CR   R16 ($s0): usado dentro de Max   R29: pilha   R31: $ra

        .text
        lwu   r1, CR(r0)          ; r1 = endereco de CR
        lwu   r2, DR(r0)          ; r2 = endereco de DR
        daddi r29, r0, 0x200      ; inicializa a pilha (cresce para baixo)

        jal   Rotina1             ; f) o programa principal so chama a Rotina1
        halt

; =============================================================================
; Rotina1: pede dois valores, chama Max, mostra o maior
; =============================================================================
Rotina1:
        ; f) guardar $ra na pilha: Rotina1 vai fazer jal (PRINT e Max), e cada
        ; jal escreve em r31. Sem isto, o jr r31 do fim voltava ao sitio errado.
        daddi r29, r29, -8
        sd    r31, 0(r29)

        ; ---- pedir A ----
        daddi r8, r0, MsgPedeA
        jal   PRINT
        daddi r9, r0, 8           ; CR = 8 (ler do teclado)
        sd    r9, (r1)
        ld    r4, (r2)            ; r4 = A

        ; ---- pedir B ----
        daddi r8, r0, MsgPedeB
        jal   PRINT
        daddi r9, r0, 8
        sd    r9, (r1)
        ld    r5, (r2)            ; r5 = B

        ; b) passar os parametros pela pilha (empilha A, depois B)
        daddi r29, r29, -8
        sd    r4, 0(r29)          ; push A
        daddi r29, r29, -8
        sd    r5, 0(r29)          ; push B

        ; e) reservar na pilha o espaco para o resultado
        daddi r29, r29, -8        ; slot do resultado (por preencher)

        ; Pilha neste momento (do topo para baixo):
        ;   0(sp) = resultado (vazio)    8(sp) = B    16(sp) = A

        jal   Max

        ld    r6, 0(r29)          ; e) r6 = resultado, lido da pilha
        daddi r29, r29, 24        ; limpa o resultado, B e A (3 x 8 bytes)

        ; ---- mostrar o maior ----
        daddi r8, r0, MsgMaior
        jal   PRINT
        sd    r6, (r2)
        daddi r9, r0, 2           ; CR = 2 (inteiro com sinal)
        sd    r9, (r1)

        ; f) restabelecer $ra e voltar ao programa principal
        ld    r31, 0(r29)
        daddi r29, r29, 8
        jr    r31

; =============================================================================
; Max: recebe A e B pela pilha, devolve o maior pela pilha
;
; Ao entrar:           0(sp) = resultado   8(sp) = B   16(sp) = A
; Depois do prologo:   0(sp) = $s0 guardado   8(sp) = i
;                      16(sp) = resultado  24(sp) = B  32(sp) = A
; =============================================================================
Max:
        ; ---- prologo: abre a frame de Max (2 x 8 bytes) ----
        daddi r29, r29, -16
        sd    r16, 0(r29)         ; d) guarda $s0 (r16) do caller

        ; c) variavel local i na pilha:  i = 6;  i++;
        daddi r8, r0, 6
        sd    r8, 8(r29)          ; i = 6
        ld    r8, 8(r29)
        daddi r8, r8, 1
        sd    r8, 8(r29)          ; i++  (i fica a 7; nao influencia o maximo)

        ; ---- corpo: maximo de A e B ----
        ld    r16, 32(r29)        ; $s0 = A  (d: aqui usamos o $s0 como registo extra)
        ld    r10, 24(r29)        ; r10 = B

        slt   r11, r16, r10       ; r11 = (A < B)
        beqz  r11, GuardaMax      ; se A >= B, o maior ja esta' em $s0
        daddi r16, r10, 0         ; senao $s0 = B

GuardaMax:
        sd    r16, 16(r29)        ; e) escreve o resultado no slot reservado pelo caller

        ; ---- epilogo ----
        ld    r16, 0(r29)         ; d) restabelece $s0 antes de sair
        daddi r29, r29, 16        ; fecha a frame de Max
        jr    r31

; =============================================================================
; PRINT: mostra a string cujo endereco esta' em r8
; =============================================================================
PRINT:``
        sd    r8, (r2)
        daddi r9, r0, 4
        sd    r9, (r1)
        jr    r31I