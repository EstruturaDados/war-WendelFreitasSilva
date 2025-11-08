#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> // Para calloc, free, rand
#include <time.h>   // Para srand

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

// --- PROTÓTIPOS (Declarando as funções antes da main) ---
// Notação importante: const *mapa significa que a função só VAI LER (não modifica)
void exibirMapa(const Territorio *mapa, int num_territorios);
// Notação importante: *atacante e *defensor significa que a função PODE MODIFICAR os territórios
void simularAtaque(Territorio *atacante, Territorio *defensor);
void faseDeAtaque(Territorio *mapa, int num_territorios); 


// --- 3. Função Principal (main) ---
int main() {
    // Agora o mapa é um PONTEIRO! (Alocação Dinâmica)
    Territorio *mapa = NULL; 
    int i; 

    // Novo: Inicializando o gerador de números aleatórios (para as batalhas)
    srand(time(NULL)); 

    // Novo: Alocar a memória usando calloc
    mapa = (Territorio *)calloc(NUM_TERRITORIOS, sizeof(Territorio));
    
    // Checar se a alocação deu certo!
    if (mapa == NULL) {
        printf("ERRO: Falha ao alocar memória. O jogo não pode continuar.\n");
        return 1; 
    }

    printf("--- Nível Aventureiro: Cadastro e Combate ---\n");

    // FASE DE CADASTRO (Mantida do Nível Novato)
    for (i = 0; i < NUM_TERRITORIOS; i++) {
        printf("\nTerritório %d de %d:\n", i + 1, NUM_TERRITORIOS);
        printf("Nome do Território: ");
        fgets(mapa[i].nome, MAX_STRING, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';
        
        printf("Cor do Exército Dominante (Ex: Azul, Verde): ");
        fgets(mapa[i].cor_exercito, MAX_STRING, stdin);
        mapa[i].cor_exercito[strcspn(mapa[i].cor_exercito, "\n")] = '\0';

        printf("Número de Tropas: ");
        scanf("%d", &mapa[i].num_tropas);
        
        // Limpar buffer após o scanf
        while (getchar() != '\n' && getchar() != EOF); 
    }

    // ----------------------------------------------------
    // NOVO: Chamada para a FASE DE ATAQUE INTERATIVA
    // ----------------------------------------------------
    faseDeAtaque(mapa, NUM_TERRITORIOS); 
    
    // ----------------------------------------------------
    // FASE FINAL: Exibição e Limpeza
    // ----------------------------------------------------
    printf("\n\n=== MAPA FINAL DO NÍVEL AVENTUREIRO ===\n");
    exibirMapa(mapa, NUM_TERRITORIOS); 

    // CRUCIAL: Liberar a memória que alocamos
    printf("\nLiberando memória do mapa...\n");
    free(mapa);
    mapa = NULL; 

    printf("Nível Aventureiro concluído. Próximo: MESTRE! 🧠\n");

    return 0;
}


// --- IMPLEMENTAÇÃO DAS FUNÇÕES MODULARES ---

// Função para exibir o mapa em formato de tabela (Só lê os dados)
void exibirMapa(const Territorio *mapa, int num_territorios) {
    int i;
    printf("+----+--------------------+-----------+--------+\n");
    printf("| ID | NOME DO TERRITÓRIO | EXÉRCITO  | TROPAS |\n");
    printf("+----+--------------------+-----------+--------+\n");

    for (i = 0; i < num_territorios; i++) {
        // A notação mapa[i] funciona mesmo com ponteiro!
        printf("| %-2d | %-20s | %-9s | %-6d |\n", 
            i + 1,
            mapa[i].nome,
            mapa[i].cor_exercito,
            mapa[i].num_tropas
        );
    }
    printf("+----+--------------------+-----------+--------+\n");
}


// Função para simular a lógica de um ataque (Modifica os dados)
void simularAtaque(Territorio *atacante, Territorio *defensor) {
    printf("\n⚔️ BATALHA: %s (%s) ATACA %s (%s) ⚔️\n", 
        atacante->nome, atacante->cor_exercito,
        defensor->nome, defensor->cor_exercito
    );

    // Rolar os dados
    int dado_ataque = rand() % 6 + 1; 
    int dado_defesa = rand() % 6 + 1;

    printf("   Dados Rolados: Atacante tirou %d vs Defensor tirou %d\n", 
        dado_ataque, dado_defesa
    );

    // Lógica de Resultado: Empates favorecem o atacante (>=)
    if (dado_ataque >= dado_defesa) {
        printf("   Resultado: 🛡️ Defensor perde 1 tropa!\n");
        defensor->num_tropas--; 

    } else { 
        printf("   Resultado: ⚔️ Atacante perde 1 tropa!\n");
        atacante->num_tropas--; 
    } 

    // Checagem de Conquista
    if (defensor->num_tropas <= 0) {
        printf("\n   🎉 CONQUISTA! %s perdeu todas as tropas!\n", defensor->nome);
        
        // Defensor tem 1 tropa e a cor do atacante
        defensor->num_tropas = 1;
        strcpy(defensor->cor_exercito, atacante->cor_exercito); 
        
        // Atacante move 1 tropa (se tiver mais de 1)
        if (atacante->num_tropas > 1) { 
             atacante->num_tropas--; 
        }

        printf("   %s agora é dominado pelo exército %s e tem 1 tropa.\n", 
            defensor->nome, defensor->cor_exercito
        );
    }
    
    // Garante no mínimo 1 tropa (defesa)
    if (atacante->num_tropas < 1) {
        atacante->num_tropas = 1;
        printf("   [AVISO] %s deve ter no mínimo 1 tropa. Tropas ajustadas para 1.\n", atacante->nome);
    }
}


// Função principal para a fase de ataque interativa
void faseDeAtaque(Territorio *mapa, int num_territorios) {
    int atacante_id, defensor_id;
    int continuar = 1; 

    printf("\n\n=== FASE DE ATAQUE INTERATIVA ⚔️ ===\n");
    printf("Digite 0 para o ID do atacante para SAIR desta fase.\n");
    
    while (continuar) {
        exibirMapa(mapa, num_territorios); 

        // 1. ESCOLHA DO ATACANTE
        printf("\nEscolha o ID do Território ATACANTE (1 a %d ou 0 para sair): ", num_territorios);
        if (scanf("%d", &atacante_id) != 1) { atacante_id = -1; } // Trata falha na leitura
        while (getchar() != '\n' && getchar() != EOF); // Limpa buffer

        if (atacante_id == 0) {
            continuar = 0;
            break; 
        }
        
        // 2. VALIDAÇÃO DO ATACANTE
        if (atacante_id < 1 || atacante_id > num_territorios) {
            printf("⚠️ ID do atacante inválido! Tente novamente.\n");
            continue;
        }

        // Não pode atacar se só tiver 1 tropa
        if (mapa[atacante_id - 1].num_tropas <= 1) {
            printf("⚠️ O atacante precisa de no mínimo 2 tropas para atacar! (Uma fica na defesa).\n");
            continue;
        }

        // 3. ESCOLHA DO DEFENSOR
        printf("Escolha o ID do Território DEFENSOR (1 a %d): ", num_territorios);
        if (scanf("%d", &defensor_id) != 1) { defensor_id = -1; } // Trata falha na leitura
        while (getchar() != '\n' && getchar() != EOF); // Limpa buffer

        // 4. VALIDAÇÕES DO DEFENSOR
        if (defensor_id < 1 || defensor_id > num_territorios) {
            printf("⚠️ ID do defensor inválido! Tente novamente.\n");
            continue;
        }

        // Não pode atacar a si mesmo!
        if (atacante_id == defensor_id) {
            printf("⚠️ Um território não pode atacar a si mesmo! Escolha IDs diferentes.\n");
            continue;
        }

        // 5. EXECUTAR A BATALHA
        // Passamos o ENDEREÇO (&) dos elementos para a função que espera ponteiros
        simularAtaque(&mapa[atacante_id - 1], &mapa[defensor_id - 1]);
        
        printf("\n✅ Rodada concluída. Pressione ENTER para continuar atacando...");
        getchar(); 
    }
    printf("\n--- FASE DE ATAQUE ENCERRADA ---\n");
}