; =============================================================================
; ex3A.s - somar dois numeros E mostrar o resultado no ecra
;
; O que este programa faz:
;   soma A (10) com B (8), guarda o resultado em C, e mostra 18 no
;   terminal. E' o primeiro programa desta serie que escreve no ecra.
;
; Como executar: a partir desta pasta, run.bat e escolhe o ex3A.s
;
; -----------------------------------------------------------------------------
; O CONVENIO CR / DR (explicacao completa no ex_test4.s, linhas do topo)
; -----------------------------------------------------------------------------
;   CR = 1  mostra um inteiro SEM sinal
;   CR = 4  mostra a string cujo endereco esta' em DR
;
; Regra: escreve-se o valor em DR, escreve-se o codigo em CR, e e' a
; escrita no CR que dispara a accao. Escrever no DR, sozinho, nao faz
; nada visivel.
; -----------------------------------------------------------------------------
;
; NOTA: aqui usa-se o CR=1 (inteiro sem sinal) para mostrar a soma. Como
; 10 + 8 = 18, um numero positivo, tanto 1 como 2 dariam o mesmo
; resultado. A partir do momento em que os numeros pudessem ser
; negativos, o certo passa a ser o CR=2 (inteiro com sinal) - com o
; CR=1 um valor negativo apareceria como um numero enorme em vez do
; sinal menos. No ex4A.s a soma e' de numeros que a pessoa escreve, e
; por isso o CR=1 chega aqui - o enunciado do exercicio nao pede
; negativos, e com valores positivos 1 e 2 dao o mesmo resultado.
; -----------------------------------------------------------------------------

	.data
A: .word 10
B: .word 8
C: .word 0
CR: .word32 0x10000 ; Enderço de registo de controlo
DR: .word32 0x10008 ; Enderço de registo de dados
	; A, B e C sao tres numeros normais (10, 8 e 0, o espaco onde vai
	; o resultado).
	; CR e DR sao outra coisa: NAO sao variaveis do programa, sao os
	; enderecos de dois registos de hardware do simulador (0x10000 e
	; 0x10008). Guardam-se na memoria como dados normais so' para
	; poderem ser carregados com um lwu. E' um endereco "magnetico":
	; mesmo que o valor guardado em C ou DR na memoria nao mude, o
	; que interessa e' o registo de hardware que esta' la apontado.

	.text ; .code
	; (.code e' um sinonimo de .text - os dois servem para dizer que a
	;  seguir vem codigo. O ISET.TXT lista os dois.)

	lwu r1, CR(r0) ; Coloco em r1 o valor do controlo
	lwu r2, DR(r0) ; Coloco em r2 o valor do data
	; lwu = load word unsigned: le 32 bits sem sinal. A "u" (unsigned)
	; esta' aqui por causa do bit do sinal: o endereco 0x10008, lido
	; como um numero de 32 bits COM sinal, seria negativo (o bit mais
	; alto a 1), e o endereco dava a volta e acabava a apontar para o
	; sitio errado. Por isso, sempre que se le um endereco, usa-se lwu.
	; (lw sem a "u" dava problemas com estes enderecos)

	ld r4, A(r0)   ; le o valor de A (10) para r4
	ld r5, B(r0)   ; le o valor de B (8) para r5
	dadd r3, r4, r5 ; r3 = 10 + 8 = 18
	sd r3, C(r0)   ; guarda 18 na posicao C
	; Isto e' exactamente o programa do ex1A.s, mas com so' dois numeros
	; em vez de tres. ld = load (da memoria para o registo), dadd = soma,
	; sd = store (do registo para a memoria).

	sd r3, (r2) ; Coloco o resultado da soma de r3 no Data register
	; Guarda o 18 no registo de hardware DR. Repara na diferenca para a
	; linha de cima: la foi "sd r3, C(r0)" (endereco do C, escrito no
	; codigo), aqui e' "sd r3, (r2)" (endereco que esta' DENTRO de r2,
	; escrito nos parenteses). E' a mesma instrucao - em MIPS o endereco
	; e' sempre "algum registo + um deslocamento", e quando o
	; deslocamento e' zero, o endereco e' so' o registo, entre parenteses.
	; (r1 e r2) sao os enderecos de CR e de DR que carregamos la em
	; cima, por isso escrever em (r2) e' escrever no registo de dados.

	daddi r10, r0, 1 ; Coloca o valor 1 em r10 para depois mostrar o inteiro
	; r10 = 0 + 1. Este 1 e' o CODIGO que vai no CR: 1 quer dizer
	; "mostra um inteiro sem sinal". Nao e' o numero a mostrar - o
	; numero a mostrar ja esta' no DR, desde a linha de cima.

	sd r10, (r1) ; Ativa o controlo e mostra no terminal o valor
	; E' esta linha que faz aparecer o 18 no ecra. Ou seja: ate aqui
	; nada aparecia, porque o 18 estava so' no DR. Ao escrever em CR, o
	; simulador le o que esta' no DR, sabe que o codigo 1 e' "inteiro
	; sem sinal", e mostra.
	; E' a licao central deste ficheiro: DR sozinho nao faz nada; e' o
	; CR que dispara. E a ordem conta - escreve-se primeiro o DR, depois
	; o CR. Se inverter, o CR dispara com o que estivesse no DR antes.

	halt
