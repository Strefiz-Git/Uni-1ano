#ifndef FICHEIROS_TEXTO_H
#define FICHEIROS_TEXTO_H

#include "estudante.h"

/* ==================================================================
 * FICHEIROS DE TEXTO
 * Funcoes usadas: fopen, fclose, fprintf, fscanf, fgets, fgetc,
 *                 fputc, fputs
 *
 * Convencao dos valores devolvidos:
 *   - funcoes que gravam/copiam: devolvem 1 se correu bem, 0 se falhou
 *   - lerEstudantesDeFicheiroTexto: devolve a quantidade lida, ou -1 se falhou
 * ================================================================== */

/* Grava os estudantes em "estudantes.txt" (uma linha por estudante). */
int escreverEstudantesEmFicheiroTexto(const tipoEstudante estudantes[], int quantidadeDeEstudantes);

/* Le os estudantes de "estudantes.txt" para o vetor.
 * capacidadeMaxima = quantos estudantes cabem no vetor. */
int lerEstudantesDeFicheiroTexto(tipoEstudante estudantes[], int capacidadeMaxima);

/* Mostra no ecra o conteudo de um ficheiro de texto, linha a linha. */
int mostrarFicheiroTextoLinhaALinha(const char nomeDoFicheiro[]);

/* Copia um ficheiro de texto caracter a caracter. */
int copiarFicheiroCaracterACaracter(const char nomeDoFicheiroOrigem[], const char nomeDoFicheiroDestino[]);

/* Acrescenta uma linha ao fim do ficheiro de registo (log). */
int acrescentarMensagemAoRegisto(const char mensagem[]);

#endif
