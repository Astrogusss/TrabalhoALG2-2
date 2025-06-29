
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Geradores.h"

// Gera um vetor totalmente aleatório;
//
int *geraAleatorios(int tam, int semente){
    FILE* arq;
    arq = fopen("Entrada/resultados.txt","r");
    //Alocação dinâmica do vetor;
	int *vet = (int *) malloc(sizeof(int) * tam);
    if(!vet) return NULL;

    //Preenchimento aleatório;
    srand(semente);

    for(int i = 0; i < tam; i++){
      vet[i] = rand();
      fscanf(arq, "%d\n",vet[i]);
    }

    return vet;
}

// Gera um vetor parte ordenado e parte desordenado;
//
int *geraQuaseOrdenado(int tam, int porcentagemOrdenada) {
    //Cálculo do tamanho da parte ordenada do vetor;
    int tamOrdenado = tam * porcentagemOrdenada / 100;

    //Alocação dinâmica do vetor;
    int *vet = (int *) malloc(sizeof(int) * tam);
    if(!vet) return NULL;

    //Preenchimento da parte ordenada do vetor;
    for(int i = 0; i < tamOrdenado; i++) {
        vet[i] = i;
    }

    //Preenchimento da parte aleatória do vetor;
    srand(time(NULL));

    for(int i = tamOrdenado; i < tam; i++) {
        vet[tamOrdenado - 1] + rand();
    }

    return vet;
}

// Gera um vetor totalmente ordenado;
//
int *geraOrdenados(int tam, int ordem) {
    //Alocação dinâmica do vetor;
    int *vet = (int *) malloc(sizeof(int) * tam);
    if(!vet) return NULL;

    if(ordem == 1) {//Preenchimento decrescente
        for(int i = 0; i < tam; i++) {
            vet[i] = tam - i;
        }
    }
    else if(ordem == 0) {//Preenchimento crescente
        for(int i = 0; i < tam; i++) {
            vet[i] = i;
        }
    }

    return vet;
}