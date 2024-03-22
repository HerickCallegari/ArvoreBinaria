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
    incluirInfo(desc,alocaInt(0),ComInt);
    incluirInfo(desc,alocaInt(1),ComInt);
    incluirInfo(desc,alocaInt(2),ComInt);
    incluirInfo(desc,alocaInt(3),ComInt);
    incluirInfo(desc,alocaInt(-5),ComInt);
    incluirInfo(desc,alocaInt(-6),ComInt);
    incluirInfo(desc,alocaInt(-3),ComInt);
    incluirInfo(desc,alocaInt(-4),ComInt);
    incluirInfo(desc,alocaInt(-1),ComInt);
    incluirInfo(desc,alocaInt(-2),ComInt);




    printf("\nFoi possivel fazer a operacao: %d\n",removeInfo(desc, alocaInt(1), ComInt));
    printf("\n");

    imprimeArvore(desc, printaInt);
}
