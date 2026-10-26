; =============================================================================
; ex5A.s - mostrar uma frase no ecra ("Hello World")
;
; O que este programa faz:
;   escreve "Hello World" seguido de uma mudança de linha, no terminal.
;   Nao ha calculo nenhum - e' so' a frase mais minima que se pode
;   mostrar, e por isso e' o primeiro exemplo de qualquer linguagem.
;
; Como executar: a partir desta pasta, run.bat e escolhe o ex5A.s
;
; Este e' o ultimo da serie ex1A a ex5A. Ja se viu, por ordem:
;   ex1A - calculo com numeros na memoria
;   ex2A - reatribuir valores (troca de duas variaveis)
;   ex3A - mostrar um numero no ecra
;   ex4A - pedir numeros ao utilizador
;   ex5A - mostrar texto
;
; -----------------------------------------------------------------------------
; O CONVENIO CR / DR (explicacao completa no ex_test4.s, linhas do topo)
; -----------------------------------------------------------------------------
;   CR = 4  mostra a string cujo ENDERECO esta' no DR
;
; Regra: escreve-se o valor em DR, escreve-se o codigo em CR, e e' a
; escrita no CR que dispara a accao.
; -----------------------------------------------------------------------------
;
; A IDEIA DESTA VEZ: para mostrar texto, o que vai para o DR nao e' o
; texto - e' o ENDERECO do texto. O texto em si fica parado na memoria,
; no sitio onde o .asciiz o colocou, e o DR leva so' o address desse
; sitio. O simulador recebe o endereco e vai la buscar as letras.
;
; Por isso sao sempre duas linhas (e nao uma): uma para pôr o endereco no
; DR, outra para dizer no CR que o que esta' no DR e' um endereco de
; string.
; -----------------------------------------------------------------------------

	.data
CR: .word32 0x10000
DR: .word32 0x10008
msg:    .asciiz "Hello World\n"
	; A string "Hello World" seguida de \n (quebrar linha), e terminada
	; por um zero automatico.
	; O \n e' o codigo 10, e significa "muda de linha". Sem ele, tudo o
	; que aparecesse a seguir continuaria na mesma linha, colado ao
	; "World".
	; O .asciiz (com o "z" no fim) acrescenta sozinho um zero no fim da
	; string. E' esse zero que diz ao simulador onde as letras acabam.
	; Usando .ascii em vez disso, esse zero nao era posto e o simulador
	; ia ler a memoria a seguir, a mostrar lixo, ate encontrar um zero por
	; acaso.
	; (o nome do rotulo e' "msg", em minusculas, ao contrario dos CR e DR
	;  que estao em maiusculas. Nao faz mal nenhum - o assembler distingue
	;  maiusculas de minusculas e sao nomes diferentes. E' so' estilo.)

	.text
	lwu r1, CR(r0)     ; r1 = endereco do registo CR (0x10000)
	lwu r2, DR(r0)     ; r2 = endereco do registo DR (0x10008)
	; lwu = load word unsigned: le 32 bits sem sinal. A "u" (unsigned) e'
	; precisa porque estes dois enderecos, lidos com sinal, seriam
	; negativos (o bit mais alto a 1) e a soma dava a volta, apontando
	; para o sitio errado. Por isso, enderecos leem-se sempre com lwu.

	daddi r4, r0, msg    ; r4 = endereco da string "Hello World"
	sd r4, (r2)          ; DR = esse endereco
	daddi r3, r0, 4      ; r3 = 4 -> "mostrar string"
	sd r3, (r1)          ; CR = 4 -> mostra a frase no terminal
	; Sao estas quatro linhas que fazem o trabalho. As duas ultimas sao o
	; par que se repete em todos os programas de texto: o valor no DR,
	; o codigo no CR.
	; Repara que o DR leva um ENDERECO (o que o assembler calculou para o
	; rotulo "msg") e nao as letras. As letras continuam no sitio onde
	; estavam, e o simulador vai la buscar quando o CR=4 lhe diz o que
	; aquilo e'. E' a diferenca entre "o que mostrar" e "onde esta' o que
	; mostrar".

	halt                  ; acaba o programa
