/* ====================================================================
 * KIT BASE DE FUNCOES - Programacao I
 *
 * Funcoes "modulares" que aparecem em quase todos os programas.
 * Tudo num unico ficheiro (sem .h), escrito em estilo C: funciona
 * em .cpp e em .c.
 *
 * COMO TRABALHAR:
 *   1. Le a especificacao de cada funcao (no fim do ficheiro, na zona
 *      "A TUA PARTE") e escreve o corpo no lugar do TODO.
 *   2. Corre o programa e usa o menu para testar.
 *      - Vetores, texto e ficheiros tem testes AUTOMATICOS ([OK]/[FALHOU]).
 *      - Teclado e interativo: tenta partir as tuas funcoes (letras,
 *        valores fora do intervalo, textos enormes...).
 *   3. Faz por esta ordem: teclado -> vetores -> texto -> ficheiros.
 *
 * REGRAS:
 *   - Nao uses variaveis globais nas tuas funcoes.
 *   - Cada funcao faz UMA coisa. As mais complexas usam as mais simples
 *     (ex: lerInteiroEntre usa lerInteiro).
 *   - Nomes legiveis e comentarios nos passos importantes.
 * ==================================================================== */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* ------------------------ KIT: TECLADO ------------------------ */
void limparBufferTeclado(void);
int lerInteiro(const char mensagem[]);
int lerInteiroEntre(const char mensagem[], int minimo, int maximo);
float lerRealEntre(const char mensagem[], float minimo, float maximo);
char lerOpcaoValida(const char mensagem[], const char opcoesValidas[]);
void lerTexto(const char mensagem[], char texto[], int tamanhoMaximo);
int lerSimOuNao(const char mensagem[]);
void esperarPorEnter(void);

/* ------------------------ KIT: VETORES ------------------------ */
int somarVetor(const int vetor[], int tamanho);
float calcularMedia(const int vetor[], int tamanho);
int posicaoDoMaior(const int vetor[], int tamanho);
int procurarValor(const int vetor[], int tamanho, int valorProcurado);
void trocarInteiros(int *primeiro, int *segundo);
void ordenarVetor(int vetor[], int tamanho);
int removerPosicao(int vetor[], int tamanho, int posicao);

/* ------------------------ KIT: TEXTO ------------------------ */
int contarVogais(const char texto[]);
void converterParaMaiusculas(char texto[]);

/* ------------------------ KIT: FICHEIROS ------------------------ */
int ficheiroExiste(const char nomeDoFicheiro[]);
int gravarVetorEmTexto(const char nomeDoFicheiro[], const int vetor[], int tamanho);
int lerVetorDeTexto(const char nomeDoFicheiro[], int vetor[], int capacidade);
int gravarVetorEmBinario(const char nomeDoFicheiro[], const int vetor[], int tamanho);
int lerVetorDeBinario(const char nomeDoFicheiro[], int vetor[], int capacidade);
int acrescentarLinhaAoFicheiro(const char nomeDoFicheiro[], const char linha[]);
int contarLinhasDoFicheiro(const char nomeDoFicheiro[]);

/* ------------------------ TESTES (nao mexer) ------------------------ */
void testarTeclado(void);
void testarVetores(void);
void testarTexto(void);
void testarFicheiros(void);
void verificar(const char descricao[], int condicao);
void mostrarResumoDosTestes(void);

int testesFeitos = 0;
int testesCertos = 0;

int main(void) {
    int opcao = -1;
    int caracterLido;

    setvbuf(stdout, NULL, _IONBF, 0);

    do {
        printf("\n========== KIT BASE: MENU DE TESTES ==========\n");
        printf("1 - Testar TECLADO   (interativo)\n");
        printf("2 - Testar VETORES   (automatico)\n");
        printf("3 - Testar TEXTO     (automatico)\n");
        printf("4 - Testar FICHEIROS (automatico)\n");
        printf("0 - Sair\n");
        printf("Opcao: ");

        /* Este menu NAO usa nenhuma funcao do kit DE PROPOSITO,
         * para funcionar mesmo antes de escreveres as tuas funcoes. */
        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
        }
        do {
            caracterLido = getchar();
        } while (caracterLido != '\n' && caracterLido != EOF);

        switch (opcao) {
            case 1: testarTeclado(); break;
            case 2: testarVetores(); break;
            case 3: testarTexto(); break;
            case 4: testarFicheiros(); break;
            case 0: printf("Ate ja!\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}

/* ====================================================================
 * ===================== A TUA PARTE COMECA AQUI ======================
 * ==================================================================== */


/* ####################### 1. TECLADO ####################### */

/* ---------------------------------------------------------------
 * limparBufferTeclado
 * O QUE FAZ: descarta tudo o que ficou por ler no teclado, ate ao ENTER.
 * PARA QUE SERVE: depois de um scanf fica sempre um '\n' no buffer, que
 *                 estraga o proximo fgets/scanf(" %c").
 * DICA: ciclo com getchar(). A variavel que guarda o resultado do
 *       getchar tem de ser int (porque do EOF). Para quando ler '\n' ou EOF.
 * --------------------------------------------------------------- */
void limparBufferTeclado(void) {
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

/* ---------------------------------------------------------------
 * lerInteiro   (o teu "getInt")
 * O QUE FAZ: mostra a mensagem, le um inteiro e devolve-o.
 *            Se o utilizador escrever letras, avisa e repete.
 * DICA: o scanf DEVOLVE quantos valores leu: 1 = leu o inteiro, 0 = falhou.
 *       Usa esse valor como condicao do ciclo do...while.
 *       Chama limparBufferTeclado() depois de cada scanf.
 * Se a entrada terminar (scanf devolve EOF), devolve 0 para nao ficar preso.
 * --------------------------------------------------------------- */
int lerInteiro(const char mensagem[]) {
    int valor = 0;
    int valoresLidos;
    do {
        printf("%s: ", mensagem);
        valoresLidos = scanf("%d", &valor);
        limparBufferTeclado();
        if (valoresLidos == EOF) {
            return 0;
        }
        if (valoresLidos != 1) {
            printf("ERRO! Introduza um numero inteiro.\n");
        }
    } while (valoresLidos != 1);
    return valor;
}

/* ---------------------------------------------------------------
 * lerInteiroEntre   (o teu "getIntLimit")
 * O QUE FAZ: le um inteiro e so o aceita se estiver entre minimo e maximo
 *            (inclusive). Caso contrario avisa e repete.
 * DICA: NAO repitas o codigo do scanf! Chama lerInteiro dentro de um
 *       do...while e testa so o intervalo.
 * --------------------------------------------------------------- */
int lerInteiroEntre(const char mensagem[], int minimo, int maximo) {
    int valor;

    do {
        /* O lerInteiro ja trata de letras e de limpar o buffer */
        valor = lerInteiro(mensagem);

        if (valor < minimo || valor > maximo) {
            printf("ERRO! Introduza um numero inteiro entre %d e %d.\n", minimo, maximo);
        }
    } while (valor < minimo || valor > maximo);

    return valor;
}

float lerReal(const char mensagem[]) {
    float valor = 0;
    int valoresLidos;
    do {
        printf("%s: ", mensagem);
        valoresLidos = scanf("%f", &valor);
        limparBufferTeclado();
        if (valoresLidos == EOF) {
            return 0;
        }
        if (valoresLidos != 1) {
            printf("ERRO! Introduza um numero real.\n");
        }
    } while (valoresLidos != 1);
    return valor;
}

/* ---------------------------------------------------------------
 * lerRealEntre
 * O QUE FAZ: igual ao lerInteiroEntre, mas para numeros com virgula (float).
 * DICA: scanf("%f", ...). Atencao ao tipo de variavel e ao %f.
 * --------------------------------------------------------------- */
float lerRealEntre(const char mensagem[], float minimo, float maximo) {
    float valor;

    do {
        valor = lerReal(mensagem);

        if (valor < minimo || valor > maximo) {
            printf("ERRO! Introduza um numero real entre %.4f e %.4f.\n", minimo, maximo);
        }
    } while (valor < minimo || valor > maximo);
    return valor;
}

/* ---------------------------------------------------------------
 * lerOpcaoValida
 * O QUE FAZ: le UM caracter e so o aceita se for um dos de opcoesValidas.
 *            Devolve-o sempre em MAIUSCULA ('i' passa a 'I').
 * EXEMPLO: lerOpcaoValida("Opcao", "IDS") aceita i, I, d, D, s, S.
 * DICA: scanf(" %c", ...) (o espaco antes do %c e importante!),
 *       toupper() (ctype.h) e strchr() (string.h) para ver se o
 *       caracter existe dentro de opcoesValidas.
 * --------------------------------------------------------------- */
char lerCaracter(const char mensagem[]) {
    char valor;
    int valoresLidos;
    do {
        printf("%s", mensagem);
        valoresLidos = scanf(" %c", &valor);
        limparBufferTeclado();
        if (valoresLidos == EOF) {
            return 0;
        }
    }while (valoresLidos != 1);
    return valor;
}

char lerCaracterToUpper(const char mensagem[]) {
    char valor;
    valor = lerCaracter(mensagem);
    valor = (char) toupper(valor);
    return valor;
}

char lerCaracterToLower(const char mensagem[]) {
    char valor;
    valor = lerCaracter(mensagem);
    valor = (char) tolower(valor);
    return valor;
}

char lerOpcaoValida(const char mensagem[], const char opcoesValidas[]) {
    char valor;
    do {
        valor = lerCaracterToUpper(mensagem);
    }while (strchr(opcoesValidas, valor) == NULL);
    return valor;
}

/* ---------------------------------------------------------------
 * lerTexto
 * O QUE FAZ: le uma linha de texto (com espacos) para o vetor "texto".
 *   - nunca ultrapassa tamanhoMaximo (incluindo o '\0')
 *   - remove o '\n' que o fgets deixa no fim
 *   - se o utilizador escrever mais do que cabe, descarta o resto
 *   - nao aceita texto vazio (repete)
 * DICA: fgets(texto, tamanhoMaximo, stdin). Se a linha NAO contem '\n'
 *       (strchr(texto, '\n') == NULL), ficou lixo no buffer: limpa-o.
 *       Se contem, troca o '\n' por '\0'  (truque: strcspn(texto, "\n")).
 * --------------------------------------------------------------- */
void lerTexto(const char mensagem[], char texto[], int tamanhoMaximo) {
    /* TODO */
}

/* ---------------------------------------------------------------
 * lerSimOuNao
 * O QUE FAZ: devolve 1 se o utilizador responder S/s e 0 se responder N/n.
 * DICA: e UMA linha, usando a lerOpcaoValida com "SN".
 *       (A mensagem que passares deve incluir "(S/N)".)
 * --------------------------------------------------------------- */
int lerSimOuNao(const char mensagem[]) {
    /* TODO */
    return 0;
}

/* ---------------------------------------------------------------
 * esperarPorEnter
 * O QUE FAZ: mostra "Pressione ENTER para continuar..." e espera pelo ENTER.
 * DICA: ja tens a funcao que espera por um ENTER: limparBufferTeclado.
 * --------------------------------------------------------------- */
void esperarPorEnter(void) {
    /* TODO */
}


/* ####################### 2. VETORES ####################### */
/* (todas trabalham com um vetor de int + o seu "tamanho" = quantos
 *  elementos estao realmente preenchidos) */

/* ---------------------------------------------------------------
 * somarVetor
 * O QUE FAZ: devolve a soma dos primeiros "tamanho" elementos. Vetor vazio = 0.
 * --------------------------------------------------------------- */
int somarVetor(const int vetor[], int tamanho) {
    /* TODO */
    return -1;
}

/* ---------------------------------------------------------------
 * calcularMedia
 * O QUE FAZ: devolve a media dos elementos (com casas decimais!).
 *            Vetor vazio devolve 0 (nao pode dividir por zero).
 * DICA: reutiliza somarVetor. Cuidado com a divisao inteira: usa (float).
 * --------------------------------------------------------------- */
float calcularMedia(const int vetor[], int tamanho) {
    /* TODO */
    return -1;
}

/* ---------------------------------------------------------------
 * posicaoDoMaior
 * O QUE FAZ: devolve a POSICAO (nao o valor!) do maior elemento.
 *            Se houver empate, devolve a primeira. Vetor vazio = -1.
 * DICA: guarda a posicao do maior ate agora e compara os restantes com ele.
 * --------------------------------------------------------------- */
int posicaoDoMaior(const int vetor[], int tamanho) {
    /* TODO */
    return -2;
}

/* ---------------------------------------------------------------
 * procurarValor   (pesquisa sequencial)
 * O QUE FAZ: devolve a posicao da PRIMEIRA ocorrencia de valorProcurado,
 *            ou -1 se nao existir.
 * NOTA: este padrao "devolver a posicao ou -1" e o mesmo da procuraEstudante
 *       das fichas, e aparece em muitos testes!
 * --------------------------------------------------------------- */
int procurarValor(const int vetor[], int tamanho, int valorProcurado) {
    /* TODO */
    return -2;
}

/* ---------------------------------------------------------------
 * trocarInteiros   (passagem por REFERENCIA)
 * O QUE FAZ: troca os valores das duas variaveis que receber.
 * DICA: parametros com *, e variavel auxiliar. Na chamada: &a, &b.
 * --------------------------------------------------------------- */
void trocarInteiros(int *primeiro, int *segundo) {
    /* TODO */
}

/* ---------------------------------------------------------------
 * ordenarVetor
 * O QUE FAZ: ordena o vetor por ordem CRESCENTE (bubble sort).
 * DICA: dois ciclos for. No interior compara elementos vizinhos
 *       e, se estiverem na ordem errada, usa trocarInteiros.
 * --------------------------------------------------------------- */
void ordenarVetor(int vetor[], int tamanho) {
    /* TODO */
}

/* ---------------------------------------------------------------
 * removerPosicao
 * O QUE FAZ: remove o elemento da posicao indicada, "puxando" todos os
 *            seguintes uma posicao para tras. Devolve o NOVO tamanho.
 *            Se a posicao for invalida (< 0 ou >= tamanho), nao altera nada
 *            e devolve o tamanho original.
 * DICA: ciclo de "posicao" ate "tamanho - 2": vetor[i] = vetor[i + 1].
 * --------------------------------------------------------------- */
int removerPosicao(int vetor[], int tamanho, int posicao) {
    /* TODO */
    return -1;
}


/* ####################### 3. TEXTO ####################### */

/* ---------------------------------------------------------------
 * contarVogais
 * O QUE FAZ: devolve quantas vogais (a e i o u, maiusculas ou minusculas)
 *            o texto tem.
 * DICA: percorre ate encontrar '\0'. toupper() simplifica a comparacao.
 * --------------------------------------------------------------- */
int contarVogais(const char texto[]) {
    /* TODO */
    return -1;
}

/* ---------------------------------------------------------------
 * converterParaMaiusculas
 * O QUE FAZ: passa todas as letras do texto para maiusculas (altera o proprio vetor).
 * --------------------------------------------------------------- */
void converterParaMaiusculas(char texto[]) {
    /* TODO */
}


/* ####################### 4. FICHEIROS ####################### */
/* Convencao: as funcoes devolvem 1 se correu bem e 0 se falhou,
 * exceto as de leitura, que devolvem a quantidade lida (ou -1 se falhou). */

/* ---------------------------------------------------------------
 * ficheiroExiste
 * O QUE FAZ: devolve 1 se o ficheiro existe, 0 se nao existe.
 * DICA: tenta abrir em modo "r". Se der NULL, nao existe.
 *       Se abriste, nao te esquecas do fclose!
 * --------------------------------------------------------------- */
int ficheiroExiste(const char nomeDoFicheiro[]) {
    FILE *f = fopen(nomeDoFicheiro, "r");

    if (f == NULL) {
        return 0;
    }

    fclose(f);
    return 1;
}

/* ---------------------------------------------------------------
 * gravarVetorEmTexto
 * O QUE FAZ: grava os "tamanho" elementos num ficheiro de TEXTO,
 *            um numero por linha. Apaga o que la estava.
 * DICA: modo "w", fprintf(f, "%d\n", ...), fclose. Testa o NULL.
 * --------------------------------------------------------------- */
int gravarVetorEmTexto(const char nomeDoFicheiro[], const int vetor[], int tamanho) {
    FILE *f = fopen(nomeDoFicheiro, "w");
    int i;

    if (f == NULL) {
        return 0;
    }

    for (i = 0; i < tamanho; i++) {
        fprintf(f, "%d\n", vetor[i]);
    }

    fclose(f);
    return 1;
}

/* ---------------------------------------------------------------
 * lerVetorDeTexto
 * O QUE FAZ: le os numeros (um por linha) para o vetor, ate ao fim do
 *            ficheiro OU ate encher a capacidade. Devolve quantos leu,
 *            ou -1 se nao conseguiu abrir o ficheiro.
 * DICA: modo "r". Ciclo while com duas condicoes:
 *           quantidade < capacidade  &&  fscanf(f, "%d", &vetor[quantidade]) == 1
 *       NAO uses feof() como condicao do ciclo!
 * --------------------------------------------------------------- */
int lerVetorDeTexto(const char nomeDoFicheiro[], int vetor[], int capacidade) {
    FILE *f = fopen(nomeDoFicheiro, "r");
    int quantidade = 0;

    if (f == NULL) {
        return -1;
    }

    while (quantidade < capacidade && fscanf(f, "%d", &vetor[quantidade]) == 1) {
        quantidade++;
    }

    fclose(f);
    return quantidade;
}

/* ---------------------------------------------------------------
 * gravarVetorEmBinario
 * O QUE FAZ: grava PRIMEIRO a quantidade (um int) e DEPOIS o vetor todo,
 *            num ficheiro BINARIO. Apaga o que la estava.
 *            Devolve 1 se gravou tudo, 0 se falhou.
 * DICA: modo "wb", dois fwrite. Confirma o valor devolvido pelo fwrite
 *       do vetor (deve ser igual a tamanho).
 * --------------------------------------------------------------- */
int gravarVetorEmBinario(const char nomeDoFicheiro[], const int vetor[], int tamanho) {
    /* TODO */
    return -1;
}

/* ---------------------------------------------------------------
 * lerVetorDeBinario
 * O QUE FAZ: le a quantidade e depois os elementos para o vetor.
 *            Devolve a quantidade lida, ou -1 se: o ficheiro nao abre,
 *            esta incompleto, ou a quantidade NAO CABE na capacidade.
 * DICA: modo "rb". fread da quantidade (testa se devolveu 1), valida-a,
 *       fread do vetor (testa se devolveu a quantidade certa).
 * --------------------------------------------------------------- */
int lerVetorDeBinario(const char nomeDoFicheiro[], int vetor[], int capacidade) {
    /* TODO */
    return -2;
}

/* ---------------------------------------------------------------
 * acrescentarLinhaAoFicheiro   (ficheiro de registo / log)
 * O QUE FAZ: escreve a linha no FIM do ficheiro (mudando de linha) sem
 *            apagar o que la esta. Cria o ficheiro se nao existir.
 * DICA: o modo "a" faz tudo isto.
 * --------------------------------------------------------------- */
int acrescentarLinhaAoFicheiro(const char nomeDoFicheiro[], const char linha[]) {
    /* TODO */
    return -1;
}

/* ---------------------------------------------------------------
 * contarLinhasDoFicheiro
 * O QUE FAZ: devolve o numero de linhas (conta os '\n') do ficheiro de
 *            texto, ou -1 se nao conseguiu abrir.
 * DICA: fgetc em ciclo ate EOF (a variavel e int!), conta os '\n'.
 * --------------------------------------------------------------- */
int contarLinhasDoFicheiro(const char nomeDoFicheiro[]) {
    /* TODO */
    return -2;
}

/* ====================================================================
 * ====================== A TUA PARTE ACABA AQUI ======================
 * ==================================================================== */


/* ====================================================================
 * ============================ TESTES ================================
 * (nao precisas de mexer daqui para baixo, mas le para perceberes
 *  como se testam funcoes!)
 * ==================================================================== */

/* Mostra [OK] ou [FALHOU] e conta os resultados */
void verificar(const char descricao[], int condicao) {
    testesFeitos++;
    if (condicao) {
        testesCertos++;
        printf("  [OK]     %s\n", descricao);
    } else {
        printf("  [FALHOU] %s\n", descricao);
    }
}

void mostrarResumoDosTestes(void) {
    printf("  ---> %d de %d testes certos\n", testesCertos, testesFeitos);
    testesFeitos = 0;
    testesCertos = 0;
}

void testarTeclado(void) {
    int numero;
    float real;
    char opcao;
    char texto[20];
    int resposta;

    printf("\n--- TESTE INTERATIVO DO TECLADO ---\n");
    printf("Tenta partir as funcoes: escreve letras, valores fora do\n");
    printf("intervalo, 'i' minusculo, e um texto com mais de 19 caracteres.\n\n");

    numero = lerInteiro("Escreva um inteiro");
    printf("Leu: %d\n\n", numero);

    numero = lerInteiroEntre("Idade (0 a 120)", 0, 120);
    printf("Leu: %d\n\n", numero);

    real = lerRealEntre("Nota (0 a 20)", 0.0f, 20.0f);
    printf("Leu: %.2f\n\n", real);

    opcao = lerOpcaoValida("Opcao: (I)ncrementar, (D)ecrementar, (S)air", "IDS");
    printf("Leu: '%c' (tem de vir sempre em maiuscula)\n\n", opcao);

    lerTexto("Nome (ate 19 caracteres, pode ter espacos): ", texto, 20);
    printf("Leu: [%s]  (os [] devem colar ao texto, sem mudanca de linha)\n\n", texto);

    resposta = lerSimOuNao("Quer continuar? (S/N)");
    printf("Leu: %d (1 = sim, 0 = nao)\n", resposta);

    esperarPorEnter();
}

void testarVetores(void) {
    int numeros[6] = {4, 9, 2, 9, 7, 1};
    int copia[6];
    int vazio[1] = {0};
    int a = 3;
    int b = 8;
    int i;
    int novoTamanho;

    printf("\n--- TESTES DOS VETORES ---\n");

    verificar("somarVetor soma os elementos", somarVetor(numeros, 6) == 32);
    verificar("somarVetor so soma 'tamanho' elementos", somarVetor(numeros, 3) == 15);
    verificar("somarVetor com tamanho 0 devolve 0", somarVetor(vazio, 0) == 0);

    verificar("calcularMedia tem casas decimais (32/6)",
              calcularMedia(numeros, 6) > 5.33f && calcularMedia(numeros, 6) < 5.34f);
    verificar("calcularMedia com tamanho 0 devolve 0", calcularMedia(vazio, 0) == 0.0f);

    verificar("posicaoDoMaior devolve a POSICAO (e a 1.a em caso de empate)", posicaoDoMaior(numeros, 6) == 1);
    verificar("posicaoDoMaior com tamanho 0 devolve -1", posicaoDoMaior(vazio, 0) == -1);

    verificar("procurarValor encontra o 7 na posicao 4", procurarValor(numeros, 6, 7) == 4);
    verificar("procurarValor devolve a 1.a ocorrencia (9 -> posicao 1)", procurarValor(numeros, 6, 9) == 1);
    verificar("procurarValor devolve -1 se nao existe", procurarValor(numeros, 6, 5) == -1);
    verificar("procurarValor respeita o tamanho (7 nao esta nos 3 primeiros)", procurarValor(numeros, 3, 7) == -1);

    trocarInteiros(&a, &b);
    verificar("trocarInteiros troca os valores (3,8 -> 8,3)", a == 8 && b == 3);

    for (i = 0; i < 6; i++) {
        copia[i] = numeros[i];
    }
    ordenarVetor(copia, 6);
    verificar("ordenarVetor ordena por ordem crescente",
              copia[0] == 1 && copia[1] == 2 && copia[2] == 4 &&
              copia[3] == 7 && copia[4] == 9 && copia[5] == 9);
    verificar("ordenarVetor nao mexeu no vetor original", numeros[0] == 4 && numeros[5] == 1);

    novoTamanho = removerPosicao(copia, 6, 2);   /* remove o 4 de {1,2,4,7,9,9} */
    verificar("removerPosicao devolve o novo tamanho (5)", novoTamanho == 5);
    verificar("removerPosicao puxa os seguintes para tras ({1,2,7,9,9})",
              copia[0] == 1 && copia[1] == 2 && copia[2] == 7 && copia[3] == 9 && copia[4] == 9);
    verificar("removerPosicao com posicao invalida nao altera o tamanho", removerPosicao(copia, 5, 10) == 5);
    verificar("removerPosicao com posicao negativa nao altera o tamanho", removerPosicao(copia, 5, -1) == 5);
    verificar("removerPosicao da ultima posicao funciona", removerPosicao(copia, 5, 4) == 4);

    mostrarResumoDosTestes();
}

void testarTexto(void) {
    char frase[50] = "Programacao Em C";
    char vazia[5] = "";

    printf("\n--- TESTES DO TEXTO ---\n");

    verificar("contarVogais (Programacao Em C tem 6)", contarVogais(frase) == 6);
    verificar("contarVogais de texto vazio e 0", contarVogais(vazia) == 0);

    converterParaMaiusculas(frase);
    verificar("converterParaMaiusculas", strcmp(frase, "PROGRAMACAO EM C") == 0);
    verificar("contarVogais tambem conta maiusculas", contarVogais(frase) == 6);

    mostrarResumoDosTestes();
}

void testarFicheiros(void) {
    int numeros[5] = {10, 20, 30, 40, 50};
    int lidos[10];
    int quantidade;

    printf("\n--- TESTES DOS FICHEIROS ---\n");
    printf("(os ficheiros de teste sao criados na pasta de trabalho e apagados no fim)\n");

    /* Garante que partimos sem ficheiros de testes anteriores */
    remove("teste_vetor.txt");
    remove("teste_vetor.dat");
    remove("teste_log.txt");

    verificar("ficheiroExiste devolve 0 quando nao existe", ficheiroExiste("teste_vetor.txt") == 0);

    /* ---- texto ---- */
    verificar("gravarVetorEmTexto devolve 1", gravarVetorEmTexto("teste_vetor.txt", numeros, 5) == 1);
    verificar("ficheiroExiste devolve 1 quando existe", ficheiroExiste("teste_vetor.txt") == 1);
    verificar("contarLinhasDoFicheiro (1 numero por linha = 5 linhas)",
              contarLinhasDoFicheiro("teste_vetor.txt") == 5);

    quantidade = lerVetorDeTexto("teste_vetor.txt", lidos, 10);
    verificar("lerVetorDeTexto devolve 5", quantidade == 5);
    verificar("lerVetorDeTexto le os valores certos (10 ... 50)",
              quantidade == 5 && lidos[0] == 10 && lidos[2] == 30 && lidos[4] == 50);
    verificar("lerVetorDeTexto respeita a capacidade (3)", lerVetorDeTexto("teste_vetor.txt", lidos, 3) == 3);
    verificar("lerVetorDeTexto devolve -1 se o ficheiro nao existe",
              lerVetorDeTexto("nao_existe.txt", lidos, 10) == -1);
    verificar("gravarVetorEmTexto com tamanho 0 cria ficheiro vazio",
              gravarVetorEmTexto("teste_vetor.txt", numeros, 0) == 1 &&
              lerVetorDeTexto("teste_vetor.txt", lidos, 10) == 0);

    /* ---- binario ---- */
    verificar("gravarVetorEmBinario devolve 1", gravarVetorEmBinario("teste_vetor.dat", numeros, 5) == 1);

    quantidade = lerVetorDeBinario("teste_vetor.dat", lidos, 10);
    verificar("lerVetorDeBinario devolve 5", quantidade == 5);
    verificar("lerVetorDeBinario le os valores certos (10 ... 50)",
              quantidade == 5 && lidos[0] == 10 && lidos[2] == 30 && lidos[4] == 50);
    verificar("lerVetorDeBinario devolve -1 se nao cabe (capacidade 3)",
              lerVetorDeBinario("teste_vetor.dat", lidos, 3) == -1);
    verificar("lerVetorDeBinario devolve -1 se o ficheiro nao existe",
              lerVetorDeBinario("nao_existe.dat", lidos, 10) == -1);

    /* ---- registo (log) ---- */
    verificar("acrescentarLinhaAoFicheiro cria o ficheiro", acrescentarLinhaAoFicheiro("teste_log.txt", "primeira") == 1);
    verificar("acrescentarLinhaAoFicheiro acrescenta sem apagar",
              acrescentarLinhaAoFicheiro("teste_log.txt", "segunda") == 1 &&
              contarLinhasDoFicheiro("teste_log.txt") == 2);
    verificar("contarLinhasDoFicheiro devolve -1 se nao existe", contarLinhasDoFicheiro("nao_existe.txt") == -1);

    remove("teste_vetor.txt");
    remove("teste_vetor.dat");
    remove("teste_log.txt");

    mostrarResumoDosTestes();
}