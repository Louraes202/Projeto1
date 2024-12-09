#include <stdio.h>
#include <string.h>
#include "estacionamento.h"

void configurar_parque(Parque *parque) {
    printf("Digite o nome do parque: ");
    scanf("%s", parque->nome);

    printf("Digite a morada do parque: ");
    scanf(" %[^\n]", parque->morada);

    printf("Quantos pisos terá o parque? (1-%d): ", MAX_PISOS);
    scanf("%d", &parque->num_pisos);

    int total_lugares = 0;

    for (int i = 0; i < parque->num_pisos; i++) {
        parque->pisos[i].numero = i + 1;
        printf("Configurar piso %d\n", i + 1);

        printf("Número de filas (1-%d): ", MAX_FILAS);
        int filas;
        scanf("%d", &filas);

        printf("Lugares por fila (1-%d): ", MAX_LUGARES);
        int lugares;
        scanf("%d", &lugares);

        for (int f = 0; f < filas; f++) {
            for (int l = 0; l < lugares; l++) {
                Lugar *lugar = &parque->pisos[i].lugares[f][l];
                lugar->fila = 'A' + f;
                lugar->lugar = l + 1;
                lugar->estado = 'L';
                lugar->num_piso = i + 1;
                sprintf(lugar->codigo, "%d%c%02d", i + 1, 'A' + f, l + 1);
            }
        }

        parque->pisos[i].livres = filas * lugares;
        parque->pisos[i].ocupados = 0;
        parque->pisos[i].indisponiveis = 0;
        total_lugares += filas * lugares;
    }

    parque->total_lugares = total_lugares;
    parque->lugares_livres = total_lugares;
    parque->lugares_ocupados = 0;

    gravar_configuracao_parque(parque);
}

void configurar_tarifario(Tarifario *tarifa_parque) {
    int num_tarifas;
    printf("Quantas tarifas deseja configurar? ");
    scanf("%d", &num_tarifas);

    tarifa_parque->lista_tarifas = malloc(num_tarifas * sizeof(Tarifa));

    for (int i = 0; i < num_tarifas; i++) {
        Tarifa *tarifa = &tarifa_parque->lista_tarifas[i];
        printf("Configuração da tarifa %d\n", i + 1);

        printf("Nome da tarifa: ");
        scanf("%s", tarifa->nome);

        printf("Código da tarifa: ");
        scanf("%s", tarifa->cod_tarifa);

        printf("Valor por hora (€): ");
        scanf("%f", &tarifa->valor_hora);

        printf("Horário de início (hh:mm:ss): ");
        scanf("%d:%d:%d", &tarifa->inicio.hora, &tarifa->inicio.min, &tarifa->inicio.seg);

        printf("Horário de fim (hh:mm:ss): ");
        scanf("%d:%d:%d", &tarifa->fim.hora, &tarifa->fim.min, &tarifa->fim.seg);

        printf("Tipo de tarifa (H para horário, D para diária): ");
        scanf(" %c", &tarifa->tp_tarifa);

        if (tarifa->tp_tarifa == 'D') {
            printf("Número de dias: ");
            scanf("%d", &tarifa->dias);
        } else {
            tarifa->dias = 0;
        }
    }

    gravar_tarifario(tarifa_parque, num_tarifas);
}

void gravar_configuracao_parque(const Parque *parque) {
    FILE *file = fopen("config_parque.txt", "w");
    if (file == NULL) {
        printf("Erro ao gravar configuração do parque.\n");
        return;
    }

    fprintf(file, "%s\n%s\n%d\n", parque->nome, parque->morada, parque->num_pisos);
    for (int i = 0; i < parque->num_pisos; i++) {
        fprintf(file, "Piso %d: %d lugares livres\n", parque->pisos[i].numero, parque->pisos[i].livres);
    }

    fclose(file);
    printf("Configuração do parque gravada com sucesso.\n");
}

void gravar_tarifario(const Tarifario *tarifa_parque, int num_tarifas) {
    FILE *file = fopen("tarifas.txt", "w");
    if (file == NULL) {
        printf("Erro ao gravar tarifário.\n");
        return;
    }

    for (int i = 0; i < num_tarifas; i++) {
        Tarifa tarifa = tarifa_parque->lista_tarifas[i];
        fprintf(file, "%s %s %.2f %d:%d:%d %d:%d:%d %c %d\n",
                tarifa.nome, tarifa.cod_tarifa, tarifa.valor_hora,
                tarifa.inicio.hora, tarifa.inicio.min, tarifa.inicio.seg,
                tarifa.fim.hora, tarifa.fim.min, tarifa.fim.seg,
                tarifa.tp_tarifa, tarifa.dias);
    }

    fclose(file);
    printf("Tarifário gravado com sucesso.\n");
}
