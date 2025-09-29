#include <stdio.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
}Territorio;


int main(){
    Territorio territorios[5];

    printf("Vamos cadastrar os 5 territórios iniciais do nosso mundo\n");

    for (int i = 0; i < 5; i++)
    {
        printf("--------Cadastrando território %d--------\n", (i+1));
        printf("Nome do Território: \n");
        scanf("%s", territorios[i].nome);
        printf("Cor do Exército: \n");
        scanf("%s", territorios[i].cor);
        printf("número de tropas: \n");
        scanf("%d", &territorios[i].tropas);
    }
    
    


    return 0;
}
