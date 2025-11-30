/*
 * TETRIS STACK - Sistema Avançado de Fila e Pilha de Peças
 * ByteBros Game Development
 * 
 * Implementa fila circular e pilha com operações avançadas de troca.
 * Inclui troca simples (1 peça) e troca múltipla (3 peças).
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

// Estrutura que representa cada peça do Tetris
typedef struct {
    char nome;  // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;     // Identificador único da peça
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca pecas[MAX_FILA];
    int frente;    // índice do primeiro elemento
    int tras;      // índice do último elemento
    int tamanho;   // quantidade de elementos na fila
} FilaPecas;

// Estrutura da pilha de reserva
typedef struct {
    Peca pecas[MAX_PILHA];
    int topo;      // índice do topo da pilha (-1 = vazia)
} PilhaReserva;

// Contador global para IDs únicos
int proximoId = 0;

// Protótipos das funções
void inicializarFila(FilaPecas* fila);
void inicializarPilha(PilhaReserva* pilha);
Peca gerarPeca();
int filaCheia(FilaPecas* fila);
int filaVazia(FilaPecas* fila);
int pilhaCheia(PilhaReserva* pilha);
int pilhaVazia(PilhaReserva* pilha);
void enqueue(FilaPecas* fila);
Peca dequeue(FilaPecas* fila);
void push(PilhaReserva* pilha, Peca peca);
Peca pop(PilhaReserva* pilha);
void jogarPeca(FilaPecas* fila);
void reservarPeca(FilaPecas* fila, PilhaReserva* pilha);
void usarPecaReservada(PilhaReserva* pilha);
void trocarPecaSimples(FilaPecas* fila, PilhaReserva* pilha);
void trocarPecasMultiplas(FilaPecas* fila, PilhaReserva* pilha);
void exibirEstado(FilaPecas* fila, PilhaReserva* pilha);
void exibirMenu();

int main() {
    FilaPecas fila;
    PilhaReserva pilha;
    int opcao;
    
    // Inicializa gerador de números aleatórios
    srand(time(NULL));
    
    printf("===========================================================\n");
    printf("     TETRIS STACK - SISTEMA AVANCADO (NIVEL 3)\n");
    printf("===========================================================\n\n");
    
    // Inicializa as estruturas
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    
    printf("[+] Sistema inicializado!\n");
    printf("[+] Fila: %d pecas\n", fila.tamanho);
    printf("[+] Pilha de reserva: %d/%d espacos\n\n", pilha.topo + 1, MAX_PILHA);
    
    exibirEstado(&fila, &pilha);
    
    // Loop principal do jogo
    do {
        exibirMenu();
        scanf("%d", &opcao);
        printf("\n");
        
        switch(opcao) {
            case 1:
                jogarPeca(&fila);
                exibirEstado(&fila, &pilha);
                break;
                
            case 2:
                reservarPeca(&fila, &pilha);
                exibirEstado(&fila, &pilha);
                break;
                
            case 3:
                usarPecaReservada(&pilha);
                exibirEstado(&fila, &pilha);
                break;
                
            case 4:
                trocarPecaSimples(&fila, &pilha);
                exibirEstado(&fila, &pilha);
                break;
                
            case 5:
                trocarPecasMultiplas(&fila, &pilha);
                exibirEstado(&fila, &pilha);
                break;
                
            case 0:
                printf("===========================================================\n");
                printf("           Encerrando Tetris Stack...\n");
                printf("===========================================================\n");
                break;
                
            default:
                printf("[!] Opcao invalida! Tente novamente.\n");
        }
        
    } while(opcao != 0);
    
    return 0;
}

// Inicializa a fila com peças geradas automaticamente
void inicializarFila(FilaPecas* fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
    
    // Preenche a fila com peças iniciais
    for (int i = 0; i < MAX_FILA; i++) {
        fila->tras = (fila->tras + 1) % MAX_FILA;
        fila->pecas[fila->tras] = gerarPeca();
        fila->tamanho++;
    }
}

// Inicializa a pilha vazia
void inicializarPilha(PilhaReserva* pilha) {
    pilha->topo = -1;  // pilha vazia
}

// Gera uma peça aleatória com tipo e ID único
Peca gerarPeca() {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L'};
    
    // Sorteia um tipo aleatório
    novaPeca.nome = tipos[rand() % 4];
    
    // Atribui ID único
    novaPeca.id = proximoId++;
    
    return novaPeca;
}

// Verifica se a fila está cheia
int filaCheia(FilaPecas* fila) {
    return fila->tamanho == MAX_FILA;
}

// Verifica se a fila está vazia
int filaVazia(FilaPecas* fila) {
    return fila->tamanho == 0;
}

// Verifica se a pilha está cheia
int pilhaCheia(PilhaReserva* pilha) {
    return pilha->topo == MAX_PILHA - 1;
}

// Verifica se a pilha está vazia
int pilhaVazia(PilhaReserva* pilha) {
    return pilha->topo == -1;
}

// Insere uma nova peça no final da fila (enqueue)
void enqueue(FilaPecas* fila) {
    if (filaCheia(fila)) {
        return;  // não adiciona se já está cheia
    }
    
    // Avança o índice traseiro de forma circular
    fila->tras = (fila->tras + 1) % MAX_FILA;
    
    // Gera e insere nova peça
    fila->pecas[fila->tras] = gerarPeca();
    fila->tamanho++;
}

// Remove e retorna a peça da frente da fila (dequeue)
Peca dequeue(FilaPecas* fila) {
    Peca pecaRemovida = fila->pecas[fila->frente];
    
    // Avança o índice frontal de forma circular
    fila->frente = (fila->frente + 1) % MAX_FILA;
    fila->tamanho--;
    
    return pecaRemovida;
}

// Insere uma peça no topo da pilha (push)
void push(PilhaReserva* pilha, Peca peca) {
    if (pilhaCheia(pilha)) {
        return;  // não adiciona se já está cheia
    }
    
    pilha->topo++;
    pilha->pecas[pilha->topo] = peca;
}

// Remove e retorna a peça do topo da pilha (pop)
Peca pop(PilhaReserva* pilha) {
    Peca pecaRemovida = pilha->pecas[pilha->topo];
    pilha->topo--;
    
    return pecaRemovida;
}

// Joga a peça da frente da fila
void jogarPeca(FilaPecas* fila) {
    if (filaVazia(fila)) {
        printf("===========================================================\n");
        printf("[!] FILA VAZIA! Nao ha pecas para jogar.\n");
        printf("===========================================================\n");
        return;
    }
    
    // Remove peça da fila
    Peca pecaJogada = dequeue(fila);
    
    printf("===========================================================\n");
    printf("[+] Peca [%c %d] jogada com sucesso!\n", 
           pecaJogada.nome, pecaJogada.id);
    
    // Gera nova peça automaticamente
    enqueue(fila);
    printf("[+] Nova peca gerada e adicionada a fila.\n");
    printf("===========================================================\n");
}

// Reserva a peça da frente da fila (move para a pilha)
void reservarPeca(FilaPecas* fila, PilhaReserva* pilha) {
    if (filaVazia(fila)) {
        printf("===========================================================\n");
        printf("[!] FILA VAZIA! Nao ha pecas para reservar.\n");
        printf("===========================================================\n");
        return;
    }
    
    if (pilhaCheia(pilha)) {
        printf("===========================================================\n");
        printf("[!] PILHA CHEIA! Nao e possivel reservar mais pecas.\n");
        printf("[!] Use uma peca reservada primeiro.\n");
        printf("===========================================================\n");
        return;
    }
    
    // Remove peça da fila
    Peca pecaReservada = dequeue(fila);
    
    // Adiciona à pilha
    push(pilha, pecaReservada);
    
    printf("===========================================================\n");
    printf("[+] Peca [%c %d] movida para a pilha de reserva!\n", 
           pecaReservada.nome, pecaReservada.id);
    
    // Gera nova peça automaticamente
    enqueue(fila);
    printf("[+] Nova peca gerada e adicionada a fila.\n");
    printf("===========================================================\n");
}

// Usa uma peça da pilha de reserva
void usarPecaReservada(PilhaReserva* pilha) {
    if (pilhaVazia(pilha)) {
        printf("===========================================================\n");
        printf("[!] PILHA VAZIA! Nao ha pecas reservadas.\n");
        printf("[!] Reserve uma peca primeiro.\n");
        printf("===========================================================\n");
        return;
    }
    
    // Remove peça da pilha
    Peca pecaUsada = pop(pilha);
    
    printf("===========================================================\n");
    printf("[+] Peca reservada [%c %d] usada com sucesso!\n", 
           pecaUsada.nome, pecaUsada.id);
    printf("===========================================================\n");
}

// Troca a peça da frente da fila com o topo da pilha
void trocarPecaSimples(FilaPecas* fila, PilhaReserva* pilha) {
    if (filaVazia(fila)) {
        printf("===========================================================\n");
        printf("[!] FILA VAZIA! Nao e possivel trocar.\n");
        printf("===========================================================\n");
        return;
    }
    
    if (pilhaVazia(pilha)) {
        printf("===========================================================\n");
        printf("[!] PILHA VAZIA! Nao e possivel trocar.\n");
        printf("[!] Reserve uma peca primeiro.\n");
        printf("===========================================================\n");
        return;
    }
    
    // Guarda as peças antes da troca
    Peca pecaFila = fila->pecas[fila->frente];
    Peca pecaPilha = pilha->pecas[pilha->topo];
    
    // Realiza a troca
    fila->pecas[fila->frente] = pecaPilha;
    pilha->pecas[pilha->topo] = pecaFila;
    
    printf("===========================================================\n");
    printf("[+] TROCA SIMPLES REALIZADA!\n");
    printf("[+] Fila (frente): [%c %d] <- [%c %d]\n", 
           pecaFila.nome, pecaFila.id, pecaPilha.nome, pecaPilha.id);
    printf("[+] Pilha (topo):  [%c %d] <- [%c %d]\n", 
           pecaPilha.nome, pecaPilha.id, pecaFila.nome, pecaFila.id);
    printf("===========================================================\n");
}

// Troca as 3 primeiras peças da fila com as 3 da pilha
void trocarPecasMultiplas(FilaPecas* fila, PilhaReserva* pilha) {
    // Verifica se ambas têm pelo menos 3 peças
    if (fila->tamanho < 3) {
        printf("===========================================================\n");
        printf("[!] FILA INSUFICIENTE! Necessario pelo menos 3 pecas.\n");
        printf("[!] Pecas na fila: %d/3\n", fila->tamanho);
        printf("===========================================================\n");
        return;
    }
    
    if (pilha->topo + 1 < 3) {
        printf("===========================================================\n");
        printf("[!] PILHA INSUFICIENTE! Necessario pelo menos 3 pecas.\n");
        printf("[!] Pecas na pilha: %d/3\n", pilha->topo + 1);
        printf("===========================================================\n");
        return;
    }
    
    printf("===========================================================\n");
    printf("[+] TROCA MULTIPLA INICIADA!\n");
    printf("===========================================================\n");
    
    // Armazena temporariamente as 3 peças da fila
    Peca tempFila[3];
    int indice = fila->frente;
    for (int i = 0; i < 3; i++) {
        tempFila[i] = fila->pecas[indice];
        indice = (indice + 1) % MAX_FILA;
    }
    
    // Armazena temporariamente as 3 peças da pilha (topo -> base)
    Peca tempPilha[3];
    for (int i = 0; i < 3; i++) {
        tempPilha[i] = pilha->pecas[pilha->topo - i];
    }
    
    // Transfere peças da pilha para a fila (topo da pilha vai para frente da fila)
    indice = fila->frente;
    for (int i = 0; i < 3; i++) {
        fila->pecas[indice] = tempPilha[i];
        indice = (indice + 1) % MAX_FILA;
    }
    
    // Transfere peças da fila para a pilha (frente da fila vai para topo da pilha)
    for (int i = 0; i < 3; i++) {
        pilha->pecas[pilha->topo - i] = tempFila[i];
    }
    
    printf("[+] 3 pecas trocadas entre fila e pilha!\n");
    printf("[+] Fila recebeu: [%c %d] [%c %d] [%c %d]\n",
           tempPilha[0].nome, tempPilha[0].id,
           tempPilha[1].nome, tempPilha[1].id,
           tempPilha[2].nome, tempPilha[2].id);
    printf("[+] Pilha recebeu: [%c %d] [%c %d] [%c %d]\n",
           tempFila[0].nome, tempFila[0].id,
           tempFila[1].nome, tempFila[1].id,
           tempFila[2].nome, tempFila[2].id);
    printf("===========================================================\n");
}

// Exibe o estado atual da fila e da pilha
void exibirEstado(FilaPecas* fila, PilhaReserva* pilha) {
    printf("\n===========================================================\n");
    printf("                    ESTADO ATUAL\n");
    printf("===========================================================\n");
    
    // Exibe a fila
    printf("\nFila de pecas: ");
    if (filaVazia(fila)) {
        printf("(vazia)");
    } else {
        int indice = fila->frente;
        for (int i = 0; i < fila->tamanho; i++) {
            printf("[%c %d] ", fila->pecas[indice].nome, fila->pecas[indice].id);
            indice = (indice + 1) % MAX_FILA;
        }
    }
    printf("\n");
    
    // Exibe a pilha (topo -> base)
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(pilha)) {
        printf("(vazia)");
    } else {
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->pecas[i].nome, pilha->pecas[i].id);
        }
    }
    printf("\n");
    
    printf("\nPecas na fila: %d/%d\n", fila->tamanho, MAX_FILA);
    printf("Pecas na pilha: %d/%d\n", pilha->topo + 1, MAX_PILHA);
    printf("===========================================================\n");
}

// Exibe o menu de opções
void exibirMenu() {
    printf("\n");
    printf("-----------------------------------------------------------------------------------\n");
    printf("                            OPCOES DISPONIVEIS\n");
    printf("-----------------------------------------------------------------------------------\n");
    printf("Codigo 1 - Acao 1: Jogar peca na frente da fila\n");
    printf("Codigo 2 - Acao 2: Enviar peca da fila para a pilha reserva\n");
    printf("Codigo 3 - Acao 3: Usar peca da pilha de reserva\n");
    printf("Codigo 4 - Acao 4: Trocar peca da frente da fila com o topo da pilha\n");
    printf("Codigo 5 - Acao 5: Trocar os 3 primeiros da fila com as 3 pecas da pilha\n");
    printf("Codigo 0 - Acao 0: Sair\n");
    printf("-----------------------------------------------------------------------------------\n");
    printf("Escolha uma opcao: ");
}