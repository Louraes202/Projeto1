#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estacionamento.h"

// ------------------------ Configuração do Parque ------------------------

void configurar_parque(Parque *parque) {
    printf("Digite o nome do parque: ");
    scanf("%s", parque->nome);

    printf("Digite a morada do parque: ");
    scanf(" %[^\n]", parque->morada);

    printf("Quantos pisos terá o parque? (1-%d): ", MAX_PISOS);
    scanf("%d", &parque->num_pisos);

    int total_lugares = 0;

    for (int i = 0; i < parque->num_pisos; i++) {
        configurar_piso(&parque->pisos[i], i + 1, &total_lugares);
    }

    parque->total_lugares = total_lugares;
    parque->lugares_livres = total_lugares;
    parque->lugares_ocupados = 0;

    gravar_configuracao_parque(parque);
}

void configurar_piso(Piso *piso, int numero, int *total_lugares) {
    piso->numero = numero;

    printf("Configurar piso %d\n", numero);
    printf("Número de filas (1-%d): ", MAX_FILAS);
    int filas;
    scanf("%d", &filas);

    printf("Lugares por fila (1-%d): ", MAX_LUGARES);
    int lugares;
    scanf("%d", &lugares);

    for (int f = 0; f < filas; f++) {
        for (int l = 0; l < lugares; l++) {
            Lugar *lugar = &piso->lugares[f][l];
            lugar->fila = 'A' + f;
            lugar->lugar = l + 1;
            lugar->estado = 'L';
            lugar->num_piso = numero;
            sprintf(lugar->codigo, "%d%c%02d", numero, 'A' + f, l + 1);
        }
    }

    piso->livres = filas * lugares;
    piso->ocupados = 0;
    piso->indisponiveis = 0;
    *total_lugares += filas * lugares;
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

int carregar_configuracao_parque(Parque *parque) {
    FILE *file = fopen("config_parque.txt", "r");
    if (file == NULL) {
        printf("Erro: Não foi possível abrir o ficheiro de configuração do parque.\n");
        return 0; // Falha
    }

    // Ler o nome do parque
    fgets(parque->nome, sizeof(parque->nome), file);
    parque->nome[strcspn(parque->nome, "\n")] = 0; // Remover o '\n'

    // Ler a morada do parque
    fgets(parque->morada, sizeof(parque->morada), file);
    parque->morada[strcspn(parque->morada, "\n")] = 0; // Remover o '\n'

    // Ler o número de pisos
    fscanf(file, "%d\n", &parque->num_pisos);

    // Inicializar os pisos
    for (int i = 0; i < parque->num_pisos; i++) {
        int livres;
        fscanf(file, "Piso %*d: %d lugares livres\n", &livres);
        parque->pisos[i].livres = livres;
        parque->pisos[i].numero = i + 1;
    }

    fclose(file);
    printf("Configuração do parque carregada com sucesso.\n");
    return 1; // Sucesso
}

// ------------------------ Configuração do Tarifário ------------------------

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

int carregar_tarifario(Tarifario *tarifa_parque) {
    FILE *file = fopen("tarifas.txt", "r");
    if (file == NULL) {
        printf("Erro: Não foi possível abrir o ficheiro de tarifário.\n");
        return 0; // Falha
    }

    int num_tarifas = 0;
    while (!feof(file)) {
        Tarifa tarifa;
        char inicio[9], fim[9];

        fscanf(file, "%s %s %f %s %s %c %d\n", 
               tarifa.nome, tarifa.cod_tarifa, &tarifa.valor_hora, 
               inicio, fim, &tarifa.tp_tarifa, &tarifa.dias);

        sscanf(inicio, "%d:%d:%d", &tarifa.inicio.hora, &tarifa.inicio.min, &tarifa.inicio.seg);
        sscanf(fim, "%d:%d:%d", &tarifa.fim.hora, &tarifa.fim.min, &tarifa.fim.seg);

        tarifa_parque->lista_tarifas[num_tarifas++] = tarifa;
    }

    fclose(file);
    printf("Tarifário carregado com sucesso.\n");
    return num_tarifas;
}
