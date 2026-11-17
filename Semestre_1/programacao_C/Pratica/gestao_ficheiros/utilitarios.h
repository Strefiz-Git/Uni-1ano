#ifndef UTILITARIOS_H
#define UTILITARIOS_H

/* Funcoes auxiliares para ler dados do teclado de forma segura. */

/* Descarta tudo o que ficou por ler no teclado ate ao ENTER. */
void limparBufferDoTeclado(void);

/* Mostra "Pressione ENTER para continuar..." e espera pelo ENTER. */
void esperarPorEnter(void);

/* Pede um numero inteiro entre valorMinimo e valorMaximo.
 * Repete a pergunta ate o utilizador escrever um valor valido. */
int lerNumeroInteiro(const char mensagem[], int valorMinimo, int valorMaximo);

/* Pede um texto (com espacos) que nao pode ficar vazio. */
void lerTextoDoTeclado(const char mensagem[], char textoLido[], int tamanhoMaximo);

/* Troca todas as ocorrencias de um caracter por outro, dentro de um texto. */
void substituirCaracterNoTexto(char texto[], char caracterAntigo, char caracterNovo);

#endif
