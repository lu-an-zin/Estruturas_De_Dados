#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10   // tamanho da tabela hash

// ------------------ Estrutura da Sala ------------------
typedef struct Sala {
    char nome[50];
    char pista[100]; // pista estática do cômodo
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// ------------------ Estrutura da Pista (BST) ------------------
typedef struct Pista {
    char descricao[100];
    struct Pista *esquerda;
    struct Pista *direita;
} Pista;

// ------------------ Estrutura para Hash ------------------
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode *proximo;
} HashNode;

HashNode* tabelaHash[TAM_HASH];

// ------------------ Função: criarSala ------------------
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = nova->direita = NULL;
    return nova;
}

// ------------------ Função: inserirPista (BST) ------------------
Pista* inserirPista(Pista *raiz, const char *descricao) {
    if (raiz == NULL) {
        Pista *nova = (Pista*) malloc(sizeof(Pista));
        strcpy(nova->descricao, descricao);
        nova->esquerda = nova->direita = NULL;
        return nova;
    }
    if (strcmp(descricao, raiz->descricao) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, descricao);
    else if (strcmp(descricao, raiz->descricao) > 0)
        raiz->direita = inserirPista(raiz->direita, descricao);
    return raiz;
}

// ------------------ Função: exibirPistas ------------------
void exibirPistas(Pista *raiz) {
    if (raiz == NULL) return;
    exibirPistas(raiz->esquerda);
    printf(" %s\n", raiz->descricao);
    exibirPistas(raiz->direita);
}

// ------------------ Funções Hash ------------------

// Hash simples por soma dos caracteres módulo TAM_HASH
int hashFunc(const char *str) {
    int soma = 0;
    for (int i = 0; str[i]; i++) soma += str[i];
    return soma % TAM_HASH;
}

// Inserir associação pista -> suspeito
void inserirNaHash(const char *pista, const char *suspeito) {
    int idx = hashFunc(pista);
    HashNode *novo = (HashNode*) malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = tabelaHash[idx];
    tabelaHash[idx] = novo;
}

// Encontrar suspeito associado a uma pista
char* encontrarSuspeito(const char *pista) {
    int idx = hashFunc(pista);
    HashNode *aux = tabelaHash[idx];
    while (aux) {
        if (strcmp(aux->pista, pista) == 0)
            return aux->suspeito;
        aux = aux->proximo;
    }
    return NULL;
}

// ------------------ Função: explorarSalas ------------------
void explorarSalas(Sala *salaAtual, Pista **arvorePistas) {
    char escolha;

    printf("\n Você está em: %s\n", salaAtual->nome);

    // Se houver pista, coleta automaticamente
    if (strlen(salaAtual->pista) > 0) {
        printf(" Pista encontrada: %s\n", salaAtual->pista);
        *arvorePistas = inserirPista(*arvorePistas, salaAtual->pista);
    }

    if (!salaAtual->esquerda && !salaAtual->direita) {
        printf(" Este cômodo não possui saídas.\n");
        return;
    }

    if (salaAtual->esquerda)
        printf(" Caminho à esquerda: %s\n", salaAtual->esquerda->nome);
    if (salaAtual->direita)
        printf(" Caminho à direita: %s\n", salaAtual->direita->nome);

    printf("Para onde deseja ir? (e = esquerda, d = direita, s = sair): ");
    scanf(" %c", &escolha);

    switch (escolha) {
        case 'e': case 'E':
            if (salaAtual->esquerda) explorarSalas(salaAtual->esquerda, arvorePistas);
            else printf(" Não há caminho à esquerda!\n");
            break;
        case 'd': case 'D':
            if (salaAtual->direita) explorarSalas(salaAtual->direita, arvorePistas);
            else printf(" Não há caminho à direita!\n");
            break;
        case 's': case 'S':
            printf(" Encerrando exploração.\n");
            return;
        default:
            printf(" Opção inválida!\n");
            explorarSalas(salaAtual, arvorePistas);
    }
}

// ------------------ Função: verificarSuspeitoFinal ------------------
void verificarSuspeitoFinal(Pista *arvorePistas, const char *acusado) {
    int contador = 0;
    if (!arvorePistas) return;

    // Contar pistas que apontam para o suspeito
    if (strcmp(encontrarSuspeito(arvorePistas->descricao), acusado) == 0)
        contador++;
    verificarSuspeitoFinal(arvorePistas->esquerda, acusado);
    verificarSuspeitoFinal(arvorePistas->direita, acusado);

    if (contador >= 2)
        printf("\n Acusação correta! Há evidências suficientes contra %s.\n", acusado);
    else
        printf("\n Acusação insuficiente! %s não possui pistas suficientes.\n", acusado);
}

// ------------------ Função principal ------------------
int main() {
    // ---------- Criando salas ----------
    Sala *hall = criarSala("Hall de Entrada", "Chave dourada no tapete");
    Sala *salaEstar = criarSala("Sala de Estar", "Carta suspeita na estante");
    Sala *cozinha = criarSala("Cozinha", "Pegadas de barro");
    Sala *biblioteca = criarSala("Biblioteca", "Livro fora do lugar");
    Sala *jardim = criarSala("Jardim", "Pegada de sapato");
    Sala *escritorio = criarSala("Escritório", "Diário aberto na gaveta");

    // ---------- Conectando salas ----------
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->direita = escritorio;

    // ---------- Inicializando BST e Hash ----------
    Pista *arvorePistas = NULL;
    for(int i = 0; i < TAM_HASH; i++) tabelaHash[i] = NULL;

    // ---------- Inserir associação pista -> suspeito ----------
    inserirNaHash("Chave dourada no tapete", "Sr. Verde");
    inserirNaHash("Carta suspeita na estante", "Sra. Rosa");
    inserirNaHash("Pegadas de barro", "Sr. Verde");
    inserirNaHash("Livro fora do lugar", "Sra. Rosa");
    inserirNaHash("Pegada de sapato", "Sr. Azul");
    inserirNaHash("Diário aberto na gaveta", "Sr. Azul");

    // ---------- Exploração ----------
    printf(" Bem-vindo(a) à mansão Enigma!\n");
    explorarSalas(hall, &arvorePistas);

    // ---------- Exiir todas as pistas ----------
    printf("\n Pistas coletadas (em ordem alfabética):\n");
    exibirPistas(arvorePistas);

    // ---------- Fase de acusação ----------
    char acusado[50];
    printf("\n Quem você acusa como culpado? Digite o nome: ");
    scanf(" %[^\n]", acusado);

    verificarSuspeitoFinal(arvorePistas, acusado);

    // ---------- Liberação de memória ----------
    free(hall); free(salaEstar); free(cozinha);
    free(biblioteca); free(jardim); free(escritorio);

    printf("\n Fim do jogo.\n");
    return 0;
}