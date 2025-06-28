#include <stdio.h>
#include "RB.h"

int main() {
    FILE *arq;
    int valor, opcao;
    char nomeArquivo1[30];
    rb *T1 = alocaArvore();
    noRB *novoNo;

    scanf("%s", nomeArquivo1);

    arq = fopen(nomeArquivo1, "r");
    if (!arq)
        return 1;

    while(!feof(arq))
    {
        fscanf(arq, "%d", &valor);
        novoNo = alocaNo(T1, valor);
        insereNo(T1, novoNo);
    }

    percorrePreOrdem(T1, retornaRaiz(T1));

    return 0;
}
