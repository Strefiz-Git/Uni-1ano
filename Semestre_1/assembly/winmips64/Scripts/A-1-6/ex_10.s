; =============================================================================
; ex_10.s - rotina Max (maior de 2 inteiros), com pilha
;
; Alineas:
;   a) programa de teste da rotina                -> Rotina1 + Max
;   b) parametros de entrada passados pela pilha  -> A e B empilhados
;   c) variavel local na pilha (i = 6; i++)       -> dentro de Max
;   d) guardar/restaurar $s0 na pilha             -> prologo/epilogo de Max
;   e) resultado devolvido pela pilha             -> espaco reservado pelo caller
;   f) Rotina1 pede valores, chama Max, mostra    -> guarda/restaura $ra
;
; O $ e so o prefixo que marca um nome de registo ($sp = r29, etc).
;   $zero = r0   $v0 = r2   $a0,$a1 = r4,r5   $t0..$t7 = r8..r15
;   $s0..$s7 = r16..r23   $sp = r29   $ra = r31
; Convencao: $t*, $a*, $v* podem ser estragados por qualquer rotina;
;            $s* tem de ser reposto por quem os usar (por isso a alinea d).
; Convencao CR/DR: ver o topo do ex_test4.s
; =============================================================================

        .data
CR:         .word32 0x10000
DR:         .word32 0x10008
MsgPedeA:   .asciiz "Introduza o 1o num inteiro: "
MsgPedeB:   .asciiz "Introduza o 2o num inteiro: "
MsgMaior:   .asciiz "O maior e: "

; $s1: CR   $s2: DR   $t0: A   $t1: B   $t3: codigo para CR
; $a0: string a mostrar (em ESCREVE) / A (em Max)   $a1: B (em Max)
; $v0: resultado   $s0: usado dentro de Max

        .text
        lwu   $s1, CR($zero)          ; $s1 = endereco de CR
        lwu   $s2, DR($zero)          ; $s2 = endereco de DR
        daddi $sp, $zero, 0x200       ; inicializa a pilha (cresce para baixo)

        jal   Rotina1
        halt

; =============================================================================
; Rotina1: pede dois valores, chama Max, mostra o maior
; =============================================================================
Rotina1:
        ; f) guardar $ra: vamos fazer jal (ESCREVE e Max) e cada jal escreve em $ra
        daddi $sp, $sp, -8
        sd    $ra, 0($sp)

        ; ---- pedir A ----
        daddi $a0, $zero, MsgPedeA
        jal   ESCREVE
        daddi $t3, $zero, 8           ; CR = 8 (ler do teclado)
        sd    $t3, 0($s1)
        ld    $t0, 0($s2)             ; $t0 = A

        ; ---- pedir B ----
        daddi $a0, $zero, MsgPedeB
        jal   ESCREVE
        daddi $t3, $zero, 8
        sd    $t3, 0($s1)
        ld    $t1, 0($s2)             ; $t1 = B

        ; b) parametros pela pilha
        daddi $sp, $sp, -8
        sd    $t0, 0($sp)             ; push A
        daddi $sp, $sp, -8
        sd    $t1, 0($sp)             ; push B

        ; e) espaco na pilha para o resultado
        daddi $sp, $sp, -8

        ; Pilha: 0($sp) = resultado   8($sp) = B   16($sp) = A

        jal   Max

        ld    $v0, 0($sp)             ; e) resultado lido da pilha
        daddi $sp, $sp, 24            ; limpa resultado, B e A

        ; ---- mostrar o maior ----
        daddi $a0, $zero, MsgMaior
        jal   ESCREVE
        sd    $v0, 0($s2)
        daddi $t3, $zero, 2           ; CR = 2 (inteiro com sinal)
        sd    $t3, 0($s1)

        ; f) restabelecer $ra e voltar
        ld    $ra, 0($sp)
        daddi $sp, $sp, 8
        jr    $ra

; =============================================================================
; Max: recebe A e B pela pilha, devolve o maior pela pilha
;
; Depois do prologo:  0($sp) = $s0 guardado   8($sp) = i
;                     16($sp) = resultado  24($sp) = B  32($sp) = A
; =============================================================================
Max:
        daddi $sp, $sp, -16           ; abre a frame de Max
        sd    $s0, 0($sp)             ; d) guarda $s0 do caller

        ; c) variavel local i:  i = 6;  i++;
        daddi $t4, $zero, 6
        sd    $t4, 8($sp)             ; i = 6
        ld    $t4, 8($sp)
        daddi $t4, $t4, 1
        sd    $t4, 8($sp)             ; i++

        ld    $a0, 32($sp)            ; $a0 = A
        ld    $a1, 24($sp)            ; $a1 = B

        slt   $s0, $a0, $a1           ; d) $s0 = (A < B)
        bnez  $s0, a1Maior

a0MaiorOuIgual:
        dadd  $v0, $a0, $zero         ; $v0 = A
        j     GuardaMax

a1Maior:
        dadd  $v0, $a1, $zero         ; $v0 = B

GuardaMax:
        sd    $v0, 16($sp)            ; e) resultado no slot reservado

        ld    $s0, 0($sp)             ; d) restabelece $s0
        daddi $sp, $sp, 16            ; fecha a frame
        jr    $ra

; =============================================================================
; ESCREVE: mostra a string cujo endereco esta em $a0
; =============================================================================
ESCREVE:
        sd    $a0, 0($s2)
        daddi $t3, $zero, 4
        sd    $t3, 0($s1)
        jr    $ra