#include "Util.h"

//------------------------------------ Structs -------------------------

typedef struct DescArvore DArvore;
typedef DArvore* pDArvore;

typedef struct NohArvore nohArvore;
typedef nohArvore* pNohArvore;

struct NohArvore {
    void* info;
    nohArvore* direita;
    nohArvore* esquerda;
};

struct DescArvore {
    nohArvore* raiz;
    int quantidade;
};

// ####################### UTILS #######################

typedef int (*FuncaoComparacao) (void*, void*);

typedef void (*FuncaoImpressao) (void*);

void freeNoh(pNohArvore raiz) {
    free(raiz->esquerda);
    free(raiz->direita);
    free(raiz->info);
    free(raiz);
}

// ####################### cabeçalhos #######################

pNohArvore findBy(pDArvore arvore, void* info, FuncaoComparacao fcp);

pNohArvore proxLeaf(pNohArvore raiz, void* info, FuncaoComparacao fcp);

int isLeaf(pNohArvore raiz);

// ####################### Implementações #######################

//------------------------------ instantiate --------------------------------------------
pDArvore criarArvore() {
    pDArvore dArvore = malloc(sizeof(DArvore));
    dArvore->quantidade = 0;
    dArvore->raiz = NULL;

    return dArvore;
}

//------------------------------ Add --------------------------------------------

pNohArvore incluirInfoRecursivo(pNohArvore raiz, void* info, FuncaoComparacao fcp) {
    if (raiz == NULL) {
        pNohArvore noh = malloc(sizeof(nohArvore));
        noh->info = info;
        noh->esquerda = NULL;
        noh->direita = NULL;

        return noh;
    } else if (fcp(info, raiz->info) > 0) {
        raiz->direita = incluirInfoRecursivo(raiz->direita, info, fcp);
    } else if (fcp(info, raiz->info) < 0) {
        raiz->esquerda = incluirInfoRecursivo(raiz->esquerda, info, fcp);
    } else if ( fcp(info, raiz->info) == 0 ) {
        if ( raiz->esquerda != NULL  && raiz->direita == NULL) {
            raiz->direita = incluirInfoRecursivo(raiz->direita, info, fcp);
        }
        else {
            raiz->esquerda = incluirInfoRecursivo(raiz->direita, info, fcp);
        }
    }

    return raiz;
}

void incluirInfo(pDArvore arvore, void* info, FuncaoComparacao fcp) {
    arvore->raiz = incluirInfoRecursivo(arvore->raiz, info, fcp);
    arvore->quantidade++;
}

//--------------------------- Print -----------------------------------------------

void imprimeArvoreRecursiva(pNohArvore raiz, FuncaoImpressao fip ) {
    fip(raiz->info);
    if (raiz ->direita != NULL) {
            printf(", ");
        imprimeArvoreRecursiva(raiz->direita, fip);
    }
    if (raiz->esquerda != NULL) {
         printf(", ", AlturaArvoreRecursiva(raiz));
        imprimeArvoreRecursiva(raiz->esquerda, fip);
    }
}

void imprimeArvore ( pDArvore arvore, FuncaoImpressao fip ) {
    printf("Arvore: ");
    imprimeArvoreRecursiva(arvore->raiz, fip);
}

//---------------------- Height ----------------------------------------------------
int AlturaArvoreRecursiva(pNohArvore raiz) {
    if ( raiz == NULL)
        return 0;
    if ( isLeaf(raiz))
        return 1;

    int alturaDir = 1 + AlturaArvoreRecursiva(raiz->direita);
    int alturaEsq = 1 + AlturaArvoreRecursiva(raiz->esquerda);

    if (alturaEsq > alturaDir)
        return alturaEsq;
    else
        return alturaDir;


}

int AlturaArvore(pDArvore arvore) {
    return AlturaArvoreRecursiva(arvore->raiz);
}

// ---------------------- Remove ----------------------------------------------------
pNohArvore removeInfoRecursivo(pNohArvore raiz, void* info, FuncaoComparacao fcp){
    //procura o Noh que se quer remover
    if ( fcp(info, raiz->info) > 0)
        raiz->direita = removeInfoRecursivo(raiz->direita, info, fcp);
    else if ( fcp(info, raiz->info) < 0)
        raiz->esquerda = removeInfoRecursivo(raiz->esquerda, info, fcp);
    else {
            //encontrou o noh
        printaInt(raiz->info);
        if ( isLeaf(raiz) == 1) { //caso nao tenha filhos
            freeNoh(raiz);
            return NULL;
        }
        else if ( raiz->direita != NULL && raiz->esquerda != NULL ) { //caso tenha 2 filhos
                // Funcao para encontrar o noh mais proximo da raiz desejada
            pNohArvore preNewRoot = preMaxNoh(raiz);
            pNohArvore newRoot = preNewRoot->direita;

                //caso o nó mais proximo da raiz nao seja uma folha
            if ( newRoot != NULL)
                preNewRoot->direita = newRoot->esquerda;
            else
                preNewRoot->direita = NULL;

                //faco que meu newNoh aponte para os mesmos Nohs da raiz
            newRoot->direita = raiz->direita;
            newRoot->esquerda = raiz->esquerda;

                //tiro as ligacoes da raiz com o resto da arvore para limpar adequadamente
            raiz->direita = NULL;
            raiz->esquerda = NULL;
            freeNoh(raiz);

            return newRoot;


        }
        else if ( raiz->direita != NULL ) {

                //Faz com que o codigo retorne a parna direita da raiz e garanta que a raiz seja adequadamente liberada
            pNohArvore aux = raiz->direita;
            raiz->direita = NULL;
            raiz->esquerda = NULL;
            freeNoh(raiz);

            return aux;
        }
        else if ( raiz->esquerda != NULL ) {
            printf("\nfilho a esquerda");

              //Faz com que o codigo retorne a parna esquerda da raiz e garanta que a raiz seja adequadamente liberada
            pNohArvore aux = raiz->esquerda;
            raiz->direita = NULL;
            raiz->esquerda = NULL;
            freeNoh(raiz);

            return aux;
        }
    }

  return raiz;
}


int removeInfo(pDArvore arvore, void* info, FuncaoComparacao fcp) {
    if ( findBy(arvore, info, fcp) == NULL) {
            printf("nao achou");
        return 0;
    } else {
        arvore->raiz = removeInfoRecursivo(arvore->raiz, info, fcp);
        arvore->quantidade--;
        return 1;
    }
}

pNohArvore preMaxNoh(pNohArvore raiz) {
    pNohArvore aux = raiz->esquerda;
    while ( aux->direita->direita != NULL) {
        aux = aux->direita;
    }
    return aux;
}

// ---------------------- FindBy ----------------------------------------------------

pNohArvore proxLeaf(pNohArvore raiz, void* info, FuncaoComparacao fcp) {
    if (raiz == NULL) {
            printf("raiz é nula");
        return NULL;
    }

    if ( raiz->direita != NULL && fcp(raiz->info, info) < 0) {
            return raiz->direita;
    }

    if ( raiz->esquerda != NULL && fcp( raiz->info, info ) > 0) {
            return raiz->esquerda;

    }

    return NULL;
}


pNohArvore findByRecursivo(pNohArvore raiz, void* info, FuncaoComparacao fcp) {
    if (raiz == NULL) {
        return NULL; // Se raiz for nulo, não há mais nó para procurar
    }

    // Verifica se a raiz atual corresponde ao valor buscado
    if (fcp(raiz->info, info) == 0) {
        return raiz;
    }

    pNohArvore prox = proxLeaf(raiz, info, fcp);

    return findByRecursivo(prox, info, fcp);

}



pNohArvore findBy(pDArvore arvore, void* info, FuncaoComparacao fcp) {
    return findByRecursivo(arvore->raiz, info, fcp);;
}



//---------------------- isLeaf ---------------------------------------

int isLeaf(pNohArvore raiz) {
    if ( raiz->direita == NULL && raiz->esquerda == NULL ) {
    return 1;
    }else {
    return 0;
    }
}

