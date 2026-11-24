.data
C:		.word	7
A:		.space	800
B:		.space	800

		.text

	DADDI	R1,R0,0		;i = 0 (agora vive no registo R1)
	LD	R5,C(R0)	;C carregado UMA vez, fica em R5
	DADDI	R9,R0,8		;tamanho da dword (constante, fora do ciclo)

loop:
	DMUL	R2,R1,R9	;R2 = offset de i (calculado uma só vez)
	LD	R4,B(R2)	;carrega o valor de B[i]
	DADD	R6,R4,R5	;B[i]+C
	SD	R6,A(R2)	;A[i] = B[i]+C (reutiliza R2)

	DADDI	R1,R1,1		;incrementa i
	DADDI	R8,R1,-100	;o contador chegou a 100?
	BNEZ	R8,loop		;se nao chegou, repete o ciclo

	halt			;termina a execucao