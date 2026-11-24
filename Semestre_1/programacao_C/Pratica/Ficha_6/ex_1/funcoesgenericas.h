#ifndef EX_1_FUNCOESGENERICAS_H
#define EX_1_FUNCOESGENERICAS_H

typedef struct {
    int dia;
    int mes;
    int ano;
} tipoData;

void continuar(void);
void limpaBufferStdin(void);
int lerInteiro(char mensagem[], int minimo, int maximo);
void lerString(char mensagem[], char vetorCaracteres[], int maximoCaracteres);
tipoData obterDataAtual(void);
int obterDiaMaximo(int ano, int mes);
tipoData lerData(void);
void gravaFicheiroBinario(tipoEstudante v[], int limite);
int leFicheiroBinario(tipoEstudante v[]);
void gravaFicheiroTexto(tipoEstudante v[], int limite);
int leFicheiroTexto(tipoEstudante v[]);

#endif //EX_1_FUNCOESGENERICAS_H
