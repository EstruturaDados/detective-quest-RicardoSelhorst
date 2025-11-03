#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 10

// ---------------- Structs ---------------- //

// Nó da árvore de salas
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Nó da BST de pistas coletadas
typedef struct PistaNode {
    char conteudo[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// Nó da tabela hash
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode *prox;
} HashNode;

// ---------------- Funções ---------------- //

// Cria dinamicamente uma sala
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = nova->direita = NULL;
    return nova;
}

// Cria um nó de pista
PistaNode* criarPistaNode(const char *conteudo) {
    PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
    strcpy(novo->conteudo, conteudo);
    novo->esquerda = novo->direita = NULL;
    return novo;
}

// Insere pista na BST
PistaNode* inserirPista(PistaNode *raiz, const char *conteudo) {
    if(!raiz) return criarPistaNode(conteudo);
    if(strcmp(conteudo, raiz->conteudo) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, conteudo);
    else if(strcmp(conteudo, raiz->conteudo) > 0)
        raiz->direita = inserirPista(raiz->direita, conteudo);
    return raiz;
}

// Exibe pistas em ordem alfabética
void exibirPistas(PistaNode *raiz) {
    if(!raiz) return;
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->conteudo);
    exibirPistas(raiz->direita);
}

// ---------------- Hash Table ---------------- //

// Função hash simples baseada na soma dos caracteres
int hash(const char *str) {
    int soma = 0;
    for(int i = 0; str[i] != '\0'; i++)
        soma += str[i];
    return soma % HASH_SIZE;
}

// Insere pista e suspeito na hash
void inserirNaHash(HashNode *tabela[], const char *pista, const char *suspeito) {
    int idx = hash(pista);
    HashNode *novo = (HashNode*) malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela[idx];
    tabela[idx] = novo;
}

// Consulta suspeito pela pista
char* encontrarSuspeito(HashNode *tabela[], const char *pista) {
    int idx = hash(pista);
    HashNode *atual = tabela[idx];
    while(atual) {
        if(strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}

// Libera memória da hash
void liberarHash(HashNode *tabela[]) {
    for(int i=0;i<HASH_SIZE;i++) {
        HashNode *atual = tabela[i];
        while(atual) {
            HashNode *tmp = atual;
            atual = atual->prox;
            free(tmp);
        }
    }
}

// ---------------- Exploração ---------------- //

void explorarSalas(Sala *atual, PistaNode **raizPistas, HashNode *tabela[]) {
    if(!atual) return;

    printf("\nVoce esta na sala: %s\n", atual->nome);

    if(strlen(atual->pista) > 0) {
        printf("Voce encontrou uma pista: %s\n", atual->pista);
        *raizPistas = inserirPista(*raizPistas, atual->pista);
    }

    char escolha;
    do {
        printf("Escolha: esquerda (e), direita (d), sair (s): ");
        scanf(" %c", &escolha);

        if(escolha == 'e') {
            explorarSalas(atual->esquerda, raizPistas, tabela);
            break;
        } else if(escolha == 'd') {
            explorarSalas(atual->direita, raizPistas, tabela);
            break;
        } else if(escolha == 's') {
            printf("Exploracao encerrada.\n");
            break;
        } else {
            printf("Opcao invalida!\n");
        }
    } while(1);
}

// Conta quantas pistas apontam para o suspeito
int contarPistasSuspeito(PistaNode *raiz, HashNode *tabela[], const char *suspeito) {
    if(!raiz) return 0;
    int count = 0;
    if(strcmp(encontrarSuspeito(tabela, raiz->conteudo), suspeito) == 0)
        count = 1;
    return count + contarPistasSuspeito(raiz->esquerda, tabela, suspeito) + contarPistasSuspeito(raiz->direita, tabela, suspeito);
}

// ---------------- Liberação de memória ---------------- //

void liberarMansao(Sala *atual) {
    if(!atual) return;
    liberarMansao(atual->esquerda);
    liberarMansao(atual->direita);
    free(atual);
}

void liberarPistas(PistaNode *raiz) {
    if(!raiz) return;
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

// ---------------- Main ---------------- //

int main() {
    // Criando mansão
    Sala *hall = criarSala("Hall", "Pegada misteriosa");
    Sala *cozinha = criarSala("Cozinha", "Receita suspeita");
    Sala *sala = criarSala("Sala de Estar", "Chave dourada");
    Sala *biblioteca = criarSala("Biblioteca", "Livro antigo aberto");

    hall->esquerda = cozinha;
    hall->direita = sala;
    cozinha->esquerda = biblioteca;

    // Inicializa BST
    PistaNode *raizPistas = NULL;

    // Inicializa hash
    HashNode *tabela[HASH_SIZE] = {0};
    inserirNaHash(tabela, "Pegada misteriosa", "Sr. Black");
    inserirNaHash(tabela, "Receita suspeita", "Sra. White");
    inserirNaHash(tabela, "Chave dourada", "Sr. Black");
    inserirNaHash(tabela, "Livro antigo aberto", "Sra. Green");

    printf("Bem-vindo ao Detective Quest!\nExplore a mansao e colete pistas.\n");

    explorarSalas(hall, &raizPistas, tabela);

    printf("\nPistas coletadas:\n");
    exibirPistas(raizPistas);

    // Acusacao final
    char suspeito[50];
    printf("\nQuem e o culpado? ");
    scanf(" %[^\n]", suspeito);

    int pistas = contarPistasSuspeito(raizPistas, tabela, suspeito);
    if(pistas >= 2)
        printf("Acusacao correta! %s e o culpado!\n", suspeito);
    else
        printf("Acusacao incorreta ou insuficiente. %s nao tem pistas suficientes.\n", suspeito);

    // Libera memória
    liberarMansao(hall);
    liberarPistas(raizPistas);
    liberarHash(tabela);

    printf("\nFim do jogo. Obrigado por jogar!\n");
    return 0;
}
