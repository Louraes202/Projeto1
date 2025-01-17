#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "estacionamento.h"

// ------------------------ Funções do menu  ------------------------
void exibir_lugares_por_piso(const Parque *parque) {
    if (parque->num_pisos == 0) {
        printf("%50s\n", "N.º de lugares por pisos: Sem pisos configurados");
    } else {
        printf("%50s", "Lugares livres por piso: ");
        for (int i = 0; i < parque->num_pisos; i++) {
            printf("Piso %d: %d  ", i + 1, parque->pisos[i].livres);
        }
        printf("\n");
    }
}


// ------------------------ Manipulação de ficheiros do Parque ------------------------

// Função para gravar a configuração no ficheiro
void gravar_configuracao_parque(const Parque *parque) {
    FILE *file = fopen("config_parque.txt", "w");
    if (file == NULL) {
        printf("Erro ao gravar configuração do parque no ficheiro.\n");
        return;
    }

    // Escrever nome, morada e número de pisos
    fprintf(file, "%s\n%s\n%d\n", parque->nome, parque->morada, parque->num_pisos);

    for (int i = 0; i < parque->num_pisos; i++) {
        const Piso *piso = &parque->pisos[i];

        // Gravar o marcador do piso e o número de filas
        fprintf(file, "P%d %d\n", i + 1, piso->num_filas);

        // Gravar a configuração de cada fila
        for (int j = 0; j < piso->num_filas; j++) {
            fprintf(file, "F%d %d\n", j + 1, piso->lugares_por_fila[j]);
        }
    }

    if (ferror(file)) {
        printf("Erro ao gravar dados no ficheiro.\n");
    } else {
        printf("Configuração do parque gravada com sucesso.\n");
    }

    fclose(file);
}


// Função para carregar a configuração do ficheiro
int carregar_configuracao_parque(Parque *parque) {
    FILE *file = fopen("config_parque.txt", "r");
    if (file == NULL) {
        printf("Sistema: O ficheiro de configuração não existe.\n");
        return 0;
    }

    memset(parque, 0, sizeof(Parque)); // Limpa a estrutura do parque

    // Ler nome, morada e número de pisos
    if (fgets(parque->nome, sizeof(parque->nome), file) == NULL ||
        fgets(parque->morada, sizeof(parque->morada), file) == NULL ||
        fscanf(file, "%d\n", &parque->num_pisos) != 1) {
        printf("Erro: Formato inválido no ficheiro de configuração.\n");
        fclose(file);
        return 0;
    }

    parque->nome[strcspn(parque->nome, "\n")] = '\0';
    parque->morada[strcspn(parque->morada, "\n")] = '\0';

    if (parque->num_pisos <= 0 || parque->num_pisos > MAX_PISOS) {
        printf("Erro: Número de pisos inválido.\n");
        fclose(file);
        return 0;
    }

    for (int i = 0; i < parque->num_pisos; i++) {
        Piso *piso = &parque->pisos[i];
        int numero_piso, num_filas;

        if (fscanf(file, "P%d %d\n", &numero_piso, &num_filas) != 2 || numero_piso != i + 1) {
            printf("Erro ao carregar dados do Piso %d.\n", i + 1);
            fclose(file);
            return 0;
        }

        piso->num_filas = num_filas;
        piso->livres = 0;
        piso->ocupados = 0;
        piso->indisponiveis = 0;

        if (num_filas <= 0 || num_filas > MAX_FILAS) {
            printf("Erro: Número de filas inválido no Piso %d.\n", i + 1);
            fclose(file);
            return 0;
        }

        for (int j = 0; j < num_filas; j++) {
            int numero_fila, lugares;
            if (fscanf(file, "F%d %d\n", &numero_fila, &lugares) != 2 || numero_fila != j + 1) {
                printf("Erro ao carregar dados da Fila %d no Piso %d.\n", j + 1, i + 1);
                fclose(file);
                return 0;
            }

            if (lugares <= 0 || lugares > MAX_LUGARES) {
                printf("Erro: Número de lugares inválido na Fila %d do Piso %d.\n", j + 1, i + 1);
                fclose(file);
                return 0;
            }

            piso->lugares_por_fila[j] = lugares;
            piso->livres += lugares;

            for (int lugar = 0; lugar < lugares; lugar++) {
                Lugar *lugar_atual = &piso->lugares[j][lugar];
                lugar_atual->estado = 'L'; // Livre
                memset(lugar_atual->codigo, 0, sizeof(lugar_atual->codigo));
            }
        }
    }

    recalcular_estatisticas_parque(parque); // Atualiza estatísticas globais

    printf("Configuração do parque carregada com sucesso.\n");
    fclose(file);
    return 1;
}

// Função para carregar tarifas de Tarifas.txt
int carregar_tarifas(Tarifario *tarifario) {
    FILE *file = fopen("Tarifas.txt", "r");
    if (file == NULL) {
        printf("Sistema: Não foi possível abrir o ficheiro Tarifas.txt.\n");
        return 0;
    }

    // Lê o número de tarifas do ficheiro
    fscanf(file, "%d\n", &tarifario->num_tarifas);
    tarifario->lista_tarifas = malloc(sizeof(Tarifa) * tarifario->num_tarifas);
    if (tarifario->lista_tarifas == NULL) {
        printf("Sistema: Falha ao alocar memória para as tarifas.\n");
        fclose(file);
        return 0;
    }

    // Lê cada tarifa do ficheiro
    for (int i = 0; i < tarifario->num_tarifas; i++) {
        fscanf(file, "%10s %3s %f %d:%d %d:%d %c %d\n", 
               tarifario->lista_tarifas[i].nome, 
               tarifario->lista_tarifas[i].cod_tarifa, 
               &tarifario->lista_tarifas[i].valor_hora,
               &tarifario->lista_tarifas[i].inicio.hora, &tarifario->lista_tarifas[i].inicio.min,
               &tarifario->lista_tarifas[i].fim.hora, &tarifario->lista_tarifas[i].fim.min,
               &tarifario->lista_tarifas[i].tp_tarifa,
               &tarifario->lista_tarifas[i].dias);
    }

    fclose(file);
    printf("Tarifas carregadas com sucesso.\n");
    return 1;
}


// Função para guardar o estado no ficheiro binário
void guardar_estado_binario(const Parque *parque, Estacionamento estacionamentos[], int total_estacionamentos) {
    FILE *file = fopen("estado_parque.bin", "wb");
    if (file == NULL) {
        printf("Erro ao abrir o ficheiro binário para gravação.\n");
        return;
    }

    // Gravar o nome e a morada do parque
    fwrite(parque->nome, sizeof(parque->nome), 1, file);
    fwrite(parque->morada, sizeof(parque->morada), 1, file);

    // Gravar o número de pisos
    fwrite(&parque->num_pisos, sizeof(int), 1, file);

    // Gravar cada piso
    for (int i = 0; i < parque->num_pisos; i++) {
        const Piso *piso = &parque->pisos[i];

        // Gravar o número de filas do piso
        fwrite(&piso->num_filas, sizeof(int), 1, file);

        // Gravar os lugares por fila
        fwrite(piso->lugares_por_fila, sizeof(int), piso->num_filas, file);

        // Gravar os detalhes dos lugares
        for (int j = 0; j < piso->num_filas; j++) {
            fwrite(piso->lugares[j], sizeof(Lugar), piso->lugares_por_fila[j], file);
        }
    }

    // Gravar o número total de registos
    fwrite(&total_estacionamentos, sizeof(int), 1, file);

    // Gravar os registos de estacionamento
    fwrite(estacionamentos, sizeof(Estacionamento), total_estacionamentos, file);

    fclose(file);
    printf("Estado do parque guardado com sucesso no ficheiro binário.\n");
}

// Função para carregar o estado do ficheiro binário
int carregar_estado_binario(Parque *parque, Estacionamento estacionamentos[], int *total_estacionamentos, int *ultimo_id) {
    FILE *file = fopen("estado_parque.bin", "rb");
    if (file == NULL) {
        printf("Nenhum ficheiro binário encontrado. Inicializando com dados padrão.\n");
        *total_estacionamentos = 0;
        *ultimo_id = 0;
        return 0;
    }

    // Limpar a estrutura antes de carregar
    memset(parque, 0, sizeof(Parque));

    // Ler o nome e a morada do parque
    fread(parque->nome, sizeof(parque->nome), 1, file);
    fread(parque->morada, sizeof(parque->morada), 1, file);

    // Ler o número de pisos
    if (fread(&parque->num_pisos, sizeof(int), 1, file) != 1 || parque->num_pisos <= 0 || parque->num_pisos > MAX_PISOS) {
        printf("Erro ao carregar o número de pisos. Inicializando com dados padrão.\n");
        fclose(file);
        memset(parque, 0, sizeof(Parque));
        return 0;
    }

    // Ler cada piso
    for (int i = 0; i < parque->num_pisos; i++) {
        Piso *piso = &parque->pisos[i];

        // Ler o número de filas do piso
        if (fread(&piso->num_filas, sizeof(int), 1, file) != 1 || piso->num_filas <= 0 || piso->num_filas > MAX_FILAS) {
            printf("Erro ao carregar o número de filas no Piso %d. Inicializando com dados padrão.\n", i + 1);
            fclose(file);
            memset(parque, 0, sizeof(Parque));
            return 0;
        }

        // Ler os lugares por fila
        if (fread(piso->lugares_por_fila, sizeof(int), piso->num_filas, file) != (size_t)piso->num_filas) {
            printf("Erro ao carregar os lugares por fila no Piso %d. Inicializando com dados padrão.\n", i + 1);
            fclose(file);
            memset(parque, 0, sizeof(Parque));
            return 0;
        }

        piso->livres = 0;
        piso->ocupados = 0;
        piso->indisponiveis = 0;

        // Ler os detalhes de cada lugar
        for (int j = 0; j < piso->num_filas; j++) {
            if (fread(piso->lugares[j], sizeof(Lugar), piso->lugares_por_fila[j], file) != (size_t)piso->lugares_por_fila[j]) {
                printf("Erro ao carregar os dados do lugar no Piso %d, Fila %d.\n", i + 1, j + 1);
                fclose(file);
                memset(parque, 0, sizeof(Parque));
                return 0;
            }

            // Atualizar estatísticas do piso
            for (int k = 0; k < piso->lugares_por_fila[j]; k++) {
                Lugar *lugar_atual = &piso->lugares[j][k];
                if (lugar_atual->estado == 'L') {
                    piso->livres++;
                } else if (lugar_atual->estado == 'O') {
                    piso->ocupados++;
                } else if (lugar_atual->estado == 'I') {
                    piso->indisponiveis++;
                }
            }
        }
    }

    // Ler o número total de registos
    if (fread(total_estacionamentos, sizeof(int), 1, file) != 1) {
        printf("Erro ao carregar o total de registos. Inicializando com 0 registos.\n");
        fclose(file);
        *total_estacionamentos = 0;
        return 0;
    }

    // Ler os registos de estacionamento
    if (fread(estacionamentos, sizeof(Estacionamento), *total_estacionamentos, file) != (size_t)*total_estacionamentos) {
        printf("Erro ao carregar registos. Inicializando com registos vazios.\n");
        fclose(file);
        *total_estacionamentos = 0;
        return 0;
    }

    // Atualizar o último ID com base nos registos carregados
    for (int i = 0; i < *total_estacionamentos; i++) {
        if (estacionamentos[i].numE > *ultimo_id) {
            *ultimo_id = estacionamentos[i].numE;
        }
    }

    // Recalcular as estatísticas do parque
    recalcular_estatisticas_parque(parque);

    fclose(file);
    printf("Sistema: Estado do parque carregado com sucesso.\n");
    return 1;
}

void recalcular_estatisticas_parque(Parque *parque) {
    parque->total_lugares = 0;
    parque->lugares_livres = 0;
    parque->lugares_ocupados = 0;

    for (int i = 0; i < parque->num_pisos; i++) {
        Piso *piso = &parque->pisos[i];
        piso->livres = 0;
        piso->ocupados = 0;
        piso->indisponiveis = 0;

        for (int fila = 0; fila < piso->num_filas; fila++) {
            int lugares_na_fila = piso->lugares_por_fila[fila];
            parque->total_lugares += lugares_na_fila;

            for (int lugar = 0; lugar < lugares_na_fila; lugar++) {
                Lugar *lugar_atual = &piso->lugares[fila][lugar];
                if (lugar_atual->estado == 'L') {
                    piso->livres++;
                    parque->lugares_livres++;
                } else if (lugar_atual->estado == 'O') {
                    piso->ocupados++;
                    parque->lugares_ocupados++;
                } else if (lugar_atual->estado == 'I') {
                    piso->indisponiveis++;
                }
            }
        }
    }

    printf("Estatísticas recalculadas: Total Lugares: %d, Livres: %d, Ocupados: %d, Indisponíveis: %d\n",
           parque->total_lugares, parque->lugares_livres, parque->lugares_ocupados,
           parque->total_lugares - (parque->lugares_livres + parque->lugares_ocupados));
}


int carregar_registos_txt(const char *nome_ficheiro, Estacionamento estacionamentos[], int *total_estacionamentos, Parque *parque) {
    FILE *file = fopen(nome_ficheiro, "r");
    if (file == NULL) {
        printf("Sistema: Não foi possível abrir o ficheiro %s.\n", nome_ficheiro);
        return 0;
    }

    int registos_descartados = 0;

    while (fscanf(file, "%d %10s %d %d %d %d %d %4s %d %d %d %d %d",
                  &estacionamentos[*total_estacionamentos].numE,
                  estacionamentos[*total_estacionamentos].matricula,
                  &estacionamentos[*total_estacionamentos].data_entrada.ano,
                  &estacionamentos[*total_estacionamentos].data_entrada.mes,
                  &estacionamentos[*total_estacionamentos].data_entrada.dia,
                  &estacionamentos[*total_estacionamentos].entrada.hora,
                  &estacionamentos[*total_estacionamentos].entrada.min,
                  estacionamentos[*total_estacionamentos].lugar.codigo,
                  &estacionamentos[*total_estacionamentos].data_saida.ano,
                  &estacionamentos[*total_estacionamentos].data_saida.mes,
                  &estacionamentos[*total_estacionamentos].data_saida.dia,
                  &estacionamentos[*total_estacionamentos].saida.hora,
                  &estacionamentos[*total_estacionamentos].saida.min) == 13) {

        // Extrair Piso, Fila e Lugar
        int piso, lugar;
        char fila; // Alterado para char para o sscanf
        sscanf(estacionamentos[*total_estacionamentos].lugar.codigo, "%d%c%d", &piso, &fila, &lugar);

        // Converter a fila para índice numérico
        int fila_idx = fila - 'A';

        // Validação de compatibilidade do registo com o parque
        if (piso < 1 || piso > parque->num_pisos) {
            printf("Aviso: Registo incompatível (Piso: %d, Fila: %c, Lugar: %d). Ignorado.\n", piso, fila, lugar);
            registos_descartados++;
            continue;
        }

        Piso *piso_atual = &parque->pisos[piso - 1];

        if (fila_idx < 0 || fila_idx >= piso_atual->num_filas ||
            lugar < 1 || lugar > piso_atual->lugares_por_fila[fila_idx]) {
            printf("Aviso: Registo incompatível (Piso: %d, Fila: %c, Lugar: %d). Ignorado.\n", piso, fila, lugar);
            registos_descartados++;
            continue;
        }

        // Validação do estado do lugar
        Lugar *lugar_atual = &piso_atual->lugares[fila_idx][lugar - 1];
        if (lugar_atual->estado != 'L') { // Verifica se o lugar está livre
            printf("Aviso: Lugar já ocupado ou indisponível (Piso: %d, Fila: %c, Lugar: %d). Ignorado.\n", piso, fila, lugar);
            registos_descartados++;
            continue;
        }

        // Atualizar o estado do lugar
        lugar_atual->estado = 'O'; // Marca como ocupado
        strcpy(lugar_atual->codigo, estacionamentos[*total_estacionamentos].matricula);

        // Atualizar o estado do parque
        piso_atual->ocupados++;
        piso_atual->livres--;

        // Incrementar o contador de registos válidos
        (*total_estacionamentos)++;
    }

    fclose(file);

    if (registos_descartados > 0) {
        printf("Aviso: %d registo(s) incompatível(eis) foram descartado(s).\n", registos_descartados);
    }

    printf("Registos carregados com sucesso a partir do ficheiro %s.\n", nome_ficheiro);
    return 1;
}




// ------------------------ Configuração do Parque ------------------------

// Função para limpar a memória e apagar ficheiros associados
void limpar_memoria(Parque *parque, Estacionamento estacionamentos[], int *total_estacionamentos) {
    // Resetar a estrutura Parque
    memset(parque, 0, sizeof(Parque));

    // Resetar os registos de estacionamento
    memset(estacionamentos, 0, sizeof(Estacionamento) * (*total_estacionamentos));
    *total_estacionamentos = 0;

    // Esvaziar o ficheiro config_parque.txt
    FILE *file = fopen("config_parque.txt", "w");
    if (file == NULL) {
        printf("Erro ao limpar o ficheiro de configuração.\n");
    } else {
        fclose(file);
        printf("Ficheiro config_parque.txt limpo com sucesso.\n");
    }

    // Apagar o ficheiro binário do estado do parque
    if (remove("estado_parque.bin") == 0) {
        printf("Ficheiro binário apagado com sucesso.\n");
    } else {
        printf("Nenhum ficheiro binário encontrado para apagar.\n");
    }

    printf("Memória do programa e ficheiros associados foram limpos.\n");
}


// Função para configurar os pisos do parque
void configurar_pisos(Parque *parque) {
    for (int piso_idx = 0; piso_idx < parque->num_pisos; piso_idx++) {
        printf("\nConfiguração para o Piso %d:\n", piso_idx + 1);

        // Solicitar o número de filas no piso
        int num_filas;
        do {
            printf("Digite o número de filas neste piso (1-26, -1 para voltar): ");
            if (scanf("%d", &num_filas) != 1 || num_filas == -1) {
                printf("Cancelando configuração do Piso %d.\n", piso_idx + 1);
                return;
            }

            if (num_filas < 1 || num_filas > MAX_FILAS) {
                printf("Erro: O número de filas deve estar entre 1 e %d.\n", MAX_FILAS);
            } else {
                break;
            }
        } while (1);

        Piso *piso = &parque->pisos[piso_idx];
        piso->num_filas = num_filas;
        piso->livres = 0;
        piso->ocupados = 0;
        piso->indisponiveis = 0;

        // Escolher o tipo de distribuição de lugares
        printf("Selecione o tipo de distribuição:\n");
        printf("1. Distribuição Quadrada (número uniforme de lugares por fila)\n");
        printf("2. Distribuição Manual (definir o número de lugares por fila)\n");
        printf("Escolha uma opção: ");
        int tipo_distribuicao;
        if (scanf("%d", &tipo_distribuicao) != 1 || (tipo_distribuicao != 1 && tipo_distribuicao != 2)) {
            printf("Opção inválida. Cancelando configuração do Piso %d.\n", piso_idx + 1);
            return;
        }

        // Configurar as filas e lugares com base na escolha
        if (tipo_distribuicao == 1) { // Distribuição Quadrada
            int lugares_por_fila;
            do {
                printf("Digite o número de lugares por fila (1-50): ");
                if (scanf("%d", &lugares_por_fila) != 1 || lugares_por_fila < 1 || lugares_por_fila > MAX_LUGARES) {
                    printf("Erro: Número de lugares inválido. Deve estar entre 1 e %d.\n", MAX_LUGARES);
                } else {
                    break;
                }
            } while (1);

            for (int fila = 0; fila < num_filas; fila++) {
                piso->lugares_por_fila[fila] = lugares_por_fila;
                piso->livres += lugares_por_fila;

                for (int lugar_idx = 0; lugar_idx < lugares_por_fila; lugar_idx++) {
                    Lugar *lugar = &piso->lugares[fila][lugar_idx];
                    lugar->estado = 'L'; // Livre
                    memset(lugar->codigo, 0, sizeof(lugar->codigo));
                }
            }
        } else if (tipo_distribuicao == 2) { // Distribuição Manual
            for (int fila = 0; fila < num_filas; fila++) {
                int lugares_na_fila;
                do {
                    printf("Digite o número de lugares para a Fila %c (1-50): ", 'A' + fila);
                    if (scanf("%d", &lugares_na_fila) != 1 || lugares_na_fila < 1 || lugares_na_fila > MAX_LUGARES) {
                        printf("Erro: Número de lugares inválido. Deve estar entre 1 e %d.\n", MAX_LUGARES);
                    } else {
                        break;
                    }
                } while (1);

                piso->lugares_por_fila[fila] = lugares_na_fila;
                piso->livres += lugares_na_fila;

                for (int lugar_idx = 0; lugar_idx < lugares_na_fila; lugar_idx++) {
                    Lugar *lugar = &piso->lugares[fila][lugar_idx];
                    lugar->estado = 'L'; // Livre
                    memset(lugar->codigo, 0, sizeof(lugar->codigo));
                }
            }
        }
    }

    printf("Configuração dos pisos concluída com sucesso.\n");
}


// Função para configurar o parque a ser gerido pelo programa
void configurar_parque(Parque *parque) {
    printf("Digite o nome do parque (até 30 caracteres, -1 para voltar): ");
    if (scanf("%30s", parque->nome) == 1 && strcmp(parque->nome, "-1") == 0) {
        printf("Operação cancelada.\n");
        return; // Voltar ao menu anterior
    }

    printf("Digite a morada do parque (até 30 caracteres, -1 para voltar): ");
    char morada_temp[30];
    scanf(" %[^\n]%*c", morada_temp);
    if (strcmp(morada_temp, "-1") == 0) {
        printf("Operação cancelada.\n");
        return;
    }
    strcpy(parque->morada, morada_temp);

    do {
        printf("Digite o número de andares (1-5, -1 para voltar): ");
        if (scanf("%d", &parque->num_pisos) != 1 || parque->num_pisos == -1) {
            printf("Operação cancelada.\n");
            return; // Voltar ao menu anterior
        }

        if (parque->num_pisos < 1 || parque->num_pisos > MAX_PISOS) {
            printf("Erro: O número de pisos deve estar entre 1 e %d.\n", MAX_PISOS);
        } else {
            break;
        }
    } while (1);

    configurar_pisos(parque);

    gravar_configuracao_parque(parque);

    printf("Configuração do parque concluída com sucesso.\n");
}



// Função para alterar os dados do parque
void alterar_parque(Parque *parque) {
    int opcao;

    printf("\n--- Alterar Configuração do Parque ---\n");
    printf("1. Alterar nome do parque\n");
    printf("2. Alterar número de pisos\n");
    printf("3. Alterar filas ou lugares em um piso específico\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            printf("Digite o novo nome do parque (até 30 caracteres): ");
            scanf("%30s", parque->nome);
            break;
        case 2:
            do {
                printf("Digite o novo número de andares (1-5): ");
                scanf("%d", &parque->num_pisos);
                if (parque->num_pisos < 1 || parque->num_pisos > MAX_PISOS) {
                    printf("Erro: O número de andares deve estar entre 1 e %d.\n", MAX_PISOS);
                }
            } while (parque->num_pisos < 1 || parque->num_pisos > MAX_PISOS);
            break;
        case 3: {
            int piso;
            do {
                printf("Digite o número do piso que deseja alterar (1-%d): ", parque->num_pisos);
                scanf("%d", &piso);
                if (piso < 1 || piso > parque->num_pisos) {
                    printf("Erro: Piso inválido. Escolha entre 1 e %d.\n", parque->num_pisos);
                }
            } while (piso < 1 || piso > parque->num_pisos);

            Piso *piso_atual = &parque->pisos[piso - 1];

            printf("Alterar configuração do Piso %d:\n", piso);
            printf("1. Alterar número de filas\n");
            printf("2. Alterar lugares em uma fila específica\n");
            printf("0. Voltar\n");
            printf("Escolha uma opção: ");
            int sub_opcao;
            scanf("%d", &sub_opcao);

            switch (sub_opcao) {
                case 1: {
                    int num_filas;
                    do {
                        printf("Digite o novo número de filas (1-26): ");
                        scanf("%d", &num_filas);
                        if (num_filas < 1 || num_filas > MAX_FILAS) {
                            printf("Erro: O número de filas deve estar entre 1 e %d.\n", MAX_FILAS);
                        }
                    } while (num_filas < 1 || num_filas > MAX_FILAS);

                    piso_atual->num_filas = num_filas;
                    // Redefinir os lugares por fila
                    for (int i = 0; i < num_filas; i++) {
                        piso_atual->lugares_por_fila[i] = 0;
                    }
                    printf("Número de filas alterado com sucesso.\n");
                    break;
                }
                case 2: {
                    int fila;
                    do {
                        printf("Digite o número da fila que deseja alterar (1-%d): ", piso_atual->num_filas);
                        scanf("%d", &fila);
                        if (fila < 1 || fila > piso_atual->num_filas) {
                            printf("Erro: Fila inválida. Escolha entre 1 e %d.\n", piso_atual->num_filas);
                        }
                    } while (fila < 1 || fila > piso_atual->num_filas);

                    int num_lugares;
                    do {
                        printf("Digite o novo número de lugares para a Fila %c (1-50): ", 'A' + fila - 1);
                        scanf("%d", &num_lugares);
                        if (num_lugares < 1 || num_lugares > MAX_LUGARES) {
                            printf("Erro: O número de lugares deve estar entre 1 e %d.\n", MAX_LUGARES);
                        }
                    } while (num_lugares < 1 || num_lugares > MAX_LUGARES);

                    piso_atual->lugares_por_fila[fila - 1] = num_lugares;
                    printf("Número de lugares na Fila %c alterado com sucesso.\n", 'A' + fila - 1);
                    break;
                }
                case 0:
                    printf("Voltando ao menu anterior.\n");
                    return;
                default:
                    printf("Opção inválida.\n");
                    return;
            }
            break;
        }
        case 0:
            printf("Nenhuma alteração realizada.\n");
            return;
        default:
            printf("Opção inválida.\n");
            return;
    }

    // Recalcular os valores derivados
    recalcular_estatisticas_parque(parque);

    // Gravar as alterações no ficheiro
    gravar_configuracao_parque(parque);
    printf("Alterações realizadas com sucesso e salvas no ficheiro.\n");
}


void visualizar_dados_parque(const Parque *parque) {
    // Verificar se os dados básicos do parque estão configurados
    if (strlen(parque->nome) == 0 || parque->num_pisos <= 0) {
        printf("\nSistema: O parque ainda não está configurado ou os dados não foram carregados.\n");
        printf("Por favor, configure o parque ou carregue os dados de um ficheiro.\n");
        return;
    }

    printf("\n-- Dados Gerais do Parque --\n");
    printf("Nome: %s\n", parque->nome);
    printf("Morada: %s\n", parque->morada);
    printf("Número de pisos: %d\n", parque->num_pisos);
    printf("Total de lugares no parque: %d\n", parque->total_lugares);
    printf("Lugares ocupados: %d\n", parque->lugares_ocupados);
    printf("Lugares livres: %d\n", parque->lugares_livres);
    printf("------------------------------\n");

    // Início da navegação interativa
    int piso_atual = 0;
    char opcao[10];
    do {
        const Piso *piso = &parque->pisos[piso_atual];
        printf("\n--- Piso %d ---\n", piso_atual + 1);
        printf("  Número de filas: %d\n", piso->num_filas);
        printf("  Lugares livres: %d\n", piso->livres);
        printf("  Lugares ocupados: %d\n", piso->ocupados);
        printf("  Lugares indisponíveis: %d\n", piso->indisponiveis);
        printf("------------------------------\n");

        // Menu para explorar filas ou navegar
        printf("Opções disponíveis:\n");
        printf("  F - Ver detalhes das filas\n");
        printf("  N - Próximo piso\n");
        printf("  P - Piso anterior\n");
        printf("  S - Sair\n");
        printf("  Ou insira o número de um piso (1-%d) para ir diretamente.\n", parque->num_pisos);
        printf("Escolha uma opção: ");
        scanf(" %s", opcao);

        if (opcao[0] == 'N' || opcao[0] == 'n') {
            if (piso_atual < parque->num_pisos - 1) {
                piso_atual++;
            } else {
                printf("\nVocê já está no último piso.\n");
            }
        } else if (opcao[0] == 'P' || opcao[0] == 'p') {
            if (piso_atual > 0) {
                piso_atual--;
            } else {
                printf("\nVocê já está no primeiro piso.\n");
            }
        } else if (opcao[0] == 'F' || opcao[0] == 'f') {
            int fila_atual = 0;
            do {
                printf("\n--- Detalhes da Fila %c no Piso %d ---\n", 'A' + fila_atual, piso_atual + 1);
                printf("  Número de lugares: %d\n", piso->lugares_por_fila[fila_atual]);
                printf("\nOpções disponíveis:\n");
                printf("  N - Próxima fila\n");
                printf("  P - Fila anterior\n");
                printf("  B - Voltar ao menu do piso\n");
                printf("  Ou insira o número de uma fila (1-%d) para ir diretamente.\n", piso->num_filas);
                printf("Escolha uma opção: ");
                scanf(" %s", opcao);

                if (opcao[0] == 'N' || opcao[0] == 'n') {
                    if (fila_atual < piso->num_filas - 1) {
                        fila_atual++;
                    } else {
                        printf("\nVocê já está na última fila.\n");
                    }
                } else if (opcao[0] == 'P' || opcao[0] == 'p') {
                    if (fila_atual > 0) {
                        fila_atual--;
                    } else {
                        printf("\nVocê já está na primeira fila.\n");
                    }
                } else if (opcao[0] == 'B' || opcao[0] == 'b') {
                    break;
                } else if (isdigit(opcao[0])) {
                    int fila_escolhida = atoi(opcao);
                    if (fila_escolhida >= 1 && fila_escolhida <= piso->num_filas) {
                        fila_atual = fila_escolhida - 1;
                    } else {
                        printf("\nErro: Fila inválida.\n");
                    }
                } else {
                    printf("\nOpção inválida. Tente novamente.\n");
                }
            } while (opcao[0] != 'B' && opcao[0] != 'b');
        } else if (isdigit(opcao[0])) {
            int piso_escolhido = atoi(opcao);
            if (piso_escolhido >= 1 && piso_escolhido <= parque->num_pisos) {
                piso_atual = piso_escolhido - 1;
            } else {
                printf("\nErro: Piso inválido.\n");
            }
        } else if (opcao[0] != 'S' && opcao[0] != 's') {
            printf("\nOpção inválida. Tente novamente.\n");
        }

    } while (opcao[0] != 'S' && opcao[0] != 's');

    printf("\nSaindo da visualização do parque.\n");
}




// Função para calcular o valor pago com base nas tarifas
float calcular_valor_pago(const Estacionamento *estacionamento, const Tarifario *tarifario) {
    float valor_total = 0.0f;
    int entrada_hora = estacionamento->entrada.hora;
    int entrada_min = estacionamento->entrada.min;
    int saida_hora = estacionamento->saida.hora;
    int saida_min = estacionamento->saida.min;

    // Itera sobre as tarifas para calcular o valor baseado nos horários
    for (int i = 0; i < tarifario->num_tarifas; i++) {
        Tarifa *tarifa = &tarifario->lista_tarifas[i];

        // Verifica se o horário de entrada/saída sobrepõe o intervalo da tarifa
        if ((entrada_hora < tarifa->fim.hora || 
            (entrada_hora == tarifa->fim.hora && entrada_min <= tarifa->fim.min)) &&
            (saida_hora > tarifa->inicio.hora || 
            (saida_hora == tarifa->inicio.hora && saida_min >= tarifa->inicio.min))) {
            
            // Calcula o tempo efetivo dentro do intervalo da tarifa
            int inicio_tarifa = tarifa->inicio.hora * 60 + tarifa->inicio.min;
            int fim_tarifa = tarifa->fim.hora * 60 + tarifa->fim.min;
            int entrada_total = entrada_hora * 60 + entrada_min;
            int saida_total = saida_hora * 60 + saida_min;

            int inicio_calculo = (entrada_total > inicio_tarifa) ? entrada_total : inicio_tarifa;
            int fim_calculo = (saida_total < fim_tarifa) ? saida_total : fim_tarifa;

            if (fim_calculo > inicio_calculo) {
                int minutos = fim_calculo - inicio_calculo;
                valor_total += (minutos / 60.0f) * tarifa->valor_hora;
            }
        }
    }

    return valor_total;
}

// ------------------------ Registos do Parque ------------------------
void exibir_estatisticas_ocupacao(const Parque *parque) {
    if (parque->num_pisos == 0) {
        printf("\nSistema: O parque ainda não está configurado.\n");
        return;
    }

    printf("\n--- Estatísticas de Ocupação do Parque ---\n");
    for (int i = 0; i < parque->num_pisos; i++) {
        printf("Piso %d:\n", i + 1);
        printf("  Lugares Livres: %d\n", parque->pisos[i].livres);
        printf("  Lugares Ocupados: %d\n", parque->pisos[i].ocupados);
        printf("  Lugares Indisponíveis: %d\n", parque->pisos[i].indisponiveis);
    }

    // Estatísticas gerais
    printf("\nResumo Geral:\n");
    printf("  Lugares Totais: %d\n", parque->total_lugares);
    printf("  Lugares Livres: %d\n", parque->lugares_livres);
    printf("  Lugares Ocupados: %d\n", parque->lugares_ocupados);
    printf("  Lugares Indisponíveis: %d\n\n", parque->total_lugares - (parque->lugares_livres + parque->lugares_ocupados));
}

// Função para registar a entrada de um veículo
void registar_entrada_veiculo(Parque *parque, Estacionamento estacionamentos[], int *total_estacionamentos, int *ultimo_id) {
    char matricula[11];
    printf("Digite a matrícula do veículo (XX-XX-XX): ");
    scanf("%10s", matricula);

    // Verifica se a matrícula já está associada a um veículo no parque
    for (int i = 0; i < *total_estacionamentos; i++) {
        if (strcmp(estacionamentos[i].matricula, matricula) == 0 &&
            estacionamentos[i].data_saida.ano == 0) { // Veículo ainda estacionado
            printf("Erro: O veículo com a matrícula %s já está estacionado.\n", matricula);
            return;
        }
    }

    // Atribuir lugar disponível
    int lugar_atribuido = 0;
    for (int piso = 0; piso < parque->num_pisos && !lugar_atribuido; piso++) {
        for (int fila = 0; fila < parque->pisos[piso].num_filas && !lugar_atribuido; fila++) {
            for (int lugar = 0; lugar < parque->pisos[piso].lugares_por_fila[fila] && !lugar_atribuido; lugar++) {
                Lugar *lugar_atual = &parque->pisos[piso].lugares[fila][lugar];
                if (lugar_atual->estado == 'L') {
                    // Atualiza o lugar para ocupado
                    lugar_atual->estado = 'O';
                    strcpy(lugar_atual->codigo, matricula);

                    // Atualiza o parque
                    parque->pisos[piso].ocupados++;
                    parque->pisos[piso].livres--;
                    recalcular_estatisticas_parque(parque); // **ALTERAÇÃO**


                    // Regista o estacionamento
                    Estacionamento *novo_estacionamento = &estacionamentos[*total_estacionamentos];
                    (*total_estacionamentos)++;
                    (*ultimo_id)++;

                    novo_estacionamento->numE = *ultimo_id;
                    strcpy(novo_estacionamento->matricula, matricula);

                    // Atribuir data e hora
                    printf("Pretende especificar a data e hora do estacionamento?\n");
                    printf("1. Sim\n");
                    printf("2. Não (Automático)\n");
                    printf("Escolha uma opção: ");
                    int opcao_data;
                    scanf("%d", &opcao_data);

                    if (opcao_data == 1) {
                        printf("Digite a data de entrada (AAAA-MM-DD): ");
                        scanf("%d-%d-%d", &novo_estacionamento->data_entrada.ano,
                              &novo_estacionamento->data_entrada.mes,
                              &novo_estacionamento->data_entrada.dia);
                        printf("Digite a hora de entrada (HH:MM): ");
                        scanf("%d:%d", &novo_estacionamento->entrada.hora,
                              &novo_estacionamento->entrada.min);
                    } else {
                        time_t t = time(NULL);
                        struct tm tm = *localtime(&t);
                        novo_estacionamento->data_entrada.ano = tm.tm_year + 1900;
                        novo_estacionamento->data_entrada.mes = tm.tm_mon + 1;
                        novo_estacionamento->data_entrada.dia = tm.tm_mday;
                        novo_estacionamento->entrada.hora = tm.tm_hour;
                        novo_estacionamento->entrada.min = tm.tm_min;
                    }

                    // Atribuir lugar
                    sprintf(novo_estacionamento->lugar.codigo, "%d%c%d", piso + 1, 'A' + fila, lugar + 1);

                    printf("Veículo com matrícula %s estacionado com sucesso.\n", matricula);
                    printf("Lugar atribuído: Piso %d, Fila %c, Lugar %d\n", piso + 1, 'A' + fila, lugar + 1);
                    printf("Data de entrada: %04d-%02d-%02d, Hora: %02d:%02d\n",
                           novo_estacionamento->data_entrada.ano, novo_estacionamento->data_entrada.mes,
                           novo_estacionamento->data_entrada.dia, novo_estacionamento->entrada.hora,
                           novo_estacionamento->entrada.min);

                    lugar_atribuido = 1;
                }
            }
        }
    }

    if (!lugar_atribuido) {
        printf("Erro: Não há lugares disponíveis no parque.\n");
    }
}

// Função para consultar um registo de estacionamento
void consultar_registo(int numero_entrada, const Estacionamento estacionamentos[], int total_estacionamentos) {
    for (int i = 0; i < total_estacionamentos; i++) {
        if (estacionamentos[i].numE == numero_entrada) {
            printf("Detalhes do registo:\n");
            printf("Número de Entrada: %d\n", estacionamentos[i].numE);
            printf("Matrícula: %s\n", estacionamentos[i].matricula);
            printf("Lugar: Piso %d, Fila %c, Lugar %d\n", estacionamentos[i].lugar.num_piso, estacionamentos[i].lugar.fila, estacionamentos[i].lugar.lugar);
            printf("Data de Entrada: %02d/%02d/%04d\n", estacionamentos[i].data_entrada.dia, estacionamentos[i].data_entrada.mes, estacionamentos[i].data_entrada.ano);
            return;
        }
    }
    printf("Registo não encontrado.\n");
}


// Função para eliminar um registo de estacionamento
void eliminar_registo(int numero_entrada, Parque *parque, Estacionamento estacionamentos[], int *total_estacionamentos) {
    for (int i = 0; i < *total_estacionamentos; i++) {
        if (estacionamentos[i].numE == numero_entrada) {
            // Marca o lugar como livre no parque
            Lugar *lugar = &estacionamentos[i].lugar;
            parque->pisos[lugar->num_piso - 1].lugares[lugar->fila - 'A'][lugar->lugar - 1].estado = 'L';

            // Remove o registo deslocando os restantes para preencher o espaço
            for (int j = i; j < *total_estacionamentos - 1; j++) {
                estacionamentos[j] = estacionamentos[j + 1];
            }
            (*total_estacionamentos)--;

            printf("Registo com número de entrada %d foi eliminado com sucesso.\n", numero_entrada);
            return;
        }
    }
    printf("Registo não encontrado.\n");
}


// Função para alterar um registo de estacionamento
void alterar_registo(int numero_entrada, const char *novo_lugar, Parque *parque, Estacionamento estacionamentos[], int total_estacionamentos) {
    for (int i = 0; i < total_estacionamentos; i++) {
        if (estacionamentos[i].numE == numero_entrada) {
            // Liberta o lugar atual
            Lugar *lugar_atual = &estacionamentos[i].lugar;
            parque->pisos[lugar_atual->num_piso - 1].lugares[lugar_atual->fila - 'A'][lugar_atual->lugar - 1].estado = 'L';

            // Atualiza para o novo lugar
            int piso, lugar_num;
            char fila;
            sscanf(novo_lugar, "%d%c%d", &piso, &fila, &lugar_num);

            Lugar *novo = &parque->pisos[piso - 1].lugares[fila - 'A'][lugar_num - 1];
            if (novo->estado == 'L') {
                novo->estado = 'O';
                strncpy(novo->codigo, lugar_atual->codigo, MAX_CODIGO);
                *lugar_atual = *novo; // Atualiza o lugar no registo

                printf("Lugar atualizado com sucesso para %s.\n", novo_lugar);
            } else {
                printf("O novo lugar especificado não está disponível.\n");
            }
            return;
        }
    }
    printf("Registo não encontrado.\n");
}
