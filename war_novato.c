#include <stdio.h> 
#include <string.h> 

// --- 1. Constantes ---
#define MAX_STRING 50 
#define NUM_TERRITORIOS 5 

// --- 2. Estrutura de Dados (struct) ---
// Minha ficha pra guardar as infos de CADA território
typedef struct {
    char nome[MAX_STRING];
    char cor_exercito[MAX_STRING];
    int num_tropas; 
} Territorio;

// --- 3. Função Principal ---
int main() {
    // Declarando o mapa! É um vetor que guarda 5 fichas de Territorio
    Territorio mapa[NUM_TERRITORIOS];
    int i; // Contador pro loop

    printf("--- Cadastro Inicial de %d Territórios ---\n", NUM_TERRITORIOS);

    // Loop pra cadastrar cada um dos 5 territórios
    for (i = 0; i < NUM_TERRITORIOS; i++) {
        printf("\nTerritório %d de %d:\n", i + 1, NUM_TERRITORIOS);

        // Pedindo o NOME
        printf("Nome do Território: ");
        // Uso fgets pra ler strings com espaços de forma segura.
        fgets(mapa[i].nome, MAX_STRING, stdin);
        // strcspn remove o '\n' que o fgets deixa no final da string. É importante!
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';
        
        // Pedindo a COR do Exército
        printf("Cor do Exército Dominante (Ex: Azul, Verde): ");
        // Uso fgets de novo
        fgets(mapa[i].cor_exercito, MAX_STRING, stdin);
        mapa[i].cor_exercito[strcspn(mapa[i].cor_exercito, "\n")] = '\0';

        // Pedindo o NÚMERO DE TROPAS
        printf("Número de Tropas: ");
        // Uso scanf pra ler número (é mais fácil assim)
        scanf("%d", &mapa[i].num_tropas);
        
        // IMPORTANTE: Depois de um scanf, sempre tem que limpar o buffer!
        // Senão, o próximo fgets vai ler o "Enter" que sobrou do scanf.
        while (getchar() != '\n' && getchar() != EOF); 
    }

    // Agora vamos pra exibição! (Próximo Passo)

    return 0;
}