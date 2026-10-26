; =============================================================================
; ex1A.s - somar tres numeros que estao na memoria
;
; O que este programa faz:
;   le A, B e Z da memoria, soma os tres, e guarda o resultado em C.
;   Nao ha nada disto no ecra - e' o primeiro programa so com calculo,
;   sem usar o terminal.
;
; Como executar: a partir desta pasta, run.bat e escolhe o ex1A.s
; (para ver o resultado, meter um breakpoint no halt e olhar para o
;  registo r2, ou entao imprimir o C como no ex3A.s)
;
; Este ficheiro e' o primeiro de uma serie de cinco (ex1A a ex5A) que vai
; introduzindo uma coisa de cada vez: guardar em memoria, trocar valores,
; mostrar no ecra, pedir ao utilizador, e dizer uma frase.
;
; -----------------------------------------------------------------------------
; A IDEIA BASICA
; -----------------------------------------------------------------------------
; Nao existem variaveis em assembly. O que ha sao dois recursos:
;
;   - os registos (r0 a r31): 32 lugares rapidos para numeros. Cabem
;     64 bits cada. Vivem dentro do processador, portanto sao a coisa
;     mais rapida que existe.
;   - a memoria: lenta, mas com espaco praticamente ilimitado.
;
; Este programa faz o caminho completo: le da memoria para os registos,
; soma nos registos, e escreve de volta na memoria. Guardar o resultado
; na memoria e' so' para ficar guardado depois de o programa acabar.
; -----------------------------------------------------------------------------

	.data
A:	.word 10
B:	.word 8
C:	.word 0
Z:	.word 2
	; Quatro posicoes de memoria, cada uma com um numero. Os valores sao
	; inventados - o importante e' que existem e que estao. Inicialmente
	; C e' 0 porque e' onde o resultado vai ser escrito.
	; (os nomes A, B, C e Z sao rotulos: apenas nomes para enderecos.
	;  O assembler troca cada rotulo pelo endereco da memoria onde a
	;  posicao foi posta.)

	.text

main:
	ld r4, A(r0)     ; le o valor de A (10) para o registo r4
	ld r5, B(r0)     ; le o valor de B (8) para o registo r5
	ld r6, Z(r0)     ; le o valor de Z (2) para o registo r6
	; ld = load doubleword: traz 64 bits de uma vez da memoria para o
	; registo. O "(r0)" no fim e' o endereco base mais o deslocamento, e
	; como o r0 e' sempre zero, "(r0)" quer dizer simplesmente "neste
	; endereco". O endereco em si e' o do rotulo A, que o assembler
	; calculou.
	; Os tres numeros estao em tres registos diferentes porque ainda vao
	; ser preciso mais tarde. Se usassemos o mesmo registo, o segundo
	; ld apagaria o primeiro.

	dadd r3, r4, r5  ; r3 = r4 + r5 = 10 + 8 = 18
	dadd r2, r3, r6  ; r2 = r3 + r6 = 18 + 2 = 20
	; dadd = soma de 64 bits. Divide-se em duas somas porque cada
	; instrucao dadd soma apenas dois operandos. Assim: primeiro 10+8,
	; depois 18+2.
	; (o resultado vai para r3 primeiro e so' no fim para r2, porque
	;  dadd r2, r4, r6 somaria 10 com 2 e o 8 desaparecia - e' um erro
	;  classico)

	sd r2, C(r0)     ; guarda r2 (20) na memoria, no endereco C
	; sd = store doubleword: o inverso do ld. Agora o caminho e' inverso
	; ao do inicio: o resultado, que estava nos registos, volta para a
	; memoria. C era 0 e passa a ser 20.

	halt              ; acaba o programa
	; Nada disto aparece no ecra, porque o terminal so e' acionado pelo
	; CR (como se ve no ex3A.s). Para ver o resultado, o mais simples e'
	; mostrar o C no ecra como no ex3A.s, ou entao parar aqui e olhar
	; para o registo r2 no simulador.
