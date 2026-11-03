#include <stdio.h>
#include <stdlib.h>

#define TAM_NOME 50
#define MAX_ESTUDANTES 100

typedef struct {
    int dia;
    int mes;
    int ano;
} tipoData;

typedef struct {
    int numero;
    int nota;
    char nome[TAM_NOME];
    tipoData dataAvaliacao;
} tipoEstudante;

int lerInteiro(int min, int max);
int lerQuantidadeEstudantes(void);
void limparBuffer(void);
tipoEstudante lerDadosEstudante(void);
void mostrarDados(tipoEstudante vetor[], int contador);
void lerNotas(tipoEstudante vetor[], int contador);

int main(void)
{
    tipoEstudante vetorEstudantes[MAX_ESTUDANTES];
    int contadorEstudantes;
    int i;
    contadorEstudantes = 0;

    contadorEstudantes = lerQuantidadeEstudantes();
    for (i = 0; i < contadorEstudantes; i++) {
        vetorEstudantes[i] = lerDadosEstudante();
    }
    mostrarDados(vetorEstudantes, contadorEstudantes);
    lerNotas(vetorEstudantes, contadorEstudantes);
    mostrarDados(vetorEstudantes, contadorEstudantes);
    return 0;
}

void lerNotas(tipoEstudante vetor[], int contador) {
    int i;
    if (contador == 0) {
        printf("\n--- Nao existem estudantes para avaliar ---\n");
    } else {
        for (i = 0; i < contador; i++) {
            printf("\nIndique a nota do estudante %d - %s: ", vetor[i].numero, vetor[i].nome);
            vetor[i].nota = lerInteiro(0, 20);
            printf("Indique o ano da avaliacao");
            vetor[i].dataAvaliacao.ano = lerInteiro(2020, 2030);
            printf("Indique o mes da avaliacao");
            vetor[i].dataAvaliacao.mes = lerInteiro(1, 12);
            printf("Indique o dia da avaliacao");
            vetor[i].dataAvaliacao.dia = lerInteiro(1, 31);
        }
    }
}

void mostrarDados(tipoEstudante vetor[], int contador) {
    int i;
    if (contador == 0) {
        printf("\n--- Nao existem estudantes ---\n");
    } else {
        for (i = 0; i < contador; i++) {
            printf("\nEstudante %d", i+1);
            printf("\nNome: %s", vetor[i].nome);
            printf("Numero: %d", vetor[i].numero);
            if (vetor[i].nota != -1) {
                printf("\nNota: %d", vetor[i].nota);
                printf("\nData de avaliacao: ");
                printf("%02d", vetor[i].dataAvaliacao.dia);
                printf("/%02d", vetor[i].dataAvaliacao.mes);
                printf("/%d", vetor[i].dataAvaliacao.ano);
            }
        }
    }
}

tipoEstudante lerDadosEstudante(void) {
    tipoEstudante estudante;

    printf("Indique o numero do estudante");
    estudante.numero = lerInteiro(1, 9999);

    printf("Indique o nome do estudante: ");
    fgets(estudante.nome, TAM_NOME, stdin);

    estudante.nota = -1;

    return estudante;
}

int lerQuantidadeEstudantes(void) {
    int quantidade;

    printf("Indique a quantidade de estudantes");
    quantidade = lerInteiro(0, MAX_ESTUDANTES);

    return quantidade;
}

int lerInteiro(int min, int max) {
    int numero;
    int controlo;

    do {
        printf(" [%d-%d]: ", min, max);
        controlo = scanf("%d", &numero);
        limparBuffer();
        if (numero < min || numero > max || controlo == 0) {
            printf("Numero invalido, insira novamente");
        }
    } while (numero < min || numero > max || controlo == 0);

    return numero;
}

void limparBuffer(void) {
    char lixo;
    do {
        lixo = getchar();
    } while (lixo != '\n');
}



