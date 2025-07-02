#include <stdio.h>
#include <stdlib.h>
#include "RB.h"

struct RB{
    struct NO *raiz;
    int qtd;
};

struct NO{
    struct NO *esq;
    struct NO *dir;
    struct NO *pai;
    char cor;
    int chave;
};


rb *alocaArvore(){
    rb *arv = (rb*)malloc(sizeof(rb));
    if(!arv){
        return NULL;
    }
    arv->raiz = NULL;
    arv->qtd = 0;
    return arv;
}

noRB *alocaNo(rb *arv, int chave){
    noRB *novoNo = (noRB*)malloc(sizeof(noRB));
    
    if(!novoNo){
        return NULL;
    }
    
    novoNo->esq = NULL;
    novoNo->dir = NULL;
    novoNo->pai = NULL;
    novoNo->cor = 'V';
    novoNo->chave = chave;
    return novoNo;
}
void setRaiz(rb* arv, noRB* noRaiz){
    if (arv) {
        arv->raiz = noRaiz;
    }
}

// --- NOVA FUNÇÃO PARA AJUSTAR A COR DA RAIZ ---
void ajustaRaizPosConversao(rb* arv) {
    if (arv && arv->raiz) {
        arv->raiz->cor = 'P'; // A raiz de uma árvore Rubro-Negra é sempre preta.
    }
}
void insereNo(rb *arv, noRB *novoNo){
    if (!arv || !novoNo) return;

    if (arv->raiz == NULL) {
        arv->raiz = novoNo;
        novoNo->pai = NULL;
        novoNo->cor = 'P'; 
        arv->qtd++;
        return;
    }

    noRB *atual = arv->raiz;
    noRB *anterior = NULL;

    while (atual != NULL) {
        anterior = atual;
        if (novoNo->chave < atual->chave)
            atual = atual->esq;
        else
            atual = atual->dir;
    }

    if (novoNo->chave < anterior->chave)
        anterior->esq = novoNo;
    else
        anterior->dir = novoNo;

    novoNo->pai = anterior;
    arv->qtd++;
    balanceamentoInsercao(arv, novoNo);
}

int removeNo(rb *arv, int valor){
    if (!arv || !arv->raiz){
        return 0;
    }

    noRB *atual = arv->raiz;
    noRB *pai = NULL;
    
    while (atual && atual->chave != valor){
        pai = atual;
        if (valor < atual->chave){
            atual = atual->esq;
        }
        else{
            atual = atual->dir;
        }
            
    }
    
    if (!atual){
        return 0;
    }
    
    noRB *substituto = NULL;
    
    if (!atual->esq && !atual->dir){
        substituto = NULL;
    }
    
    else if (!atual->esq){
        substituto = atual->dir;
        substituto->pai = atual->pai;
    }
    
    else if (!atual->dir){
        substituto = atual->esq;
        substituto->pai = atual->pai;
    }
    
    else {
        
        noRB *sucessor = atual->dir;
        while (sucessor->esq)
            sucessor = sucessor->esq;

     
        atual->cor = sucessor->cor;
        atual->chave = sucessor->chave;

        noRB *paiSuc = sucessor->pai;
        noRB *filhoSuc = sucessor->dir;

        if (paiSuc->esq == sucessor)
            paiSuc->esq = filhoSuc;
        else
            paiSuc->dir = filhoSuc;

        if (filhoSuc)
            filhoSuc->pai = paiSuc;

        free(sucessor);
        return 1;
    }

    if (atual == pai->esq)
        pai->esq = substituto;
    else
        pai->dir = substituto;

    balanceamentoRemocao(arv, substituto, pai); 

    free(atual);
    return 1;
}

void percorrePreOrdem(rb *arv, noRB *aux){
    if(aux){
        printf("%d -- %c\n", aux->chave, aux->cor);
        percorrePreOrdem(arv, aux->esq);
        percorrePreOrdem(arv, aux->dir);
    }
}

noRB *retornaRaiz(rb *arv){
    if(arv->raiz != NULL){
        return arv->raiz;
    }
    return NULL;
}

void balanceamentoInsercao(rb *arv, noRB *novoNo){
    if (!arv || !novoNo || !novoNo->pai) return;
    
    while (novoNo != NULL && novoNo->pai && novoNo->pai->cor == 'V') {
        noRB *pai = novoNo->pai;
        noRB *avo = pai->pai;

        if (!avo) break; 

        if (pai == avo->esq) {
            noRB *tio = avo->dir;

            if (tio && tio->cor == 'V') {
                pai->cor = 'P';
                tio->cor = 'P';
                avo->cor = 'V';
                novoNo = avo;
            } else {
                if (novoNo == pai->dir) {
                    novoNo = pai;
                    rotacaoEsquerda(arv, novoNo);
                    pai = novoNo->pai;
                }
                pai->cor = 'P';
                avo->cor = 'V';
                rotacaoDireita(arv, avo);
            }
        } else {
            noRB *tio = avo->esq;
            if (tio && tio->cor == 'V') {
                pai->cor = 'P';
                tio->cor = 'P';
                avo->cor = 'V';
                novoNo = avo;
            } else {
                if (novoNo == pai->esq) {
                    novoNo = pai;
                    rotacaoDireita(arv, novoNo);
                    pai = novoNo->pai;
                }
                pai->cor = 'P';
                avo->cor = 'V';
                rotacaoEsquerda(arv, avo);
            }
        }
    }
    arv->raiz->cor = 'P';
}

void balanceamentoRemocao(rb *arv, noRB *NoSucessor, noRB *NoPai) {
    noRB *irmao;

    if (NoPai->esq == NoSucessor) {
        irmao = NoPai->dir;

        if (irmao != NULL && irmao->cor == 'V') {
            irmao->cor = 'P';
            NoPai->cor = 'V';
            rotacaoEsquerda(arv, NoPai);
            irmao = NoPai->dir;
        }
        if (irmao != NULL && irmao->cor == 'P' && (irmao->esq == NULL || irmao->esq->cor == 'P') &&
        (irmao->dir == NULL || irmao->dir->cor == 'P')) {

            irmao->cor = 'V';
            if (NoPai->cor == 'V') {
                NoPai->cor = 'P';
            } else {
                balanceamentoRemocao(arv, NoPai, NoPai->pai);
            }
        }
        else if (irmao != NULL && irmao->cor == 'P' && irmao->dir != NULL && irmao->dir->cor == 'P' && irmao->esq != NULL && irmao->esq->cor == 'V') {

            irmao->esq->cor = 'P';
            irmao->cor = 'V';
            rotacaoDireita(arv, irmao);
            irmao = NoPai->dir;
        }
        if (irmao != NULL &&
            irmao->cor == 'P' &&
            irmao->dir != NULL && irmao->dir->cor == 'V') {

            irmao->cor = NoPai->cor;
            NoPai->cor = 'P';
            irmao->dir->cor = 'P';
            rotacaoEsquerda(arv, NoPai);
        }
    } else {
        irmao = NoPai->esq;
        if (irmao != NULL && irmao->cor == 'V') {
            irmao->cor = 'P';
            NoPai->cor = 'V';
            rotacaoDireita(arv, NoPai);
            irmao = NoPai->esq;
        }

        if (irmao != NULL && irmao->cor == 'P' && (irmao->esq == NULL || irmao->esq->cor == 'P') && (irmao->dir == NULL || irmao->dir->cor == 'P')) {
            irmao->cor = 'V';
            if (NoPai->cor == 'V') {
                NoPai->cor = 'P';
            } else {
                balanceamentoRemocao(arv, NoPai, NoPai->pai);
            }
            
        }else if (irmao != NULL &&irmao->cor == 'P' && irmao->esq != NULL && irmao->esq->cor == 'P' &&irmao->dir != NULL && irmao->dir->cor == 'V') {
            irmao->dir->cor = 'P';
            irmao->cor = 'V';
            rotacaoEsquerda(arv, irmao);
            irmao = NoPai->esq;
        }
           if (irmao != NULL &&irmao->cor == 'P' &&irmao->esq != NULL && irmao->esq->cor == 'V') {
            irmao->cor = NoPai->cor;
            NoPai->cor = 'P';
            irmao->esq->cor = 'P';
            rotacaoDireita(arv, NoPai);
        }
    }
    if (arv->raiz != NULL){
        arv->raiz->cor = 'P';
    }
}

void rotacaoDireita(rb *arv, noRB *noDesbalanceado){
    noRB *x = noDesbalanceado->esq;
    noDesbalanceado->esq = x->dir;
    
    if(x->dir) x->dir->pai = noDesbalanceado;
    x->pai = noDesbalanceado->pai;
    
    if(noDesbalanceado->pai == NULL){
        arv->raiz = x;
    }else if(noDesbalanceado->pai->dir == noDesbalanceado){
        noDesbalanceado->pai->dir = x;
    }else{
        noDesbalanceado->pai->esq = x;
    }
    
    x->dir = noDesbalanceado;
    noDesbalanceado->pai = x;
}

void rotacaoEsquerda(rb *arv, noRB *noDesbalanceado){
    noRB *y = noDesbalanceado->dir;
    noDesbalanceado->dir = y->esq;
    
    if(y->esq) y->esq->pai = noDesbalanceado;
    
    y->pai = noDesbalanceado->pai;
    
    if(noDesbalanceado->pai == NULL){
        arv->raiz = y;
    }else if(noDesbalanceado->pai->dir == noDesbalanceado){
        noDesbalanceado->pai->dir = y;
    }else{
        noDesbalanceado->pai->esq = y;
    }
    
    y->esq = noDesbalanceado;
    noDesbalanceado->pai = y;
}
 
noRB* converte234(no234* raiz234, noRB* pai){
    if(!raiz234)
        return NULL;

    noRB* raizRB = NULL;
    int qtdChaves = obtemQtdChaves(raiz234);
    int* chaves = obtemChaves(raiz234);
    no234** filhos = obtemFilhos(raiz234);

    // Nó 2 -> um nó preto
    if (qtdChaves == 1){
        raizRB = (noRB*) malloc(sizeof(noRB));
        raizRB->chave = chaves[0];
        raizRB->cor = 'P';
        raizRB->pai = pai;
        raizRB->esq = converte234(filhos[0], raizRB);
        raizRB->dir = converte234(filhos[1], raizRB);
    }
    // Nó 3 -> um nó preto com filho vermelho à direita
    else if (qtdChaves == 2){
        raizRB = (noRB*) malloc(sizeof(noRB));
        raizRB->chave = chaves[0];
        raizRB->cor = 'P';
        raizRB->pai = pai;

        noRB* vermelho = (noRB*) malloc(sizeof(noRB));
        vermelho->chave = chaves[1];
        vermelho->cor = 'V';
        vermelho->pai = raizRB;

        raizRB->esq = converte234(filhos[0], raizRB);
        raizRB->dir = vermelho;

        vermelho->esq = converte234(filhos[1], vermelho);
        vermelho->dir = converte234(filhos[2], vermelho);
    }
    // Nó 4 -> um nó preto com dois filhos vermelhos
    else if (qtdChaves == 3){
        raizRB = (noRB*) malloc(sizeof(noRB));
        raizRB->chave = chaves[1];
        raizRB->cor = 'P';
        raizRB->pai = pai;

        noRB* vermelhoEsq = (noRB*) malloc(sizeof(noRB));
        vermelhoEsq->chave = chaves[0];
        vermelhoEsq->cor = 'V';
        vermelhoEsq->pai = raizRB;

        noRB* vermelhoDir = (noRB*) malloc(sizeof(noRB));
        vermelhoDir->chave = chaves[2];
        vermelhoDir->cor = 'V';
        vermelhoDir->pai = raizRB;

        raizRB->esq = vermelhoEsq;
        raizRB->dir = vermelhoDir;

        vermelhoEsq->esq = converte234(filhos[0], vermelhoEsq);
        vermelhoEsq->dir = converte234(filhos[1], vermelhoEsq);

        vermelhoDir->esq = converte234(filhos[2], vermelhoDir);
        vermelhoDir->dir = converte234(filhos[3], vermelhoDir);
    }

    return raizRB;
}