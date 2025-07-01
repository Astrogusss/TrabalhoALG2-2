#include "B-tree.h"

#include <stdio.h>
#include <stdlib.h>

/////////////////////////////////Declaração de Variáveis Globais e Structs////////////////////////////////

int maximoChaves = 3;
int minimioChaves = 1;
int maximoFilhos = 4;
int minimioFilhos = 2;

struct arvore234{
    no234* raiz;
    
    int qtdSplit;
    int qtdMerge;
    int qtdRotacoes;
};

struct no234{
    int* chaves;
    no234** filhos;
    
    int folha;
    int qtdChaves;
};

//////////////////////////////////Métodos de Alocação da Árvore e dos Nós/////////////////////////////////

arvore234* alocaArvore234(){
    arvore234* novaArvore = (arvore234*) malloc(sizeof(arvore234));
    if(!novaArvore)
        return NULL;

    novaArvore->qtdSplit = 0;
    novaArvore->qtdMerge = 0;
    novaArvore->qtdRotacoes = 0;

    novaArvore->raiz = NULL;

    return novaArvore;
}

no234* alocaNo234(int folha){
    no234* novoNo = (no234*) malloc(sizeof(no234));
    if(!novoNo){
        return NULL;
        printf("Erro ao alocar o nó1\n");
    }

    novoNo->chaves = (int*) malloc((maximoChaves + 1) * sizeof(int));
    novoNo->filhos = (no234**) malloc((maximoFilhos + 1) * sizeof(no234*));
    
    if(!novoNo->chaves || !novoNo->filhos){
        free(novoNo->chaves);
        free(novoNo->filhos);
        free(novoNo);

        printf("Erro ao alocar o nó\n");

        return NULL;
    }

    for(int i = 0; i < maximoChaves; i++){
        novoNo->chaves[i] = -1;
        novoNo->filhos[i] = NULL;
    }
    novoNo->filhos[maximoFilhos - 1] = NULL;

    novoNo->folha = folha;
    novoNo->qtdChaves = 0;

    return novoNo;
}

////////////////////////////////Métodos de Inserção de Elementos na Árvore////////////////////////////////

int preencheArvore234(arvore234* arv, char* nomeArquivo){
    FILE* arquivo = fopen(nomeArquivo, "r");
    if(!arquivo)
        return 0;

    int chave;
    while(fscanf(arquivo, "%d", &chave) == 1)
        insereChaveArvore(arv, chave);

    fclose(arquivo);

    return 1;
}

void insereChaveArvore(arvore234* arv, int chave){
    // Verificar se a árvore está vazia
    if(arv->raiz == NULL){
        arv->raiz = alocaNo234(1);
        arv->raiz->chaves[0] = chave;
        arv->raiz->qtdChaves = 1;

        return;
    }

    insereChaveArvoreAux(arv, arv->raiz, chave);
}

void insereChaveArvoreAux(arvore234* arv, no234* noAtual, int chave){
    if(noAtual->folha){
        // Se é folha, insere a chave
        insereChaveNo(noAtual, chave);
        // E depois repara a árvore se o nó ficou cheio demais
        reparaInsercao(arv, noAtual);
    } 
    else{
        // Se não é folha, encontrar o filho correto para descer
        int i = 0;

        while(i < noAtual->qtdChaves && chave > noAtual->chaves[i])
            i++;

        // Descer para o filho encontrado
        insereChaveArvoreAux(arv, noAtual->filhos[i], chave);
    }
}

void insereChaveNo(no234* no, int chave){
    int index = no->qtdChaves - 1; // Índice do último elemento no nó
    
    while(index >= 0 && no->chaves[index] > chave){
        no->chaves[index + 1] = no->chaves[index]; // Desloca as chaves para a direita
        index--;
    }

    no->chaves[index + 1] = chave; // Insere a nova chave na posição correta

    no->qtdChaves++;
}

void reparaInsercao(arvore234* arv, no234* noAtual){
    // Se o nó não está cheio demais, não precisa reparar  
    if(noAtual->qtdChaves <= maximoChaves)
        return;
    
    // Se é a raiz, dividir e criar nova raiz
    if(noAtual == arv->raiz){
        arv->raiz = divideNo(arv, noAtual, NULL, -1);
        return;
    }
    
    // Encontrar o pai e dividir o nó
    no234* pai = encontraPai(arv->raiz, noAtual);

    if(pai != NULL){
        int indicePai = 0;

        while(indicePai <= pai->qtdChaves && pai->filhos[indicePai] != noAtual)
            indicePai++;
        
        divideNo(arv, noAtual, pai, indicePai);
        
        // Reparar recursivamente o pai se necessário
        reparaInsercao(arv, pai);
    }
}

no234* divideNo(arvore234* arv, no234* noCheio, no234* pai, int indicePai){
    int meio = maximoChaves / 2;  // Para ordem 4: meio = 1
    int intermediario = noCheio->chaves[meio];
    
    // Criar novo nó direito
    no234* novoNo = alocaNo234(noCheio->folha);
    if (!novoNo) return pai;
    
    // Transferir chaves à direita da chave que sobe para o novo nó
    novoNo->qtdChaves = noCheio->qtdChaves - meio - 1;
    for(int i = 0; i < novoNo->qtdChaves; i++) 
        novoNo->chaves[i] = noCheio->chaves[meio + 1 + i];
    
    // Se não é folha, transferir filhos também
    if(!noCheio->folha){
        for(int i = 0; i <= novoNo->qtdChaves; i++){
            novoNo->filhos[i] = noCheio->filhos[meio + 1 + i];
            noCheio->filhos[meio + 1 + i] = NULL;
        }
    }
    
    //Atualizar número de chaves no nó original (lado esquerdo)
    noCheio->qtdChaves = meio;
    
    arv->qtdSplit++;
    
    // Se não tem pai, criar nova raiz
    if(pai == NULL) {
        no234* novaRaiz = alocaNo234(0);
        if(!novaRaiz) return noCheio;
        
        novaRaiz->chaves[0] = intermediario;
        novaRaiz->qtdChaves = 1;
        novaRaiz->filhos[0] = noCheio;
        novaRaiz->filhos[1] = novoNo;
        
        return novaRaiz;
    }
    
    // Inserir chave no pai
    // Primeiro, deslocar ponteiros para filhos para a direita
    for(int i = pai->qtdChaves; i >= indicePai + 1; i--)
        pai->filhos[i + 1] = pai->filhos[i];
    
    // Depois, deslocar chaves para a direita
    for(int i = pai->qtdChaves - 1; i >= indicePai; i--)
        pai->chaves[i + 1] = pai->chaves[i];
    
    // Inserir a chave que subiu e o novo nó filho
    pai->chaves[indicePai] = intermediario;
    pai->filhos[indicePai + 1] = novoNo;
    pai->qtdChaves++;
    
    return pai;
}

////////////////////////////////Métodos de Remoção de Elementos na Árvore/////////////////////////////////

void removeChaveArvore(arvore234 *arv, int chave){
    if(arv->raiz->qtdChaves == 0){
        printf("A árvore está vazia.\n");
        return;
    }

    removeChaveArvoreAux(arv, arv->raiz, chave);
}

void removeChaveArvoreAux(arvore234* arv, no234* noAtual, int chave){
    int i = 0;
    
    //Encontrar a posição da chave
    while(i < noAtual->qtdChaves && noAtual->chaves[i] < chave)
        i++;

    //Filho mais à direita
    if(i == noAtual->qtdChaves){
        if(!noAtual->folha)
            removeChaveArvoreAux(arv, noAtual->filhos[noAtual->qtdChaves], chave);

        return;
    }
     
    //Chave é maior que a chave atual
    else if(noAtual->chaves[i] > chave){
        if(!noAtual->folha)
            removeChaveArvoreAux(arv, noAtual->filhos[i], chave);

        return;
    }
    
    //Nó encontrado
    else{
        if(noAtual->folha){
            //Remover a chave e repara depois
            removeChaveNo(noAtual, chave);
            reparaRemocao(arv, noAtual);
        }
        else{
            //Encontrar predecessor
            no234* predecessor = noAtual->filhos[i];
            while(!predecessor->folha)
                predecessor = predecessor->filhos[predecessor->qtdChaves];
            
            //Substituir a chave pelo predecessor
            noAtual->chaves[i] = predecessor->chaves[predecessor->qtdChaves - 1];
            
            //Remover o predecessor da folha
            predecessor->qtdChaves--;
            
            //Reparar a árvore após a remoção
            reparaRemocao(arv, predecessor);
        }
    }
}

void removeChaveNo(no234* no, int chave){
    int index = 0;

    //Encontrar a chave a ser removida
    while(index < no->qtdChaves && no->chaves[index] < chave)
        index++;

    //Se a chave não foi encontrada
    if(index == no->qtdChaves || no->chaves[index] != chave)
        return;

    //Deslocar as chaves para a esquerda
    for(int i = index; i < no->qtdChaves - 1; i++) 
        no->chaves[i] = no->chaves[i + 1];

    no->qtdChaves--;
}

void reparaRemocao(arvore234* arv, no234* noAtual){
    //Se o nó tem chaves suficientes, não precisa reparar
    if(noAtual->qtdChaves >= minimioChaves)
        return;
    
    //Se é a raiz
    if(noAtual == arv->raiz) {
        if(noAtual->qtdChaves == 0){
            //Raiz vazia
            no234* raizAntiga = arv->raiz;

            if(!arv->raiz->folha)
                arv->raiz = arv->raiz->filhos[0];
            else 
                arv->raiz = NULL;
            
            free(raizAntiga);
        }

        return;
    }
    
    //Encontrar o pai e o índice do nó atual
    no234* pai = encontraPai(arv->raiz, noAtual);
    if(pai == NULL) return;
    
    int indicePai = 0;
    while(indicePai <= pai->qtdChaves && pai->filhos[indicePai] != noAtual)
        indicePai++;
    
    //Tentar emprestar do irmão da esquerda
    if(indicePai > 0 && pai->filhos[indicePai - 1]->qtdChaves > minimioChaves)
        emprestaEsquerda(arv, pai, indicePai);
    
    //Tentar emprestar do irmão da direita
    else if(indicePai < pai->qtdChaves && pai->filhos[indicePai + 1]->qtdChaves > minimioChaves)
        emprestaDireita(arv, pai, indicePai);
    
    //Juntar com um irmão
    else{
        no234* noMesclado;

        //Juntar com irmão da direita
        if(indicePai == 0)
            noMesclado = juntaNos(arv, pai, indicePai);

        //Juntar com irmão da esquerda
        else 
            noMesclado = juntaNos(arv, pai, indicePai - 1);
        
        // Reparar recursivamente o pai
        reparaRemocao(arv, pai);
    }
}

no234* emprestaEsquerda(arvore234* arv, no234* pai, int index){
    no234* filho = pai->filhos[index];
    no234* irmaoEsquerdo = pai->filhos[index - 1];

    //Mover todas as chaves do filho uma posição para a direita
    for(int i = filho->qtdChaves - 1; i >= 0; i--)
        filho->chaves[i + 1] = filho->chaves[i];

    //Se não for folha, mover os filhos também
    if(!filho->folha){
        for(int i = filho->qtdChaves; i >= 0; i--)
            filho->filhos[i + 1] = filho->filhos[i];
        
        //O último filho do irmão esquerdo se torna o primeiro filho do filho
        filho->filhos[0] = irmaoEsquerdo->filhos[irmaoEsquerdo->qtdChaves];
    }

    //A chave do pai desce para o filho
    filho->chaves[0] = pai->chaves[index - 1];

    //A última chave do irmão esquerdo sobe para o pai
    pai->chaves[index - 1] = irmaoEsquerdo->chaves[irmaoEsquerdo->qtdChaves - 1];

    //Atualizar contadores
    filho->qtdChaves++;
    irmaoEsquerdo->qtdChaves--;
    arv->qtdRotacoes++;
    
    return filho;
}

no234* emprestaDireita(arvore234* arv, no234* pai, int index){
    no234* filho = pai->filhos[index];
    no234* irmaoDireito = pai->filhos[index + 1];

    //A chave do pai desce para o final do filho
    filho->chaves[filho->qtdChaves] = pai->chaves[index];

    //Se não for folha, o primeiro filho do irmão direito se torna o último filho do filho
    if(!filho->folha)
        filho->filhos[filho->qtdChaves + 1] = irmaoDireito->filhos[0];

    //A primeira chave do irmão direito sobe para o pai
    pai->chaves[index] = irmaoDireito->chaves[0];

    //Mover todas as chaves do irmão direito uma posição para a esquerda
    for(int i = 1; i < irmaoDireito->qtdChaves; i++)
        irmaoDireito->chaves[i - 1] = irmaoDireito->chaves[i];

    //Se não for folha, mover os filhos também
    if(!irmaoDireito->folha){
        for(int i = 1; i <= irmaoDireito->qtdChaves; i++)
            irmaoDireito->filhos[i - 1] = irmaoDireito->filhos[i];
    }

    //Atualizar contadores
    filho->qtdChaves++;
    irmaoDireito->qtdChaves--;
    arv->qtdRotacoes++;
    
    return filho;
}

no234* juntaNos(arvore234* arv, no234* pai, int index){
    no234* filho = pai->filhos[index];
    no234* irmao = pai->filhos[index + 1];

    //Chave do pai desce e se torna a nova chave no filho
    filho->chaves[filho->qtdChaves] = pai->chaves[index];

    // Copia chaves do irmão para o final do filho
    for(int i = 0; i < irmao->qtdChaves; i++)
        filho->chaves[filho->qtdChaves + 1 + i] = irmao->chaves[i];

    //Copia filhos do irmão
    if(!filho->folha){
        for(int i = 0; i <= irmao->qtdChaves; i++)
            filho->filhos[filho->qtdChaves + 1 + i] = irmao->filhos[i];
    }

    //Move chaves e filhos do pai para preencher o buraco
    for(int i = index + 1; i < pai->qtdChaves; i++){
        pai->chaves[i - 1] = pai->chaves[i];
        pai->filhos[i] = pai->filhos[i + 1];
    }

    //Atualiza o número de chaves no filho e no pai
    filho->qtdChaves += irmao->qtdChaves + 1;
    pai->qtdChaves--;

    arv->qtdMerge++; //Incrementa o contador de merges

    free(irmao);

    return filho; //Retorna o nó que contém os elementos juntados
}

////////////////////////////////////////////Métodos Auxiliares////////////////////////////////////////////

no234* encontraPai(no234* raiz, no234* filho){
    if(raiz == NULL || raiz->folha)
        return NULL;
    
    // Verificar se algum dos filhos é o nó procurado
    for(int i = 0; i <= raiz->qtdChaves; i++){
        if(raiz->filhos[i] == filho)
            return raiz;
    }
    
    // Buscar recursivamente nos filhos
    for(int i = 0; i <= raiz->qtdChaves; i++){
        no234* pai = encontraPai(raiz->filhos[i], filho);
        if(pai != NULL)
            return pai;
    }
    
    return NULL;
}

int calculaAltura234(arvore234* arv){
    if(arv->raiz == NULL)
        return 0; // A árvore está vazia

    no234* raiz = arv->raiz;
    int altura = 1; // Começa contando a raiz

    while(!raiz->folha){
        altura++;
        raiz = raiz->filhos[0]; // Desce para o primeiro filho
    }

    return altura;
}

int obtemQtdNos(no234* noAtual){
    if(noAtual == NULL)
        return 0;

    int contagem = 1; // Conta o nó atual

    // Se não for folha, conta recursivamente os nós em cada subárvore filha
    if(!noAtual->folha){
        for(int i = 0; i <= noAtual->qtdChaves; i++) 
            contagem += obtemQtdNos(noAtual->filhos[i]);
    }

    return contagem;
}

no234* obtemRaiz234(arvore234* arv){
    return arv->raiz;
}
int obtemQtdSplit(arvore234* arv){
    return arv->qtdSplit;
}
int obtemQtdMerge(arvore234* arv){
    return arv->qtdMerge;
}
int obtemQtdRotacoes(arvore234* arv){
    return arv->qtdRotacoes;
}
int* obtemChaves(no234* no){
    return no->chaves;
}
no234** obtemFilhos(no234* no){
    return no->filhos;
}
int obtemQtdChaves(no234* no){
    return no->qtdChaves;
}

void imprimePreOrdem234(arvore234* arv, no234* aux){
    if(!aux) return; // Condição de parada;

    printf("[");
    for (int i = 0; i < aux->qtdChaves; i++){
        printf("%d", aux->chaves[i]);

        if(i != aux->qtdChaves - 1) printf(" | ");            
    }
    printf("]\n");

    for(int i = 0; i <= aux->qtdChaves; i++){
        imprimePreOrdem234(arv, aux->filhos[i]);
    }
}

void imprimeArvore234(arvore234 *arv){
    no234* raiz = arv->raiz;

    if(raiz == NULL){
        printf("A árvore está vazia.\n");
        return;
    }

    printf("[");
    for(int i = 0; i < raiz->qtdChaves; i++){
        printf("%d", raiz->chaves[i]);

        if(i < raiz->qtdChaves - 1) 
            printf(", ");
    }
    printf("]\n");

    if(!raiz->folha){
        for(int i = 0; i <= raiz->qtdChaves; i++) 
            imprimeNo234(raiz->filhos[i], "", i == raiz->qtdChaves);
    }
}

void imprimeNo234(no234* no, const char *prefixo, int is_last){
    if(no == NULL) 
        return;

    printf("%s", prefixo);
    printf(is_last ? "└── " : "├── ");
    
    printf("[");
    for(int i = 0; i < no->qtdChaves; i++){
        printf("%d", no->chaves[i]);

        if(i < no->qtdChaves - 1)
            printf(", ");
    }
    printf("]\n");

    if(!no->folha){
        char novoPrefixo[1024];
        if(is_last)
            snprintf(novoPrefixo, sizeof(novoPrefixo), "%s    ", prefixo);
        else
            snprintf(novoPrefixo, sizeof(novoPrefixo), "%s│   ", prefixo);

        for(int i = 0; i <= no->qtdChaves; i++) 
            imprimeNo234(no->filhos[i], novoPrefixo, i == no->qtdChaves);
    }
}