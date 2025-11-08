#include <stdio.h> // Pra entrada e saída de dados
#include <string.h> // Pra mexer com textos, tipo o nome

// --- 1. Constantes ---
#define MAX_STRING 50 // Tamanho máximo que o nome ou cor pode ter
#define NUM_TERRITORIOS 5 // Tem que ser 5, o desafio pediu vetor estático de 5

// --- 2. Estrutura de Dados (struct) ---
// Minha ficha pra guardar as infos de CADA território
typedef struct {
    char nome[MAX_STRING];
    char cor_exercito[MAX_STRING]; // Qual exército domina (tipo Azul, Vermelho...)
    int num_tropas; // Quantas tropas estão lá
} Territorio;

// --- 3. Função Principal ---
int main() {
    // Declarando o mapa! É um vetor que guarda 5 fichas de Territorio
    Territorio mapa[NUM_TERRITORIOS];
    
    // Agora tem que preencher e depois mostrar!

    return 0;
}