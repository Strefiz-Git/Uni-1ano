.data
C:		.word	7
A:		.space	800
B:		.space	800

		.text

	DADDI	R1,R0,0		;i = 0 (registo R1)
	LD	R5,C(R0)	;C carregado UMA vez, fica em R5

loop:
	DSLL	R2,R1,3		;R2 = i*8 (shift de 3 bits no lugar do DMUL)
	LD	R4,B(R2)	;carrega o valor de B[i]
	DADD	R6,R4,R5	;B[i]+C
	SD	R6,A(R2)	;A[i] = B[i]+C

	DADDI	R1,R1,1		;incrementa i
	DADDI	R8,R1,-100	;o contador chegou a 100?
	BNEZ	R8,loop		;se nao chegou, repete o ciclo

	halt			;termina a execucao