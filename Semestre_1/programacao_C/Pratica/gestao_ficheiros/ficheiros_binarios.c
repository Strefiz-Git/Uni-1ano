#include <stdio.h>
#include <string.h>
#include "ficheiros_binarios.h"

/* ------------------------------------------------------------------
 * GRAVAR BINARIO: fopen("wb") + fwrite + fclose
 *
 * fwrite(endereco_dos_dados, tamanho_de_UM_elemento, quantidade, ficheiro)
 * Devolve quantos ELEMENTOS foram escritos (nao bytes).
 * ------------------------------------------------------------------ */
int gravarEstudantesEmFicheiroBinario(const tipoEstudante estudantes[], int quantidadeDeEstudantes) {
    int elementosGravados;

    /* "wb" = write + binary: cria o ficheiro, ou APAGA o conteudo anterior */
    FILE *ficheiro = fopen(NOME_FICHEIRO_BINARIO, "wb");

    if (ficheiro == NULL) {
        printf("ERRO! Nao foi possivel abrir '%s' para escrita.\n", NOME_FICHEIRO_BINARIO);
        return 0;
    }

    /* 1.o: gravar a quantidade. E um int simples, por isso leva '&'.
     * Assim, ao ler, sabemos quantos estudantes ler a seguir. */
    fwrite(&quantidadeDeEstudantes, sizeof(int), 1, ficheiro);

    /* 2.o: gravar o vetor INTEIRO de uma so vez.
     * O nome do vetor ja e o endereco dele, por isso NAO leva '&'. */
    elementosGravados = (int) fwrite(estudantes, sizeof(tipoEstudante), quantidadeDeEstudantes, ficheiro);

    fclose(ficheiro);

    /* Se gravou menos elementos do que devia, houve um erro (ex: disco cheio) */
    if (elementosGravados != quantidadeDeEstudantes) {
        printf("ERRO! Foram gravados apenas %d de %d estudantes.\n", elementosGravados, quantidadeDeEstudantes);
        return 0;
    }
    return 1;
}

/* ------------------------------------------------------------------
 * LER BINARIO: fopen("rb") + fread + fclose
 *
 * fread(onde_guardar, tamanho_de_UM_elemento, quantidade, ficheiro)
 * Devolve quantos ELEMENTOS foram lidos (nao bytes).
 * ------------------------------------------------------------------ */
int lerEstudantesDeFicheiroBinario(tipoEstudante estudantes[]) {
    int quantidadeNoFicheiro = 0;
    tipoEstudante estudantesTemporarios[MAX_ESTUDANTES];

    /* "rb" = read + binary: o ficheiro TEM de existir */
    FILE *ficheiro = fopen(NOME_FICHEIRO_BINARIO, "rb");

    if (ficheiro == NULL) {
        printf("ERRO! Nao foi possivel abrir '%s' (existe? ja gravou?).\n", NOME_FICHEIRO_BINARIO);
        return -1;
    }

    /* 1.o: ler a quantidade. Se nao leu 1 elemento, o ficheiro e invalido. */
    if (fread(&quantidadeNoFicheiro, sizeof(int), 1, ficheiro) != 1) {
        printf("ERRO! Ficheiro invalido (nao tem quantidade).\n");
        fclose(ficheiro);
        return -1;
    }

    /* Protecao: a quantidade tem de ser valida e caber no vetor */
    if (quantidadeNoFicheiro < 0 || quantidadeNoFicheiro > MAX_ESTUDANTES) {
        printf("ERRO! Quantidade invalida no ficheiro: %d.\n", quantidadeNoFicheiro);
        fclose(ficheiro);
        return -1;
    }

    /* 2.o: ler os estudantes. Le primeiro para um vetor TEMPORARIO, assim,
     * se o ficheiro estiver incompleto, os dados que estao em memoria nao se estragam. */
    if ((int) fread(estudantesTemporarios, sizeof(tipoEstudante), quantidadeNoFicheiro, ficheiro) != quantidadeNoFicheiro) {
        printf("ERRO! Ficheiro incompleto.\n");
        fclose(ficheiro);
        return -1;
    }

    fclose(ficheiro);

    /* Tudo correu bem: copia do temporario para o vetor verdadeiro */
    memcpy(estudantes, estudantesTemporarios, sizeof(tipoEstudante) * quantidadeNoFicheiro);
    return quantidadeNoFicheiro;
}

/* ------------------------------------------------------------------
 * CONTAR SEM LER: fseek(SEEK_END) + ftell
 *
 * fseek(ficheiro, deslocamento, origem) move a "posicao atual" do ficheiro.
 *   SEEK_SET = inicio    SEEK_CUR = posicao atual    SEEK_END = fim
 * ftell(ficheiro) diz a posicao atual, em bytes, desde o inicio.
 * Saltando para o fim, a posicao atual e o TAMANHO TOTAL do ficheiro.
 * ------------------------------------------------------------------ */
int contarEstudantesNoFicheiroBinario(void) {
    long tamanhoTotalEmBytes;
    long bytesDosEstudantes;
    FILE *ficheiro = fopen(NOME_FICHEIRO_BINARIO, "rb");

    if (ficheiro == NULL) {
        return -1;
    }

    fseek(ficheiro, 0, SEEK_END);          /* salta para o fim */
    tamanhoTotalEmBytes = ftell(ficheiro); /* posicao no fim = tamanho do ficheiro */
    fclose(ficheiro);

    /* Tira os bytes da quantidade (o int do inicio) */
    bytesDosEstudantes = tamanhoTotalEmBytes - (long) sizeof(int);
    if (bytesDosEstudantes < 0) {
        return -1;   /* ficheiro mais pequeno do que o minimo: invalido */
    }

    /* Cada estudante ocupa sizeof(tipoEstudante) bytes */
    return (int) (bytesDosEstudantes / (long) sizeof(tipoEstudante));
}

/* ------------------------------------------------------------------
 * ACESSO DIRETO: fseek(SEEK_SET) + fread
 * Salta diretamente para o estudante pretendido, sem ler os anteriores.
 *
 * "estudanteLido" e um PONTEIRO: e assim que a funcao "devolve" a
 * estrutura para o sitio que a chamou (passagem por referencia).
 * ------------------------------------------------------------------ */
int lerEstudanteNaPosicao(int posicao, tipoEstudante *estudanteLido) {
    long deslocamento;
    FILE *ficheiro = fopen(NOME_FICHEIRO_BINARIO, "rb");

    if (ficheiro == NULL) {
        printf("ERRO! Nao foi possivel abrir '%s'.\n", NOME_FICHEIRO_BINARIO);
        return 0;
    }

    /* Para chegar ao estudante "posicao" temos de saltar:
     *   - o int da quantidade que esta no inicio, e
     *   - todos os estudantes que vem antes dele. */
    deslocamento = (long) sizeof(int) + (long) posicao * (long) sizeof(tipoEstudante);

    /* fseek devolve 0 se correu bem */
    if (fseek(ficheiro, deslocamento, SEEK_SET) != 0) {
        printf("ERRO! Nao foi possivel saltar para a posicao %d.\n", posicao);
        fclose(ficheiro);
        return 0;
    }

    if (fread(estudanteLido, sizeof(tipoEstudante), 1, ficheiro) != 1) {
        printf("ERRO! Nao foi possivel ler o estudante da posicao %d.\n", posicao);
        fclose(ficheiro);
        return 0;
    }

    fclose(ficheiro);
    return 1;
}

/* ------------------------------------------------------------------
 * ALTERAR UM REGISTO: fopen("rb+") + fseek + fread + fseek(SEEK_CUR) + fwrite
 *
 * "rb+" = ler e escrever em binario, SEM apagar o conteudo existente.
 * (Atencao: "wb+" apagaria tudo!)
 * ------------------------------------------------------------------ */
int alterarNotaNoFicheiroBinario(int posicao, int novaNota) {
    long deslocamento;
    tipoEstudante estudante;
    FILE *ficheiro = fopen(NOME_FICHEIRO_BINARIO, "rb+");

    if (ficheiro == NULL) {
        printf("ERRO! Nao foi possivel abrir '%s'.\n", NOME_FICHEIRO_BINARIO);
        return 0;
    }

    deslocamento = (long) sizeof(int) + (long) posicao * (long) sizeof(tipoEstudante);

    /* 1.o: ir ate ao estudante e le-lo para memoria */
    if (fseek(ficheiro, deslocamento, SEEK_SET) != 0 ||
        fread(&estudante, sizeof(tipoEstudante), 1, ficheiro) != 1) {
        printf("ERRO! Nao foi possivel ler o estudante da posicao %d.\n", posicao);
        fclose(ficheiro);
        return 0;
    }

    /* 2.o: alterar a nota (so em memoria, por enquanto) */
    estudante.nota = novaNota;

    /* 3.o: depois do fread, a posicao atual ficou no estudante SEGUINTE.
     * Voltamos para tras o tamanho de um estudante com SEEK_CUR (relativo a posicao atual).
     * Alem disso, quando um ficheiro esta aberto para ler E escrever, e
     * obrigatorio um fseek entre uma leitura e uma escrita. */
    fseek(ficheiro, -(long) sizeof(tipoEstudante), SEEK_CUR);

    /* 4.o: escrever o estudante alterado por cima do antigo */
    if (fwrite(&estudante, sizeof(tipoEstudante), 1, ficheiro) != 1) {
        printf("ERRO! Nao foi possivel gravar a alteracao.\n");
        fclose(ficheiro);
        return 0;
    }

    fclose(ficheiro);
    return 1;
}

/* ------------------------------------------------------------------
 * ACRESCENTAR NO FIM: fseek(SEEK_END) + fwrite + rewind + fwrite
 *
 * Como o inicio do ficheiro tem a quantidade, nao basta escrever o novo
 * estudante no fim: tambem temos de atualizar o numero no inicio.
 * ------------------------------------------------------------------ */
int acrescentarEstudanteAoFicheiroBinario(const tipoEstudante *novoEstudante) {
    int quantidade = 0;
    FILE *ficheiro = fopen(NOME_FICHEIRO_BINARIO, "rb+");

    /* Se o ficheiro ainda nao existe, "rb+" falha: cria-se um novo com 1 estudante */
    if (ficheiro == NULL) {
        ficheiro = fopen(NOME_FICHEIRO_BINARIO, "wb");
        if (ficheiro == NULL) {
            printf("ERRO! Nao foi possivel criar '%s'.\n", NOME_FICHEIRO_BINARIO);
            return 0;
        }
        quantidade = 1;
        fwrite(&quantidade, sizeof(int), 1, ficheiro);
        fwrite(novoEstudante, sizeof(tipoEstudante), 1, ficheiro);
        fclose(ficheiro);
        return 1;
    }

    /* 1.o: ler a quantidade atual (esta no inicio) */
    if (fread(&quantidade, sizeof(int), 1, ficheiro) != 1) {
        printf("ERRO! Ficheiro invalido.\n");
        fclose(ficheiro);
        return 0;
    }

    /* 2.o: saltar para o fim e escrever o novo estudante la */
    fseek(ficheiro, 0, SEEK_END);
    fwrite(novoEstudante, sizeof(tipoEstudante), 1, ficheiro);

    /* 3.o: voltar ao INICIO do ficheiro e atualizar a quantidade */
    rewind(ficheiro);                         /* rewind = voltar ao inicio */
    quantidade++;
    fwrite(&quantidade, sizeof(int), 1, ficheiro);

    fclose(ficheiro);
    return 1;
}
