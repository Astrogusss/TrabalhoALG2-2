
#ifndef RB_RB_H
#define RB_RB_H

#include "../B-Tree/B-Tree.h"

typedef struct RB rb;
typedef struct NO noRB;

rb *alocaArvore();

noRB *alocaNo(rb *arv, int chave);

void insereNo(rb *arv, noRB *novoNo);
void setRaiz(rb* arv, noRB* noRaiz);

int removeNo(rb *arv, int valor);
void ajustaRaizPosConversao(rb* arv);

void percorrePreOrdem(rb *arv, noRB *aux);

noRB *retornaRaiz(rb *arv);

void balanceamentoInsercao(rb *arv, noRB *novoNo);

void balanceamentoRemocao(rb *arv, noRB *NoSucessor, noRB *NoPai);

void rotacaoDireita(rb *arv, noRB *noDesbalanceado);

void rotacaoEsquerda(rb *arv, noRB *noDesbalanceado);

noRB* converte234(noB* raiz234, noRB* pai);

#endif //RB_RB_H