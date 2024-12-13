#include <stdio.h>
#include "estacionamento.h"
#include <locale.h>  // Para usar setlocale()
#include <windows.h>


int main() {

    // Definir a localidade para UTF-8
    setlocale(LC_ALL, "pt_PT.UTF-8");

    // Configurar UTF-8 no Windows
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif

    Parque parque;
    Tarifario tarifario;

    int opcao;
    do {
        printf("1. Configurar Parque\n");
        printf("2. Configurar Tarifário\n");
        printf("3. Carregar Configuração do Parque\n");
        printf("4. Carregar Tarifário\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                configurar_parque(&parque);
                break;
            case 2:
                configurar_tarifario(&tarifario);
                break;
            case 3:
                carregar_configuracao_parque(&parque);
                break;
            case 4:
                carregar_tarifario(&tarifario);
                break;
            case 0:
                printf("Encerrando o programa.\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);

    return 0;
}
