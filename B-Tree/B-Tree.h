#ifndef ARVORE234
#define ARVORE234

//DECLARACAO DE VARIAVEIS GLOBAIS E DA FILA, USADADA PARA IMPRESSAO DA BTREE
typedef struct arvore234 arvore234;
typedef struct no234 no234;
#define MAX_QUEUE_SIZE 1000 
//utilizada para imprimir a b tree da melhor forma possivel
typedef struct Fila {
    no234* items[MAX_QUEUE_SIZE];
    int frente;
    int tras;
    int tamanho;
} Fila;

//Alocar árvore e nos

arvore234* alocaArvore234();

no234* alocaNo234(int folha);

// funcoes para inserção

int preencheArvore234(arvore234* arv, char* nomeArquivo);

void insereChaveArvore(arvore234* arv, int chave);

void insereChaveArvoreAux(arvore234* arv, no234* noAtual, int chave);

void insereChaveNo(no234* no, int chave);

void reparaInsercao(arvore234* arv, no234* noAtual);

no234* divideNo(arvore234* arv, no234* noCheio, no234* pai, int indicePai);

// funcoes de remocao

void removeChaveArvore(arvore234 *arv, int chave);

void removeChaveArvoreAux(arvore234* arv, no234* noAtual, int chave);

void removeChaveNo(no234* no, int chave);

void reparaRemocao(arvore234* arv, no234* noAtual);

no234* emprestaEsquerda(arvore234* arv, no234* pai, int index);

no234* emprestaDireita(arvore234* arv, no234* pai, int index);

no234* juntaNos(arvore234* arv, no234* pai, int index);

//FUNCOES UTILITARIAS
no234* encontraPai(no234* raiz, no234* filho);

int calculaAltura234(arvore234* arv);

int obtemQtdNos(no234* raiz);

no234* obtemRaiz234(arvore234* arv);

int obtemQtdSplit(arvore234* arv);

int obtemQtdMerge(arvore234* arv);

int obtemQtdRotacoes(arvore234* arv);

int* obtemChaves(no234* no);

no234** obtemFilhos(no234* no);

int obtemQtdChaves(no234* no);

//METODO DE IMPRESSAO DA BTREE

void imprimeArvore234PorNivel(arvore234 *arv);
void inicializaFila(Fila *f);
int estaVazia(Fila *f);
int estaCheia(Fila *f);
void enfileirar(Fila *f, no234* no);
no234* desenfileirar(Fila *f);

#endif