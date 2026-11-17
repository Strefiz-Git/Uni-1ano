#ifndef FICHEIROS_BINARIOS_H
#define FICHEIROS_BINARIOS_H

#include "estudante.h"

/* ==================================================================
 * FICHEIROS BINARIOS
 * Funcoes usadas: fopen, fclose, fwrite, fread, fseek, ftell, rewind
 *
 * Formato do ficheiro "estudantes.dat":
 *     [ int quantidade ][ estudante 0 ][ estudante 1 ][ estudante 2 ] ...
 *
 * As posicoes dos estudantes comecam em 0 (como nos vetores).
 *
 * Convencao dos valores devolvidos:
 *   - funcoes que gravam/alteram/leem um registo: 1 se correu bem, 0 se falhou
 *   - funcoes que devolvem quantidades: -1 se falhou
 * ================================================================== */

/* Grava a quantidade e depois todos os estudantes. Apaga o que havia antes. */
int gravarEstudantesEmFicheiroBinario(const tipoEstudante estudantes[], int quantidadeDeEstudantes);

/* Le a quantidade e os estudantes. Devolve a quantidade lida, ou -1 se falhou. */
int lerEstudantesDeFicheiroBinario(tipoEstudante estudantes[]);

/* Conta quantos estudantes existem no ficheiro SEM os ler (usa fseek e ftell). */
int contarEstudantesNoFicheiroBinario(void);

/* Le apenas o estudante que esta numa determinada posicao (acesso direto). */
int lerEstudanteNaPosicao(int posicao, tipoEstudante *estudanteLido);

/* Altera apenas a nota de um estudante, diretamente no ficheiro. */
int alterarNotaNoFicheiroBinario(int posicao, int novaNota);

/* Acrescenta um estudante ao fim do ficheiro e atualiza a quantidade. */
int acrescentarEstudanteAoFicheiroBinario(const tipoEstudante *novoEstudante);

#endif
