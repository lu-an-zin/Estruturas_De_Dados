#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// Estrutura da peça
typedef struct {
    char tipo; // 'I', 'O', 'T', 'L'
    int id;
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca pecas[TAM_FILA];
    int frente;
    int tras;
    int qtd;
} Fila;

// Estrutura da pilha
typedef struct {
    Peca pecas[TAM_PILHA];
    int topo;
} Pilha;

// ---------- FUNÇÕES AUXILIARES ----------

void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->qtd = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int filaVazia(Fila *f) {
    return f->qtd == 0;
}

int filaCheia(Fila *f) {
    return f->qtd == TAM_FILA;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

// Gera nova peça automaticamente
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.tipo = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// Enfileira (inserir no final)
void enfileirar(Fila *f, Peca nova) {
    if (filaCheia(f)) return;
    f->tras = (f->tras + 1) % TAM_FILA;
    f->pecas[f->tras] = nova;
    f->qtd++;
}

// Desenfileira (remover da frente)
Peca desenfileirar(Fila *f) {
    Peca removida = {'?', -1};
    if (filaVazia(f)) return removida;
    removida = f->pecas[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->qtd--;
    return removida;
}

// Empilha (push)
void empilhar(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) {
        printf("\n  Pilha cheia! Não é possível reservar mais peças.\n");
        return;
    }
    p->topo++;
    p->pecas[p->topo] = nova;
    printf("\n Peça [%c %d] reservada!\n", nova.tipo, nova.id);
}

// Desempilha (pop)
Peca desempilhar(Pilha *p) {
    Peca removida = {'?', -1};
    if (pilhaVazia(p)) {
        printf("\n  Pilha vazia! Nenhuma peça reservada.\n");
        return removida;
    }
    removida = p->pecas[p->topo];
    p->topo--;
    printf("\n Peça reservada usada: [%c %d]\n", removida.tipo, removida.id);
    return removida;
}

// ---------- TROCAS ----------

// Troca simples entre frente da fila e topo da pilha
void trocarSimples(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("\n  Não é possível trocar — fila ou pilha vazia.\n");
        return;
    }
    Peca temp = f->pecas[f->frente];
    f->pecas[f->frente] = p->pecas[p->topo];
    p->pecas[p->topo] = temp;
    printf("\n Troca simples realizada entre frente da fila e topo da pilha!\n");
}

// Troca múltipla (as 3 primeiras da fila com as 3 da pilha)
void trocaMultipla(Fila *f, Pilha *p) {
    if (f->qtd < 3 || p->topo < 2) {
        printf("\n  Não é possível realizar troca múltipla — precisa de pelo menos 3 peças em cada.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int idxFila = (f->frente + i) % TAM_FILA;
        Peca temp = f->pecas[idxFila];
        f->pecas[idxFila] = p->pecas[p->topo - i];
        p->pecas[p->topo - i] = temp;
    }
    printf("\n Troca múltipla de 3 peças realizada!\n");
}

// ---------- EXIBIÇÃO ----------

void exibirFila(Fila *f) {
    printf("\nFila de peças futuras:\n");
    if (filaVazia(f)) {
        printf("(vazia)\n");
        return;
    }
    int i, idx = f->frente;
    for (i = 0; i < f->qtd; i++) {
        printf("[%c %d] ", f->pecas[idx].tipo, f->pecas[idx].id);
        idx = (idx + 1) % TAM_FILA;
    }
    printf("\n");
}

void exibirPilha(Pilha *p) {
    printf("\nPilha de peças reservadas:\n");
    if (pilhaVazia(p)) {
        printf("(vazia)\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d]\n", p->pecas[i].tipo, p->pecas[i].id);
    }
}

void exibirEstado(Fila *f, Pilha *p) {
    printf("\n==============================");
    exibirFila(f);
    exibirPilha(p);
    printf("==============================\n");
}

// ---------- MAIN ----------

int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    int contadorID = 0;

    srand(time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche fila inicial
    for (int i = 0; i < TAM_FILA; i++)
        enfileirar(&fila, gerarPeca(contadorID++));

    do {
        exibirEstado(&fila, &pilha);

        printf("\nAções disponíveis:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Reservar peça (fila → pilha)\n");
        printf("3 - Usar peça reservada (pop)\n");
        printf("4 - Trocar peça atual (fila ↔ pilha topo)\n");
        printf("5 - Troca múltipla (3 primeiras ↔ 3 do topo)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Peca jogada = desenfileirar(&fila);
                if (jogada.id != -1)
                    printf("\n Peça jogada: [%c %d]\n", jogada.tipo, jogada.id);
                // Repor nova peça
                enfileirar(&fila, gerarPeca(contadorID++));
                break;
            }
            case 2: {
                if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    // Repor nova peça
                    enfileirar(&fila, gerarPeca(contadorID++));
                } else if (pilhaCheia(&pilha)) {
                    printf("\n  Pilha cheia, não é possível reservar!\n");
                }
                break;
            }
            case 3:
                desempilhar(&pilha);
                break;
            case 4:
                trocarSimples(&fila, &pilha);
                break;
            case 5:
                trocaMultipla(&fila, &pilha);
                break;
            case 0:
                printf("\n Encerrando o programa...\n");
                break;
            default:
                printf("\n Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
