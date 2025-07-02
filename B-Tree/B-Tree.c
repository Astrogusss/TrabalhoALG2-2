#include "B-Tree.h"

#include <stdio.h>
#include <stdlib.h>

//VAMOS DECLARAR AS VARIAVEIS E AS STRUCT QUE IREMOS USAR

int maxkey = 3; // maximo de chaves
int minkey = 1; // minimo de chaves
int maxFilhos = 4; // maximo de filhos por nó
int minFilhos = 2; // minimo de filhos por nó

struct arvoreB{
    noB* raiz;
    int quantidadeRota;
    int quantidadeJuncao;
    int quantidadeSplit;
};

struct noB{
    noB** filhos;
    int* chaves;
    int quantidadeKey;
    int folha;
};

void inicializaFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->tamanho = 0;
}

int estaVazia(Fila *f) {
    return f->tamanho == 0;
}

int estaCheia(Fila *f) {
    return f->tamanho == MAX_QUEUE_SIZE;
}

void enfileirar(Fila *f, noB* no) {
    if (estaCheia(f)) {
        printf("Fila cheia!\n");
        return;
    }
    f->tras = (f->tras + 1) % MAX_QUEUE_SIZE;
    f->items[f->tras] = no;
    f->tamanho++;
}

noB* desenfileirar(Fila *f) {
    if (estaVazia(f)) {
        return NULL;
    }
    noB* no = f->items[f->frente];
    f->frente = (f->frente + 1) % MAX_QUEUE_SIZE;
    f->tamanho--;
    return no;
}


//Alocar árvore e nos

arvoreB* alocaArvB(){
    arvoreB* novaArvore = (arvoreB*) malloc(sizeof(arvoreB));
    if(!novaArvore) return NULL; //prevencao de erro
        
    novaArvore->raiz = NULL;
    
    novaArvore->quantidadeRota = 0;
    novaArvore->quantidadeJuncao = 0;
    novaArvore->quantidadeSplit = 0;

    return novaArvore;
}

noB* alocaNoB(int folha){
    noB* novoNo = (noB*) malloc(sizeof(noB));
    if(!novoNo){ // prevencao de erro
        return NULL;
        printf("Falha em alocar nó");
    }

    novoNo->chaves = (int*) malloc((maxkey + 1) * sizeof(int)); // metodo de prevencao em numero maximo de chaves
    novoNo->filhos = (noB**) malloc((maxFilhos + 1) * sizeof(noB*)); // metodo de prevencao em numero maximo de filhos
    
    if(!novoNo->chaves || !novoNo->filhos){
        free(novoNo);
        free(novoNo->filhos);
        free(novoNo->chaves);
        printf("Nao foi alocado o nó");
        return NULL;
    }

    for(int i = 0; i < maxkey; i++){
        novoNo->chaves[i] = -1;
        novoNo->filhos[i] = NULL; // todos filhos sao inicialmente nulos
    }
    novoNo->folha = folha;
    novoNo->quantidadeKey = 0;
    novoNo->filhos[maxFilhos - 1] = NULL;

    return novoNo;
}

// funcoes para inserção

int preencheArvB(arvoreB* arv, char* nomeArquivo){
    FILE* arquivo = fopen(nomeArquivo, "r");
    if(!arquivo) return 0;// caso nao exista o arquivo

    int chave;
    while(fscanf(arquivo, "%d", &chave) == 1) insereChaveArvore(arv, chave);
    
    fclose(arquivo); // fecha arquivo para melhor organizacao da memoria

    return 1; // ocorreu tudo certo
}

void insereChaveArvore(arvoreB* arv, int chave){
    // Verificar se a árvore está vazia
    if(arv->raiz == NULL){
        arv->raiz = alocaNoB(1);
        arv->raiz->chaves[0] = chave;
        arv->raiz->quantidadeKey = 1;
    }
    else insereChaveArvoreAux(arv, arv->raiz, chave);
}

void insereChaveArvoreAux(arvoreB* arv, noB* noAtual, int chave){
    if(noAtual->folha){
        insereChaveNo(noAtual, chave); // caso o nó atual é folha, insere
        reparaInsercao(arv, noAtual); // E depois analisa se ficou desbalanceada
    } 
    // Caso nao seja o nó seja folha, encontrar o filho correto para descer na árvore
    else{
        int i = 0;
        while(i < noAtual->quantidadeKey && chave > noAtual->chaves[i])  i++; // achando o filho para descer
        // Descer para o filho encontrado
        insereChaveArvoreAux(arv, noAtual->filhos[i], chave);
    }
}

void insereChaveNo(noB* no, int chave){
    int index = no->quantidadeKey - 1; 
    //ultimo elemento do nó
    
    while(index >= 0 && no->chaves[index] > chave){
        no->chaves[index + 1] = no->chaves[index]; //fazemos um insertion sort
        index--;
    }
    no->quantidadeKey++;
    no->chaves[index + 1] = chave; // Insere a nova chave na posição correta
}

void reparaInsercao(arvoreB* arv, noB* noAtual){
    
    if(noAtual->quantidadeKey <= maxkey) return; // se nao ultrapassou o numero maximo de chaves, nao precisa reparar
    
    if(noAtual == arv->raiz){
        arv->raiz = divideNo(arv, noAtual, NULL, -1);//caso seja raiz, dividimos o nó pra subir, pra criar outra raiz
        return;
    }
    
    noB* pai = encontraPai(arv->raiz, noAtual); //serve para encontrar o pai do nó e faze-lo dividir

    if(pai != NULL){
        int indicePai = 0;

        while(indicePai <= pai->quantidadeKey && pai->filhos[indicePai] != noAtual) indicePai++;
        divideNo(arv, noAtual, pai, indicePai);
        reparaInsercao(arv, pai); // analise recursiva para analisar pai
    }
}

noB* divideNo(arvoreB* arv, noB* noCheio, noB* pai, int indicePai){
    int meio = maxkey / 2;
    // como a ordem é 4, o meio seria 1
    
    int intermediario = noCheio->chaves[meio];
    // elemento que esta no meio das chaves
    
    // criar novo no para ir a direita
    noB* novoNo = alocaNoB(noCheio->folha);
    if (!novoNo) return pai;
    //copiando as chaves a direita da chave que sobe para o novo nó alocado e analisado
    novoNo->quantidadeKey = noCheio->quantidadeKey - meio - 1;
    for(int i = 0; i < novoNo->quantidadeKey; i++) novoNo->chaves[i] = noCheio->chaves[meio + 1 + i];
    
    //caso nao seja folha, tem que transferir os filhos tambem
    if(!noCheio->folha){
        for(int i = 0; i <= novoNo->quantidadeKey; i++){
            novoNo->filhos[i] = noCheio->filhos[meio + 1 + i];
            noCheio->filhos[meio + 1 + i] = NULL;
        }
    }

    arv->quantidadeSplit++; // sempre contando a quantidade de split para exibir no final
    
    // temos que atualizar o numero de chaves do no que esta no lado esquerdo, antes analisado
    noCheio->quantidadeKey = meio;
    
    // se nao tiver pai, temos que criar uma nova raiz
    if(pai == NULL) {
        noB* novaRaiz = alocaNoB(0);
        if(!novaRaiz) return noCheio;
        
        novaRaiz->filhos[0] = noCheio;
        novaRaiz->filhos[1] = novoNo;
        novaRaiz->quantidadeKey = 1;
        novaRaiz->chaves[0] = intermediario;
        
        return novaRaiz;
    }
    
    // colocando chaves no pai ,deslocar ponteiros para filhos para a direita
    for(int i = pai->quantidadeKey; i >= indicePai + 1; i--) pai->filhos[i + 1] = pai->filhos[i];
    
    // Depois, deslocar o que ficou a direita 
    for(int i = pai->quantidadeKey - 1; i >= indicePai; i--) pai->chaves[i + 1] = pai->chaves[i];
    
    // colocar a chave que subiu e o novo nó filho
    pai->chaves[indicePai] = intermediario;
    pai->quantidadeKey++;
    pai->filhos[indicePai + 1] = novoNo;
    
    return pai;
}

// funcoes de remocao

void removeChaveArvore(arvoreB *arv, int chave){
    if(arv->raiz->quantidadeKey == 0) printf("a árvore nao tem nós na raiz");
    
    else removeChaveArvoreAux(arv, arv->raiz, chave);
}

void removeChaveArvoreAux(arvoreB* arv, noB* noAtual, int chave){
    int i = 0;
    
    //pos chave
    while(i < noAtual->quantidadeKey && noAtual->chaves[i] < chave) i++;

    //mais a direita
    if(i == noAtual->quantidadeKey){
        if(!noAtual->folha) removeChaveArvoreAux(arv, noAtual->filhos[noAtual->quantidadeKey], chave);
        return;
    }
     
    else if(noAtual->chaves[i] > chave){ // verificar se a chave é maior que a chave atual
        if(!noAtual->folha) removeChaveArvoreAux(arv, noAtual->filhos[i], chave);
        return;
    }
    
    //caso o nó seja encontrado
    else{
        if(noAtual->folha){
            //Remover e repara 
            removeChaveNo(noAtual, chave);
            reparaRemocao(arv, noAtual);
        }
        else{
            // encontrando o predecessor
            noB* predecessor = noAtual->filhos[i];
            while(!predecessor->folha) predecessor = predecessor->filhos[predecessor->quantidadeKey];
            //substui a chave pelo predecessor
            noAtual->chaves[i] = predecessor->chaves[predecessor->quantidadeKey - 1];
            //Temos que remover o predecessor da chave analisada
            predecessor->quantidadeKey--;
            //analisar se a arvore ficou desbalanceada
            reparaRemocao(arv, predecessor);
        }
    }
}

void removeChaveNo(noB* no, int chave){
    int index = 0;

    //ver aonde que a chave a ser removida esta
    while(index < no->quantidadeKey && no->chaves[index] < chave) index++;

    //caso a chave nao foi encontrada pelo algortimo
    if(index == no->quantidadeKey || no->chaves[index] != chave) return;

    //Deslocar as chaves para a esquerda
    for(int i = index; i < no->quantidadeKey - 1; i++) no->chaves[i] = no->chaves[i + 1];

    no->quantidadeKey--;
}

void reparaRemocao(arvoreB* arv, noB* noAtual){
    //analisar se tem chaves suficiente, nao precisa balancear a arvore
    if(noAtual->quantidadeKey >= minkey)
        return;
    
    //Se é a raiz
    if(noAtual == arv->raiz) {
        if(noAtual->quantidadeKey == 0){
            //Raiz vazia
            noB* raizAntiga = arv->raiz;

            if(!arv->raiz->folha)
                arv->raiz = arv->raiz->filhos[0];
            else 
                arv->raiz = NULL;
            
            free(raizAntiga); // libera raiz antiga 
        }

        return;
    }
    
    //ver o pai e o índice do nó atual
    noB* pai = encontraPai(arv->raiz, noAtual);
    if(pai == NULL) return;
    
    int indicePai = 0;
    while(indicePai <= pai->quantidadeKey && pai->filhos[indicePai] != noAtual) indicePai++;

    //comeca a tentativa de emprestar as chaves do irmao, sendo direita ou esquerda
    //Tentar emprestar do irmão da esquerda
    //esquerda
    if(indicePai > 0 && pai->filhos[indicePai - 1]->quantidadeKey > minkey) empESQ(arv, pai, indicePai);
    
    //direita
    else if(indicePai < pai->quantidadeKey && pai->filhos[indicePai + 1]->quantidadeKey > minkey) empDIR(arv, pai, indicePai);
    
    //junta os nos caso nao de para emprestar tanto do lado direito quanto do lado esquerda
    else{
        noB* noMesclado;

        //irmão da direita
        if(indicePai == 0)
            noMesclado = juntaNos(arv, pai, indicePai);

        //irmão da esquerda
        else 
            noMesclado = juntaNos(arv, pai, indicePai - 1);
        
        // chama recursivamente para reparar o pai
        reparaRemocao(arv, pai);
    }
}

noB* empESQ(arvoreB* arv, noB* pai, int index){
    noB* filho = pai->filhos[index];
    noB* irmaoEsquerdo = pai->filhos[index - 1];

    //move tudo mundo para a direita caso tenha como emprestar
    for(int i = filho->quantidadeKey - 1; i >= 0; i--)
        filho->chaves[i + 1] = filho->chaves[i];

    //Se não for folha, mover os filhos também
    if(!filho->folha){
         //move tudo pra direita
        for(int i = filho->quantidadeKey; i >= 0; i--) filho->filhos[i + 1] = filho->filhos[i];
       
        //o maior filhor do irmao a esquerda se torna o primeiro filho do filho
        filho->filhos[0] = irmaoEsquerdo->filhos[irmaoEsquerdo->quantidadeKey];
    }

    //chave do pai desce para o no do filho
    filho->chaves[0] = pai->chaves[index - 1];

    //maior chave do irmao a esquerada sobre pro pai
    pai->chaves[index - 1] = irmaoEsquerdo->chaves[irmaoEsquerdo->quantidadeKey - 1];

    arv->quantidadeRota++;
    filho->quantidadeKey++;
    irmaoEsquerdo->quantidadeKey--;
    
    return filho;
}

noB* empDIR(arvoreB* arv, noB* pai, int index){
    noB* filho = pai->filhos[index];
    noB* irmaoDireito = pai->filhos[index + 1];

    //a chave do pai desce e vai pra posicao final do filho
    filho->chaves[filho->quantidadeKey] = pai->chaves[index];

    //caso nao seja folha, o menor filho do irmao a direita, sera o ultimo filho do filho
    if(!filho->folha) filho->filhos[filho->quantidadeKey + 1] = irmaoDireito->filhos[0];

    //A primeira chave do irmão a direito sobe para o pai
    pai->chaves[index] = irmaoDireito->chaves[0];

    //temos que mover as chaves do irmao a direita, empurrar uma posicao para a esquerda
    for(int i = 1; i < irmaoDireito->quantidadeKey; i++) irmaoDireito->chaves[i - 1] = irmaoDireito->chaves[i];

    //caso nao seja folha, temos que mover os filhos tambem
    if(!irmaoDireito->folha)
        for(int i = 1; i <= irmaoDireito->quantidadeKey; i++) irmaoDireito->filhos[i - 1] = irmaoDireito->filhos[i];
    

    arv->quantidadeRota++;
    irmaoDireito->quantidadeKey--;
    filho->quantidadeKey++;
    
    return filho;
}

noB* juntaNos(arvoreB* arv, noB* pai, int index){
    noB* filho = pai->filhos[index];
    noB* irmao = pai->filhos[index + 1];

    //Chave do pai desce e se torna a nova chave no filho
    filho->chaves[filho->quantidadeKey] = pai->chaves[index];

    //copia as chaves do irmao depois dos maiores chaves do filho
    for(int i = 0; i < irmao->quantidadeKey; i++) filho->chaves[filho->quantidadeKey + 1 + i] = irmao->chaves[i];

    //Copia filhos do irmão
    if(!filho->folha)
        for(int i = 0; i <= irmao->quantidadeKey; i++) filho->filhos[filho->quantidadeKey + 1 + i] = irmao->filhos[i];
    

    //mover chaves e filhos do api para preencher o que foi removido
    for(int i = index + 1; i < pai->quantidadeKey; i++){
        pai->chaves[i - 1] = pai->chaves[i];
        pai->filhos[i] = pai->filhos[i + 1];
    }

    filho->quantidadeKey += irmao->quantidadeKey + 1;
    arv->quantidadeJuncao++;
    pai->quantidadeKey--;

    free(irmao);

    return filho; //Retorna o nó que contém os elementos juntados
}

//
noB* encontraPai(noB* raiz, noB* filho){
    if(raiz == NULL || raiz->folha) return NULL;
    
    //ver se algum dos filhos é o nó que estamos procurando
    for(int i = 0; i <= raiz->quantidadeKey; i++)
        if(raiz->filhos[i] == filho) return raiz;
    
    
    //funcao recursivo que procura nos filhos
    for(int i = 0; i <= raiz->quantidadeKey; i++){
        noB* pai = encontraPai(raiz->filhos[i], filho);
        if(pai != NULL) return pai;
    }
    
    return NULL;
}

int AlturaB(arvoreB* arv){
    //caso a arvore estaja vazia
    if(arv->raiz == NULL) return 0;

    noB* raiz = arv->raiz;
    //conta a raiz
    int altura = 1; 

    while(!raiz->folha){
        altura++;
        raiz = raiz->filhos[0]; 
        //desce para o primeiro filho
    }

    return altura;
}

int obtemQtdNos(noB* noAtual){
    if(noAtual == NULL)
        return 0;

    int contagem = 1;

    //caso nao seja folha, conta quantos nos tem recursivamente em cada subarvore das filhas
    if(!noAtual->folha)
        for(int i = 0; i <= noAtual->quantidadeKey; i++) contagem += obtemQtdNos(noAtual->filhos[i]);
    

    return contagem;
}

noB* raizB(arvoreB* arv){
    return arv->raiz;
}
int obtemQtdSplit(arvoreB* arv){
    return arv->quantidadeSplit;
}
int obtemQtdMerge(arvoreB* arv){
    return arv->quantidadeJuncao;
}
int obtemQtdRotacoes(arvoreB* arv){
    return arv->quantidadeRota;
}
int* obtemChaves(noB* no){
    return no->chaves;
}
noB** obtemFilhos(noB* no){
    return no->filhos;
}
int obtemQtdChaves(noB* no){
    return no->quantidadeKey;
}

void imprimeArvore234PorNivel(arvoreB *arv) {
    noB* raiz = arv->raiz;

    if (raiz == NULL) {
        printf("A arvore nao tem no.\n");
        return;
    }

    Fila fila;
    inicializaFila(&fila);
    enfileirar(&fila, raiz);

    int nivelAtual = 0;
    // Usamos um nó sentinela (NULL) para marcar o fim de cada nível
    enfileirar(&fila, NULL); // Marca o fim do nível 0

    printf("Nivel 0: ");

    while (!estaVazia(&fila)) {
        noB* no = desenfileirar(&fila);

        if (no == NULL) {
            if (!estaVazia(&fila)) { // Se ainda há nós na fila, significa que o próximo nível existe
                nivelAtual++;
                printf("\nNivel %d: ", nivelAtual);
                enfileirar(&fila, NULL); // Marca o fim do próximo nível
            }
            continue;
        }

        printf("[");
        for (int i = 0; i < no->quantidadeKey; i++) {
            printf("%d", no->chaves[i]);
            if (i < no->quantidadeKey - 1) {
                printf(", ");
            }
        }
        printf("] ");

        if (!no->folha) {
            for (int i = 0; i <= no->quantidadeKey; i++) {
                if (no->filhos[i] != NULL) {
                    enfileirar(&fila, no->filhos[i]);
                }
            }
        }
    }
    printf("\n");
}