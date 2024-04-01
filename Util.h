int CompInt(void* info1, void* info2) {

    int* inteiro1 = (int*)info1;
    int* inteiro2 = (int*)info2;

    return *inteiro1 - *inteiro2;
}

int* alocaInt(int n){
   int *pi = (int *) malloc(sizeof(int));
   *pi = n;
   return pi;
}

void printaInt(void* info) {
    int* numero = (int*)info;
    printf("%d", *numero);
}


