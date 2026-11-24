.data
C:		.word	7
A:		.space	800
B:		.space	800

		.text

	DADDI	R1,R0,0		;i = 0 (contador)
	DADDI	R2,R0,0		;indice/offset = 0
	LD	R5,C(R0)	;C carregado UMA vez, fica em R5

loop:
	LD	R4,B(R2)	;carrega o valor de B[i]
	DADDI	R1,R1,1		;incrementa i        (enche o atraso do LD)
	DADD	R6,R4,R5	;B[i]+C              (sem bolha load-use)
	DADDI	R8,R1,-100	;o contador chegou a 100? (longe do BNEZ)
	SD	R6,A(R2)	;A[i] = B[i]+C
	DADDI	R2,R2,8		;offset += 8
	BNEZ	R8,loop		;se nao chegou, repete o ciclo

	halt			;termina a execucaow