#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define MAX_ESTUDANTES 100
#define TAM_NOME 50
#define NOTA_MAX 20
#define NOTA_MIN 0
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
    tipoData dataAvalicao;
} tipoEstudante;


void continuar(void);
void limparBuffer(void);
void menuPrincipal(int quantidade, int quantidadeAvaliados, float percentagemNotasPostivas);
int lerInteiro(int min, int max);
float lerFloat(int min, int max);
void lerString(char string[], int tamanho);
int lerQuantidadeEstudantes(void);
tipoEstudante lerEstudante(tipoEstudante estudantes[], int quantidade);
void lerNotas(tipoEstudante estudantes[], int quantidade);
void mostrarDadosEstudante(tipoEstudante estudante);
void mostrarDadosTodosEstudantes(tipoEstudante estudantes[], int quantidade);
tipoData obterDataAtual(void);
int obterDiaMaximo(int ano, int mes);
tipoData lerData(void);
void acrescentaAvaliado(tipoEstudante estudantes[], int *quantidade);
int procurarEstudante(tipoEstudante estudantes[], int quantidade, int numero);
int obterQuantidadeAvaliados(tipoEstudante estudantes[], int quantidade);
float obterPercentagemNotasPostivas(tipoEstudante estudantes[], int quantidade);
float obterMedia(tipoEstudante estudantes[], int quantidade);
void gravaFicheiroBinario(tipoEstudante estudantes[], int quantidade);
int leFicheiroBinario(tipoEstudante estudantes[]);
void gravaFicheiroTexto(tipoEstudante estudantes[], int quantidade);

int main(void) {
    tipoEstudante estudantes[MAX_ESTUDANTES];
    int quantidade = 0;
    int executar = 1;
    char opcao;

    // sem buffer: o texto aparece logo, antes do system("cls") correr
    setvbuf(stdout, NULL, _IONBF, 0);

    quantidade = lerQuantidadeEstudantes();
    for (int i = 0; i < quantidade; i++) {
        printf("\n--- Estudante %d de %d ---\n", i + 1, quantidade);
        estudantes[i] = lerEstudante(estudantes, i);
    }

    do {
        system("cls");
        menuPrincipal(quantidade, obterQuantidadeAvaliados(estudantes, quantidade), obterPercentagemNotasPostivas(estudantes, quantidade));
        scanf(" %c", &opcao);
        limparBuffer();
        opcao = toupper(opcao);
        switch (opcao) {
            case 'A':
                acrescentaAvaliado(estudantes, &quantidade);
                continuar();
                break;
            case 'I':
                lerNotas(estudantes, quantidade);
                continuar();
                break;
            case 'M':
                mostrarDadosTodosEstudantes(estudantes, quantidade);
                if (obterQuantidadeAvaliados(estudantes, quantidade) > 0) {
                    printf("Media das notas: %.2f\n", obterMedia(estudantes, quantidade));
                    printf("Notas positivas (%%): %.2f\n", obterPercentagemNotasPostivas(estudantes, quantidade));
                }
                continuar();
                break;
            case 'G':
                gravaFicheiroBinario(estudantes, quantidade);
                gravaFicheiroTexto(estudantes, quantidade);
                continuar();
                break;
            case 'L':
                quantidade = leFicheiroBinario(estudantes);
                continuar();
                break;
            case 'F':
                printf("Fim do programa!\n");
                executar = 0;
                continuar();
                break;
            default:
                printf("Erro!\n");
                continuar();
        }
    } while (executar == 1);
    return 0;
}

void continuar(void) {
    printf("Pressione ENTER para continuar...");
    limparBuffer();
}

void limparBuffer(void) {
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

void menuPrincipal(int quantidade, int quantidadeAvaliados, float percentagemNotasPostivas) {
    printf("************************* Menu Principal *************************\n\n");
    printf("Estudantes Inseridos : %d\n\n", quantidade);
    printf("Estudantes Avaliados : %d                Notas Positivas (%%) : %.2f\n\n", quantidadeAvaliados, percentagemNotasPostivas);
    printf("A - Acrescentar Estudante\n");
    printf("I - Introducao Notas\n");
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
        printf("Escreva um inteiro entre %d e %d : ", min, max);
        controlo = scanf("%d", &valor);
        limparBuffer();
        if (controlo == 0) {
            printf("\nERRO!, nao escreveu um numero\n");
        } else if (valor < min || valor > max) {
            printf("\nERRO!, valor fora da escala\n");
        }
    } while (controlo == 0 || valor < min || valor > max);
    return valor;
}

float lerFloat(int min, int max) {
    float valor = min;
    int controlo;
    do {
        printf("Escreva um numero entre %d e %d : ", min, max);
        controlo = scanf("%f", &valor);
        limparBuffer();
        if (controlo == 0) {
            printf("\nERRO!, nao escreveu um numero\n");
        } else if (valor < min || valor > max) {
            printf("\nERRO!, valor fora da escala\n");
        }
    } while (controlo == 0 || valor < min || valor > max);
    return valor;
}

void lerString(char string[], int tamanho) {
    do {
        fgets(string, tamanho, stdin);
        // se a linha era maior que o tamanho, o '\n' nao esta na string
        if (strchr(string, '\n') == NULL) {
            limparBuffer();
        } else {
            string[strcspn(string, "\n")] = '\0';
        }
        if (strlen(string) == 0) {
            printf("ERRO!, o texto nao pode estar vazio: ");
        }
    } while (strlen(string) == 0);
}

int lerQuantidadeEstudantes(void) {
    int quantidade = 0;
    printf("Indique a quantidade de estudantes: ");
    quantidade = lerInteiro(0, MAX_ESTUDANTES);
    return quantidade;
}

tipoEstudante lerEstudante(tipoEstudante estudantes[], int quantidade) {
    tipoEstudante estudante;
    int existe;
    do {
        printf("Indique o numero do estudante: ");
        estudante.numero = lerInteiro(1, 9999);
        existe = procurarEstudante(estudantes, quantidade, estudante.numero);
        if (existe != -1) {
            printf("ERRO!, ja existe um estudante com esse numero\n");
        }
    } while (existe != -1);
    printf("Indique o nome do estudante: ");
    lerString(estudante.nome, TAM_NOME);
    // ainda nao foi avaliado
    estudante.nota = NOTA_NAO_AVALIADO;
    estudante.dataAvalicao.dia = 0;
    estudante.dataAvalicao.mes = 0;
    estudante.dataAvalicao.ano = 0;
    return estudante;
}

void lerNotas(tipoEstudante estudantes[], int quantidade) {
    int i;
    if (quantidade == 0) {
        printf("Nao existem estudantes.\n");
        return;
    }
    printf("Data da avaliacao:\n");
    tipoData data = lerData();
    for (i = 0; i < quantidade; i++) {
        printf("\nEstudante %d - %s\n", estudantes[i].numero, estudantes[i].nome);
        printf("Nota final (%d se nao foi avaliado)\n", NOTA_NAO_AVALIADO);
        estudantes[i].nota = lerInteiro(NOTA_NAO_AVALIADO, NOTA_MAX);
        if (estudantes[i].nota != NOTA_NAO_AVALIADO) {
            estudantes[i].dataAvalicao = data;
        }
    }
}

void mostrarDadosEstudante(tipoEstudante estudante) {
    printf("Numero: %d\n", estudante.numero);
    printf("Nome: %s\n", estudante.nome);
    if (estudante.nota == NOTA_NAO_AVALIADO) {
        printf("Nota: Nao avaliado\n\n");
    } else {
        printf("Nota: %d\n", estudante.nota);
        printf("Data-Avaliacao: %02d/%02d/%d\n\n", estudante.dataAvalicao.dia, estudante.dataAvalicao.mes, estudante.dataAvalicao.ano);
    }
}

void mostrarDadosTodosEstudantes(tipoEstudante estudantes[], int quantidade) {
    if (quantidade > 0) {
        for (int i = 0; i < quantidade; i++) {
            mostrarDadosEstudante(estudantes[i]);
        }
    } else {
        printf("Nao existem estudantes.\n");
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
    printf("Indique o ano da avaliacao do estudante: ");
    data.ano = lerInteiro(2000, obterDataAtual().ano);
    printf("Indique o mes da avaliacao do estudante: ");
    data.mes = lerInteiro(1, 12);
    printf("Indique o dia da avaliacao do estudante: ");
    data.dia = lerInteiro(1, obterDiaMaximo(data.ano, data.mes));
    return data;
}

void acrescentaAvaliado(tipoEstudante estudantes[], int *quantidade) {
    if (*quantidade >= MAX_ESTUDANTES) {
        printf("Erro! Ja atingiu o maximo de %d estudantes.\n", MAX_ESTUDANTES);
        return;
    }
    estudantes[*quantidade] = lerEstudante(estudantes, *quantidade);
    (*quantidade)++;
    printf("Estudante acrescentado com sucesso!\n");
}

// devolve a posicao do estudante ou -1 se nao existir
int procurarEstudante(tipoEstudante estudantes[], int quantidade, int numero) {
    int encontrado = -1;
    for (int i = 0; i < quantidade && encontrado == -1; i++) {
        if (estudantes[i].numero == numero) {
            encontrado = i;
        }
    }
    return encontrado;
}

int obterQuantidadeAvaliados(tipoEstudante estudantes[], int quantidade) {
    int contador = 0;
    for (int i = 0; i < quantidade; i++) {
        if (estudantes[i].nota >= NOTA_MIN && estudantes[i].nota <= NOTA_MAX) {
            contador++;
        }
    }
    return contador;
}

float obterPercentagemNotasPostivas(tipoEstudante estudantes[], int quantidade) {
    int contador = 0;
    int avaliados = obterQuantidadeAvaliados(estudantes, quantidade);
    if (avaliados == 0) {
        return 0;
    }
    for (int i = 0; i < quantidade; i++) {
        if (estudantes[i].nota >= NOTA_POSITIVA) {
            contador++;
        }
    }
    return ((float)contador / avaliados) * 100;
}

float obterMedia(tipoEstudante estudantes[], int quantidade) {
    int soma = 0;
    int avaliados = obterQuantidadeAvaliados(estudantes, quantidade);
    if (avaliados == 0) {
        return 0;
    }
    for (int i = 0; i < quantidade; i++) {
        if (estudantes[i].nota >= NOTA_MIN && estudantes[i].nota <= NOTA_MAX) {
            soma += estudantes[i].nota;
        }
    }
    return (float)soma / avaliados;
}

void gravaFicheiroBinario(tipoEstudante estudantes[], int quantidade) {
    FILE *ficheiro = fopen(FICHEIRO_BINARIO, "wb");
    if (ficheiro == NULL) {
        printf("Erro ao abrir o ficheiro %s!\n", FICHEIRO_BINARIO);
        return;
    }
    fwrite(&quantidade, sizeof(int), 1, ficheiro);
    fwrite(estudantes, sizeof(tipoEstudante), quantidade, ficheiro);
    fclose(ficheiro);
    printf("Dados gravados em %s\n", FICHEIRO_BINARIO);
}

// devolve a quantidade de estudantes lidos
int leFicheiroBinario(tipoEstudante estudantes[]) {
    int quantidade = 0;
    FILE *ficheiro = fopen(FICHEIRO_BINARIO, "rb");
    if (ficheiro == NULL) {
        printf("Erro ao abrir o ficheiro %s!\n", FICHEIRO_BINARIO);
        return 0;
    }
    if (fread(&quantidade, sizeof(int), 1, ficheiro) != 1 || quantidade < 0 || quantidade > MAX_ESTUDANTES) {
        printf("Ficheiro invalido!\n");
        fclose(ficheiro);
        return 0;
    }
    quantidade = (int)fread(estudantes, sizeof(tipoEstudante), quantidade, ficheiro);
    fclose(ficheiro);
    printf("Lidos %d estudantes de %s\n", quantidade, FICHEIRO_BINARIO);
    return quantidade;
}

void gravaFicheiroTexto(tipoEstudante estudantes[], int quantidade) {
    FILE *ficheiro = fopen(FICHEIRO_TEXTO, "w");
    if (ficheiro == NULL) {
        printf("Erro ao abrir o ficheiro %s!\n", FICHEIRO_TEXTO);
        return;
    }
    fprintf(ficheiro, "Numero;Nome;Nota;Data\n");
    for (int i = 0; i < quantidade; i++) {
        if (estudantes[i].nota == NOTA_NAO_AVALIADO) {
            fprintf(ficheiro, "%d;%s;Nao avaliado;-\n", estudantes[i].numero, estudantes[i].nome);
        } else {
            fprintf(ficheiro, "%d;%s;%d;%02d/%02d/%d\n", estudantes[i].numero, estudantes[i].nome, estudantes[i].nota,
                    estudantes[i].dataAvalicao.dia, estudantes[i].dataAvalicao.mes, estudantes[i].dataAvalicao.ano);
        }
    }
    fprintf(ficheiro, "Media: %.2f\n", obterMedia(estudantes, quantidade));
    fprintf(ficheiro, "Notas positivas (%%): %.2f\n", obterPercentagemNotasPostivas(estudantes, quantidade));
    fclose(ficheiro);
    printf("Dados gravados em %s\n", FICHEIRO_TEXTO);
}