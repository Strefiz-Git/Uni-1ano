#include <stdio.h>
#include <ctype.h>
#include "funcoesalunos.h"

// mostra o menu e devolve a opcao escolhida (em maiuscula)
char menu(int numA, int numAval, float percPos) {
    char opcao = 'F'; // se a entrada terminar (EOF), o programa fecha
    printf("\n\n************************* Menu Principal *************************\n\n");
    printf("Estudantes Inseridos : %d\n\n", numA);
    printf("Estudantes Avaliados : %d                Notas Positivas (%%) : %.2f\n\n", numAval, percPos);
    printf("A - Acrescentar Estudante\n");
    printf("I - Introducao Notas\n");
    printf("N - Mudar Nota\n");
    printf("M - Mostrar Dados\n");
    printf("G - Guardar dados em ficheiros\n");
    printf("L - Ler dados de ficheiro binario\n");
    printf("T - Ler dados de ficheiro de texto\n");
    printf("F - Fim\n");
    printf("        Opcao --> ");
    scanf(" %c", &opcao);
    limpaBufferStdin();
    return (char)toupper(opcao);
}

// pede a quantidade de estudantes a acrescentar (0 cancela, maximo = lugares livres)
int lerQuantidadeEstudantes(int max) {
    return lerInteiro("Quantos estudantes quer acrescentar (0 para cancelar)", 0, max);
}

tipoEstudante lerDadosEstudante(tipoEstudante v[], int limite) {
    tipoEstudante e;
    do {
        e.numero = lerInteiro("Indique o numero do estudante", 1, 9999);
        if (procuraEstudante(v, limite, e.numero) != -1) {
            printf("ERRO!, ja existe um estudante com esse numero\n");
        }
    } while (procuraEstudante(v, limite, e.numero) != -1);
    lerString("Indique o nome do estudante: ", e.nome, TAM_NOME);
    // ainda nao foi avaliado
    e.nota = NOTA_NAO_AVALIADO;
    e.dataAvaliacao = (tipoData){0, 0, 0};
    return e;
}

// pede a nota (e a data, uma so vez) apenas aos estudantes ainda nao avaliados
void lerNotas(tipoEstudante v[], int limite) {
    int i;
    int pendentes = 0;
    tipoData data;
    for (i = 0; i < limite; i++) {
        if (v[i].nota == NOTA_NAO_AVALIADO) {
            pendentes++;
        }
    }
    if (limite == 0) {
        printf("Nao existem estudantes.\n");
        return;
    }
    if (pendentes == 0) {
        printf("Todos os estudantes ja foram avaliados.\n");
        return;
    }
    printf("Data da avaliacao:\n");
    data = lerData();
    for (i = 0; i < limite; i++) {
        if (v[i].nota == NOTA_NAO_AVALIADO) {
            printf("\nEstudante %d - %s\n", v[i].numero, v[i].nome);
            v[i].nota = lerInteiro("Nota final (-1 se nao foi avaliado)", NOTA_NAO_AVALIADO, NOTA_MAX);
            if (v[i].nota != NOTA_NAO_AVALIADO) {
                v[i].dataAvaliacao = data;
            }
        }
    }
}

// altera a nota (e a data) de um estudante ja existente
void mudarNota(tipoEstudante v[], int limite) {
    int num;
    int pos;
    if (limite == 0) {
        printf("Nao existem estudantes.\n");
        return;
    }
    num = lerInteiro("Numero do estudante para mudar a nota", 1, 9999);
    pos = procuraEstudante(v, limite, num);
    if (pos == -1) {
        printf("Estudante nao encontrado.\n");
        return;
    }
    printf("Estudante %d - %s\n", v[pos].numero, v[pos].nome);
    v[pos].nota = lerInteiro("Nova nota (-1 para nao avaliado)", NOTA_NAO_AVALIADO, NOTA_MAX);
    if (v[pos].nota != NOTA_NAO_AVALIADO) {
        printf("Data da avaliacao:\n");
        v[pos].dataAvaliacao = lerData();
    } else {
        v[pos].dataAvaliacao = (tipoData){0, 0, 0};
    }
    printf("Nota alterada com sucesso!\n");
}

void mostrarDados(tipoEstudante v[], int limite) {
    int i;
    if (limite == 0) {
        printf("Nao existem estudantes.\n");
        return;
    }
    for (i = 0; i < limite; i++) {
        printf("Numero: %d\n", v[i].numero);
        printf("Nome: %s\n", v[i].nome);
        if (v[i].nota == NOTA_NAO_AVALIADO) {
            printf("Nota: Nao avaliado\n\n");
        } else {
            printf("Nota: %d\n", v[i].nota);
            printf("Data-Avaliacao: %02d/%02d/%d\n\n", v[i].dataAvaliacao.dia, v[i].dataAvaliacao.mes, v[i].dataAvaliacao.ano);
        }
    }
}

// acrescenta um estudante (com nota opcional) e devolve a nova quantidade
int acrescentaAvaliado(tipoEstudante v[], int limite) {
    tipoEstudante novo;
    if (limite >= MAX_ESTUDANTES) {
        printf("Erro! Ja atingiu o maximo de %d estudantes.\n", MAX_ESTUDANTES);
        return limite;
    }
    novo = lerDadosEstudante(v, limite);
    novo.nota = lerInteiro("Nota final (-1 se nao foi avaliado)", NOTA_NAO_AVALIADO, NOTA_MAX);
    if (novo.nota != NOTA_NAO_AVALIADO) {
        printf("Data da avaliacao:\n");
        novo.dataAvaliacao = lerData();
    }
    v[limite] = novo;
    printf("Estudante acrescentado com sucesso!\n");
    return limite + 1;
}

// pergunta quantos estudantes acrescentar, le os dados de cada um e devolve a nova quantidade
int acrescentaEstudantes(tipoEstudante v[], int limite) {
    int livres = MAX_ESTUDANTES - limite;
    int total;
    int i;
    if (livres <= 0) {
        printf("Erro! Ja atingiu o maximo de %d estudantes.\n", MAX_ESTUDANTES);
        return limite;
    }
    total = lerQuantidadeEstudantes(livres);
    for (i = 0; i < total; i++) {
        printf("\n--- Estudante %d de %d ---\n", i + 1, total);
        limite = acrescentaAvaliado(v, limite);
    }
    return limite;
}

int procuraEstudante(tipoEstudante v[], int limite, int numAProcurar) {
    int i;
    int resultado = -1;
    for (i = 0; i < limite && resultado == -1; i++) {
        if (v[i].numero == numAProcurar) {
            resultado = i;
        }
    }
    return resultado;
}

// calcula a quantidade de estudantes avaliados e a percentagem de notas positivas
void contas(tipoEstudante v[], int limite, int *numAval, float *percPos) {
    int i;
    int positivas = 0;
    *numAval = 0;
    for (i = 0; i < limite; i++) {
        if (v[i].nota != NOTA_NAO_AVALIADO) {
            (*numAval)++;
            if (v[i].nota >= NOTA_POSITIVA) {
                positivas++;
            }
        }
    }
    if (*numAval > 0) {
        *percPos = ((float)positivas / *numAval) * 100;
    } else {
        *percPos = 0;
    }
}

float obterMedia(tipoEstudante v[], int limite) {
    int i;
    int soma = 0;
    int avaliados = 0;
    for (i = 0; i < limite; i++) {
        if (v[i].nota != NOTA_NAO_AVALIADO) {
            soma += v[i].nota;
            avaliados++;
        }
    }
    if (avaliados == 0) {
        return 0;
    }
    return (float)soma / avaliados;
}