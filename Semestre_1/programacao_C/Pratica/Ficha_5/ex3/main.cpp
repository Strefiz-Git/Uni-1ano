#include <stdio.h>
#include <stdlib.h>

#define MAX_INSCRITOS 25
#define NOTA_MIN 0
#define NOTA_MAX 20
#define NOTA_POSITIVA 10

int lerInteiro(int limMin, int limMax);
int lerQuantidadeAvaliados(void);
void lerNotas(int notas[], int qtd);
void mostrarDados(const int notas[], int qtd);
float calculaMedia(const int notas[], int qtd);
void alteraNota(int notas[], int qtd);
void notasPositivas(const int notas[], int qtd);

int main(void)
{
    int vetorNotasFinais[MAX_INSCRITOS];
    int qtdAvaliados;

    qtdAvaliados = lerQuantidadeAvaliados();
    lerNotas(vetorNotasFinais, qtdAvaliados);

    mostrarDados(vetorNotasFinais, qtdAvaliados);
    printf("\nMedia das classificacoes: %.2f\n", calculaMedia(vetorNotasFinais, qtdAvaliados));

    alteraNota(vetorNotasFinais, qtdAvaliados);
    mostrarDados(vetorNotasFinais, qtdAvaliados);
    printf("\nNova media das classificacoes: %.2f\n", calculaMedia(vetorNotasFinais, qtdAvaliados));

    notasPositivas(vetorNotasFinais, qtdAvaliados);

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

void lerNotas(int notas[], int qtd)
{
    int i;
    for (i = 0; i < qtd; i++)
    {
        printf("\nNota do estudante %d\n", i + 1);
        notas[i] = lerInteiro(NOTA_MIN, NOTA_MAX);
    }
}

void mostrarDados(const int notas[], int qtd)
{
    int i;
    printf("\n--- Notas dos estudantes ---\n");
    for (i = 0; i < qtd; i++)
    {
        printf("Estudante %d: %d\n", i + 1, notas[i]);
    }
}

float calculaMedia(const int notas[], int qtd)
{
    if (qtd != 0)
    {
        int i, soma = 0;
        for (i = 0; i < qtd; i++)
        {
            soma += notas[i];
        }
        return (float)soma / qtd;
    }
    printf("\n ERRO! NAO FOI POSSIVEL CALCULAR A MEDIA.\n");
    return -1;
}

void alteraNota(int notas[], int qtd)
{
    int estudante;

    printf("\nQual o estudante cuja nota quer alterar?\n");
    estudante = lerInteiro(1, qtd);

    printf("Nova nota do estudante %d\n", estudante);
    notas[estudante - 1] = lerInteiro(NOTA_MIN, NOTA_MAX);
}

void notasPositivas(const int notas[], int qtd)
{
    int i, positivas = 0;

    printf("\n--- Notas positivas ---\n");
    for (i = 0; i < qtd; i++)
    {
        if (notas[i] >= NOTA_POSITIVA)
        {
            printf("Estudante %d: %d\n", i + 1, notas[i]);
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