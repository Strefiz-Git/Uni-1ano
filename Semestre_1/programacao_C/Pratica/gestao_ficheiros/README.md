# gestao_ficheiros - Projeto CLion (linguagem C)

Programa de menu para experimentar as funcoes base de ficheiros em C,
usando uma estrutura `tipoEstudante` (numero, nome, nota).

## Como abrir no CLion
1. `File > Open...` e escolhe a pasta `gestao_ficheiros` (a que tem o `CMakeLists.txt`).
2. Aceita "Trust Project" e espera que o CMake carregue.
3. Carrega em Run (shift+F10).

Os ficheiros criados pelo programa (`estudantes.txt`, `estudantes.dat`, `registo.log`...)
aparecem na **pasta de trabalho**, normalmente `cmake-build-debug`.
Para a mudar: `Run > Edit Configurations > Working directory`.

## Organizacao dos ficheiros do projeto
| Ficheiro | Para que serve |
|---|---|
| `main.c` | menu e ligacao de tudo |
| `estudante.h` | constantes, nomes dos ficheiros e `tipoEstudante` |
| `utilitarios.c/.h` | ler numeros e textos do teclado em seguranca |
| `ficheiros_texto.c/.h` | funcoes de ficheiros de TEXTO |
| `ficheiros_binarios.c/.h` | funcoes de ficheiros BINARIOS |

## Onde esta cada funcao base
| Funcao | Onde ver | Opcao do menu |
|---|---|---|
| `fopen` / `fclose` | em todas as funcoes dos ficheiros | - |
| `fprintf` | `escreverEstudantesEmFicheiroTexto` | 3 |
| `fgets` | `lerEstudantesDeFicheiroTexto` (cabecalho), `mostrarFicheiroTextoLinhaALinha` | 4, 5 |
| `fscanf` | `lerEstudantesDeFicheiroTexto` | 4 |
| `fgetc` / `fputc` | `copiarFicheiroCaracterACaracter` | 6 |
| `fputs` + modo `"a"` | `acrescentarMensagemAoRegisto` | 13 |
| `fwrite` | `gravarEstudantesEmFicheiroBinario` | 7 |
| `fread` | `lerEstudantesDeFicheiroBinario` | 8 |
| `fseek` + `ftell` | `contarEstudantesNoFicheiroBinario` | 9 |
| `fseek` (SEEK_SET) | `lerEstudanteNaPosicao` | 10 |
| modo `"rb+"` + `SEEK_CUR` | `alterarNotaNoFicheiroBinario` | 11 |
| `SEEK_END` + `rewind` | `acrescentarEstudanteAoFicheiroBinario` | 12 |
| `rename` / `remove` | `renomearFicheiroTextoParaBackup`, `apagarFicheiro` | 14, 15 |

## Ordem sugerida para experimentar
1. Opcao 1 duas ou tres vezes, para criar estudantes.
2. Opcao 3 e opcao 5: grava em texto e ve o ficheiro. Abre tambem o `.txt` no CLion.
3. Opcao 7 e opcao 9: grava em binario e conta os estudantes.
4. Opcao 10, 11 e 12: acesso direto ao ficheiro binario.
5. Opcao 8 para recarregar a memoria e opcao 2 para ver as alteracoes.
6. Opcao 13 para ver o registo (log) de tudo o que fizeste.

## Resumo dos modos de fopen
| Modo | Efeito |
|---|---|
| `"r"` / `"rb"` | ler, o ficheiro tem de existir |
| `"w"` / `"wb"` | escrever, APAGA o conteudo anterior |
| `"a"` / `"ab"` | escrever no fim, nunca apaga |
| `"r+"` / `"rb+"` | ler e escrever, sem apagar |
