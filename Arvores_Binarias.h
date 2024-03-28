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
    //caso a raiz seja nula
    if (raiz == NULL) {
            printf("\nNulo\n");
        return NULL;
    }

    if ( raiz->esquerda != NULL && fcp(info, raiz->info) < 0 ) {
        raiz->esquerda = removeInfoRecursivo(raiz->esquerda, info, fcp);
    }
    else if ( raiz->direita != NULL && fcp(info, raiz->info ) > 0 ) {
        raiz->direita = removeInfoRecursivo(raiz->direita, info, fcp);
    }
    // Primeiro caso: se o nó encontrado é uma folha

    if (fcp(raiz->info, info) == 0 && isLeaf(raiz) == 1 ) {
            printaInt(raiz->info);
                    /*
                    desc
                     |
                      0
                    /   \
                  -5     1
                  / \     \
                -6  -3     3
                    / \   /
                  -4  -1 2
                       /
                      -2 <- esse por exemplo

                    */
        freeNoh(raiz);
        return NULL;
    }

    // se o nó nao é folha
    if ( fcp(raiz->info, info) == 0 && isLeaf(raiz) == 0) {
            // caso 3: é uma raiz com 2 filhos
        if ( raiz->direita != NULL && raiz->esquerda != NULL) {
                /*
                    desc
                      |
                      0 <- esse por exemplo
                    /   \
                  -5     1
                  / \     \
                -6  -3<PNR 2
                    / \     \
                  -4  -1<NR  3
                       /
                      -2
                Neste caso o ideal é procurar o nó mais proximo desta raiz com 2 filhos
                que por padrao deifinimos que sera o à esquerda e mais a direita
                possivel neste caso o -1
                    */
                        //PNR
            pNohArvore preNewRoot = raiz->esquerda;
            pNohArvore newRoot = preNewRoot;

            while (newRoot->direita != NULL) {
                preNewRoot = newRoot;
                newRoot = newRoot->direita;
            }

            if ( newRoot != preNewRoot && newRoot->esquerda != NULL ) {
                preNewRoot->direita = newRoot->esquerda;
                newRoot->direita = raiz->direita;
                newRoot->esquerda = raiz->esquerda;
            }

            if( newRoot == preNewRoot) {
                newRoot->direita = raiz->direita;
            }
            /*
            //para teste
            printf("\npreNewRoot: ");
            printaInt(preNewRoot->info);
            printf("NewRoot: ");
            printaInt(newRoot->info);
            printf("raiz: ");
            printaInt(raiz->info);
            printf("\n");
            */
            raiz->direita = NULL;
            raiz->esquerda = NULL;
            free(raiz);

            return newRoot;

        }

        // caso 2: é uma raiz com 1 filho
        else if (raiz->direita != NULL && raiz->esquerda == NULL) {
                    /*
                    desc
                     |
                      0
                    /   \
                  -5     1
                  / \     \
                -6  -3     2 <- esse por exemplo
                    / \     \
                  -4  -1     3
                       /
                      -2

                    */
            pNohArvore aux = raiz->direita;
            raiz->direita = NULL;
            freeNoh(raiz);
            return aux;

        }

        // caso 2: é uma raiz com 1 filho
        else if (raiz->direita == NULL && raiz->esquerda != NULL) {
                     /*
                    desc
                     |
                      0
                    /   \
                  -5     1
                  / \     \
                -6  -3     2 - 3
                    / \
                  -4  -1 <- esse por exemplo
                       /
                      -2

                    */
            pNohArvore aux = raiz->esquerda;
            raiz->esquerda = NULL;
            freeNoh(raiz);
            return aux;
        }

        else { printf("ERRO GRAVE!!!!");}
    }

    return raiz;
}

int removeInfo(pDArvore arvore, void* info, FuncaoComparacao fcp) {
    if ( findBy(arvore, info, fcp) == NULL) {
        return 0;
    } else {
        arvore->raiz = removeInfoRecursivo(arvore->raiz, info, fcp);
        arvore->quantidade--;
        return 1;
    }
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

