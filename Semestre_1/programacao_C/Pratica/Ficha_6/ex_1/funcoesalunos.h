#ifndef EX_1_FUNCOESALUNOS_H
#define EX_1_FUNCOESALUNOS_H

#include "funcoesgenericas.h"

#define MAX_ESTUDANTES 100
#define TAM_NOME 50

typedef struct {
    int numero;
    int nota;
    char nome[TAM_NOME];
    tipoData dataAvaliacao;
} tipoEstudante;

int lerQuantidadeEstudantes(int max);
tipoEstudante lerDadosEstudante(tipoEstudante v[], int limite);
void lerNotas(tipoEstudante v[], int limite);
void mudarNota(tipoEstudante v[], int limite);
void mostrarDados(tipoEstudante v[], int limite);
int acrescentaAvaliado(tipoEstudante v[], int limite);
int acrescentaEstudantes(tipoEstudante v[], int limite);
int procuraEstudante(tipoEstudante v[], int limite, int numAProcurar);
void contas(tipoEstudante v[], int limite, int *numAval, float *percPos);
float obterMedia(tipoEstudante v[], int limite);
char menu(int numA, int numAval, float percPos);
void gravaFicheiroBinario(tipoEstudante v[], int limite);
int leFicheiroBinario(tipoEstudante v[]);
void gravaFicheiroTexto(tipoEstudante v[], int limite);
int leFicheiroTexto(tipoEstudante v[]);

#endif