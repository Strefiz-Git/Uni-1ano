#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ESTUDANTES 100
#define TAM_NOME 50
#define NOTA_MAX 20
#define NOTA_MIN 0

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


void continuar(void);
void limparBuffer(void);
void menuPrincipal(int quantidade, int quantidadeAvaliados, float percentagemNotasPostivas);
int lerInteiro(int min, int max);
int lerQuantidadeEstudantes();
tipoEstudante lerEstudante(void);
int lerNotasEstudantes(int nota_min, int nota_max);
void mostrarDados(tipoEstudante estudante[]);
tipoData obterDataAtual(void);
int obterDiaMaximo(int ano, int mes);
tipoData lerData(void);
void acrescentaAvaliado(tipoEstudante *estudantes[], int *quantidade);
int procurarEstudante(tipoEstudante estudantes[], int quantidade);

int main(void) {
    int quantidade = 0;
    int quantidadeAvaliados = 0;
    float percentagemNotasPostivas = 0.0;
    int executar = 1;
    char opcao;
    do {
        system("cls");
        menuPrincipal(quantidade, quantidadeAvaliados, percentagemNotasPostivas);
        scanf(" %c", &opcao);
        limparBuffer();
        switch (opcao) {
            'A':
                break;
            'I':
                break;
            'M':
                break;
            'G':
                break;
            'L':
                break;
            'F':
                printf("Fim do programa!\n");
                executar = 0;
                continuar();
                break;
            default:
                printf("Erro!\n");
                continuar();
        }
    }while (executar == 1);
    return 0;
}

void continuar(void) {
    printf("Pressione ENTER para continuar...");
    limparBuffer();
}

void limparBuffer(void) {
    char c;
    do {
        c = getchar();
    }while (c != '\n');
}

void menuPrincipal(int quantidade, int quantidadeAvaliados, float percentagemNotasPostivas) {
    printf("************************* Menu Principal *************************\n\n");
    printf("Estudantes Inseridos : %d\n\n", quantidade);
    printf("Estudantes Avaliados : %d                Notas Positivas (%%) : %.2f\n\n", quantidadeAvaliados, percentagemNotasPostivas);
    printf("A - Acrescentar Estudante\n");
    printf("I - Introdução Notas\n");
    printf("M - Mostrar Dados\n");
    printf("G - Guardar dados em ficheiros\n");
    printf("L - Ler dados de ficheiro\n");
    printf("F - Fim\n");
    printf("        Opcao --> ");
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

tipoEstudante lerEstudante(void) {
    tipoEstudante estudante;
    printf("Indique o numero do estudante: ");
    estudante.numero = lerInteiro(0, 999999);
    printf("Indique o nome do estudante: ");
    scanf("%s", estudante.nome);
    limparBuffer();
    return estudante;
}

int lerNotasEstudantes(int nota_min, int nota_max) {

}

void mostrarDados(tipoEstudante estudante) {
    printf("Numero: %d\n", estudante.numero);
    printf("Nome: %s\n", estudante.nome);
    printf("Nota: %.2f\n", estudante.nota);
    printf("Data-Avaliacao: %d/%d/%d\n\n", estudante.dataAvalicao.dia, estudante.dataAvalicao.mes, estudante.dataAvalicao.ano);
}

tipoData obterDataAtual(void) {
    tipoData data;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    data.dia = tm.tm_mday;
    data.mes = tm.tm_mon + 1;
    data.ano = tm.tm_year + 1900;
    return data;
}

int obterDiaMaximo(int ano, int mes) {
    if (mes == 2) {
        if (ano % 4 == 0 && (ano % 100 != 0 || ano % 400 == 0)) { // ano bissexto
            return 29;
        } else { // ano nao bissexto
            return 28;
        }
    }
    if (mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes == 8 || mes == 10 || mes == 12) {
        return 31;
    } else {
        return 30;
    }
}

tipoData lerData(void) {
    tipoData data;
    printf("Indique o ano da avaliacao do estudante: ");
    data.ano = lerInteiro(2000, obterDataAtual().ano);
    data.mes = lerInteiro(1, 12);
    printf("Indique o mes da avaliacao do estudante: ");
    data.dia = lerInteiro(1, obterDiaMaximo(data.ano, data.mes));
    printf("Indique o dia da avaliacao do estudante: ");
    return data;
}

void acrescentaAvaliado(tipoEstudante *estudantes[], int *quantidade) {

}

int procurarEstudante(tipoEstudante estudantes[], int quantidade) {
    printf("Indique o numero do estudante: ");
    int numero = lerInteiro(1, quantidade);
    int encontrado = 0;
    int i = 0;
    do {
        if (estudantes[i].numero==numero) {
            encontrado = i;
        }
        i++;
    }while (encontrado == false);
    tipoEstudante estudante = estudantes[encontrado];
    return encontrado;
}

