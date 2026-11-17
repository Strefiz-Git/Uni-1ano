#include <stdio.h>
#include "estudante.h"
#include "utilitarios.h"
#include "ficheiros_texto.h"
#include "ficheiros_binarios.h"

/* ------------------------------------------------------------------
 * PROJETO: gestao de estudantes com FICHEIROS
 *
 * Os estudantes vivem em memoria (vetor "turma") e o menu permite
 * experimentar cada funcao base de ficheiros, uma de cada vez.
 * Cada acao importante e registada no ficheiro "registo.log".
 * ------------------------------------------------------------------ */

/* Prototipos das funcoes deste ficheiro */
int mostrarMenuELerOpcao(void);
tipoEstudante lerDadosDeUmEstudante(void);
int acrescentarEstudanteEmMemoria(tipoEstudante estudantes[], int quantidadeAtual);
void mostrarEstudantesEmMemoria(const tipoEstudante estudantes[], int quantidade);
void mostrarEstudanteGravadoNoFicheiroBinario(void);
void alterarNotaGravadaNoFicheiroBinario(void);
void acrescentarNovoEstudanteAoFicheiroBinario(void);
void renomearFicheiroTextoParaBackup(void);
void apagarFicheiro(const char nomeDoFicheiro[]);
void apagarTodosOsFicheiros(void);

int main(void) {
    tipoEstudante turma[MAX_ESTUDANTES];  /* estudantes em memoria */
    int numeroDeEstudantes = 0;           /* quantos estudantes estao em memoria */
    int quantidadeLida;                   /* resultado das leituras de ficheiros */
    int opcao;

    /* Faz o texto aparecer logo no ecra (util no CLion) */
    setvbuf(stdout, NULL, _IONBF, 0);

    do {
        opcao = mostrarMenuELerOpcao();

        switch (opcao) {
            case 1:
                numeroDeEstudantes = acrescentarEstudanteEmMemoria(turma, numeroDeEstudantes);
                break;

            case 2:
                mostrarEstudantesEmMemoria(turma, numeroDeEstudantes);
                break;

            /* ---------- FICHEIROS DE TEXTO ---------- */
            case 3:
                if (escreverEstudantesEmFicheiroTexto(turma, numeroDeEstudantes)) {
                    printf("Gravados %d estudantes em '%s'.\n", numeroDeEstudantes, NOME_FICHEIRO_TEXTO);
                    acrescentarMensagemAoRegisto("Gravou estudantes no ficheiro de texto");
                }
                break;

            case 4:
                quantidadeLida = lerEstudantesDeFicheiroTexto(turma, MAX_ESTUDANTES);
                if (quantidadeLida >= 0) {   /* so substitui a memoria se a leitura correu bem */
                    numeroDeEstudantes = quantidadeLida;
                    printf("Lidos %d estudantes de '%s'.\n", quantidadeLida, NOME_FICHEIRO_TEXTO);
                    acrescentarMensagemAoRegisto("Leu estudantes do ficheiro de texto");
                }
                break;

            case 5:
                mostrarFicheiroTextoLinhaALinha(NOME_FICHEIRO_TEXTO);
                break;

            case 6:
                if (copiarFicheiroCaracterACaracter(NOME_FICHEIRO_TEXTO, NOME_FICHEIRO_COPIA)) {
                    printf("Copia criada: '%s'.\n", NOME_FICHEIRO_COPIA);
                    acrescentarMensagemAoRegisto("Copiou o ficheiro de texto");
                }
                break;

            /* ---------- FICHEIROS BINARIOS ---------- */
            case 7:
                if (gravarEstudantesEmFicheiroBinario(turma, numeroDeEstudantes)) {
                    printf("Gravados %d estudantes em '%s'.\n", numeroDeEstudantes, NOME_FICHEIRO_BINARIO);
                    acrescentarMensagemAoRegisto("Gravou estudantes no ficheiro binario");
                }
                break;

            case 8:
                quantidadeLida = lerEstudantesDeFicheiroBinario(turma);
                if (quantidadeLida >= 0) {
                    numeroDeEstudantes = quantidadeLida;
                    printf("Lidos %d estudantes de '%s'.\n", quantidadeLida, NOME_FICHEIRO_BINARIO);
                    acrescentarMensagemAoRegisto("Leu estudantes do ficheiro binario");
                }
                break;

            case 9:
                quantidadeLida = contarEstudantesNoFicheiroBinario();
                if (quantidadeLida >= 0) {
                    printf("O ficheiro binario tem %d estudantes.\n", quantidadeLida);
                } else {
                    printf("O ficheiro binario nao existe ou e invalido.\n");
                }
                break;

            case 10:
                mostrarEstudanteGravadoNoFicheiroBinario();
                break;

            case 11:
                alterarNotaGravadaNoFicheiroBinario();
                break;

            case 12:
                acrescentarNovoEstudanteAoFicheiroBinario();
                break;

            /* ---------- OUTRAS OPERACOES COM FICHEIROS ---------- */
            case 13:
                mostrarFicheiroTextoLinhaALinha(NOME_FICHEIRO_REGISTO);
                break;

            case 14:
                renomearFicheiroTextoParaBackup();
                break;

            case 15:
                apagarTodosOsFicheiros();
                break;

            case 0:
                printf("Fim do programa!\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

        if (opcao != 0) {
            esperarPorEnter();
        }
    } while (opcao != 0);

    return 0;
}

/* Mostra o menu e devolve a opcao escolhida pelo utilizador */
int mostrarMenuELerOpcao(void) {
    printf("\n\n=================== GESTAO DE FICHEIROS ===================\n");
    printf("                  (Estudantes em memoria)\n\n");
    printf(" 1 - Acrescentar estudante (em memoria)\n");
    printf(" 2 - Mostrar estudantes (em memoria)\n");
    printf("\n--- FICHEIRO DE TEXTO ---------------------------------------\n");
    printf(" 3 - Gravar em texto            (fopen \"w\", fprintf)\n");
    printf(" 4 - Ler de texto               (fopen \"r\", fgets, fscanf)\n");
    printf(" 5 - Mostrar texto por linhas   (fgets)\n");
    printf(" 6 - Copiar ficheiro de texto   (fgetc, fputc)\n");
    printf("\n--- FICHEIRO BINARIO ----------------------------------------\n");
    printf(" 7 - Gravar em binario          (fopen \"wb\", fwrite)\n");
    printf(" 8 - Ler de binario             (fopen \"rb\", fread)\n");
    printf(" 9 - Contar estudantes no ficheiro (fseek, ftell)\n");
    printf("10 - Ver estudante numa posicao (fseek, fread)\n");
    printf("11 - Alterar nota no ficheiro   (fopen \"rb+\", SEEK_CUR)\n");
    printf("12 - Acrescentar ao binario     (SEEK_END, rewind)\n");
    printf("\n--- OUTRAS OPERACOES ----------------------------------------\n");
    printf("13 - Ver registo (log)          (fopen \"a\", fputs)\n");
    printf("14 - Renomear texto para backup (rename, remove)\n");
    printf("15 - Apagar todos os ficheiros  (remove)\n");
    printf(" 0 - Sair\n\n");

    return lerNumeroInteiro("Opcao", 0, 15);
}

/* Pede ao utilizador os dados de UM estudante e devolve-os */
tipoEstudante lerDadosDeUmEstudante(void) {
    tipoEstudante estudante;

    estudante.numero = lerNumeroInteiro("Numero do estudante", 1, 9999);
    lerTextoDoTeclado("Nome do estudante: ", estudante.nome, TAMANHO_NOME);
    estudante.nota = lerNumeroInteiro("Nota final", 0, 20);

    /* O ';' separa os campos no ficheiro de texto, por isso nao pode aparecer no nome */
    substituirCaracterNoTexto(estudante.nome, ';', ',');

    return estudante;
}

/* Acrescenta um estudante ao vetor e devolve a nova quantidade */
int acrescentarEstudanteEmMemoria(tipoEstudante estudantes[], int quantidadeAtual) {
    if (quantidadeAtual >= MAX_ESTUDANTES) {
        printf("ERRO! Ja atingiu o maximo de %d estudantes.\n", MAX_ESTUDANTES);
        return quantidadeAtual;
    }

    estudantes[quantidadeAtual] = lerDadosDeUmEstudante();
    printf("Estudante acrescentado com sucesso!\n");

    return quantidadeAtual + 1;
}

void mostrarEstudantesEmMemoria(const tipoEstudante estudantes[], int quantidade) {
    int posicao;

    if (quantidade == 0) {
        printf("Nao existem estudantes em memoria.\n");
        return;
    }

    for (posicao = 0; posicao < quantidade; posicao++) {
        printf("%2d) Numero: %4d | Nome: %-20s | Nota: %2d\n",
               posicao + 1,
               estudantes[posicao].numero,
               estudantes[posicao].nome,
               estudantes[posicao].nota);
    }
}

/* Pergunta uma posicao e mostra o estudante que esta nessa posicao do ficheiro binario */
void mostrarEstudanteGravadoNoFicheiroBinario(void) {
    int quantidadeNoFicheiro = contarEstudantesNoFicheiroBinario();
    int posicaoEscolhida;
    tipoEstudante estudanteLido;

    if (quantidadeNoFicheiro <= 0) {
        printf("O ficheiro binario nao existe ou esta vazio.\n");
        return;
    }

    /* O utilizador conta a partir de 1, o ficheiro conta a partir de 0 */
    posicaoEscolhida = lerNumeroInteiro("Posicao do estudante no ficheiro", 1, quantidadeNoFicheiro);

    /* Passamos o ENDERECO (&) de estudanteLido para a funcao o preencher */
    if (lerEstudanteNaPosicao(posicaoEscolhida - 1, &estudanteLido)) {
        printf("Numero: %d | Nome: %s | Nota: %d\n",
               estudanteLido.numero, estudanteLido.nome, estudanteLido.nota);
    }
}

/* Altera a nota de um estudante diretamente no ficheiro binario */
void alterarNotaGravadaNoFicheiroBinario(void) {
    int quantidadeNoFicheiro = contarEstudantesNoFicheiroBinario();
    int posicaoEscolhida;
    int novaNota;

    if (quantidadeNoFicheiro <= 0) {
        printf("O ficheiro binario nao existe ou esta vazio.\n");
        return;
    }

    posicaoEscolhida = lerNumeroInteiro("Posicao do estudante no ficheiro", 1, quantidadeNoFicheiro);
    novaNota = lerNumeroInteiro("Nova nota", 0, 20);

    if (alterarNotaNoFicheiroBinario(posicaoEscolhida - 1, novaNota)) {
        printf("Nota alterada no ficheiro! (A memoria nao mudou: use a opcao 8 para recarregar.)\n");
        acrescentarMensagemAoRegisto("Alterou uma nota no ficheiro binario");
    }
}

/* Le um novo estudante e acrescenta-o diretamente ao fim do ficheiro binario */
void acrescentarNovoEstudanteAoFicheiroBinario(void) {
    tipoEstudante novoEstudante = lerDadosDeUmEstudante();

    /* Passamos o ENDERECO do estudante (a funcao recebe um ponteiro) */
    if (acrescentarEstudanteAoFicheiroBinario(&novoEstudante)) {
        printf("Estudante acrescentado ao ficheiro! (Use a opcao 8 para recarregar a memoria.)\n");
        acrescentarMensagemAoRegisto("Acrescentou um estudante ao ficheiro binario");
    }
}

/* Muda o nome do ficheiro de texto para "estudantes_backup.txt" */
void renomearFicheiroTextoParaBackup(void) {
    /* rename falha no Windows se o destino ja existir, por isso apagamos
     * primeiro o backup antigo. remove devolve 0 se apagou, diferente de 0 se falhou
     * (nao e problema: pode simplesmente nao existir backup antigo). */
    remove(NOME_FICHEIRO_BACKUP);

    /* rename(nome_antigo, nome_novo) devolve 0 se correu bem */
    if (rename(NOME_FICHEIRO_TEXTO, NOME_FICHEIRO_BACKUP) == 0) {
        printf("'%s' foi renomeado para '%s'.\n", NOME_FICHEIRO_TEXTO, NOME_FICHEIRO_BACKUP);
        acrescentarMensagemAoRegisto("Renomeou o ficheiro de texto para backup");
    } else {
        printf("ERRO! Nao foi possivel renomear (o ficheiro '%s' existe?).\n", NOME_FICHEIRO_TEXTO);
    }
}

/* Apaga um ficheiro e diz se conseguiu */
void apagarFicheiro(const char nomeDoFicheiro[]) {
    /* remove(nome) apaga o ficheiro. Devolve 0 se correu bem. */
    if (remove(nomeDoFicheiro) == 0) {
        printf("Apagado: %s\n", nomeDoFicheiro);
    } else {
        printf("Nao foi apagado (nao existe?): %s\n", nomeDoFicheiro);
    }
}

void apagarTodosOsFicheiros(void) {
    apagarFicheiro(NOME_FICHEIRO_TEXTO);
    apagarFicheiro(NOME_FICHEIRO_BINARIO);
    apagarFicheiro(NOME_FICHEIRO_COPIA);
    apagarFicheiro(NOME_FICHEIRO_BACKUP);
    apagarFicheiro(NOME_FICHEIRO_REGISTO);
}
