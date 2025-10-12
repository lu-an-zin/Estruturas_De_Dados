#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Necessário para calloc, free e rand
#include <time.h>   // Necessário para srand e randomizar os números

// Declaração da estrutura struct
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Função que simula o ataque
void atacar(Territorio* atacante, Territorio* defensor) {
    // Gerar números aleatórios para o ataque e defesa
    int ataque = rand() % 100 + 1;   // Valor entre 1 e 100
    int defesa = rand() % 100 + 1;   // Valor entre 1 e 100

    printf("\n[ATACANDO] %s (Exercito %s) vs %s (Exercito %s)\n",
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);
    printf("   - Dado de ataque: %d\n", ataque);
    printf("   - Dado de defesa: %d\n", defesa);

    // Verificar se o atacante venceu
    if (ataque > defesa) {
        printf("\nO ATACANTE venceu o DEFENSOR!\n");
        printf("O território %s agora é dominado pelo exercito %s!\n", defensor->nome, atacante->cor);

        // Atualizar a cor do exército (dono) do território defensor com segurança
        strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor) - 1);
        defensor->cor[sizeof(defensor->cor) - 1] = '\0';  // Garantir que a string seja terminada

        // O número de tropas do defensor diminui pela metade (ou outro critério de perda)
        defensor->tropas /= 2;
        if (defensor->tropas < 1) {
            defensor->tropas = 1; // Garantir que pelo menos uma tropa fique
        }
    } else {
        printf("\nO DEFENSOR venceu o ATACANTE!\n");
    }
}

// Função para exibir o estado do mapa
void exibirMapa(Territorio* territorios, int qtdTerritorios) {
    printf("\n====================================\n");
    printf("     MAPA DO MUNDO - ESTADO ATUAL   \n");
    printf("====================================\n");

    for (int i = 0; i < qtdTerritorios; i++) {
        printf("%d. %s (Exercito %s, Tropas: %d)\n",
               i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
    }
}

int main() {
    int quantidadeTerritorios;

    printf("Quantos territorios deseja cadastrar? ");
    while (scanf("%d", &quantidadeTerritorios) != 1 || quantidadeTerritorios <= 0) {
        printf("Digite um numero valido: ");
        while (getchar() != '\n'); // limpa o buffer
    }

    // Alocação dinâmica usando calloc
    Territorio* territorios = (Territorio*) calloc(quantidadeTerritorios, sizeof(Territorio));
    if (territorios == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    printf("\nVamos cadastrar os %d territorios iniciais do nosso mundo\n\n", quantidadeTerritorios);

    // Cadastro
    for (int i = 0; i < quantidadeTerritorios; i++) {
        printf("-------- Cadastrando territorio %d --------\n", i + 1);
        printf(" Nome do Territorio: ");
        scanf(" %[^\n]", territorios[i].nome);

        printf(" Cor do Exercito: ");
        scanf(" %[^\n]", territorios[i].cor);

        int validador = 0;
        while (!validador) {
            printf(" Numero de tropas: ");
            if (scanf("%d", &territorios[i].tropas) != 1) {
                printf("Digite um numero valido\n");
                while (getchar() != '\n'); // limpa o buffer
            } else {
                validador = 1;
            }
        }
        printf("\n");
    }

    // Exibição inicial do mapa
    exibirMapa(territorios, quantidadeTerritorios);

    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    char continuar = 's';
    while (continuar == 's' || continuar == 'S') {
        int atacanteIndex, defensorIndex;

        // Pergunta ao usuário qual território atacar
        printf("\nEscolha o território atacante (1 a %d): ", quantidadeTerritorios);
        while (scanf("%d", &atacanteIndex) != 1 || atacanteIndex < 1 || atacanteIndex > quantidadeTerritorios) {
            printf("Digite um número válido para o território atacante: ");
            while (getchar() != '\n'); // limpa o buffer
        }

        // Pergunta qual território será atacado
        printf("Escolha o território defensor (1 a %d): ", quantidadeTerritorios);
        while (scanf("%d", &defensorIndex) != 1 || defensorIndex < 1 || defensorIndex > quantidadeTerritorios || defensorIndex == atacanteIndex) {
            printf("Digite um número válido para o território defensor (diferente do atacante): ");
            while (getchar() != '\n'); // limpa o buffer
        }

        // Realiza o ataque
        atacar(&territorios[atacanteIndex - 1], &territorios[defensorIndex - 1]);

        // Exibe o mapa atualizado após o ataque
        exibirMapa(territorios, quantidadeTerritorios);

        // Pergunta se o usuário deseja continuar
        printf("\nDeseja realizar outro ataque? (s/n): ");
        scanf(" %c", &continuar); // O espaço antes de %c é para capturar o '\n' pendente
    }

    // Liberação de memória
    free(territorios);

    return 0;
}
