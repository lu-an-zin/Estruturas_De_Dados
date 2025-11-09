#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[30];
    char cor[15];
    int tropas;
} Territorio;

// ======== FUNÇÕES AUXILIARES ========

// Exibe o mapa completo
void exibirMapa(Territorio* territorios, int qtd) {
    printf("\n====================================\n");
    printf("       ESTADO ATUAL DO MAPA         \n");
    printf("====================================\n");
    for (int i = 0; i < qtd; i++) {
        printf("%d. %-15s | Exército: %-10s | Tropas: %d\n",
               i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
    }
    printf("====================================\n");
}

// Simula um ataque entre dois territórios
void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAtaque = rand() % 100 + 1;
    int dadoDefesa = rand() % 100 + 1;

    printf("\n[ATAQUE] %s (%s) está atacando %s (%s)!\n",
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);
    printf(" - Dado do ataque: %d\n", dadoAtaque);
    printf(" - Dado da defesa: %d\n", dadoDefesa);

    if (dadoAtaque > dadoDefesa) {
        printf(">>> O ATACANTE venceu a batalha!\n");
        strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor) - 1);
        defensor->cor[sizeof(defensor->cor) - 1] = '\0';
        defensor->tropas /= 2;
        if (defensor->tropas < 1) defensor->tropas = 1;
    } else {
        printf(">>> O DEFENSOR resistiu ao ataque!\n");
        atacante->tropas = atacante->tropas * 3 / 4;
        if (atacante->tropas < 1) atacante->tropas = 1;
    }
}

// ======== SISTEMA DE MISSÕES ========

// Sorteia uma missão e copia para o destino
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

// Verifica se a missão foi cumprida (lógica simples de exemplo)
int verificarMissao(char* missao, Territorio* mapa, int tamanho) {
    // Exemplo 1: "Conquistar 3 territórios seguidos" → conta territórios da mesma cor
    if (strcmp(missao, "Conquistar 3 territorios seguidos") == 0) {
        int contador = 1, maxSeq = 1;
        for (int i = 1; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, mapa[i - 1].cor) == 0) {
                contador++;
                if (contador > maxSeq) maxSeq = contador;
            } else contador = 1;
        }
        return (maxSeq >= 3);
    }

    // Exemplo 2: "Eliminar exercito vermelho" → verifica se há tropas vermelhas
    if (strcmp(missao, "Eliminar exercito vermelho") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "vermelho") == 0)
                return 0; // ainda há exército vermelho
        }
        return 1; // missão cumprida
    }

    // Exemplo 3: "Controlar todos os territorios" → verifica se todos têm a mesma cor
    if (strcmp(missao, "Controlar todos os territorios") == 0) {
        char corBase[15];
        strcpy(corBase, mapa[0].cor);
        for (int i = 1; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corBase) != 0)
                return 0;
        }
        return 1;
    }

    // Exemplo 4: "Ter mais de 50 tropas no total"
    if (strcmp(missao, "Ter mais de 50 tropas no total") == 0) {
        int totalTropas = 0;
        for (int i = 0; i < tamanho; i++)
            totalTropas += mapa[i].tropas;
        return (totalTropas > 50);
    }

    // Exemplo 5: "Possuir ao menos 2 territorios azuis"
    if (strcmp(missao, "Possuir ao menos 2 territorios azuis") == 0) {
        int contAzuis = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "azul") == 0)
                contAzuis++;
        }
        return (contAzuis >= 2);
    }

    return 0; // missão não cumprida
}

// ======== FUNÇÃO PRINCIPAL ========

int main() {
    srand(time(NULL));

    int qtd;
    printf("Quantos territórios deseja cadastrar? ");
    while (scanf("%d", &qtd) != 1 || qtd <= 0) {
        printf("Digite um número válido: ");
        while (getchar() != '\n');
    }

    Territorio* territorios = (Territorio*) calloc(qtd, sizeof(Territorio));
    if (!territorios) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    printf("\n=== CADASTRO DE TERRITÓRIOS ===\n");
    for (int i = 0; i < qtd; i++) {
        printf("\nTerritório %d:\n", i + 1);
        printf(" Nome: ");
        scanf(" %[^\n]", territorios[i].nome);
        printf(" Cor do exército: ");
        scanf(" %[^\n]", territorios[i].cor);
        printf(" Tropas: ");
        while (scanf("%d", &territorios[i].tropas) != 1 || territorios[i].tropas <= 0) {
            printf("Digite um número válido de tropas: ");
            while (getchar() != '\n');
        }
    }

    exibirMapa(territorios, qtd);

    // ======== SISTEMA DE MISSÕES ========
    char* missoes[] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar exercito vermelho",
        "Controlar todos os territorios",
        "Ter mais de 50 tropas no total",
        "Possuir ao menos 2 territorios azuis"
    };
    int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);

    // Alocação dinâmica da missão do jogador
    char* missaoJogador = (char*) malloc(100 * sizeof(char));
    if (!missaoJogador) {
        printf("Erro ao alocar memória para a missão!\n");
        free(territorios);
        return 1;
    }

    // Sorteia e exibe a missão
    atribuirMissao(missaoJogador, missoes, totalMissoes);
    printf("\n=== Sua missão secreta ===\n");
    printf("%s\n", missaoJogador);

    // ======== LOOP DE ATAQUES ========
    char continuar = 's';
    while (continuar == 's' || continuar == 'S') {
        int atacante, defensor;

        printf("\nEscolha o território ATACANTE (1 a %d): ", qtd);
        while (scanf("%d", &atacante) != 1 || atacante < 1 || atacante > qtd) {
            printf("Digite um número válido: ");
            while (getchar() != '\n');
        }

        printf("Escolha o território DEFENSOR (1 a %d, diferente de %d): ", qtd, atacante);
        while (scanf("%d", &defensor) != 1 || defensor < 1 || defensor > qtd || defensor == atacante) {
            printf("Digite um número válido: ");
            while (getchar() != '\n');
        }

        atacar(&territorios[atacante - 1], &territorios[defensor - 1]);
        exibirMapa(territorios, qtd);

        // Verificação da missão
        if (verificarMissao(missaoJogador, territorios, qtd)) {
            printf("\n🎉 MISSÃO CUMPRIDA! Você venceu o jogo! 🎉\n");
            break;
        }

        printf("\nDeseja realizar outro ataque? (s/n): ");
        scanf(" %c", &continuar);
    }

    free(territorios);
    free(missaoJogador);
    printf("\nJogo encerrado. Memória liberada.\n");
    return 0;
}