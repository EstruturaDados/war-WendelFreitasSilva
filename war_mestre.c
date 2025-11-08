#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <time.h>   
#include <stdbool.h> // Para usar o tipo 'bool' (true/false)

// --- 1. Constantes e Definições ---
#define MAX_STRING 50 
#define NUM_TERRITORIOS 5 
#define NUM_MISSOES 3

// --- 2. Estrutura de Dados (struct) ---
typedef struct {
    char nome[MAX_STRING];
    char cor_exercito[MAX_STRING];
    int num_tropas; 
} Territorio;

// --- PROTÓTIPOS ---
// Funções de Utilitário
void exibirMapa(const Territorio *mapa, int num_territorios);
void limparBuffer();
int lerInt();

// Funções do Jogo
void inicializarMapa(Territorio *mapa, int num_territorios);
void simularAtaque(Territorio *atacante, Territorio *defensor);
void faseDeAtaque(Territorio *mapa, int num_territorios); 

// Funções do Nível Mestre (Missões)
void exibirMissoes(const char missoes[][MAX_STRING]);
bool checarVitoria(const Territorio *mapa, int num_territorios, int missao_id);

// --- 3. Função Principal (main) ---
int main() {
    Territorio *mapa = NULL; 
    int i; 
    int escolha;
    bool jogoRodando = true;
    
    // Vetor de Missões (Nível Mestre)
    char missoes[NUM_MISSOES][MAX_STRING] = {
        "1. Conquistar pelo menos 3 territorios.",
        "2. Ter 10 ou mais tropas no total.",
        "3. Conquistar todos os 5 territorios (Vitoria por Dominacao)."
    };
    int missao_escolhida = 0; // O jogador sempre terá a Missão 0 (Conquistar 3)

    srand(time(NULL)); 

    // Alocar a memória usando calloc
    mapa = (Territorio *)calloc(NUM_TERRITORIOS, sizeof(Territorio));
    
    if (mapa == NULL) {
        printf("ERRO: Falha ao alocar memória. O jogo não pode continuar.\n");
        return 1; 
    }

    printf("--- WAR Simples: NÍVEL MESTRE (Inicializacao) ---\n");
    
    // Inicializar e Cadastrar
    inicializarMapa(mapa, NUM_TERRITORIOS);
    
    // ----------------------------------------------------
    // LAÇO PRINCIPAL DO JOGO (Menu Centralizado)
    // ----------------------------------------------------
    while(jogoRodando) {
        printf("\n============================================\n");
        printf("  MENU PRINCIPAL (MISSÃO ID: %d)\n", missao_escolhida + 1);
        printf("============================================\n");
        exibirMapa(mapa, NUM_TERRITORIOS);
        exibirMissoes(missoes); // Exibe as missões
        
        printf("\nEscolha sua ação:\n");
        printf("1. Atacar Território (Fase de Ataque)\n");
        printf("2. Checar Condição de Vitória\n");
        printf("0. Sair do Jogo\n");
        printf("Opção: ");

        escolha = lerInt(); // Lê a escolha do usuário

        switch (escolha) {
            case 1:
                faseDeAtaque(mapa, NUM_TERRITORIOS);
                break;
            case 2:
                if (checarVitoria(mapa, NUM_TERRITORIOS, missao_escolhida)) {
                    printf("\n\n🎉🎉 PARABÉNS! VOCÊ COMPLETOU SUA MISSÃO! 🎉🎉\n");
                    jogoRodando = false; // Fim do jogo
                } else {
                    printf("\n\n❌ Missão não concluída. Continue lutando!\n");
                }
                break;
            case 0:
                jogoRodando = false; // Sair
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
        
        // Se o jogo continuar, adicionamos um reforço simples a cada rodada
        if(jogoRodando) {
            mapa[0].num_tropas += 1; // Reforço para o primeiro território
            printf("\n-- Reforço: Seu primeiro território (%s) ganhou 1 tropa! --\n", mapa[0].nome);
        }
    }


    // CRUCIAL: Liberar a memória que alocamos
    printf("\n\n--- FIM DE JOGO ---\n");
    printf("Liberando memória do mapa...\n");
    free(mapa);
    mapa = NULL; 

    printf("Jogo encerrado. Obrigado por jogar! 🎮\n");

    return 0;
}


// --- IMPLEMENTAÇÃO DAS FUNÇÕES UTILITÁRIAS ---

// Função para exibir o mapa em formato de tabela
void exibirMapa(const Territorio *mapa, int num_territorios) {
    int i;
    printf("+----+--------------------+-----------+--------+\n");
    printf("| ID | NOME DO TERRITÓRIO | EXÉRCITO  | TROPAS |\n");
    printf("+----+--------------------+-----------+--------+\n");

    for (i = 0; i < num_territorios; i++) {
        printf("| %-2d | %-20s | %-9s | %-6d |\n", 
            i + 1,
            mapa[i].nome,
            mapa[i].cor_exercito,
            mapa[i].num_tropas
        );
    }
    printf("+----+--------------------+-----------+--------+\n");
}

// Limpa o buffer de entrada (Necessário após scanf)
void limparBuffer() {
    while (getchar() != '\n' && getchar() != EOF);
}

// Função segura para ler inteiros
int lerInt() {
    int num;
    if (scanf("%d", &num) != 1) {
        num = -1; // Valor inválido
    }
    limparBuffer();
    return num;
}


// --- IMPLEMENTAÇÃO DAS FUNÇÕES DO JOGO ---

// Função para o cadastro inicial (Encapsulamento do Passo 2)
void inicializarMapa(Territorio *mapa, int num_territorios) {
    int i;
    printf("\n--- Cadastro Inicial de %d Territórios ---\n", num_territorios);

    for (i = 0; i < num_territorios; i++) {
        printf("\nTerritório %d de %d:\n", i + 1, num_territorios);
        
        printf("Nome do Território: ");
        fgets(mapa[i].nome, MAX_STRING, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';
        
        printf("Cor do Exército Dominante (Ex: Azul, Verde): ");
        fgets(mapa[i].cor_exercito, MAX_STRING, stdin);
        mapa[i].cor_exercito[strcspn(mapa[i].cor_exercito, "\n")] = '\0';

        printf("Número de Tropas: ");
        mapa[i].num_tropas = lerInt();
        if (mapa[i].num_tropas < 1) mapa[i].num_tropas = 1; // Garante mínimo de 1
    }
}

// Função para simular a lógica de um ataque (Do Nível Aventureiro)
void simularAtaque(Territorio *atacante, Territorio *defensor) {
    printf("\n⚔️ BATALHA: %s (%s) ATACA %s (%s) ⚔️\n", 
        atacante->nome, atacante->cor_exercito,
        defensor->nome, defensor->cor_exercito
    );

    int dado_ataque = rand() % 6 + 1; 
    int dado_defesa = rand() % 6 + 1;

    printf("   Dados Rolados: Atacante tirou %d vs Defensor tirou %d\n", 
        dado_ataque, dado_defesa
    );

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
        
        defensor->num_tropas = 1;
        strcpy(defensor->cor_exercito, atacante->cor_exercito); 
        
        // Atacante move 1 tropa
        if (atacante->num_tropas > 1) { 
             atacante->num_tropas--; 
        }

        printf("   %s agora é dominado pelo exército %s e tem 1 tropa.\n", 
            defensor->nome, defensor->cor_exercito
        );
    }
    
    // Garante no mínimo 1 tropa de defesa
    if (atacante->num_tropas < 1) {
        atacante->num_tropas = 1;
        printf("   [AVISO] %s deve ter no mínimo 1 tropa. Tropas ajustadas para 1.\n", atacante->nome);
    }
}

// Função principal para a fase de ataque (Do Nível Aventureiro)
void faseDeAtaque(Territorio *mapa, int num_territorios) {
    int atacante_id, defensor_id;
    int continuar = 1; 

    printf("\n--- INÍCIO DA FASE DE ATAQUE ---\n");
    printf("Digite 0 para o ID do atacante para RETORNAR ao menu principal.\n");
    
    while (continuar) {
        exibirMapa(mapa, num_territorios); 

        printf("\nEscolha o ID do Território ATACANTE (1 a %d ou 0 para voltar): ", num_territorios);
        atacante_id = lerInt();

        if (atacante_id == 0) {
            continuar = 0;
            break; 
        }
        
        // Validações
        if (atacante_id < 1 || atacante_id > num_territorios) {
            printf("⚠️ ID do atacante inválido! Tente novamente.\n");
            continue;
        }

        if (mapa[atacante_id - 1].num_tropas <= 1) {
            printf("⚠️ O atacante precisa de no mínimo 2 tropas para atacar!\n");
            continue;
        }
        
        printf("Escolha o ID do Território DEFENSOR (1 a %d): ", num_territorios);
        defensor_id = lerInt();

        if (defensor_id < 1 || defensor_id > num_territorios || atacante_id == defensor_id) {
            printf("⚠️ ID do defensor inválido ou atacando a si mesmo! Tente novamente.\n");
            continue;
        }
        
        // Executar a Batalha
        simularAtaque(&mapa[atacante_id - 1], &mapa[defensor_id - 1]);
        
        printf("\n✅ Rodada concluída. Pressione ENTER para continuar atacando...");
        limparBuffer(); // Pausa para o jogador ler o resultado
    }
    printf("\n--- FASE DE ATAQUE ENCERRADA ---\n");
}


// --- IMPLEMENTAÇÃO DAS FUNÇÕES DO NÍVEL MESTRE (Missões) ---

// Exibe a lista de missões disponíveis
void exibirMissoes(const char missoes[][MAX_STRING]) {
    printf("\n-- MISSOES DO JOGO --\n");
    for(int i = 0; i < NUM_MISSOES; i++) {
        printf("%s\n", missoes[i]);
    }
}

// Verifica se as condições de vitória foram atingidas
bool checarVitoria(const Territorio *mapa, int num_territorios, int missao_id) {
    int i;
    int territorios_conquistados = 0;
    int total_tropas = 0;
    
    // A cor dominante é sempre a do Território 0 (o primeiro do jogador)
    const char *cor_dominante = mapa[0].cor_exercito;
    
    // 1. Contar territórios e tropas
    for (i = 0; i < num_territorios; i++) {
        total_tropas += mapa[i].num_tropas;
        if (strcmp(mapa[i].cor_exercito, cor_dominante) == 0) {
            territorios_conquistados++;
        }
    }

    printf("\n-- STATUS ATUAL --\n");
    printf("Territórios sob controle: %d\n", territorios_conquistados);
    printf("Total de Tropas: %d\n", total_tropas);
    printf("------------------\n");

    // Lógica de Vitória (switch/case)
    switch (missao_id) {
        case 0: // Missão 1: Conquistar pelo menos 3 territorios.
            return (territorios_conquistados >= 3);
        case 1: // Missão 2: Ter 10 ou mais tropas no total.
            return (total_tropas >= 10);
        case 2: // Missão 3: Conquistar todos os 5 territorios (Dominacao).
            return (territorios_conquistados == num_territorios);
        default:
            return false;
    }
}