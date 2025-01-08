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
        piso->numero = i + 1; // Inicializar o número do piso
        piso->livres = piso->ocupados = piso->indisponiveis = 0; // Inicializar contadores
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

void Ler_estacionamento(Estacionamento *estacionamento) {
    printf("Digite o número de entrada do estacionamento:\n");
    scanf("%d", &estacionamento->numE);

    printf("Digite a matrícula do veículo (máx. %d caracteres):\n", MAX_MATRICULA - 1);
    scanf("%9s", estacionamento->matricula);

    printf("Digite a data de entrada do veículo (dd mm aaaa):\n");
    scanf("%d %d %d", 
          &estacionamento->data_entrada.dia, 
          &estacionamento->data_entrada.mes, 
          &estacionamento->data_entrada.ano);

    printf("Digite a hora de entrada (hh mm ss):\n");
    scanf("%d %d %d", 
          &estacionamento->entrada.hora, 
          &estacionamento->entrada.min, 
          &estacionamento->entrada.seg);

    printf("Digite o lugar do carro (Piso Fila Lugar):\n");
    scanf("%d %c %d", 
          &estacionamento->lugar.num_piso, 
          &estacionamento->lugar.fila, 
          &estacionamento->lugar.lugar);

    printf("Digite a hora de saída (hh mm ss):\n");
    scanf("%d %d %d", 
          &estacionamento->saida.hora, 
          &estacionamento->saida.min, 
          &estacionamento->saida.seg);

    printf("Digite a data de saída (dd mm aaaa):\n");
    scanf("%d %d %d", 
          &estacionamento->data_saida.dia, 
          &estacionamento->data_saida.mes, 
          &estacionamento->data_saida.ano);

    printf("Digite o valor pago:\n");
    scanf("%f", &estacionamento->valor_pago);

    printf("Digite as observações (máx. %d caracteres):\n", 50 - 1);
    scanf("%49s", estacionamento->observacoes);
}

int carregar_estacionamentos(const char *filename, Estacionamento *estacionamentos, int *total_estacionamentos, Parque *parque) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Erro: Não foi possível abrir o ficheiro %s.\n", filename);
        return 0;
    }

    int count = 0;
    while (!feof(file)) {
        Estacionamento est;
        char lugar_code[5];

        // Leitura dos dados do ficheiro
        if (fscanf(file, "%d %9s %d/%d/%d %d:%d:%d %4s %d/%d/%d %d:%d:%d %f %[^\n]",
                   &est.numE, est.matricula,
                   &est.data_entrada.dia, &est.data_entrada.mes, &est.data_entrada.ano,
                   &est.entrada.hora, &est.entrada.min, &est.entrada.seg,
                   lugar_code,
                   &est.data_saida.dia, &est.data_saida.mes, &est.data_saida.ano,
                   &est.saida.hora, &est.saida.min, &est.saida.seg,
                   &est.valor_pago, est.observacoes) != 17) {
            break;
        }

        // Decodificar o código do lugar
        est.lugar.num_piso = lugar_code[0] - '0';
        est.lugar.fila = lugar_code[1];
        est.lugar.lugar = (lugar_code[2] - '0') * 10 + (lugar_code[3] - '0');

        // Atualizar o estado no parque se o veículo ainda estiver estacionado
        if (est.data_saida.dia == 0 && est.data_saida.mes == 0 && est.data_saida.ano == 0) {
            Piso *piso = &parque->pisos[est.lugar.num_piso - 1];
            Lugar *lugar = &piso->lugares[est.lugar.fila - 'A'][est.lugar.lugar - 1];

            if (lugar->estado != 'O') {
                lugar->estado = 'O'; // Ocupado
                piso->ocupados++;
                piso->livres--;
                parque->lugares_ocupados++;
                parque->lugares_livres--;
            }
        }

        estacionamentos[count++] = est;

        if (count >= MAX_ESTACIONAMENTOS) {
            printf("Aviso: Limite máximo de estacionamentos atingido.\n");
            break;
        }
    }

    fclose(file);
    *total_estacionamentos = count;
    printf("%d registos de estacionamento carregados com sucesso.\n", count);
    return 1;
}


float calcular_valor_pago(const Estacionamento *estacionamento, const Tarifario *tarifario) {
    for (int i = 0; i < tarifario->num_tarifas; i++) {
        const Tarifa *tarifa = &tarifario->lista_tarifas[i];
        // Verifica se a entrada e a saída estão dentro do intervalo de tempo da tarifa
        if (estacionamento->entrada.hora >= tarifa->inicio.hora && estacionamento->entrada.hora <= tarifa->fim.hora) {
            // Calculo simplificado (horas completas entre entrada e saída)
            int duracao_horas = estacionamento->saida.hora - estacionamento->entrada.hora;
            if (duracao_horas < 0) duracao_horas += 24; // Corrige caso passe da meia-noite
            return duracao_horas * tarifa->valor_hora;
        }
    }
    return 0.0f; // Caso nenhuma tarifa seja aplicável
}

void gravar_estacionamentos(const char *filename, Estacionamento *estacionamentos, int total_estacionamentos) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Erro: Não foi possível abrir o ficheiro %s para escrita.\n", filename);
        return;
    }

    for (int i = 0; i < total_estacionamentos; i++) {
        fprintf(file, "%d %s %02d/%02d/%04d %02d:%02d:%02d %d %c %d %02d/%02d/%04d %02d:%02d:%02d %.2f %s\n",
                estacionamentos[i].numE,
                estacionamentos[i].matricula,
                estacionamentos[i].data_entrada.dia, estacionamentos[i].data_entrada.mes, estacionamentos[i].data_entrada.ano,
                estacionamentos[i].entrada.hora, estacionamentos[i].entrada.min, estacionamentos[i].entrada.seg,
                estacionamentos[i].lugar.num_piso,
                estacionamentos[i].lugar.fila,
                estacionamentos[i].lugar.lugar,
                estacionamentos[i].data_saida.dia, estacionamentos[i].data_saida.mes, estacionamentos[i].data_saida.ano,
                estacionamentos[i].saida.hora, estacionamentos[i].saida.min, estacionamentos[i].saida.seg,
                estacionamentos[i].valor_pago,
                estacionamentos[i].observacoes);
    }

    fclose(file);
    printf("Registos de estacionamento gravados com sucesso em %s.\n", filename);
}

void registar_entrada(Parque *parque, const char *matricula, int piso, char fila, int lugar, Horario entrada) {
    if (piso < 1 || piso > parque->num_pisos) {
        printf("Erro: Piso %d inválido.\n", piso);
        return;
    }

    Piso *p = &parque->pisos[piso - 1];
    int fila_index = fila - 'A';

    if (fila_index < 0 || fila_index >= MAX_FILAS || lugar < 1 || lugar > MAX_LUGARES) {
        printf("Erro: Fila %c ou lugar %d inválidos.\n", fila, lugar);
        return;
    }

    Lugar *l = &p->lugares[fila_index][lugar - 1];

    if (l->estado != 'L') {
        printf("Erro: O lugar %c%d no piso %d não está disponível.\n", fila, lugar, piso);
        return;
    }

    l->estado = 'O';
    p->ocupados++;
    p->livres--;
    parque->lugares_ocupados++;
    parque->lugares_livres--;

    printf("Entrada registada com sucesso para a matrícula %s no lugar %c%d do piso %d.\n", matricula, fila, lugar, piso);
}

void registar_saida(Parque *parque, const char *matricula, Horario saida, const Tarifario *tarifario) {
    for (int i = 0; i < parque->num_pisos; i++) {
        Piso *p = &parque->pisos[i];
        for (int f = 0; f < MAX_FILAS; f++) {
            for (int l = 0; l < MAX_LUGARES; l++) {
                Lugar *lugar = &p->lugares[f][l];
                if (lugar->estado == 'O' && strcmp(lugar->codigo, matricula) == 0) {
                    lugar->estado = 'L';
                    p->ocupados--;
                    p->livres++;
                    parque->lugares_ocupados--;
                    parque->lugares_livres++;

                    printf("Saída registada com sucesso para a matrícula %s.\n", matricula);
                    return;
                }
            }
        }
    }
    printf("Erro: Matrícula %s não encontrada nos registos.\n", matricula);
}

void mostrar_ocupacao_parque(Parque *parques, int total_parques) {
    if (total_parques == 0) {
        printf("Não há parques configurados.\n");
        return;
    }

    printf("Parques disponíveis:\n");
    for (int i = 0; i < total_parques; i++) {
        printf("%d. %s\n", i + 1, parques[i].nome);
    }

    printf("Escolha o número do parque: ");
    int parque_idx;
    scanf("%d", &parque_idx);

    if (parque_idx < 1 || parque_idx > total_parques) {
        printf("Erro: Parque inválido.\n");
        return;
    }

    Parque *parque = &parques[parque_idx - 1];

    if (parque->num_pisos == 0) {
        printf("O parque '%s' não foi configurado ou não tem pisos disponíveis.\n", parque->nome);
        return;
    }

    printf("Parque: %s\n", parque->nome);
    printf("Escolha o piso a visualizar (1-%d): ", parque->num_pisos);
    int piso_num;
    scanf("%d", &piso_num);

    if (piso_num < 1 || piso_num > parque->num_pisos) {
        printf("Erro: Piso inválido.\n");
        return;
    }

    Piso *piso = &parque->pisos[piso_num - 1];
    printf("Mapa de Ocupação do Piso %d no Parque '%s':\n", piso->numero, parque->nome);
    printf("     ");
    for (int l = 1; l <= MAX_LUGARES; l++) {
        printf("%2d ", l);
    }
    printf("\n");

    for (int f = 0; f < MAX_FILAS; f++) {
        printf("%c:  ", 'A' + f);
        for (int l = 0; l < MAX_LUGARES; l++) {
            char estado = piso->lugares[f][l].estado;
            if (estado == 'O') {
                printf("X  "); // Ocupado
            } else if (estado == 'I') {
                printf("I  "); // Indisponível
            } else {
                printf(".  "); // Livre
            }
        }
        printf("\n");
    }
    printf("\n");
}



void alterar_lugar(Parque *parque, const char *matricula, int novo_piso, char nova_fila, int novo_lugar) {
    for (int i = 0; i < parque->num_pisos; i++) {
        Piso *piso = &parque->pisos[i];
        for (int f = 0; f < MAX_FILAS; f++) {
            for (int l = 0; l < MAX_LUGARES; l++) {
                Lugar *lugar = &piso->lugares[f][l];
                if (lugar->estado == 'O' && strcmp(lugar->codigo, matricula) == 0) {
                    lugar->estado = 'L';
                    piso->ocupados--;
                    piso->livres++;
                    parque->lugares_ocupados--;
                    parque->lugares_livres++;

                    int nova_fila_index = nova_fila - 'A';
                    Lugar *novo_lugar_ptr = &parque->pisos[novo_piso - 1].lugares[nova_fila_index][novo_lugar - 1];
                    if (novo_lugar_ptr->estado != 'L') {
                        printf("Erro: O novo lugar %c%d no piso %d não está disponível.\n", nova_fila, novo_lugar, novo_piso);
                        return;
                    }

                    novo_lugar_ptr->estado = 'O';
                    parque->pisos[novo_piso - 1].ocupados++;
                    parque->pisos[novo_piso - 1].livres--;
                    parque->lugares_ocupados++;
                    parque->lugares_livres--;

                    printf("Lugar alterado com sucesso para %c%d no piso %d.\n", nova_fila, novo_lugar, novo_piso);
                    return;
                }
            }
        }
    }
    printf("Erro: Matrícula %s não encontrada.\n", matricula);
}

void especificar_lugar_indisponivel(Parque *parque, int piso, char fila, int lugar, char motivo) {
    if (piso < 1 || piso > parque->num_pisos) {
        printf("Erro: Piso %d inválido.\n", piso);
        return;
    }

    Piso *p = &parque->pisos[piso - 1];
    int fila_index = fila - 'A';

    if (fila_index < 0 || fila_index >= MAX_FILAS || lugar < 1 || lugar > MAX_LUGARES) {
        printf("Erro: Fila %c ou lugar %d inválidos.\n", fila, lugar);
        return;
    }

    Lugar *l = &p->lugares[fila_index][lugar - 1];
    if (l->estado != 'L') {
        printf("Erro: O lugar %c%d no piso %d não está disponível para marcação.\n", fila, lugar, piso);
        return;
    }

    l->estado = 'I';
    l->codigo[0] = motivo; // Usa o código para armazenar o motivo como marcador simples
    p->indisponiveis++;
    p->livres--;
    parque->lugares_livres--;

    printf("Lugar %c%d no piso %d marcado como indisponível com motivo '%c'.\n", fila, lugar, piso, motivo);
}
