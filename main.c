#include <stdio.h>
#include "estacionamento.h"

int main() {
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
