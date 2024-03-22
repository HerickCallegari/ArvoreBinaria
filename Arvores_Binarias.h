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
            printf("Direita %d: ", AlturaArvoreRecursiva(raiz));
        imprimeArvoreRecursiva(raiz->direita, fip);
    }
    if (raiz->esquerda != NULL) {
         printf("Esquerda %d: ", AlturaArvoreRecursiva(raiz));
        imprimeArvoreRecursiva(raiz->esquerda, fip);
    }
}

void imprimeArvore ( pDArvore arvore, FuncaoImpressao fip ) {
    printf("Raiz Principal: ");
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
        return NULL;
    }

    // Primeiro caso: se o nó encontrado é uma folha

    if (fcp(raiz->info, info) == 0 && isLeaf(raiz) == 1 ) {
     /*

                    desc
                     |
                     0
                    / \
                  -5   1
                  / \
                -6  -3
                    / \
                  -4  -1
                       /
                      -2 <- esse por exemplo

                    */
        freeNoh(raiz);
        return NULL;
    }

    pNohArvore aux = removeInfoRecursivo(proxLeaf(raiz, info, fcp), info, fcp);
    return raiz;
}

int removeInfo(pDArvore arvore, void* info, FuncaoComparacao fcp) {
    arvore->raiz = removeInfoRecursivo(arvore->raiz, info, fcp);
return 1;
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
    if (prox == NULL) {
        printf("ERRO\n");
    }
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

