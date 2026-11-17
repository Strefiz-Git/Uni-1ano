#include <stdio.h>
#include "ficheiros_texto.h"

/* ------------------------------------------------------------------
 * ESCREVER TEXTO: fopen("w") + fprintf + fclose
 *
 * Formato do ficheiro criado:
 *     numero;nome;nota
 *     12;Ana Silva;15
 *     7;Rui Costa;9
 * ------------------------------------------------------------------ */
int escreverEstudantesEmFicheiroTexto(const tipoEstudante estudantes[], int quantidadeDeEstudantes) {
    int posicao;

    /* "w" = write: cria o ficheiro, ou APAGA o conteudo se ja existir */
    FILE *ficheiro = fopen(NOME_FICHEIRO_TEXTO, "w");

    /* fopen devolve NULL quando nao consegue abrir. Testar SEMPRE! */
    if (ficheiro == NULL) {
        printf("ERRO! Nao foi possivel abrir '%s' para escrita.\n", NOME_FICHEIRO_TEXTO);
        return 0;
    }

    /* fprintf funciona como o printf, mas escreve no ficheiro */
    fprintf(ficheiro, "numero;nome;nota\n");

    for (posicao = 0; posicao < quantidadeDeEstudantes; posicao++) {
        fprintf(ficheiro, "%d;%s;%d\n",
                estudantes[posicao].numero,
                estudantes[posicao].nome,
                estudantes[posicao].nota);
    }

    /* fclose garante que tudo foi realmente gravado no disco */
    fclose(ficheiro);
    return 1;
}

/* ------------------------------------------------------------------
 * LER TEXTO: fopen("r") + fgets (cabecalho) + fscanf (dados) + fclose
 * ------------------------------------------------------------------ */
int lerEstudantesDeFicheiroTexto(tipoEstudante estudantes[], int capacidadeMaxima) {
    char linhaDoCabecalho[100];
    int quantidadeLida = 0;

    /* "r" = read: o ficheiro TEM de existir, senao devolve NULL */
    FILE *ficheiro = fopen(NOME_FICHEIRO_TEXTO, "r");

    if (ficheiro == NULL) {
        printf("ERRO! Nao foi possivel abrir '%s' (existe? ja gravou?).\n", NOME_FICHEIRO_TEXTO);
        return -1;
    }

    /* 1.o: ler a primeira linha (o cabecalho "numero;nome;nota") e ignora-la.
     * fgets le uma linha inteira, incluindo o '\n'. */
    if (fgets(linhaDoCabecalho, sizeof(linhaDoCabecalho), ficheiro) == NULL) {
        printf("ERRO! O ficheiro '%s' esta vazio.\n", NOME_FICHEIRO_TEXTO);
        fclose(ficheiro);
        return -1;
    }

    /* 2.o: ler uma linha de dados de cada vez com fscanf.
     *
     * Formato " %d;%49[^;];%d":
     *   - o espaco inicial ignora o '\n' da linha anterior
     *   - %d             -> le o numero do estudante
     *   - ;              -> tem de aparecer um ';' no ficheiro
     *   - %49[^;]        -> le o nome ate ao proximo ';' (aceita espacos).
     *                       49 = TAMANHO_NOME - 1, para caber o '\0'
     *   - ;%d            -> le a nota
     * Strings NAO levam '&'. Inteiros levam '&'.
     *
     * fscanf devolve QUANTOS valores leu. Se nao for 3, ja nao ha mais
     * linhas validas (fim do ficheiro). Por isso NAO usamos feof(): o feof
     * so da verdadeiro DEPOIS de tentar ler alem do fim e repetia a ultima linha. */
    while (quantidadeLida < capacidadeMaxima &&
           fscanf(ficheiro, " %d;%49[^;];%d",
                  &estudantes[quantidadeLida].numero,
                  estudantes[quantidadeLida].nome,
                  &estudantes[quantidadeLida].nota) == 3) {
        quantidadeLida++;
    }

    fclose(ficheiro);
    return quantidadeLida;
}

/* ------------------------------------------------------------------
 * MOSTRAR TEXTO: fopen("r") + fgets em ciclo + fclose
 * ------------------------------------------------------------------ */
int mostrarFicheiroTextoLinhaALinha(const char nomeDoFicheiro[]) {
    char linhaLida[200];   /* linhas maiores do que 199 caracteres seriam partidas em duas */
    int numeroDaLinha = 0;
    FILE *ficheiro = fopen(nomeDoFicheiro, "r");

    if (ficheiro == NULL) {
        printf("ERRO! Nao foi possivel abrir '%s'.\n", nomeDoFicheiro);
        return 0;
    }

    printf("--- Conteudo de '%s' ---\n", nomeDoFicheiro);

    /* fgets devolve NULL quando ja nao ha mais linhas.
     * A linha lida ja traz o '\n' no fim, por isso o printf nao precisa de outro. */
    while (fgets(linhaLida, sizeof(linhaLida), ficheiro) != NULL) {
        numeroDaLinha++;
        printf("%2d | %s", numeroDaLinha, linhaLida);
    }

    printf("--- Fim (%d linhas) ---\n", numeroDaLinha);

    fclose(ficheiro);
    return 1;
}

/* ------------------------------------------------------------------
 * COPIAR: fgetc (ler 1 caracter) + fputc (escrever 1 caracter)
 * Aqui estao DOIS ficheiros abertos ao mesmo tempo.
 * ------------------------------------------------------------------ */
int copiarFicheiroCaracterACaracter(const char nomeDoFicheiroOrigem[], const char nomeDoFicheiroDestino[]) {
    int caracterLido;   /* int (e nao char) porque EOF e um valor do tipo int */
    FILE *ficheiroOrigem = fopen(nomeDoFicheiroOrigem, "r");
    FILE *ficheiroDestino;

    if (ficheiroOrigem == NULL) {
        printf("ERRO! Nao foi possivel abrir '%s' para leitura.\n", nomeDoFicheiroOrigem);
        return 0;
    }

    ficheiroDestino = fopen(nomeDoFicheiroDestino, "w");
    if (ficheiroDestino == NULL) {
        printf("ERRO! Nao foi possivel abrir '%s' para escrita.\n", nomeDoFicheiroDestino);
        fclose(ficheiroOrigem);   /* fechar o que ja estava aberto antes de sair */
        return 0;
    }

    /* Em cada volta: le um caracter, guarda-o em caracterLido e testa se e EOF
     * (EOF = End Of File, o fim do ficheiro). Se nao for, escreve-o no destino. */
    while ((caracterLido = fgetc(ficheiroOrigem)) != EOF) {
        fputc(caracterLido, ficheiroDestino);
    }

    fclose(ficheiroOrigem);
    fclose(ficheiroDestino);
    return 1;
}

/* ------------------------------------------------------------------
 * REGISTO (LOG): fopen("a") + fputs + fputc + fclose
 * ------------------------------------------------------------------ */
int acrescentarMensagemAoRegisto(const char mensagem[]) {
    /* "a" = append: escreve SEMPRE no fim e NUNCA apaga o que la esta.
     * Se o ficheiro nao existir, e criado. */
    FILE *ficheiro = fopen(NOME_FICHEIRO_REGISTO, "a");

    if (ficheiro == NULL) {
        printf("ERRO! Nao foi possivel abrir '%s'.\n", NOME_FICHEIRO_REGISTO);
        return 0;
    }

    fputs(mensagem, ficheiro);   /* fputs escreve o texto, mas NAO muda de linha */
    fputc('\n', ficheiro);       /* por isso mudamos de linha com fputc */

    fclose(ficheiro);
    return 1;
}
