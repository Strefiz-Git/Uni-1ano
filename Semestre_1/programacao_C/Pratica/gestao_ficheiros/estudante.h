#ifndef ESTUDANTE_H
#define ESTUDANTE_H

/* ------------------------------------------------------------------
 * Constantes e tipos partilhados por todo o projeto
 * ------------------------------------------------------------------ */

#define MAX_ESTUDANTES 100   /* quantidade maxima de estudantes em memoria */
#define TAMANHO_NOME 50      /* tamanho do vetor do nome (49 letras + '\0') */

/* Nomes dos ficheiros usados pelo programa.
 * Sao criados na "pasta de trabalho" (no CLion: cmake-build-debug). */
#define NOME_FICHEIRO_TEXTO "estudantes.txt"
#define NOME_FICHEIRO_BINARIO "estudantes.dat"
#define NOME_FICHEIRO_COPIA "estudantes_copia.txt"
#define NOME_FICHEIRO_BACKUP "estudantes_backup.txt"
#define NOME_FICHEIRO_REGISTO "registo.log"

/* Dados de um estudante.
 * IMPORTANTE: o nome e um VETOR de caracteres (e nao um ponteiro),
 * por isso a estrutura pode ser gravada inteira num ficheiro binario. */
typedef struct {
    int numero;
    char nome[TAMANHO_NOME];
    int nota;
} tipoEstudante;

#endif
