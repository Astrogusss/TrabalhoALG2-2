#ifndef ARVORE234
#define ARVORE234

/////////////////////////////////Declaração de Variáveis Globais e Structs////////////////////////////////
typedef struct arvore234 arvore234;
typedef struct no234 no234;

//////////////////////////////////Métodos de Alocação da Árvore e dos Nós/////////////////////////////////

/// @brief Aloca um novo objeto do tipo arvore234;
/// @return Nova árvore criada;
arvore234* alocaArvore234();

/// @brief Aloca um novo objeto do tipo no234;
/// @param folha Booleana que dita se o nó criado é folha ou não;
/// @return Novo nó criado;
no234* alocaNo234(int folha);

////////////////////////////////Métodos de Inserção de Elementos na Árvore////////////////////////////////

int preencheArvore234(arvore234* arv, char* nomeArquivo);

/// @brief Insere um novo elemento em uma árvore 2-3-4;
/// @param arv Árvore na qual o elemento será inserido;
/// @param chave Elemento a ser inserido na árvore;
void insereChaveArvore(arvore234* arv, int chave);

void insereChaveArvoreAux(arvore234* arv, no234* noAtual, int chave);

/// @brief Insere um elemento em um nó folha específico de uma árvore 2-3-4;
/// @param noAlvo Nó folha no qual o elemento será inserido;
/// @param chave Elemento a ser inserido no nó folha;
void insereChaveNo(no234* no, int chave);

/// @brief Insere um elemento em um nó interno específico de uma árvore 2-3-4;
/// @param noAlvo Nó interno no qual o elemento será inserido;
/// @param chave Elemento a ser inserido no nó folha;
/// @param filhoEsquerdo Filho esquerdo do novo elemento nó a ser inserido;
/// @param filhoDireito Filho direito do novo elemento a ser inserido;
void reparaInsercao(arvore234* arv, no234* noAtual);

/// @brief Divide um nó cheio em dois para inserção de um novo elemento;
/// @param arv Árvore na qual a operação é realizada;
/// @param pai Pai do nó a ser dividido;
/// @param noCheio Nó cheio que deverá ser dividido;
/// @param chave Valor do elemento a ser inserido no nó cheio;
/// @return Nó correto para inserção do elemento após a divisão;
no234* divideNo(arvore234* arv, no234* noCheio, no234* pai, int indicePai);

////////////////////////////////Métodos de Remoção de Elementos na Árvore/////////////////////////////////
/// @brief Inicializa a remoção de um nó na árvore 2-3-4;
/// @param arv Árvore na qual a remoção irá acontecer;
/// @param chave Valor do nó a ser removido;
void removeChaveArvore(arvore234 *arv, int chave);

/// @brief Função recursiva que remove um nó na árvore 2-3-4;
/// @param arv Árvore na qual a remoção irá acontecer;
/// @param noAtual Nó avaliado na próxima chamada recursiva;
/// @param chave Valor do nó a ser removido;
void removeChaveArvoreAux(arvore234* arv, no234* noAtual, int chave);

/// @brief Remove uma chave de um nó da árvore 2-3-4;
/// @param no Nó em que a chave a ser removida está;
/// @param chave Chave a ser removida;
void removeChaveNo(no234* no, int chave);

/// @brief Balanceia a árvore 2-3-4 após uma remoção;
/// @param no Nó em que a chave a ser removida está;
/// @param chave Chave a ser removida;
void reparaRemocao(arvore234* arv, no234* noAtual);

/// @brief Rotação na árvore 2-3-4;
/// @param arv Árvore a sofrer a rotação;
/// @param pai Nó que irá emprestar uma chave ao nó;
/// @param index Marcador da posição do nó;
no234* emprestaEsquerda(arvore234* arv, no234* pai, int index);

/// @brief Rotação na árvore 2-3-4;
/// @param arv Árvore a sofrer a rotação;
/// @param pai Nó que irá emprestar uma chave ao nó;
/// @param index Marcador da posição do nó;
no234* emprestaDireita(arvore234* arv, no234* pai, int index);

/// @brief Junção na árvore 2-3-4;
/// @param arv Árvore a sofrer a junção;
/// @param pai Pai dos nós a serem juntados;
/// @param index Marcador da posição do nó;
no234* juntaNos(arvore234* arv, no234* pai, int index);

////////////////////////////////////////////Métodos Auxiliares////////////////////////////////////////////
/// @brief Retorna o pai do nó;
/// @param raiz Raiz da árvore 234 que o nó está;
/// @param filho Nó em que se busca o pai;
/// @return Ponteiro para o pai do nó;
no234* encontraPai(no234* raiz, no234* filho);

/// @brief Calcula a altura de uma árvore 2-3-4;
/// @param raiz Raiz da árvore;
/// @return Altura da árvore;
int calculaAltura234(arvore234* arv);

/// @brief Calcula recursivamente a quantidade de nós em uma árvore 2-3-4;
/// @param raiz Nó a ser avaliado na próxima chamada recursiva;
/// @return Quantidade de nós na árvore;
int obtemQtdNos(no234* raiz);

/// @brief Obtém a raiz de uma dada árvore 2-3-4;
/// @param arv Árvore em questão;
/// @return Nó raiz da árvore;
no234* obtemRaiz234(arvore234* arv);

/// @brief Obtém a quantidade de splits realizados em uma árvore 2-3-4;
/// @param arv Árvore em questão;
/// @return Quantidade de divisões feitas;
int obtemQtdSplit(arvore234* arv);

/// @brief Obtém a quantidade de merges realizados em uma árvore 2-3-4;
/// @param arv Árvore em questão;
/// @return Quantidade de mesclagens feitas;
int obtemQtdMerge(arvore234* arv);

/// @brief Obtém a quantidade de rotações realizadas em uma árvore 2-3-4;
/// @param arv Árvore em questão;
/// @return Quantidade de rotações feitas;
int obtemQtdRotacoes(arvore234* arv);

/// @brief Obtém o vetor de chaves de um nó 2-3-4;
/// @param no Nó em questão;
/// @return Vetor de inteiros com as chaves do nó;
int* obtemChaves(no234* no);

/// @brief Obtém o vetor de ponteiros para os filhos de um nó 2-3-4;
/// @param no Nó em questão;
/// @return Vetor de ponteiros com os filhos do nó;
no234** obtemFilhos(no234* no);

/// @brief Obtém a quantidade de chaves de um nó 2-3-4;
/// @param no Nó em questão;
/// @return Quantidade de chaves do nó;
int obtemQtdChaves(no234* no);

/// @brief Imprime uma árvore 234 em pré-ordem recursivamente;
/// @param arv Árvore 234 que será impressa;
/// @param aux Nó a iniciar a rucursão;
void imprimePreOrdem234(arvore234* arv, no234* aux);

/// @brief Imprime os elementos de uma árvore 2-3-4;
/// @param arv Árvore em questão;
void imprimeArvore234(arvore234 *arv);

/// @brief Imprime as chaves de um nó de uma árvore 2-3-4;
/// @param no Nó em questão;
/// @param prefixo String a ser printada na próxima iteração;
/// @param is_last Armazena 1 se for o último nó da sub árvore e 0 caso contrário; 
void imprimeNo234(no234* no, const char *prefix, int is_last);

#endif