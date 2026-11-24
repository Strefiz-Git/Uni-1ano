#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "funcoesgenericas.h"

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

int lerInteiro(char mensagem[], int minimo, int maximo) {
    int numero = minimo;
    int controlo;
    do {
        printf("%s (%d a %d) : ", mensagem, minimo, maximo);
        controlo = scanf("%d", &numero);
        limpaBufferStdin();
        if (controlo == 0) {
            printf("\nERRO!, nao escreveu um numero\n");
        } else if (controlo == EOF) {
            return minimo;
        } else if (numero < minimo || numero > maximo) {
            printf("\nERRO!, valor fora da escala\n");
        }
    } while (controlo == 0 || numero < minimo || numero > maximo);
    return numero;
}

void lerString(char mensagem[], char vetorCaracteres[], int maximoCaracteres) {
    do {
        printf("%s", mensagem);
        if (fgets(vetorCaracteres, maximoCaracteres, stdin) == NULL) {
            strcpy(vetorCaracteres, "?");
            return;
        }
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