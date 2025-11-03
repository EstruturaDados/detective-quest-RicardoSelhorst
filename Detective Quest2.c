#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------------- Structs ---------------- //

// Nó da árvore de salas (mapa da mansão)
typedef struct Sala {
    char nome[50];          // Nome do cômodo
    char pista[100];        // Pista encontrada na sala
    struct Sala *esquerda;  // Ponteiro para sala à esquerda
    struct Sala *direita;   // Ponteiro para sala à direita
} Sala;

// Nó da BST de pistas coletadas
typedef struct PistaNode {
    char conteudo[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// ---------------- Funções ---------------- //

// Cria dinamicamente uma sala com ou sem pista
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if(!nova) {
        printf("Erro de alocacao de memoria!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    if(pista != NULL)
        strcpy(nova->pista, pista);
    else
        strcpy(nova->pista, "");
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// Cria um novo nó de pista
PistaNode* criarPistaNode(const char *conteudo) {
    PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
    if(!novo) {
        printf("Erro de alocacao de memoria!\n");
        exit(1);
    }
    strcpy(novo->conteudo, conteudo);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// Insere uma pista na BST (ordem alfabética)
PistaNode* inserirPista(PistaNode *raiz, const char *conteudo) {
    if(raiz == NULL) return criarPistaNode(conteudo);

    if(strcmp(conteudo, raiz->conteudo) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, conteudo);
    else if(strcmp(conteudo, raiz->conteudo) > 0)
        raiz->direita = inserirPista(raiz->direita, conteudo);
    // Se for igual, não insere (evita duplicados)
    return raiz;
}

// Exibe as pistas da BST em ordem alfabética
void exibirPistas(PistaNode *raiz) {
    if(raiz == NULL) return;
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->conteudo);
    exibirPistas(raiz->direita);
}

// Exploração da mansão e coleta de pistas
void explorarSalasComPistas(Sala *atual, PistaNode **raizPistas) {
    if(atual == NULL) return;

    printf("\nVoce esta na sala: %s\n", atual->nome);

    // Coleta da pista, se houver
    if(strlen(atual->pista) > 0) {
        printf("Voce encontrou uma pista: %s\n", atual->pista);
        *raizPistas = inserirPista(*raizPistas, atual->pista);
    }

    // Menu de exploração
    char escolha;
    do {
        printf("Escolha o caminho: esquerda (e), direita (d), sair (s): ");
        scanf(" %c", &escolha);

        if(escolha == 'e') {
            explorarSalasComPistas(atual->esquerda, raizPistas);
            break;
        } else if(escolha == 'd') {
            explorarSalasComPistas(atual->direita, raizPistas);
            break;
        } else if(escolha == 's') {
            printf("Exploracao encerrada pelo jogador.\n");
            break;
        } else {
            printf("Opcao invalida! Digite 'e', 'd' ou 's'.\n");
        }
    } while(1);
}

// Libera memória da árvore de salas
void liberarMansao(Sala *atual) {
    if(atual == NULL) return;
    liberarMansao(atual->esquerda);
    liberarMansao(atual->direita);
    free(atual);
}

// Libera memória da BST de pistas
void liberarPistas(PistaNode *raiz) {
    if(raiz == NULL) return;
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

// ---------------- Main ---------------- //
int main() {
    // Criando mapa da mansão com pistas
    Sala *hall = criarSala("Hall de Entrada", "Pegada misteriosa");
    Sala *salaEstar = criarSala("Sala de Estar", "Chave dourada");
    Sala *cozinha = criarSala("Cozinha", "Receita suspeita");
    Sala *biblioteca = criarSala("Biblioteca", "Livro antigo aberto");
    Sala *jardim = criarSala("Jardim", "Pegada de lama");
    Sala *quarto = criarSala("Quarto do Mestre", "Carta rasgada");
    Sala *escritorio = criarSala("Escritorio", "Diario secreto");

    // Montando árvore da mansão
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    cozinha->esquerda = quarto;
    cozinha->direita = escritorio;

    // Inicializa BST de pistas coletadas
    PistaNode *raizPistas = NULL;

    printf("Bem-vindo ao Detective Quest!\nExplore a mansao e colete pistas.\n");

    // Inicia exploração
    explorarSalasComPistas(hall, &raizPistas);

    // Exibe pistas coletadas
    printf("\nPistas coletadas em ordem alfabética:\n");
    exibirPistas(raizPistas);

    // Libera memória
    liberarMansao(hall);
    liberarPistas(raizPistas);

    printf("\nExploracao finalizada. Obrigado por jogar!\n");
    return 0;
}