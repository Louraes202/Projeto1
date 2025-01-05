#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estacionamento.h"

// ------------------------ Configuração do Parque ------------------------

void configurar_parque(Parque *parque) {
    printf("Digite o nome do parque (até 10 caracteres): ");
    scanf("%10s", parque->nome);

    printf("Digite a morada do parque (até 30 caracteres): ");
    scanf("%30s", parque->morada);

    do {
        printf("Quantos pisos terá o parque? (1-%d): ", MAX_PISOS);
        scanf("%d", &parque->num_pisos);
        if (parque->num_pisos < 1 || parque->num_pisos > MAX_PISOS) {
            printf("Erro: O número de pisos deve estar entre 1 e %d.\n", MAX_PISOS);
        }
    } while (parque->num_pisos < 1 || parque->num_pisos > MAX_PISOS);

    int total_lugares = 0;

    for (int i = 0; i < parque->num_pisos; i++) {
        configurar_piso(&parque->pisos[i], i + 1, &total_lugares);
    }

    parque->total_lugares = total_lugares;
    parque->lugares_livres = total_lugares;
    parque->lugares_ocupados = 0;

    gravar_configuracao_parque(parque);

    printf("Configuração do parque concluída com sucesso!\n");
}

void configurar_piso(Piso *piso, int numero, int *total_lugares) {
    piso->numero = numero;
    printf("Configurar piso %d\n", piso->numero);

    int filas, lugares;

    do {
        printf("Número de filas no piso %d (1-%d): ", piso->numero, MAX_FILAS);
        scanf("%d", &filas);
        if (filas < 1 || filas > MAX_FILAS) {
            printf("Erro: O número de filas deve estar entre 1 e %d.\n", MAX_FILAS);
        }
    } while (filas < 1 || filas > MAX_FILAS);

    do {
        printf("Lugares por fila no piso %d (1-%d): ", piso->numero, MAX_LUGARES);
        scanf("%d", &lugares);
        if (lugares < 1 || lugares > MAX_LUGARES) {
            printf("Erro: O número de lugares por fila deve estar entre 1 e %d.\n", MAX_LUGARES);
        }
    } while (lugares < 1 || lugares > MAX_LUGARES);

    for (int f = 0; f < filas; f++) {
        for (int l = 0; l < lugares; l++) {
            Lugar *lugar = &piso->lugares[f][l];
            lugar->fila = 'A' + f;
            lugar->lugar = l + 1;
            lugar->estado = 'L';
            lugar->num_piso = piso->numero;
            sprintf(lugar->codigo, "%d%c%02d", piso->numero, 'A' + f, l + 1);
        }
    }

    piso->livres = filas * lugares;
    piso->ocupados = 0;
    piso->indisponiveis = 0;

    *total_lugares += filas * lugares;

    printf("Piso %d configurado com sucesso!\n", piso->numero);
}

void gravar_configuracao_parque(const Parque *parque) {
    FILE *file = fopen("config_parque.txt", "w");
    if (file == NULL) {
        printf("Erro ao gravar configuração do parque.\n");
        return;
    }

    fprintf(file, "%s\n%s\n%d\n", parque->nome, parque->morada, parque->num_pisos);
    for (int i = 0; i < parque->num_pisos; i++) {
        Piso piso = parque->pisos[i];
        fprintf(file, "Piso %d: %d lugares livres\n", piso.numero, piso.livres);
    }

    fclose(file);
    printf("Configuração do parque gravada com sucesso.\n");
}

int carregar_configuracao_parque(Parque *parque) {
    FILE *file = fopen("config_parque.txt", "r");
    if (file == NULL) {
        printf("Erro: Não foi possível abrir o ficheiro de configuração do parque.\n");
        return 0;
    }

    fgets(parque->nome, sizeof(parque->nome), file);
    parque->nome[strcspn(parque->nome, "\n")] = 0;

    fgets(parque->morada, sizeof(parque->morada), file);
    parque->morada[strcspn(parque->morada, "\n")] = 0;

    fscanf(file, "%d\n", &parque->num_pisos);
    for (int i = 0; i < parque->num_pisos; i++) {
        Piso *piso = &parque->pisos[i];
        fscanf(file, "Piso %*d: %d lugares livres\n", &piso->livres);
        piso->numero = i + 1;
    }

    fclose(file);
    printf("Configuração do parque carregada com sucesso.\n");
    return 1;
}

// ------------------------ Configuração do Tarifário ------------------------

void configurar_tarifario(Tarifario *tarifa_parque) {
    int num_tarifas;
    do {
        printf("Quantas tarifas deseja configurar? (1-%d): ", MAX_TARIFAS);
        scanf("%d", &num_tarifas);
        if (num_tarifas < 1 || num_tarifas > MAX_TARIFAS) {
            printf("Erro: O número de tarifas deve estar entre 1 e %d.\n", MAX_TARIFAS);
        }
    } while (num_tarifas < 1 || num_tarifas > MAX_TARIFAS);

    tarifa_parque->lista_tarifas = malloc(num_tarifas * sizeof(Tarifa));
    tarifa_parque->num_tarifas = num_tarifas;

    for (int i = 0; i < num_tarifas; i++) {
        Tarifa *tarifa = &tarifa_parque->lista_tarifas[i];

        printf("Configuração da tarifa %d\n", i + 1);

        printf("Nome da tarifa (até 10 caracteres): ");
        scanf("%10s", tarifa->nome);

        printf("Código da tarifa (3 caracteres): ");
        do {
            scanf("%3s", tarifa->cod_tarifa);
            if (strlen(tarifa->cod_tarifa) != 3) {
                printf("Erro: O código deve ter exatamente 3 caracteres.\n");
            }
        } while (strlen(tarifa->cod_tarifa) != 3);

        do {
            printf("Valor por hora (€): ");
            scanf("%f", &tarifa->valor_hora);
            if (tarifa->valor_hora <= 0) {
                printf("Erro: O valor por hora deve ser maior que 0.\n");
            }
        } while (tarifa->valor_hora <= 0);

        do {
            printf("Horário de início (hh:mm:ss): ");
            scanf("%d:%d:%d", &tarifa->inicio.hora, &tarifa->inicio.min, &tarifa->inicio.seg);
            if (tarifa->inicio.hora < 0 || tarifa->inicio.hora > 23 ||
                tarifa->inicio.min < 0 || tarifa->inicio.min > 59 ||
                tarifa->inicio.seg < 0 || tarifa->inicio.seg > 59) {
                printf("Erro: Horário inválido. Tente novamente.\n");
            }
        } while (tarifa->inicio.hora < 0 || tarifa->inicio.hora > 23 ||
                 tarifa->inicio.min < 0 || tarifa->inicio.min > 59 ||
                 tarifa->inicio.seg < 0 || tarifa->inicio.seg > 59);

        do {
            printf("Horário de fim (hh:mm:ss): ");
            scanf("%d:%d:%d", &tarifa->fim.hora, &tarifa->fim.min, &tarifa->fim.seg);
            if (tarifa->fim.hora < 0 || tarifa->fim.hora > 23 ||
                tarifa->fim.min < 0 || tarifa->fim.min > 59 ||
                tarifa->fim.seg < 0 || tarifa->fim.seg > 59) {
                printf("Erro: Horário inválido. Tente novamente.\n");
            }
        } while (tarifa->fim.hora < 0 || tarifa->fim.hora > 23 ||
                 tarifa->fim.min < 0 || tarifa->fim.min > 59 ||
                 tarifa->fim.seg < 0 || tarifa->fim.seg > 59);

        do {
            printf("Tipo de tarifa (H para horário, D para diária): ");
            scanf(" %c", &tarifa->tp_tarifa);
            if (tarifa->tp_tarifa != 'H' && tarifa->tp_tarifa != 'D') {
                printf("Erro: O tipo de tarifa deve ser 'H' ou 'D'.\n");
            }
        } while (tarifa->tp_tarifa != 'H' && tarifa->tp_tarifa != 'D');

        if (tarifa->tp_tarifa == 'D') {
            do {
                printf("Número de dias (>= 0): ");
                scanf("%d", &tarifa->dias);
                if (tarifa->dias < 0) {
                    printf("Erro: O número de dias deve ser maior ou igual a 0.\n");
                }
            } while (tarifa->dias < 0);
        } else {
            tarifa->dias = 0;
        }
    }

    gravar_tarifario(tarifa_parque);

    printf("Configuração do tarifário concluída com sucesso!\n");
}

void gravar_tarifario(const Tarifario *tarifa_parque) {
    FILE *file = fopen("tarifas.txt", "w");
    if (file == NULL) {
        printf("Erro ao gravar tarifário.\n");
        return;
    }

    for (int i = 0; i < tarifa_parque->num_tarifas; i++) {
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
        return 0;
    }

    int num_tarifas = 0;
    tarifa_parque->lista_tarifas = malloc(MAX_TARIFAS * sizeof(Tarifa));

    while (!feof(file) && num_tarifas < MAX_TARIFAS) {
        Tarifa tarifa;
        char inicio[9], fim[9];

        fscanf(file, "%s %s %f %s %s %c %d\n",
               tarifa.nome, tarifa.cod_tarifa, &tarifa.valor_hora,
               inicio, fim, &tarifa.tp_tarifa, &tarifa.dias);

        sscanf(inicio, "%d:%d:%d", &tarifa.inicio.hora, &tarifa.inicio.min, &tarifa.inicio.seg);
        sscanf(fim, "%d:%d:%d", &tarifa.fim.hora, &tarifa.fim.min, &tarifa.fim.seg);

        tarifa_parque->lista_tarifas[num_tarifas++] = tarifa;
    }

    tarifa_parque->num_tarifas = num_tarifas;

    fclose(file);
    printf("Tarifário carregado com sucesso.\n");
    return num_tarifas;
}

//função para ler o ficheiro estacionamento.txt
void ler_estacionamentos(const char *nomeFicheiro) {
    FILE *ficheiro = fopen(nomeFicheiro, "r");
    if (ficheiro == NULL) {
        perror("Erro ao abrir o ficheiro");
        return;
    }

    Estacionamento est;
    while (fscanf(ficheiro, "%d %s %d/%d/%d %d/%d/%d %d:%d:%d %d:%d:%d %s %f %s",
                  &est.numE,
                  est.matricula,
                  &est.data_entrada.dia, &est.data_entrada.mes, &est.data_entrada.ano,
                  &est.data_saida.dia, &est.data_saida.mes, &est.data_saida.ano,
                  &est.entrada.hora, &est.entrada.min, &est.entrada.min,
                  &est.saida.hora, &est.saida.min, &est.saida.seg,
                  est.lugar,
                  &est.valor_pago,
                  est.observacoes) == 17) {
        // Imprime os dados lidos
        printf("Número: %d\n", est.numE);
        printf("Matrícula: %s\n", est.matricula);
        printf("Data Entrada: %02d/%02d/%04d\n", est.data_entrada.dia, est.data_entrada.mes, est.data_entrada.ano);
        printf("Data Saída: %02d/%02d/%04d\n", est.data_saida.dia, est.data_saida.mes, est.data_saida.ano);
        printf("Hora Entrada: %02d:%02d:%02d\n", est.entrada.hora, est.entrada.min, est.entrada.seg);
        printf("Hora Saída: %02d:%02d:%02d\n", est.saida.hora, est.saida.min, est.saida.seg);
        printf("Lugar: %s\n", est.lugar);
        printf("Valor Pago: %.2f\n", est.valor_pago);
        printf("Observações: %s\n", est.observacoes);
        printf("--------------------------\n");
    }

    fclose(ficheiro);
}

//Função para ler o ficheiro tarifario.txt
Tarifario ler_Tarifario(const char *filename) {
    Tarifario tarifario;
    tarifario.lista_tarifas = NULL;
    tarifario.num_tarifas = 0;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] != '\n') { // Ignorar linhas vazias
            tarifario.num_tarifas++;
        }
    }

    tarifario.lista_tarifas = malloc(tarifario.num_tarifas * sizeof(Tarifa));
    if (tarifario.lista_tarifas == NULL) {
        perror("Erro ao alocar memória para tarifas");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    rewind(file);
    int i = 0;
    while (fgets(line, sizeof(line), file)) {
        if (line[0] != '\n') {
            Tarifa *tarifa = &tarifario.lista_tarifas[i];

            sscanf(line, "%c\t%3s\t%2d:%2d\t%2d:%2d\t%f",
                   &tarifa->tp_tarifa,
                   tarifa->cod_tarifa,
                   &tarifa->inicio.hora, &tarifa->inicio.min,
                   &tarifa->fim.hora, &tarifa->fim.min,
                   &tarifa->valor_hora);

            tarifa->inicio.seg = 0;
            tarifa->fim.seg = 0;
            tarifa->dias = 0;
            snprintf(tarifa->nome, sizeof(tarifa->nome), "Tarifa%s", tarifa->cod_tarifa);

            i++;
        }
    }

    fclose(file);
    return tarifario;
}

//FUNÇAO SO PRA TESTE DA FUNÇAO DE LER TARIFARIO
void imprimir_tarifario(const Tarifario *tarifario) {
    for (int i = 0; i < tarifario->num_tarifas; i++) {
        Tarifa tarifa = tarifario->lista_tarifas[i];
        printf("Tipo: %c\n", tarifa.tp_tarifa);
        printf("Código: %s\n", tarifa.cod_tarifa);
        printf("Hora início: %02d:%02d:%02d\n", tarifa.inicio.hora, tarifa.inicio.min, tarifa.inicio.seg);
        printf("Hora fim: %02d:%02d:%02d\n", tarifa.fim.hora, tarifa.fim.min, tarifa.fim.seg);
        printf("Valor hora: %.2f\n", tarifa.valor_hora);
        printf("Nome: %s\n", tarifa.nome);
        printf("Dias: %d\n", tarifa.dias);
        printf("---------------------\n");
    }
}

void registar_entrada(Parque *parque, const char *matricula, int piso, char fila, int lugar, Horario entrada) {
    printf("To be implemented");
}

void registar_saida(Parque *parque, const char *matricula, Horario saida, const Tarifario *tarifario) {
    printf("To be implemented");
}
