#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "B-Tree/B-Tree.h" 
#include "Geradores/Geradores.h"

void testeDeInsercaoCSV();
void testeDeRemocaoCSV();

int main() {
    printf("Iniciando analise de comportamento da Arvore 2-3-4...\n");
    testeDeInsercaoCSV();
    testeDeRemocaoCSV();
    printf("\nAnalise concluida. Arquivos 'analise_insercao.csv' e 'analise_remocao.csv' gerados com sucesso!\n");
    return 0;
}

void testeDeInsercaoCSV() {
    FILE *arquivo_csv = fopen("analise_insercao.csv", "w");
    if (arquivo_csv == NULL) {
        printf("Erro critico: Nao foi possivel criar o arquivo analise_insercao.csv\n");
        return;
    }

    fprintf(arquivo_csv, "TamanhoAmostra,QuantidadeSplit,AlturaArvore,TotalBlocosOcupados\n");
    int tamanhos[] = {100, 1000, 10000, 100000};
    int num_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);
    printf("Processando testes de insercao...\n");

    for (int i = 0; i < num_tamanhos; i++) {
        int tam = tamanhos[i];
        printf("--> Testando insercao para %d elementos...\n", tam);

        arvore234* arv = alocaArvore234();
        int* dados = geraAleatorios(tam, time(NULL) + i);

        if (dados == NULL || arv == NULL) {
             printf("    ERRO: Falha de alocacao para o teste de %d elementos.\n", tam);
             continue;
        }

        for (int j = 0; j < tam; j++) {
            insereChaveArvore(arv, dados[j]);
        }

  
        int altura = calculaAltura234(arv);
        int blocos = obtemQtdNos(obtemRaiz234(arv));
        int splits = obtemQtdSplit(arv);

        fprintf(arquivo_csv, "%d,%d,%d,%d\n", tam, splits, altura, blocos);
        
        free(dados);

    }
    fclose(arquivo_csv);
    printf("Testes de insercao finalizados.\n\n");
}

void testeDeRemocaoCSV() {
    FILE *arquivo_csv = fopen("analise_remocao.csv", "w");
    if (arquivo_csv == NULL) {
        printf("Erro critico: Nao foi possivel criar o arquivo analise_remocao.csv\n");
        return;
    }

    fprintf(arquivo_csv, "PercentualRemocao,QuantidadeRotacoes,QuantidadeMerges,AlturaFinal,TotalBlocosFinal\n");
    float percentuais[] = {0.10, 0.20, 0.35, 0.50};
    int num_percentuais = sizeof(percentuais) / sizeof(percentuais[0]);
    const int TAMANHO_INICIAL = 10000;
    printf("Processando testes de remocao...\n");

    for (int i = 0; i < num_percentuais; i++) {
        printf("--> Testando remocao de %.0f%%...\n", percentuais[i] * 100);

    
        arvore234* arv = alocaArvore234();
        int* dados = geraAleatorios(TAMANHO_INICIAL, time(NULL) + i);
        if (dados == NULL || arv == NULL) {
            printf("    ERRO: Falha de alocacao para o teste de remocao.\n");
            continue;
        }
        for (int j = 0; j < TAMANHO_INICIAL; j++) {
            insereChaveArvore(arv, dados[j]);
        }
        
      
        int elementos_a_remover = (int)(TAMANHO_INICIAL * percentuais[i]);
        for (int j = 0; j < elementos_a_remover; j++) {
            removeChaveArvore(arv, dados[j]);
        }


        int rotacoes = obtemQtdRotacoes(arv);
        int merges = obtemQtdMerge(arv);
        int altura_final = calculaAltura234(arv);
        int blocos_final = obtemQtdNos(obtemRaiz234(arv));

        fprintf(arquivo_csv, "%.0f%%,%d,%d,%d,%d\n", percentuais[i] * 100, rotacoes, merges, altura_final, blocos_final);
        
        free(dados);
      
    }
    fclose(arquivo_csv);
    printf("Testes de remocao finalizados.\n");
}