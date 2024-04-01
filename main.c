#include <stdio.h>
#include <stdlib.h>
#include "Arvores_Binarias.h"

int main()
{
    pDArvore desc = criarArvore();

    /*
    for ( int i = 0; i < 10; i++) {
    incluirInfo(desc,alocaInt(i),ComInt);
    }
    for ( int i = -1; i > -10; i--) {
    incluirInfo(desc,alocaInt(i),ComInt);
    }
    */
    incluirInfo(desc, alocaInt(150), CompInt);
    incluirInfo(desc, alocaInt(100), CompInt);
    incluirInfo(desc, alocaInt(200), CompInt);
    incluirInfo(desc, alocaInt(99), CompInt);
    incluirInfo(desc, alocaInt(110), CompInt);
    incluirInfo(desc, alocaInt(109), CompInt);
    incluirInfo(desc, alocaInt(98), CompInt);




    printf("\nFoi possivel fazer a operacao: %d\n",removeInfo(desc, alocaInt(110), CompInt));
    printf("\nQuantidade de noh: %d\n", desc->quantidade);
    printf("\n");

    imprimeArvore(desc, printaInt);
}
