#include <stdio.h>
#include "funcoesgenericas.h"
#include "funcoesalunos.h"

int main(void) {
    tipoEstudante vetorEstudantes[MAX_ESTUDANTES];
    int numAlunos = 0;
    int numAval = 0;
    int lidos;
    float percPos = 0;
    char op;

    do {
        contas(vetorEstudantes, numAlunos, &numAval, &percPos);
        op = menu(numAlunos, numAval, percPos);
        switch (op) {
            case 'A':
                numAlunos = acrescentaEstudantes(vetorEstudantes, numAlunos);
                continuar();
                break;
            case 'I':
                lerNotas(vetorEstudantes, numAlunos);
                continuar();
                break;
            case 'N':
                mudarNota(vetorEstudantes, numAlunos);
                continuar();
                break;
            case 'M':
                mostrarDados(vetorEstudantes, numAlunos);
                if (numAval > 0) {
                    printf("Media das notas: %.2f\n", obterMedia(vetorEstudantes, numAlunos));
                    printf("Notas positivas (%%): %.2f\n", percPos);
                }
                continuar();
                break;
            case 'G':
                gravaFicheiroBinario(vetorEstudantes, numAlunos);
                gravaFicheiroTexto(vetorEstudantes, numAlunos);
                continuar();
                break;
            case 'L':
                lidos = leFicheiroBinario(vetorEstudantes);
                if (lidos >= 0) {
                    numAlunos = lidos;
                }
                continuar();
                break;
            case 'F':
                printf("Fim do programa!\n");
                continuar();
                break;
            default:
                printf("Erro!\n");
                continuar();
        }
    } while (op != 'F');
    return 0;
}