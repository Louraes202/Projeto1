#include <stdio.h>
#include <stdlib.h>
#include "estacionamento.h"

int main() {
    Parque parque;

    int opcao;

    while (1) {
        printf("----- Menu Principal -----\n");
        printf("1. Configurar Parque\n");
        printf("2. Configurar Tarifário\n");
        printf("3. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                configurar_parque(&parque);
                break;
            case 2:
                configurar_tarifario(&parque.tarifa_parque);
                break;
            case 3:
                printf("A sair...\n");
                exit(0);
            default:
                printf("Opção inválida!\n");
        }
    }

    return 0;
}
