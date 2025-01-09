#include <locale.h>
#include <windows.h>
#include "estacionamento.h"

// Função para exibir o menu principal
void exibir_menu_principal(Parque *parque) {
    exibir_lugares_por_piso(parque);
    printf("\n-- GestEstacionamento --\n");
    printf("1. Configurar\n");
    printf("2. Registos\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}

// Função para exibir o sub-menu de configuração
void exibir_menu_configuracao(Parque *parque) {
    exibir_lugares_por_piso(parque);
    printf("\n> -- Configuração --\n");
    printf("1. Configurar Parque\n");
    printf("2. Ver Dados do Parque\n");
    printf("3. Limpar Memória\n");
    printf("0. Voltar atrás\n");
    printf("Escolha uma opção: ");
}

// Função para exibir o sub-menu de registos (atualmente vazio)
void exibir_menu_registos(Parque *parque) {
    exibir_lugares_por_piso(parque);
    printf("\n> -- Registos --\n");
    printf("1. Registar Entrada de Veículo\n");
    printf("2. Carregar Registos de Ficheiro .txt\n");
    printf("3. Estatísticas de Ocupação\n");
    printf("0. Voltar atrás\n");
    printf("Escolha uma opção: ");
}


int main() {
    setlocale(LC_ALL, "pt_PT.UTF-8");

    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    int ultimo_id = 0;

    int opcao_principal, opcao_configuracao, opcao_registos;

    Parque parque = {0}; // Detalhes do parque
    Estacionamento estacionamentos[MAX_ESTACIONAMENTOS]; // Array de registos de estacionamento
    int total_estacionamentos = 0; // Total de registos

    // Inicialização no início do main
    if (!carregar_configuracao_parque(&parque)) {
        printf("Sistema: Não foi possível carregar a configuração do parque ao inicializar o programa. Inicialize o parque antes de continuar.\n");
    }


    // Carregar o estado do ficheiro binário
    if (!carregar_estado_binario(&parque, estacionamentos, &total_estacionamentos, &ultimo_id)) {
        printf("Sistema: Os dados da ocupação do parque não foram carregados.\n");
    }

    do {
        // Exibe o menu principal ao utilizador
        exibir_menu_principal(&parque);
        scanf("%d", &opcao_principal); // Lê a opção escolhida pelo utilizador
        getchar(); // Limpa o buffer do teclado

        switch (opcao_principal) {
            case 1: // Sub-menu Configuração
                do {
                    // Exibe o menu de configuração
                    exibir_menu_configuracao(&parque);
                    scanf("%d", &opcao_configuracao); // Lê a opção do sub-menu de configuração
                    getchar(); // Limpa o buffer do teclado

            switch (opcao_configuracao) {
                case 1: {
                    printf("\n1. Configurar Novo Parque\n");
                    printf("2. Carregar Configuração de Ficheiro\n");
                    printf("Escolha uma opção: ");
                    int opcao_subconfig;
                    scanf("%d", &opcao_subconfig);
                    getchar();

                    if (opcao_subconfig == 1) {
                        configurar_parque(&parque);
                    } else if (opcao_subconfig == 2) {
                        if (!carregar_configuracao_parque(&parque)) {
                            printf("Erro ao carregar configuração. Certifique-se de que o ficheiro existe e não está vazio.\n");
                        }
                    } else {
                        printf("Opção inválida. Tente novamente.\n");
                    }
                    break;
                }
                case 2:
                    visualizar_dados_parque(&parque);
                    break;
                case 3:
                    limpar_memoria(&parque, estacionamentos, &total_estacionamentos);
                    break;
                case 0:
                    printf("Voltando ao menu principal...\n");
                    break;

                default:
                    printf("Opção inválida. Tente novamente.\n");
            }
                } while (opcao_configuracao != 0); // Mantém no sub-menu de configuração até o utilizador escolher sair
                break;

            case 2: // Sub-menu Registos
                do {
                    exibir_menu_registos(&parque);
                    scanf("%d", &opcao_registos);
                    getchar();

                    switch (opcao_registos) {
                        case 1: { // Registar entrada de veículo
                            registar_entrada_veiculo(&parque, estacionamentos, &total_estacionamentos, &ultimo_id);
                            break;
                        }
                        case 2: { // Carregar registos de ficheiro
                            char nome_ficheiro[50];
                            printf("Digite o nome do ficheiro .txt a carregar (incluindo extensão): ");
                            scanf("%49s", nome_ficheiro);
                            if (carregar_registos_txt(nome_ficheiro, estacionamentos, &total_estacionamentos, &parque)) {
                                printf("Registos carregados com sucesso do ficheiro %s.\n", nome_ficheiro);
                            } else {
                                printf("Erro ao carregar registos do ficheiro %s.\n", nome_ficheiro);
                            }
                            break;
                        }
                        case 3: // Estatísticas de ocupação
                            exibir_estatisticas_ocupacao(&parque);
                            break;
                        case 0:
                            printf("Voltando ao menu principal...\n");
                            break;

                        default:
                            printf("Opção inválida. Tente novamente.\n");
                    }
                } while (opcao_registos != 0);
                break;


            case 0:
                // Verifica se o parque está configurado antes de salvar
                if (parque.num_pisos > 0 && parque.filas > 0 && parque.lugares_por_fila > 0) {
                    guardar_estado_binario(&parque, estacionamentos, total_estacionamentos);
                }

                printf("Encerrando o programa.\n");
                break;

            default:
                // Trata opções inválidas no menu principal
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao_principal != 0); // Mantém no menu principal até o utilizador escolher sair

    return 0;

}
