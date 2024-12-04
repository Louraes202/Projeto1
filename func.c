#include <stdio.h>
#include "estacionamento.h"

void configurar_parque(Parque *parque) {
    printf("Quantos pisos terá o parque? (1-%d): ", MAX_PISOS);
    scanf("%d", &parque->num_pisos);

    for (int i = 0; i < parque->num_pisos; i++) {
        parque->pisos[i].numero = i + 1;
        printf("Configurar piso %d\n", i + 1);

        printf("Número de filas (1-%d): ", MAX_FILAS);
        int filas;
        scanf("%d", &filas);

        printf("Lugares por fila (1-%d): ", MAX_LUGARES);
        int lugares;
        scanf("%d", &lugares);

        // Inicializar todos os lugares
        for (int f = 0; f < filas; f++) {
            for (int l = 0; l < lugares; l++) {
                parque->pisos[i].lugares[f][l].fila = 'A' + f;
                parque->pisos[i].lugares[f][l].lugar = l + 1;
                parque->pisos[i].lugares[f][l].estado = 'L';
            }
        }

        parque->pisos[i].livres = filas * lugares;
        parque->pisos[i].ocupados = 0;
        parque->pisos[i].indisponiveis = 0;
    }

    gravar_configuracao_parque(parque);
}

void configurar_tarifario(Tarifario *tarifario) {
    printf("Digite o valor para tarifa diurna (T1): ");
    scanf("%f", &tarifario->tarifa_diurna);

    printf("Digite o valor para tarifa noturna (T2): ");
    scanf("%f", &tarifario->tarifa_noturna);

    printf("Digite o valor para tarifa diária (T3): ");
    scanf("%f", &tarifario->tarifa_diaria);

    printf("Digite o valor para tarifa multi-dia (T4): ");
    scanf("%f", &tarifario->tarifa_multidia);

    gravar_tarifario(tarifario);
}

void gravar_configuracao_parque(const Parque *parque) {
    FILE *file = fopen("config_parque.txt", "w");
    if (file == NULL) {
        printf("Erro ao gravar configuração do parque.\n");
        return;
    }

    fprintf(file, "%d\n", parque->num_pisos);
    for (int i = 0; i < parque->num_pisos; i++) {
        fprintf(file, "Piso %d: %d lugares livres\n", i + 1, parque->pisos[i].livres);
    }

    fclose(file);
    printf("Configuração do parque gravada com sucesso.\n");
}

void gravar_tarifario(const Tarifario *tarifario) {
    FILE *file = fopen("tarifas.txt", "w");
    if (file == NULL) {
        printf("Erro ao gravar tarifário.\n");
        return;
    }

    fprintf(file, "%.2f %.2f %.2f %.2f\n", tarifario->tarifa_diurna, tarifario->tarifa_noturna,
            tarifario->tarifa_diaria, tarifario->tarifa_multidia);

    fclose(file);
    printf("Tarifário gravado com sucesso.\n");
}
