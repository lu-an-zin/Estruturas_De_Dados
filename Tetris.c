#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5 // número máximo de peças na fila

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome;  // tipo da peça ('I', 'O', 'T', 'L')
    int id;     // identificador único
} Peca;

// Estrutura que representa a fila
typedef struct {
    Peca pecas[TAMANHO_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

// ---------- FUNÇÕES AUXILIARES ----------

// Inicializa a fila vazia
void inicializarFila(Fila *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->quantidade = 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *fila) {
    return fila->quantidade == TAMANHO_FILA;
}

// Verifica se a fila está vazia
int filaVazia(Fila *fila) {
    return fila->quantidade == 0;
}

// Gera uma nova peça automaticamente
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4]; // seleciona tipo aleatoriamente
    nova.id = id;
    return nova;
}

// Insere uma nova peça na fila (enqueue)
void inserirPeca(Fila *fila, Peca nova) {
    if (filaCheia(fila)) {
        printf("\n  A fila está cheia! Não é possível inserir uma nova peça.\n");
        return;
    }
    fila->tras = (fila->tras + 1) % TAMANHO_FILA;
    fila->pecas[fila->tras] = nova;
    fila->quantidade++;
    printf("\n Peça [%c %d] adicionada à fila!\n", nova.nome, nova.id);
}

// Remove a peça da frente da fila (dequeue)
void jogarPeca(Fila *fila) {
    if (filaVazia(fila)) {
        printf("\n  A fila está vazia! Nenhuma peça para jogar.\n");
        return;
    }

    Peca jogada = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % TAMANHO_FILA;
    fila->quantidade--;

    printf("\n Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
}

// Exibe o estado atual da fila
void exibirFila(Fila *fila) {
    printf("\n--- Fila de peças ---\n");
    if (filaVazia(fila)) {
        printf("(vazia)\n");
        return;
    }

    int i;
    int indice = fila->frente;
    for (i = 0; i < fila->quantidade; i++) {
        printf("[%c %d] ", fila->pecas[indice].nome, fila->pecas[indice].id);
        indice = (indice + 1) % TAMANHO_FILA;
    }
    printf("\n----------------------\n");
}

// ---------- FUNÇÃO PRINCIPAL ----------

int main() {
    Fila fila;
    int opcao;
    int contadorIDs = 0; // controla o ID das peças criadas

    srand(time(NULL));
    inicializarFila(&fila);

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAMANHO_FILA; i++) {
        inserirPeca(&fila, gerarPeca(contadorIDs++));
    }

    // Loop principal do menu
    do {
        exibirFila(&fila);
        printf("\nOpções de ação:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                jogarPeca(&fila);
                break;
            case 2:
                inserirPeca(&fila, gerarPeca(contadorIDs++));
                break;
            case 0:
                printf("\n Saindo do jogo...\n");
                break;
            default:
                printf("\n Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}