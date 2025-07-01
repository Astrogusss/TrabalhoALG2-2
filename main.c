#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>     

#include "B-Tree/B-Tree.h"
#include "Red-Black/RB.h"


void menuArvore234(arvore234* arv234);
void menuArvoreRB(rb* arvRB);


int main() {
   
    SetConsoleOutputCP(CP_UTF8);

    FILE *arq;
    char nomeArquivo[100];
    int valor;

    
    printf("Digite o nome do arquivo de entrada (ex: entrada.txt): ");
    scanf("%s", nomeArquivo);

    arq = fopen(nomeArquivo, "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        return 1;
    }


    arvore234* arv234 = alocaArvore234();
    while (fscanf(arq, "%d", &valor) != EOF) {
        insereChaveArvore(arv234, valor);
    }
    fclose(arq);


    printf("\n--- Árvore 2-3-4 Gerada ---\n");
    imprimeArvore234(arv234);
    printf("\n");

    menuArvore234(arv234);



    return 0;
}

/**

 */
void menuArvore234(arvore234* arv234) {
    int opcao, valor;
    

    rb* arvRB = NULL;
    noRB* raizRB = NULL;
    no234* raiz234 = NULL;

    do {
        printf("--- Árvores Balanceadas : Menu (Árvore 2-3-4) ---\n");
        printf("1. Inserir novo elemento\n");
        printf("2. Remover elemento\n");
        printf("3. Imprimir árvore\n");
        printf("4. Converter em uma árvore rubro-negra\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        printf("\n");

        switch (opcao) {
            case 1:
                printf("Digite o valor para inserir: ");
                scanf("%d", &valor);
                insereChaveArvore(arv234, valor);
                printf("Elemento inserido.\n\n");
                imprimeArvore234(arv234);
                printf("\n");
                break;
            
            case 2:
          
                printf("Digite o valor para remover: ");
                scanf("%d", &valor);
                removeChaveArvore(arv234, valor);
                printf("Operação de remoção concluída.\n\n");
                imprimeArvore234(arv234);
                printf("\n");
                break;

            case 3:
                printf("\n--- Árvore 2-3-4 Atual ---\n");
                imprimeArvore234(arv234);
                printf("\n");
                break;

            case 4:
                printf("Convertendo a árvore 2-3-4 em Rubro-Negra...\n");

                raiz234 = obtemRaiz234(arv234);
                if (raiz234 && obtemQtdChaves(raiz234) > 0) {
            
                    arvRB = alocaArvore();
                    raizRB = converte234(raiz234, NULL); 
                    setRaiz(arvRB, raizRB);
                    ajustaRaizPosConversao(arvRB);
                    
                    printf("--- Árvore Rubro-Negra Gerada ---\n");
                    percorrePreOrdem(arvRB, retornaRaiz(arvRB));
                    printf("\n");

                    menuArvoreRB(arvRB);
                } else {
                    printf("A árvore 2-3-4 está vazia. Não é possível converter.\n\n");
                }
                

                opcao = 5;
                break;

            case 5:
                printf("Encerrando o programa.\n");
                break;

            default:
                printf("Opção inválida. Tente novamente.\n\n");
                break;
        }
    } while (opcao != 5);
}


void menuArvoreRB(rb* arvRB) {
    int opcao, valor;
    noRB* novoNo = NULL;

    do {
        printf("\n--- Árvores Balanceadas : Menu (Árvore Rubro-Negra) ---\n");
        printf("1. Inserir novo elemento na árvore rubro-negra\n");
        printf("2. Remover elemento da árvore rubro-negra\n");
        printf("3. Imprimir árvore rubro-negra\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        printf("\n");

        switch (opcao) {
            case 1:
                printf("Digite o valor para inserir: ");
                scanf("%d", &valor);
                novoNo = alocaNo(arvRB, valor);
                insereNo(arvRB, novoNo);
                printf("Elemento inserido.\n");
                percorrePreOrdem(arvRB, retornaRaiz(arvRB));
                break;

            case 2:
                printf("Digite o valor para remover: ");
                scanf("%d", &valor);
                if (removeNo(arvRB, valor)) {
                    printf("Elemento removido.\n");
                } else {
                    printf("Elemento não encontrado.\n");
                }
                percorrePreOrdem(arvRB, retornaRaiz(arvRB));
                break;

            case 3:
                printf("\n--- Árvore Rubro-Negra Atual ---\n");
                percorrePreOrdem(arvRB, retornaRaiz(arvRB));
                printf("\n");
                break;

            case 4:
            
                return;

            default:
                printf("Opção inválida. Tente novamente.\n\n");
                break;
        }
    } while (opcao != 4);
}