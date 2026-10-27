#include <stdio.h>
#include <stdlib.h>

#define MAX_INSCRITOS 25
#define MAX_NOME 50
#define NUM_MIN 1
#define NUM_MAX 9999
#define NOTA_MIN 0
#define NOTA_MAX 20
#define NOTA_POSITIVA 10

struct Estudante
{
    int numero;
    char nome[MAX_NOME];
    int nota;
};

int lerInteiro(int limMin, int limMax);
int lerQuantidadeAvaliados(void);
void lerDadosEstudante(Estudante *e);
void lerNotas(Estudante estudantes[], int qtd);
void mostrarDados(const Estudante estudantes[], int qtd);
float calculaMedia(const Estudante estudantes[], int qtd);
int procurarEstudante(const Estudante estudantes[], int qtd, int numero);
void alteraNota(Estudante estudantes[], int qtd);
void notasPositivas(const Estudante estudantes[], int qtd);

int main(void)
{
    Estudante estudantes[MAX_INSCRITOS];
    int qtdAvaliados;

    qtdAvaliados = lerQuantidadeAvaliados();
    lerNotas(estudantes, qtdAvaliados);

    mostrarDados(estudantes, qtdAvaliados);
    printf("\nMedia das classificacoes: %.2f\n", calculaMedia(estudantes, qtdAvaliados));

    alteraNota(estudantes, qtdAvaliados);
    mostrarDados(estudantes, qtdAvaliados);
    printf("\nNova media das classificacoes: %.2f\n", calculaMedia(estudantes, qtdAvaliados));

    notasPositivas(estudantes, qtdAvaliados);

    return 0;
}

int lerInteiro(int limMin, int limMax)
{
    int num;
    do
    {
        printf("[%d, %d]: ", limMin, limMax);
        scanf("%d", &num);
        if (num < limMin || num > limMax)
        {
            printf("\nErro: o valor nao pertence ao intervalo\n");
        }
    } while (num < limMin || num > limMax);
    return num;
}

int lerQuantidadeAvaliados(void)
{
    printf("Introduza a quantidade de estudantes avaliados (Max: %d)\n", MAX_INSCRITOS);
    return lerInteiro(1, MAX_INSCRITOS);
}

// b) Obtem o numero (1-9999) e o nome de um estudante
void lerDadosEstudante(Estudante *e)
{
    printf("Numero do estudante\n");
    e->numero = lerInteiro(NUM_MIN, NUM_MAX);

    printf("Nome do estudante: ");
    scanf(" %49[^\n]", e->nome);
}

// b) Agora le tambem os dados do estudante, alem da nota
void lerNotas(Estudante estudantes[], int qtd)
{
    int i;
    for (i = 0; i < qtd; i++)
    {
        printf("\n--- Estudante %d ---\n", i + 1);
        lerDadosEstudante(&estudantes[i]);

        printf("Nota de %s\n", estudantes[i].nome);
        estudantes[i].nota = lerInteiro(NOTA_MIN, NOTA_MAX);
    }
}

// b) Agora mostra tambem o numero e o nome
void mostrarDados(const Estudante estudantes[], int qtd)
{
    int i;
    printf("\n--- Notas dos estudantes ---\n");
    for (i = 0; i < qtd; i++)
    {
        printf("Numero: %d | Nome: %s | Nota: %d\n",
               estudantes[i].numero, estudantes[i].nome, estudantes[i].nota);
    }
}

float calculaMedia(const Estudante estudantes[], int qtd)
{
    if (qtd != 0)
    {
        int i, soma = 0;
        for (i = 0; i < qtd; i++)
        {
            soma += estudantes[i].nota;
        }
        return (float)soma / qtd;
    }
    printf("\n ERRO! NAO FOI POSSIVEL CALCULAR A MEDIA.\n");
    return -1;
}

// Devolve a posicao do estudante no vetor, ou -1 se nao existir
int procurarEstudante(const Estudante estudantes[], int qtd, int numero)
{
    int i;
    for (i = 0; i < qtd; i++)
    {
        if (estudantes[i].numero == numero)
        {
            return i;
        }
    }
    return -1;
}

// c) Pede o numero (identificacao) do estudante a que se quer alterar a nota
void alteraNota(Estudante estudantes[], int qtd)
{
    int numero, pos;

    printf("\nQual o numero do estudante cuja nota quer alterar?\n");
    numero = lerInteiro(NUM_MIN, NUM_MAX);

    pos = procurarEstudante(estudantes, qtd, numero);
    if (pos == -1)
    {
        printf("\nErro: nao existe nenhum estudante com o numero %d\n", numero);
        return;
    }

    printf("Nova nota de %s (nota atual: %d)\n", estudantes[pos].nome, estudantes[pos].nota);
    estudantes[pos].nota = lerInteiro(NOTA_MIN, NOTA_MAX);
}

void notasPositivas(const Estudante estudantes[], int qtd)
{
    int i, positivas = 0;

    printf("\n--- Notas positivas ---\n");
    for (i = 0; i < qtd; i++)
    {
        if (estudantes[i].nota >= NOTA_POSITIVA)
        {
            printf("Numero: %d | Nome: %s | Nota: %d\n",
                   estudantes[i].numero, estudantes[i].nome, estudantes[i].nota);
            positivas++;
        }
    }

    if (positivas == 0)
    {
        printf("Nenhum estudante obteve nota positiva.\n");
    }

    if (qtd != 0)
    {
        printf("\nPercentagem de notas positivas: %.2f%%\n", (float)positivas / qtd * 100);
    }
}