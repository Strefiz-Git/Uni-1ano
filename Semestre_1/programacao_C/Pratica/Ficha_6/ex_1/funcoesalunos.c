#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "funcoesalunos.h"

#define NOTA_MAX 20
#define NOTA_POSITIVA 10
#define NOTA_NAO_AVALIADO -1
#define FICHEIRO_BINARIO "estudantes.dat"
#define FICHEIRO_TEXTO "estudantes.txt"

char menu(int numA, int numAval, float percPos) {
    char opcao = 'F';
    printf("\n************************ Menu Principal ************************\n");
    printf("Estudantes Inseridos: %d\n", numA);
    printf("Estudantes Avaliados: %d                Notas Positivas (%%): %.2f\n\n", numAval, percPos);
    printf("A - Acrescenta Estudante\n");
    printf("I - Introducao Notas\n");
    printf("N - Mudar Nota\n");
    printf("M - Mostrar Dados\n");
    printf("G - Gravar dados em ficheiros\n");
    printf("L - Ler dados de ficheiro\n");
    printf("F - Fim\n");
    printf("            Opcao--> ");
    scanf(" %c", &opcao);
    limpaBufferStdin();
    return (char)toupper(opcao);
}

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

void gravaFicheiroBinario(tipoEstudante v[], int limite) {
    FILE *ficheiro = fopen(FICHEIRO_BINARIO, "wb");
    if (ficheiro == NULL) {
        printf("Erro ao abrir o ficheiro %s!\n", FICHEIRO_BINARIO);
        return;
    }
    fwrite(&limite, sizeof(int), 1, ficheiro);
    fwrite(v, sizeof(tipoEstudante), limite, ficheiro);
    fclose(ficheiro);
    printf("Dados gravados em %s\n", FICHEIRO_BINARIO);
}

int leFicheiroBinario(tipoEstudante v[]) {
    int quantidade = 0;
    tipoEstudante temporarios[MAX_ESTUDANTES];
    FILE *ficheiro = fopen(FICHEIRO_BINARIO, "rb");
    if (ficheiro == NULL) {
        printf("Erro ao abrir o ficheiro %s!\n", FICHEIRO_BINARIO);
        return -1;
    }
    if (fread(&quantidade, sizeof(int), 1, ficheiro) != 1 || quantidade < 0 || quantidade > MAX_ESTUDANTES) {
        printf("Ficheiro invalido!\n");
        fclose(ficheiro);
        return -1;
    }
    if ((int)fread(temporarios, sizeof(tipoEstudante), quantidade, ficheiro) != quantidade) {
        printf("Ficheiro invalido ou incompleto!\n");
        fclose(ficheiro);
        return -1;
    }
    fclose(ficheiro);
    memcpy(v, temporarios, sizeof(tipoEstudante) * quantidade);
    printf("Lidos %d estudantes de %s\n", quantidade, FICHEIRO_BINARIO);
    return quantidade;
}

void gravaFicheiroTexto(tipoEstudante v[], int limite) {
    int i;
    int numAval;
    float percPos;
    FILE *ficheiro = fopen(FICHEIRO_TEXTO, "w");
    if (ficheiro == NULL) {
        printf("Erro ao abrir o ficheiro %s!\n", FICHEIRO_TEXTO);
        return;
    }
    contas(v, limite, &numAval, &percPos);
    fprintf(ficheiro, "Numero;Nome;Nota;Data\n");
    for (i = 0; i < limite; i++) {
        if (v[i].nota == NOTA_NAO_AVALIADO) {
            fprintf(ficheiro, "%d;%s;Nao avaliado;-\n", v[i].numero, v[i].nome);
        } else {
            fprintf(ficheiro, "%d;%s;%d;%02d/%02d/%d\n", v[i].numero, v[i].nome, v[i].nota,
                    v[i].dataAvaliacao.dia, v[i].dataAvaliacao.mes, v[i].dataAvaliacao.ano);
        }
    }
    fprintf(ficheiro, "Media: %.2f\n", obterMedia(v, limite));
    fprintf(ficheiro, "Notas positivas (%%): %.2f\n", percPos);
    fclose(ficheiro);
    printf("Dados gravados em %s\n", FICHEIRO_TEXTO);
}

int leFicheiroTexto(tipoEstudante v[]) {
    char linha[256];
    int quantidade = 0;
    tipoEstudante temporarios[MAX_ESTUDANTES];
    FILE *ficheiro = fopen(FICHEIRO_TEXTO, "r");
    if (ficheiro == NULL) {
        printf("Erro ao abrir o ficheiro %s!\n", FICHEIRO_TEXTO);
        return -1;
    }
    if (fgets(linha, sizeof(linha), ficheiro) == NULL || strncmp(linha, "Numero;Nome;Nota;Data", 21) != 0) {
        printf("Ficheiro invalido!\n");
        fclose(ficheiro);
        return -1;
    }
    while (fgets(linha, sizeof(linha), ficheiro) != NULL) {
        tipoEstudante e;
        int consumidos = 0;
        char *resto;
        if (sscanf(linha, "%d;%49[^;];%n", &e.numero, e.nome, &consumidos) != 2 || consumidos == 0) {
            break;
        }
        if (quantidade >= MAX_ESTUDANTES || e.numero < 1 || e.numero > 9999) {
            printf("Ficheiro invalido!\n");
            fclose(ficheiro);
            return -1;
        }
        resto = linha + consumidos;
        if (strncmp(resto, "Nao avaliado", 12) == 0) {
            e.nota = NOTA_NAO_AVALIADO;
            e.dataAvaliacao = (tipoData){0, 0, 0};
        } else if (sscanf(resto, "%d;%d/%d/%d", &e.nota, &e.dataAvaliacao.dia,
                          &e.dataAvaliacao.mes, &e.dataAvaliacao.ano) != 4
                   || e.nota < 0 || e.nota > NOTA_MAX) {
            printf("Ficheiro invalido!\n");
            fclose(ficheiro);
            return -1;
        }
        temporarios[quantidade++] = e;
    }
    fclose(ficheiro);
    memcpy(v, temporarios, sizeof(tipoEstudante) * quantidade);
    printf("Lidos %d estudantes de %s\n", quantidade, FICHEIRO_TEXTO);
    return quantidade;
}