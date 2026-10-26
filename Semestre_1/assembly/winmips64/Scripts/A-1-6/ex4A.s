; =============================================================================
; ex4A.s - pedir dois numeros ao utilizador e mostrar a soma
;
; O que este programa faz:
;   1. mostra no ecra o cursor a espera
;   2. le o primeiro numero que a pessoa escreva (r4)
;   3. le o segundo numero que a pessoa escreva (r5)
;   4. mostra a soma dos dois
;
; Como executar: a partir desta pasta, run.bat e escolhe o ex4A.s
; (o programa fica a espera de uma tecla; escreve um numero, da enter,
;  e escreve outro)
;
; Este e' o primeiro programa que PEDE dados. E' o complemento natural do
; ex3A.s: la os numeros estavam escritos a mao na memoria, aqui sao
; escritos por quem esta' a usar o programa.
;
; -----------------------------------------------------------------------------
; O CONVENIO CR / DR (explicacao completa no ex_test4.s, linhas do topo)
; -----------------------------------------------------------------------------
;   CR = 1  mostra um inteiro SEM sinal
;   CR = 8  le um numero do teclado para o DR
;
; Regra: escreve-se o valor em DR, escreve-se o codigo em CR, e e' a
; escrita no CR que dispara a accao.
; -----------------------------------------------------------------------------

	.data
C:  .word 0
CR: .word32 0x10000 ; Enderço de registo de controlo
DR: .word32 0x10008 ; Enderço de registo de dados
	; Aqui so' ha uma variavel, C, que vai receber a soma. Os dois numeros
	; que a pessoa escrever NAO vao para a memoria - vao directo para os
	; registos, que sao mais rapidos e chegam perfeitamente.

	.text ; .code
	lwu r1, CR(r0) ; Coloco em r1 o valor do controlo
	lwu r2, DR(r0) ; Coloco em r2 o valor do data
	; lwu = load word unsigned: le 32 bits sem sinal. A "u" (unsigned)
	; e' importante porque estes enderecos (0x10000, 0x10008) lidos com
	; sinal seriam negativos, e o endereco dava a volta.

	ld r3, C(r0) ; Coloco em r3 o valor de C
	; Este ld nao e' preciso para o programa funcionar - o r3 nao e'
	; usado antes de ser reescrito. Ficou aqui porque o enunciado do
	; exercicio manda. Mostra, de passagem, que ler um valor que existe
	; na memoria nao tem custo nenhum.
	; (quando se comecar a escrever codigo a serio, esta linha
	;  desaparece - e o sinal de que o enunciado nao e' sempre a
	;  mesma coisa que o codigo necessario)

	daddi r10, r0, 8 ; Coloca o valor 8 em r10 para obter do teclado
	; r10 = 0 + 8. Este 8 e' o codigo de "ler do teclado". Repara que e'
	; sempre o registo r10 que se usa para os codigos do CR, e o r1/r2
	; para os enderecos de CR e DR. Nao ha razao para isto - e' so' o
	; que este programa escolheu.

	sd r10, (r1) ; Fica á espera da introducao do valor pelo user
	; Escreve 8 no CR, e o simulador para o programa e mostra o cursor a
	; piscar. O programa fica aqui "parado" (na realidade, a instrucao
	; seguinte executa-se so' quando a pessoa escrever).
	ld r4, (r2) ; Obtem o valor 1 introduzido pelo teclado
	; ld = load doubleword: le o que a pessoa escreveu. O numero
	; digitado vai parar ao registo de hardware DR, e e' desta linha que
	; o vai buscar para o r4.
	; Repara que o DR nao precisou de ser preparado. O codigo 8 e' um
	; caso especial: e' o proprio CR que escreve no DR. Em todo o resto
	; do programa a ordem e' DR -> CR; aqui e' CR -> DR.

	sd r10, (r1) ; fica á espera do 2 valor
	ld r5, (r2) ; Obtem o valor 2 introduzido
	; As mesmas duas linhas outra vez, para o segundo numero. Repara que
	; o codigo 8 continua no r10 - nao e' preciso mudar nada, o registo
	; ainda vale o que valia. E o segundo numero vai para o r5, para
	; nao se perder o primeiro, que continua no r4.
	; Em assembly, "usar uma variavel" significa escolher um registo. Nao
	; ha nomes, nao ha tipo, e nao ha verificacao - o r4 e o r5 sao apenas
	; registos vazios a espera de uso, e nada impede de se usar o r3 para
	; uma coisa e o r4 para outra e mais tarde trocar.

	dadd r3, r4, r5 ; Soma os dois valores introduzidos pelo user
	; r3 = r4 + r5. Este r3 e' o mesmo que se leu de C no inicio - pode
	; ser, porque o valor antigo de C ja nao interessava para nada.
	; Este e' o lado "economico" dos registos: ao nao haverem tipos nem
	; nomes, um registo pode ser reutilizado para outra coisa sem que
	; ninguem proteste.

	sd r3, C(r0) ; Coloca o resultado da soma em C
	; Guarda a soma na memoria. Tal como no ex3A.s, esta linha nao
	; serve para mostrar nada no ecra - serve so' para deixar o
	; resultado guardado.

	;Mostrar no ecra
	sd r3, (r2) ; Coloca o resultado da soma de r3 no Data register
	; Coloca a soma (e nao o endereco de nada) no DR. Repara na
	; diferenca para o CR=4, em que o que vai para o DR e' um ENDERECO de
	; string. O DR nao tem tipo nenhum: o que esta' la dentro depende
	; inteiramente do codigo que se escrever no CR a seguir.

	daddi r10, r0, 1 ; Coloca o valor 1 em r10 para depois mostrar o inteiro
	sd r10, (r1) ; Ativa o controlo e mostra no terminal
	; r10 = 1, e 1 e' o codigo "mostrar inteiro sem sinal". Ao escrever
	; isto no CR, o simulador vai buscar ao DR o valor e mostra-o. E'
	; esta linha que faz aparecer a soma no ecra.
	; Aqui o CR=1 chega porque a soma de dois numeros positivos e'
	; positiva. Se a pessoa pudesse escrever um numero negativo, o
	; resultado poderia ser negativo e o certo seria o CR=2. Como
	; o CR=1 e' a escolha que assume que o resultado e' positivo.

	halt
