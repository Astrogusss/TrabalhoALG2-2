#ifndef ARVORE234
#define ARVORE234

typedef struct arvore234 arvore234;
typedef struct no234 no234;

/// Aloca um novo objeto do tipo arvore234
/// Nova árvore criada
arvore234* alocaArvore234();

/// Aloca um novo objeto do tipo no234
/// folha Booleana que dita se o nó criado é folha ou não
/// Novo nó criado
no234* alocaNo234(int folha);

/// Insere um novo elemento em uma árvore 2-3-4
/// arv Árvore na qual o elemento será inserido
/// chave Elemento a ser inserido na árvore
void insereChaveArvore(arvore234* arv, int chave);

/// Insere um elemento em um nó folha específico de uma árvore 2-3-4
///  noAlvo Nó folha no qual o elemento será inserido
/// chave Elemento a ser inserido no nó folha
void insereChaveNoFolha(no234* noAlvo, int chave);

///  Insere um elemento em um nó interno específico de uma árvore 2-3-4
/// ´ noAlvo Nó interno no qual o elemento será inserido
/// ´ chave Elemento a ser inserido no nó folha
/// ´ filhoEsquerdo Filho esquerdo do novo elemento nó a ser inserido
/// ´ filhoDireito Filho direito do novo elemento a ser inserido
void insereChaveNoInterno(no234* noAlvo, int chave, no234* filhoEsquerdo, no234* filhoDireito);

///  Divide um nó cheio em dois para inserção de um novo elemento
/// ´ arv Árvore na qual a operação é realizada
/// ´ pai Pai do nó a ser dividido
/// ´ noCheio Nó cheio que deverá ser dividido
/// ´ chave Valor do elemento a ser inserido no nó cheio
///  Nó correto para inserção do elemento após a divisão
no234* divideNo(arvore234* arv, no234* pai, no234* noCheio, int chave);

///  Obtém a raiz de uma dada árvore 2-3-4
/// ´ arv Árvore em questão
///  Nó raiz da árvore
no234* obtemRaiz234(arvore234* arv);

///  Obtém a quantidade de splits realizado em uma árvore 2-3-4
/// ´ arv Árvore em questão
///  Quantidade de divisões feitas
int obtemQtdSplit(arvore234* arv);

///  Obtém a altura de uma dada árvore 2-3-4
/// ´ arv Árvore em questão
///  Altura da árvore
int obtemAltura234(arvore234* arv);

///  Obtém o vetor de chaves de um nó 2-3-4
/// ´ no Nó em questão
///  Vetor de inteiros com as chaves do nó
int* obtemChaves(no234* no);

///  Obtém o vetor de ponteiros para os filhos de um nó 2-3-4
/// ´ no Nó em questão
///  Vetor de ponteiros com os filhos do nó
no234** obtemFilhos(no234* no);

///  Obtém a quantidade de chaves de um nó 2-3-4
/// ´ no Nó em questão
///  Quantidade de chaves do nó
int obtemQtdChaves(no234* no);

///  Imprime os elementos de uma árvore 2-3-4
/// ´ arv Árvore em questão
void imprimeArvore234(arvore234 *arv);

///  Imprime as chaves de um nó de uma árvore 2-3-4
/// ´ no Nó em questão
/// ´ prefixo 
/// ´ is_last 
void imprimeNo234(no234* no, const char *prefix, int is_last);

#endif