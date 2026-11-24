#include <stdio.h>
#include "funcoesgenericas.h"
#include "funcoesalunos.h"

int main(void) {
    tipoEstudante turma[MAX_ESTUDANTES];
    int numAlunos = 0;
    int numAval = 0;
    int lidos;
    float percPos = 0;
    char op;

    // sem buffer: o texto aparece logo
    setvbuf(stdout, NULL, _IONBF, 0);

    do {
        contas(turma, numAlunos, &numAval, &percPos);
        op = menu(numAlunos, numAval, percPos);
        switch (op) {
            case 'A':
                numAlunos = acrescentaEstudantes(turma, numAlunos);
                continuar();
                break;
            case 'I':
                lerNotas(turma, numAlunos);
                continuar();
                break;
            case 'N':
                mudarNota(turma, numAlunos);
                continuar();
                break;
            case 'M':
                mostrarDados(turma, numAlunos);
                if (numAval > 0) {
                    printf("Media das notas: %.2f\n", obterMedia(turma, numAlunos));
                    printf("Notas positivas (%%): %.2f\n", percPos);
                }
                continuar();
                break;
            case 'G':
                gravaFicheiroBinario(turma, numAlunos);
                gravaFicheiroTexto(turma, numAlunos);
                continuar();
                break;
            case 'L':
                lidos = leFicheiroBinario(turma);
                if (lidos >= 0) { // so substitui os dados se a leitura correu bem
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