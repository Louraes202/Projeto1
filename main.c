#include <locale.h>
#include <windows.h>
#include "estacionamento.h"

// Função para exibir o menu principal
void exibir_menu_principal() {
    printf("\n-- GestEstacionamento --\n");
    printf("1. Configurar\n");
    printf("2. Registos\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}

// Função para exibir o sub-menu de configuração
void exibir_menu_configuracao() {
    printf("\n> -- Configuração --\n");
    printf("1. Configurar Parque\n");
    printf("2. Ver Dados do Parque\n");
    printf("0. Voltar atrás\n");
    printf("Escolha uma opção: ");
}

// Função para exibir o sub-menu de registos (atualmente vazio)
void exibir_menu_registos() {
    printf("\n> -- Registos --\n");
    printf("0. Voltar atrás\n");
    printf("Escolha uma opção: ");
}

int main() {
    setlocale(LC_ALL, "pt_PT.UTF-8");

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    Parque parque;
    int opcao_principal, opcao_configuracao, opcao_registos;

    do {
        // Exibe o menu principal ao utilizador
        exibir_menu_principal();
        scanf("%d", &opcao_principal); // Lê a opção escolhida pelo utilizador
        getchar(); // Limpa o buffer do teclado

        switch (opcao_principal) {
            case 1: // Sub-menu Configuração
                do {
                    // Exibe o menu de configuração
                    exibir_menu_configuracao();
                    scanf("%d", &opcao_configuracao); // Lê a opção do sub-menu de configuração
                    getchar(); // Limpa o buffer do teclado

                    switch (opcao_configuracao) {
                        case 1: {
                            // Sub-opções para configuração do parque
                            printf("\n1. Configurar Novo Parque\n");
                            printf("2. Carregar Configuração de Ficheiro\n");
                            printf("Escolha uma opção: ");
                            int opcao_subconfig; // Variável para armazenar a escolha do sub-menu de configuração
                            scanf("%d", &opcao_subconfig);
                            getchar(); // Limpa o buffer do teclado

                            if (opcao_subconfig == 1) {
                                // Configura um novo parque
                                configurar_parque(&parque);
                            } else if (opcao_subconfig == 2) {
                                // Carrega a configuração do parque a partir de um ficheiro
                                if (!carregar_configuracao_parque(&parque)) {
                                    printf("Erro ao carregar configuração. Certifique-se de que o ficheiro existe e não está vazio.\n");
                                }
                            } else {
                                // Trata opções inválidas no sub-menu de configuração
                                printf("Opção inválida. Tente novamente.\n");
                            }
                            break;
                        }
                        case 2:
                            // Exibe os dados atuais do parque
                            visualizar_dados_parque(&parque);
                            break;

                        case 0:
                            // Retorna ao menu principal
                            printf("Voltando ao menu principal...\n");
                            break;

                        default:
                            // Trata opções inválidas no menu de configuração
                            printf("Opção inválida. Tente novamente.\n");
                    }
                } while (opcao_configuracao != 0); // Mantém no sub-menu de configuração até o utilizador escolher sair
                break;

            case 2: // Sub-menu Registos (placeholder)
                do {
                    // Exibe o menu de registos
                    exibir_menu_registos();
                    scanf("%d", &opcao_registos); // Lê a opção escolhida no sub-menu de registos
                    getchar(); // Limpa o buffer do teclado

                    if (opcao_registos == 0) {
                        // Retorna ao menu principal
                        printf("Voltando ao menu principal...\n");
                    } else {
                        // Trata opções inválidas no menu de registos
                        printf("Opção inválida. Tente novamente.\n");
                    }
                } while (opcao_registos != 0); // Mantém no sub-menu de registos até o utilizador escolher sair
                break;

            case 0:
                // Encerra o programa
                printf("Encerrando o programa.\n");
                break;

            default:
                // Trata opções inválidas no menu principal
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao_principal != 0); // Mantém no menu principal até o utilizador escolher sair

    return 0;

}
