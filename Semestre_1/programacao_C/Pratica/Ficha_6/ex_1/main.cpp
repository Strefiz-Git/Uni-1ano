#include <stdio.h>
#include <stdlib.h>

#define MAX_ESTUDANTES 100
#define TAM_NOME 50

typedef struct {
    int dia;
    int mes;
    int ano;
} tipoData;

typedef struct {
    int numero;
    int nota;
    char nome[TAM_NOME];
    tipoData dataAvalicao;
} tipoEstudante;

void limparBuffer(void);
int lerInteiro(int min, int max);
int lerQuantidadeEstudantes();

int main(void) {

    return 0;
}

void limparBuffer(void) {
    char c;
    do {
        c = getchar();
    }while (c != '\n');
}

int lerInteiro(int min, int max) {
    int valor = min;
    int controlo;
    do {
        printf ("Escreva um inteiro entre %d e %d : ", min, max);
        scanf ("%d", &valor);
        limparBuffer();
        if (valor < min || valor > max)
            printf ("\nERRO!, valor fora da escala\n");
    }while (valor < min || valor > max || controlo == 0);
    return valor;
}

int lerQuantidadeEstudantes() {
    int quantidade = 0;
    printf("Indique a quantidade de estudantes: ");
    quantidade = lerInteiro(0, MAX_ESTUDANTES);
    return quantidade;
}

