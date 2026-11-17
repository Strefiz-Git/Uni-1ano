#include <stdio.h>
#include <string.h>
#include "utilitarios.h"

void limparBufferDoTeclado(void) {
    int caracterLido;

    /* Le caracter a caracter ate encontrar o ENTER (ou o fim da entrada).
     * "int" e nao "char" porque EOF e um valor do tipo int. */
    do {
        caracterLido = getchar();
    } while (caracterLido != '\n' && caracterLido != EOF);
}

void esperarPorEnter(void) {
    printf("\nPressione ENTER para continuar...");
    limparBufferDoTeclado();
}

int lerNumeroInteiro(const char mensagem[], int valorMinimo, int valorMaximo) {
    int numeroLido = valorMinimo;
    int resultadoDaLeitura;

    do {
        printf("%s (%d a %d): ", mensagem, valorMinimo, valorMaximo);

        /* scanf devolve quantos valores conseguiu ler (aqui: 1 ou 0).
         * Devolve EOF se a entrada terminou. */
        resultadoDaLeitura = scanf("%d", &numeroLido);
        limparBufferDoTeclado();

        if (resultadoDaLeitura == EOF) {
            return valorMinimo; /* entrada terminou: sai com o valor minimo */
        }
        if (resultadoDaLeitura == 0) {
            printf("ERRO! Nao escreveu um numero.\n");
        } else if (numeroLido < valorMinimo || numeroLido > valorMaximo) {
            printf("ERRO! Valor fora do intervalo permitido.\n");
        }
    } while (resultadoDaLeitura == 0 || numeroLido < valorMinimo || numeroLido > valorMaximo);

    return numeroLido;
}

void lerTextoDoTeclado(const char mensagem[], char textoLido[], int tamanhoMaximo) {
    do {
        printf("%s", mensagem);

        /* fgets le uma linha inteira (com espacos) e nunca ultrapassa o tamanho */
        if (fgets(textoLido, tamanhoMaximo, stdin) == NULL) {
            strcpy(textoLido, "?"); /* entrada terminou: evita ciclo infinito */
            return;
        }

        /* Se a linha nao tem '\n', era maior do que cabia: descarta o resto */
        if (strchr(textoLido, '\n') == NULL) {
            limparBufferDoTeclado();
        } else {
            /* strcspn devolve a posicao do '\n'; ali colocamos o fim do texto */
            textoLido[strcspn(textoLido, "\n")] = '\0';
        }

        if (strlen(textoLido) == 0) {
            printf("ERRO! O texto nao pode estar vazio.\n");
        }
    } while (strlen(textoLido) == 0);
}

void substituirCaracterNoTexto(char texto[], char caracterAntigo, char caracterNovo) {
    int posicao;

    for (posicao = 0; texto[posicao] != '\0'; posicao++) {
        if (texto[posicao] == caracterAntigo) {
            texto[posicao] = caracterNovo;
        }
    }
}
