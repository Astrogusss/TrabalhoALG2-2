#ifndef ARVORE234
#define ARVORE234

//DECLARACAO DE VARIAVEIS GLOBAIS E DA FILA, USADADA PARA IMPRESSAO DA BTREE
typedef struct arvoreB arvoreB;
typedef struct noB noB;
#define MAX_QUEUE_SIZE 1000 
//utilizada para imprimir a b tree da melhor forma possivel
typedef struct Fila {
    noB* items[MAX_QUEUE_SIZE];
    int frente;
    int tras;
    int tamanho;
} Fila;

//Alocar árvore e nos

arvoreB* alocaArvB();

noB* alocaNoB(int folha);

// funcoes para inserção

int preencheArvB(arvoreB* arv, char* nomeArquivo);

void insereChaveArvore(arvoreB* arv, int chave);

void insereChaveArvoreAux(arvoreB* arv, noB* noAtual, int chave);

void insereChaveNo(noB* no, int chave);

void reparaInsercao(arvoreB* arv, noB* noAtual);

noB* divideNo(arvoreB* arv, noB* noCheio, noB* pai, int indicePai);

// funcoes de remocao

void removeChaveArvore(arvoreB *arv, int chave);

void removeChaveArvoreAux(arvoreB* arv, noB* noAtual, int chave);

void removeChaveNo(noB* no, int chave);

void reparaRemocao(arvoreB* arv, noB* noAtual);

noB* empDIR(arvoreB* arv, noB* pai, int index);

noB* empESQ(arvoreB* arv, noB* pai, int index);

noB* juntaNos(arvoreB* arv, noB* pai, int index);

//FUNCOES UTILITARIAS
noB* encontraPai(noB* raiz, noB* filho);

int AlturaB(arvoreB* arv);

int obtemQtdNos(noB* raiz);

int obtemQtdRotacoes(arvoreB* arv);

noB* raizB(arvoreB* arv);

int obtemQtdSplit(arvoreB* arv);

int obtemQtdMerge(arvoreB* arv);

int* obtemChaves(noB* no);

int obtemQtdChaves(noB* no);

noB** obtemFilhos(noB* no);
//METODO DE IMPRESSAO DA BTREE

void imprimeArvore234PorNivel(arvoreB *arv);
void inicializaFila(Fila *f);
int estaVazia(Fila *f);
int estaCheia(Fila *f);
void enfileirar(Fila *f, noB* no);
noB* desenfileirar(Fila *f);

#endif