#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define MAX_ESTUDANTES 100
#define TAM_NOME 50
#define NOTA_MAX 20
#define NOTA_POSITIVA 10
#define NOTA_NAO_AVALIADO -1
#define FICHEIRO_BINARIO "estudantes.dat"
#define FICHEIRO_TEXTO "estudantes.txt"

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


void continuar(void);
void limpaBufferStdin(void);
int lerInteiro(char mensagem[], int minimo, int maximo);
void lerString(char mensagem[], char vetorCaracteres[], int maximoCaracteres);
int lerQuantidadeEstudantes(int max);
tipoEstudante lerDadosEstudante(tipoEstudante v[], int limite);
void lerNotas(tipoEstudante v[], int limite);
void mudarNota(tipoEstudante v[], int limite);
void mostrarDados(tipoEstudante v[], int limite);
tipoData obterDataAtual(void);
int obterDiaMaximo(int ano, int mes);
tipoData lerData(void);
int acrescentaAvaliado(tipoEstudante v[], int limite);
int acrescentaEstudantes(tipoEstudante v[], int limite);
int procuraEstudante(tipoEstudante v[], int limite, int numAProcurar);
void contas(tipoEstudante v[], int limite, int *numAval, float *percPos);
float obterMedia(tipoEstudante v[], int limite);
char menu(int numA, int numAval, float percPos);
void gravaFicheiroBinario(tipoEstudante v[], int limite);
int leFicheiroBinario(tipoEstudante v[]);
void gravaFicheiroTexto(tipoEstudante v[], int limite);

int main(void) {
    tipoEstudante turma[MAX_ESTUDANTES];
    int numAlunos = 0;
    int numAval = 0;
    int lidos;
    float percPos = 0;
    char op;

    // sem buffer: o texto aparece logo
    setvbuf(stdout, NULL, _IONBF, 0);

    do {
        contas(turma, numAlunos, &numAval, &percPos);
        op = menu(numAlunos, numAval, percPos);
        switch (op) {
            case 'A':
                numAlunos = acrescentaEstudantes(turma, numAlunos);
                continuar();
                break;
            case 'I':
                lerNotas(turma, numAlunos);
                continuar();
                break;
            case 'N':
                mudarNota(turma, numAlunos);
                continuar();
                break;
            case 'M':
                mostrarDados(turma, numAlunos);
                if (numAval > 0) {
                    printf("Media das notas: %.2f\n", obterMedia(turma, numAlunos));
                    printf("Notas positivas (%%): %.2f\n", percPos);
                }
                continuar();
                break;
            case 'G':
                gravaFicheiroBinario(turma, numAlunos);
                gravaFicheiroTexto(turma, numAlunos);
                continuar();
                break;
            case 'L':
                lidos = leFicheiroBinario(turma);
                if (lidos >= 0) { // so substitui os dados se a leitura correu bem
                    numAlunos = lidos;
                }
                continuar();
                break;
            case 'F':
                printf("Fim do programa!\n");
                continuar();
                break;
            default:
                printf("Erro!\n");
                continuar();
        }
    } while (op != 'F');
    return 0;
}

void continuar(void) {
    printf("Pressione ENTER para continuar...");
    limpaBufferStdin();
}

void limpaBufferStdin(void) {
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

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
    printf("L - Ler dados de ficheiro\n");
    printf("F - Fim\n");
    printf("        Opcao --> ");
    scanf(" %c", &opcao);
    limpaBufferStdin();
    return (char)toupper(opcao);
}

int lerInteiro(char mensagem[], int minimo, int maximo) {
    int numero = minimo;
    int controlo;
    do {
        printf("%s (%d a %d) : ", mensagem, minimo, maximo);
        controlo = scanf("%d", &numero);
        limpaBufferStdin();
        if (controlo == 0) {
            printf("\nERRO!, nao escreveu um numero\n");
        } else if (numero < minimo || numero > maximo) {
            printf("\nERRO!, valor fora da escala\n");
        }
    } while (controlo == 0 || numero < minimo || numero > maximo);
    return numero;
}

void lerString(char mensagem[], char vetorCaracteres[], int maximoCaracteres) {
    do {
        printf("%s", mensagem);
        fgets(vetorCaracteres, maximoCaracteres, stdin);
        // se a linha era maior que o tamanho, o '\n' nao esta na string
        if (strchr(vetorCaracteres, '\n') == NULL) {
            limpaBufferStdin();
        } else {
            vetorCaracteres[strcspn(vetorCaracteres, "\n")] = '\0';
        }
        if (strlen(vetorCaracteres) == 0) {
            printf("ERRO!, o texto nao pode estar vazio\n");
        }
    } while (strlen(vetorCaracteres) == 0);
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
    data.ano = lerInteiro("Indique o ano", 2000, obterDataAtual().ano);
    data.mes = lerInteiro("Indique o mes", 1, 12);
    data.dia = lerInteiro("Indique o dia", 1, obterDiaMaximo(data.ano, data.mes));
    return data;
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

// devolve a quantidade de estudantes lidos, ou -1 se der erro (dados em memoria ficam intactos)
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