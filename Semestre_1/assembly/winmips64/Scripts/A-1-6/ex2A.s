; =============================================================================
; ex2A.s - trocar os valores de duas variaveis
;
; O que este programa faz:
;   A tem 10 e B tem 8 no inicio.
;   No fim, A tem 8 e B tem 10. Os dois valores trocaram de lugar.
;
; Como executar: a partir desta pasta, run.bat e escolhe o ex2A.s
;
; -----------------------------------------------------------------------------
; PORQUE E' QUE ISTO E' MAIS COMPLICADO DO QUE PARECE
; -----------------------------------------------------------------------------
; A grande diferenca entre assembly e o resto das linguagens: aqui nao ha
; variaveis. Nao se pode escrever "A = B" e pronto, a troca acontece,
; porque A e' um ENDERECO na memoria, e nao um nome que o computador
; conheca. O que o computador faz e' copiar o valor que esta' num sitio
; para outro sitio.
;
; O problema real da troca: cada instrucao tem UM destino so. Nao existe
; nenhuma instrucao que troque dois sitios, e nao existe o "A = B" que em
; C resolve a troca numa linha. O unico que ha e' copiar.
;
; E' por isso que o exercicio aparece duas vezes no enunciado, com duas
; solucoes. A diferenca entre elas e' o registo que se repete:
;
;   Exercicio 2a - dois registos, sem mexer neles:
;
;       ld r4, A(r0)     ; r4 = 10
;       ld r5, B(r0)     ; r5 = 8
;       sd r4, B(r0)     ; B = 10   <- o r4 ainda tem o 10
;       sd r5, A(r0)     ; A = 8    <- o r5 ainda tem o 8
;
;     Aqui a troca e' uma copia cruzada e simples. Funciona porque os DOIS
;     valores foram lidos ANTES de qualquer escrita: no momento do store o
;     r4 ainda vale 10 e o r5 ainda vale 8.
;
;   Exercicio 2b - trocar os registos eles proprios, com um terceiro:
;
;       dadd r3, r4, r0  ; r3 = r4     (guarda A a parte)
;       dadd r4, r5, r0  ; r4 = r5     (r4 passa a ter o valor de B)
;       dadd r5, r3, r0  ; r5 = r3     (r5 passa a ter o valor de A)
;
;     Aqui o objectivo e' que os REGISTOS fiquem trocados, e as dadd
;     deixam-nos efectivamente trocados. Por isso o store tem de ser ao
;     contrario da 2a:
;
;       sd r4, A(r0)     ; A = 8       (r4 tem o valor que estava em B)
;       sd r5, B(r0)     ; B = 10      (r5 tem o valor que estava em A)
;
; ESTE FICHEIRO E' O CASO 2b. E o erro classico e' fazer o store da 2a
; depois das dadd da 2b: o codigo corre todo, nao falha nada, e no fim
; A=10 e B=8 como estavam no principio - ou seja, nao troca nada.
; -----------------------------------------------------------------------------

	.data
A:  .word 10
B:  .word 8
	; A e' 10, B e' 8. No fim deste programa, A tem de passar a ter 8 e
	; B tem de passar a ter 10.

	.text
main:
	ld r4, A(r0)     ; r4 = 10 (valor inicial de A)
	ld r5, B(r0)     ; r5 = 8  (valor inicial de B)
	; ld = load doubleword: le 64 bits da memoria para um registo.
	; Os valores 10 e 8 sao copiados para r4 e r5 ANTES de mexer em
	; qualquer coisa na memoria. E' este o ponto do programa.

	dadd r3, r4, r0  ; r3 = r4 + 0 = 10 (guarda A numa "caixa" a parte)
	dadd r4, r5, r0  ; r4 = r5 + 0 = 8  (prepara o valor de B)
	dadd r5, r3, r0  ; r5 = r3 + 0 = 10 (o valor de A, tirado da caixa)
	; Estas tres linhas sao o coracao do programa. Repara que nenhuma toca
	; na memoria - sao apenas registos a passar valores uns para os
	; outros.
	; dadd r3, r4, r0 quer dizer "r3 = r4 + r0", e como o r0 e' sempre 0
	; o resultado e' simplesmente r4. Traduzindo para C, cada uma destas
	; linhas seria "int r3 = r4;", "int r4 = r5;" e "int r5 = r3;".
	; Somar o r0 e' a forma de "copiar" um registo. Existe o "move" como
	; pseudo-instrucao em MIPS, mas o winmips64 nao a lista no ISET.TXT,
	; e somar o zero resulta em todo o lado.
	; O truque do r3 e' o seguinte: r3 e' a "caixa temporaria". Copia-se
	; A para la, para se ter o valor do A seguro, e depois usa-se essa
	; copia para repor o valor de A no fim. Sem o r3 a troca nao
	; funcionaria, como se explica no bloco de comentacao de cima.

	sd r4, A(r0)     ; guarda r4 (8) em A. Agora A = 8, que era o valor de B.
	sd r5, B(r0)     ; guarda r5 (10) em B. Agora B = 10, que era o valor de A.
	; Estes sd sao as duas ultimas instrucoes que mexem na memoria. E so
	; agora e' seguro escrever: os dois valores estao salvos nos registos,
	; portanto pode-se sobrescrever A e B sem perder nada.
	; Repara que o destino de cada registo esta TROCADO. Depois das tres
	; dadd de cima, o r4 ja tem o valor que estava em B (o 8) e o r5 tem o
	; valor que estava em A (o 10). Por isso o r4 vai para A e o r5 vai
	; para B.
	; E' aqui que esta a armadilha deste exercicio: se se escrever
	;
	;     sd r4, B(r0)     ; B = 8  (ja era 8, nao muda nada)
	;     sd r5, A(r0)     ; A = 10 (ja era 10, nao muda nada)
	;
	; o programa corre todo, nao da erro nenhum, e no fim A=10 e B=8
	; como estavam no principio. Ou seja: nao troca nada. E' o mesmo erro
	; que aparece no enunciado do exercicio 2a, que nao tem as dadd - ai
	; a ordem "r4 para B, r5 para A" e' a correcta, porque os registos
	; ainda tem os valores originais. Com as dadd pelo meio, tem de ser ao
	; contrario.
	; (o sd e' store doubleword: copia um registo para a memoria, o
	;  inverso do ld)

	halt              ; acaba o programa
	; Para verificar que a troca funcionou, no simulador ver os valores
	; de A e B depois desta linha - A tem de ser 8 e B tem de ser 10.
