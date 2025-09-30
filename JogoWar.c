#include <stdio.h>

//declaração da estrutura struct
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
}Territorio;


int main(){
    //instanciando um vetor de 5 elementos
    Territorio territorios[5];

    printf("Vamos cadastrar os 5 territórios iniciais do nosso mundo\n");

    //laço para repetição do cadastro dos territórios
    for (int i = 0; i < 5; i++)
    {
        printf("--------Cadastrando território %d--------\n", (i+1));
        printf("Nome do Território: ");
        scanf(" %[^\n]", territorios[i].nome);
        printf("Cor do Exército: ");
        scanf(" %[^\n]", territorios[i].cor);

        int validador = 0; //vai verificar se a entrada é um numero
        while (!validador){
            printf("número de tropas: ");
            if (scanf("%d", &territorios[i].tropas) != 1){
                printf("Digite um número válido\n");
                while (getchar() !='\n'); // limpa o buff
            }else{
                validador = 1;
            }
            
        }
        
    }
    printf("====================================\n");
    printf("     MAPA DO MUNDO - ESTADO ATUAL     \n");
    printf("====================================\n");
    
    //laço para saída de dados
    for (int i = 0; i < 5; i++)
    {
        printf("Território %d\n", i+1);
        printf("   -Nome: %s\n", territorios[i].nome);
        printf("   -Dominado por: %s\n", territorios[i].cor);
        printf("   -Número de tropas: %d\n", territorios[i].tropas);
    }
    
    return 0;
}
