#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------------- Struct Sala ---------------- //
typedef struct Sala {
    char nome[50];          // Nome do cômodo
    struct Sala *esquerda;  // Ponteiro para sala à esquerda
    struct Sala *direita;   // Ponteiro para sala à direita
} Sala;

// ---------------- Funções ---------------- //

// Cria uma sala de forma dinâmica
Sala* criarSala(const char *nome) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if(nova == NULL) {
        printf("Erro de alocacao de memoria!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// Função recursiva de exploração da mansão
void explorarSalas(Sala *atual) {
    if(atual == NULL) return;

    printf("\nVoce esta na sala: %s\n", atual->nome);

    // Verifica se a sala tem caminhos
    if(atual->esquerda == NULL && atual->direita == NULL) {
        printf("Nao ha mais caminhos a seguir. Fim da exploracao.\n");
        return;
    }

    char escolha;
    do {
        printf("Escolha o caminho: esquerda (e), direita (d) ou sair (s): ");
        scanf(" %c", &escolha);

        if(escolha == 'e') {
            explorarSalas(atual->esquerda);
            break;
        } else if(escolha == 'd') {
            explorarSalas(atual->direita);
            break;
        } else if(escolha == 's') {
            printf("Exploracao encerrada pelo jogador.\n");
            break;
        } else {
            printf("Opcao invalida! Digite 'e', 'd' ou 's'.\n");
        }
    } while(1);
}

// Libera memória da árvore
void liberarMansao(Sala *atual) {
    if(atual == NULL) return;
    liberarMansao(atual->esquerda);
    liberarMansao(atual->direita);
    free(atual);
}

// ---------------- Main ---------------- //
int main() {
    // Criando o mapa da mansao
    Sala *hall = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *cozinha = criarSala("Cozinha");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *jardim = criarSala("Jardim");
    Sala *quarto = criarSala("Quarto do Mestre");
    Sala *escritorio = criarSala("Escritorio");

    // Montando a árvore
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    cozinha->esquerda = quarto;
    cozinha->direita = escritorio;

    // Início da exploração
    printf("Bem-vindo ao Detective Quest! Explore a mansao.\n");
    explorarSalas(hall);

    // Liberando memória
    liberarMansao(hall);

    printf("\nExploracao finalizada. Obrigado por jogar!\n");
    return 0;
}
