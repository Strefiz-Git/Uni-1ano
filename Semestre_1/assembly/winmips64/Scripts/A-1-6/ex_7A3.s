.data
; IP 192.168.10.15 em little endian (15, 10, 168, 192)
IP:       .byte 15, 10, 168, 192 
Mask_25:  .word32 0xFFFFFF80 ; 255.255.255.128 (/25)

.code
daddi r29, r0, 0x100 ; Inicializa pilha num endereço seguro e alinhado

; ---------- a) portas (0 = aberta, 1 = fechada) ---------- 
; a1) só as portas 0 e 5 abertas -> 1101 1110 = 0xDE
daddi r3, r0, 0xDE
or r1, r3, r0
daddi r2, r0, 8
jal Bin

; a2) abrir a porta 2 -> bit 2 a 0 (AND com ~0x04 -> 0xFB)
andi r3, r3, 0xFB
or r1, r3, r0
daddi r2, r0, 8
jal Bin

; a3) fechar a porta 5 -> bit 5 a 1 (OR com 0x20)
ori r3, r3, 0x20
or r1, r3, r0
daddi r2, r0, 8
jal Bin

; a4) alterar (inverter) as portas 1 e 2 (XOR com 0x06)
xori r3, r3, 0x06
or r1, r3, r0
daddi r2, r0, 8
jal Bin

; ---------- b) IPv4 ----------
lw r4, IP(r0)        ; r4 = IP (carrega os 4 bytes como um inteiro de 32 bits)
lw r5, Mask_25(r0)   ; r5 = máscara

; b1) IP
or r1, r4, r0
daddi r2, r0, 32
jal Bin

; b2) máscara
or r1, r5, r0
daddi r2, r0, 32
jal Bin

; b3) sub-rede = IP AND máscara
and r6, r4, r5
or r1, r6, r0
daddi r2, r0, 32
jal Bin

; b4) broadcast = sub-rede com bits de host a 1 (/25 significa 7 bits de host -> OR com 0x7F)
ori r7, r6, 0x7F
or r1, r7, r0
daddi r2, r0, 32
jal Bin

halt

; =========================================================================
; Função Bin Corrigida
; Mostra binário com n bits (da esquerda para a direita) e muda de linha
; Recebe: r1 = valor, r2 = n bits (1 a 64)
; =========================================================================
Bin:
    daddi r10, r1, 0      ; r10 = cópia do valor
    daddi r11, r2, 0      ; r11 = número de bits a processar (contador inverso)
    daddi r14, r0, 0      ; r14 = contador de bits já processados (para o espaço)

    ; Endereço do CONTROL do Terminal do WinMIPS64 (0x10000)
    daddi r9, r0, 0x1000
    dsll r9, r9, 4

    ; Aloca espaço na pilha (máximo 64 bits + 8 espaços + LF + NULL = 74 bytes -> aloca 80 para alinhar)
    daddi r29, r29, -80
    daddi r30, r29, 79    ; r30 aponta para o fim do buffer alocado

    sb r0, 0(r30)         ; Terminador NULL no fim da string
    daddi r30, r30, -1
    daddi r8, r0, 0xA     ; Caractere Line Feed (Muda linha)
    sb r8, 0(r30)

WhileBin:
    daddi r30, r30, -1    ; Recua uma posição no buffer para o novo caractere

    ; Processa o bit menos significativo (LSB)
    andi r8, r10, 1
    daddi r8, r8, 48      ; Converte 0/1 para caractere '0'/'1'
    sb r8, 0(r30)

    dsrl r10, r10, 1      ; Desloca para a direita para processar o próximo bit
    daddi r11, r11, -1    ; Decrementa bits restantes
    daddi r14, r14, 1     ; Incrementa bits processados

    beqz r11, FimWhile    ; Se já processou todos os bits, termina loop

    ; Verifica se precisa de inserir um espaço (a cada 8 bits)
    andi r8, r14, 7       ; r14 mod 8
    bnez r8, WhileBin     ; Se não for múltiplo de 8, continua o loop

    ; Insere o espaço antes do próximo caractere
    daddi r30, r30, -1
    daddi r8, r0, 32      ; Código ASCII do espaço
    sb r8, 0(r30)
    j WhileBin

FimWhile:
    ; Impressão da string gerada no terminal
    sd r30, 8(r9)         ; DATA = endereço inicial da string
    daddi r8, r0, 4
    sd r8, 0(r9)          ; CONTROL = 4 (Comando para imprimir string)

    ; Desaloca o espaço da pilha e retorna
    daddi r29, r29, 80
    jr r31