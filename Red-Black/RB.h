//
// Created by Vanessa on 22/05/2025.
//

#ifndef RB_RB_H
#define RB_RB_H

typedef struct RB rb;
typedef struct NO noRB;

//Função que aloca uma árvore RB
rb *alocaArvore();

//Função que aloca um novo nó
noRB *alocaNo(rb *arv, int chave);

//Função iterativa que insere um novo nó na árvore e chama a função que ajusta o balanceamento
void insereNo(rb *arv, noRB *novoNo);

//Função que remove um nó da árvore RB
//retorna 1 se o nó foi removido
//retorna 0 se o elemento não foi encontrado na árvore
int removeNo(rb *arv, int valor);

//Percorre a árvore em pré-ordem
//printf("%d -- %s\n", chave, cor)
void percorrePreOrdem(rb *arv, noRB *aux);

//Função que retorna a raiz da árvore
noRB *retornaRaiz(rb *arv);

//**FUNÇÕES DE BALANCEAMENTO DA RUBRO NEGRA**//

//Função que corrige o balanceamento após a inserção
void balanceamentoInsercao(rb *arv, noRB *novoNo);

//Função que corrige o balanceamento após a remoção
void balanceamentoRemocao(rb *arv, noRB *NoSucessor, noRB *NoPai);

//Rotação à direita
void rotacaoDireita(rb *arv, noRB *noDesbalanceado);

//Rotação à esquerda
void rotacaoEsquerda(rb *arv, noRB *noDesbalanceado);

//**FUNÇÃO PARA OPERAÇÃO DE JUNÇÃO DE DUAS RB**//

//Função que une duas árvores RB, a partir de x
//T1 tem valores menores que x
//T2 tem valores maiores que x
rb* rb_join(rb *T1, rb *T2, int x);

//Função recursiva que atualiza a altura de preto de todos os nós
int atualiza_Altura_Preto(rb *arv, noRB *noAux);

//Função que atualiza a altura de preto da árvore como sendo a altura de preto da raiz
void atualiza_Altura_Preto_RB(rb *arv);

//Função recursiva que atualiza o ponteiro sentinela folha após a junção de duas RB
void atualiza_Sentinela_Folha(rb *T1, rb *T2, rb *novaRB, noRB *aux);

#endif //RB_RB_H
