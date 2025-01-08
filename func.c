#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estacionamento.h"

// ------------------------ Configuração do Parque ------------------------

// Função para limpar a memória e apagar ficheiros associados
void limpar_memoria(Parque *parque) {
    // Resetar a estrutura Parque
    memset(parque, 0, sizeof(Parque));

    // Esvaziar o ficheiro config_parque.txt
    FILE *file = fopen("config_parque.txt", "w");
    if (file == NULL) {
        printf("Erro ao limpar o ficheiro de configuração.\n");
    } else {
        fclose(file);
        printf("Ficheiro config_parque.txt limpo com sucesso.\n");
    }

    printf("Memória do programa e ficheiros associados foram limpos.\n");
}

void configurar_parque(Parque *parque) {
    // Solicita o nome do parque
    printf("Digite o nome do parque (até 30 caracteres): ");
    scanf("%30s", parque->nome);

    // Solicita a morada do parque
    printf("Digite a morada do parque (até 30 caracteres): ");
    scanf(" %[^\n]%*c", parque->morada); // Lê uma string com espaços

    // Solicita o número de andares
    int valid_input = 0; // Flag para controlar input válido
    do {
        printf("Digite o número de andares (1-5): ");
        valid_input = scanf("%d", &parque->num_pisos);

        if (!valid_input || parque->num_pisos < 1 || parque->num_pisos > MAX_PISOS) {
            printf("Erro: O número de andares deve estar entre 1 e %d.\n", MAX_PISOS);
            while (getchar() != '\n'); // Limpa o buffer de entrada
        }
    } while (!valid_input || parque->num_pisos < 1 || parque->num_pisos > MAX_PISOS);

    // Solicita o número de filas por piso
    do {
        printf("Digite o número de filas por piso (1-26): ");
        valid_input = scanf("%d", &parque->filas);

        if (!valid_input || parque->filas < 1 || parque->filas > MAX_FILAS) {
            printf("Erro: O número de filas deve estar entre 1 e %d.\n", MAX_FILAS);
            while (getchar() != '\n'); // Limpa o buffer de entrada
        }
    } while (!valid_input || parque->filas < 1 || parque->filas > MAX_FILAS);

    // Solicita o número de lugares por fila
    do {
        printf("Digite o número de lugares por fila (1-50): ");
        valid_input = scanf("%d", &parque->lugares_por_fila);

        if (!valid_input || parque->lugares_por_fila < 1 || parque->lugares_por_fila > MAX_LUGARES) {
            printf("Erro: O número de lugares deve estar entre 1 e %d.\n", MAX_LUGARES);
            while (getchar() != '\n'); // Limpa o buffer de entrada
        }
    } while (!valid_input || parque->lugares_por_fila < 1 || parque->lugares_por_fila > MAX_LUGARES);

    // Calcula o total de lugares e inicializa os campos relacionados
    parque->total_lugares = parque->num_pisos * parque->filas * parque->lugares_por_fila;
    parque->lugares_ocupados = 0;
    parque->lugares_livres = parque->total_lugares;

    // Grava a configuração no ficheiro
    gravar_configuracao_parque(parque);
    printf("Configuração do parque concluída e salva com sucesso.\n");
}

// Função para gravar a configuração no ficheiro
void gravar_configuracao_parque(const Parque *parque) {
    FILE *file = fopen("config_parque.txt", "w");
    if (file == NULL) {
        printf("Erro ao gravar configuração do parque no ficheiro.\n");
        return;
    }

    fprintf(file, "%s\n%d\n%d\n%d\n%d\n", 
            parque->nome, parque->num_pisos, parque->filas, 
            parque->lugares_por_fila, parque->total_lugares);
    fclose(file);
}

// Função para carregar a configuração do ficheiro
int carregar_configuracao_parque(Parque *parque) {
    FILE *file = fopen("config_parque.txt", "r");
    if (file == NULL) {
        printf("Erro: O ficheiro de configuração não existe.\n");
        printf("Sugestão: Utilize a opção 'Limpar Memória' para garantir que o ficheiro é reinicializado.\n");
        return 0;
    }

    // Verifica se o ficheiro está vazio
    fseek(file, 0, SEEK_END); // Move o cursor para o final do ficheiro
    long tamanho = ftell(file); // Obtém o tamanho do ficheiro
    if (tamanho == 0) {
        printf("Erro: O ficheiro de configuração está vazio.\n");
        printf("Sugestão: Utilize a opção 'Configurar Parque' para configurar os dados do mesmo.\n");
        fclose(file);
        return 0;
    }
    rewind(file); // Volta o cursor para o início do ficheiro

    // Tenta carregar os dados do ficheiro
    if (fscanf(file, "%30s\n%30s\n%d\n%d\n%d\n%d", 
               parque->nome, 
               parque->morada, 
               &parque->num_pisos, 
               &parque->filas, 
               &parque->lugares_por_fila, 
               &parque->total_lugares) != 6) {
        printf("Erro: O ficheiro de configuração não está no formato esperado.\n");
        printf("Sugestão: Utilize a opção 'Limpar Memória' para reinicializar o ficheiro.\n");
        fclose(file);
        return 0;
    }

    // Inicializa campos derivados
    parque->lugares_ocupados = 0;
    parque->lugares_livres = parque->total_lugares;

    printf("Configuração do parque carregada com sucesso.\n");
    fclose(file);
    return 1;
}
// Função para alterar os dados do parque
void alterar_parque(Parque *parque) {
    int opcao;

    printf("\n--- Alterar Configuração do Parque ---\n");
    printf("1. Alterar nome do parque\n");
    printf("2. Alterar número de pisos\n");
    printf("3. Alterar número de filas por piso\n");
    printf("4. Alterar número de lugares por fila\n");
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
        case 3:
            do {
                printf("Digite o novo número de filas por piso (1-26): ");
                scanf("%d", &parque->filas);
                if (parque->filas < 1 || parque->filas > MAX_FILAS) {
                    printf("Erro: O número de filas deve estar entre 1 e %d.\n", MAX_FILAS);
                }
            } while (parque->filas < 1 || parque->filas > MAX_FILAS);
            break;
        case 4:
            do {
                printf("Digite o novo número de lugares por fila (1-50): ");
                scanf("%d", &parque->lugares_por_fila);
                if (parque->lugares_por_fila < 1 || parque->lugares_por_fila > MAX_LUGARES) {
                    printf("Erro: O número de lugares deve estar entre 1 e %d.\n", MAX_LUGARES);
                }
            } while (parque->lugares_por_fila < 1 || parque->lugares_por_fila > MAX_LUGARES);
            break;
        case 0:
            printf("Nenhuma alteração realizada.\n");
            return;
        default:
            printf("Opção inválida.\n");
            return;
    }

    // Atualizar os valores derivados
    parque->total_lugares = parque->num_pisos * parque->filas * parque->lugares_por_fila;
    parque->lugares_livres = parque->total_lugares - parque->lugares_ocupados;

    // Gravar as alterações no ficheiro
    gravar_configuracao_parque(parque);
    printf("Alterações realizadas com sucesso e salvas no ficheiro.\n");
}

// Função para visualizar os dados atuais do parque, validando se está configurado
void visualizar_dados_parque(const Parque *parque) {
    // Verificar se os dados básicos do parque estão configurados
    if (strlen(parque->nome) == 0 || parque->num_pisos <= 0) {
        printf("\nErro: O parque ainda não está configurado ou os dados não foram carregados.\n");
        printf("Por favor, configure o parque ou carregue os dados de um ficheiro.\n");
        return;
    }

    printf("\n-- Dados Atuais do Parque --\n");
    printf("Nome: %s\n", parque->nome);
    printf("Morada: %s\n", parque->morada);
    printf("Número de pisos: %d\n", parque->num_pisos);
    printf("Número de filas por piso: %d\n", parque->filas);
    printf("Número de lugares por fila: %d\n", parque->lugares_por_fila);
    printf("Total de lugares: %d\n", parque->total_lugares);
    printf("Lugares ocupados: %d\n", parque->lugares_ocupados);
    printf("Lugares livres: %d\n", parque->lugares_livres);
    printf("------------------------------\n");
}

// Função para carregar tarifas de Tarifas.txt
int carregar_tarifas(Tarifario *tarifario) {
    FILE *file = fopen("Tarifas.txt", "r");
    if (file == NULL) {
        printf("Erro: Não foi possível abrir o ficheiro Tarifas.txt.\n");
        return 0;
    }

    // Lê o número de tarifas do ficheiro
    fscanf(file, "%d\n", &tarifario->num_tarifas);
    tarifario->lista_tarifas = malloc(sizeof(Tarifa) * tarifario->num_tarifas);
    if (tarifario->lista_tarifas == NULL) {
        printf("Erro: Falha ao alocar memória para as tarifas.\n");
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

// Função para carregar os registos de Estacionamentos.txt
int carregar_registos(Estacionamento estacionamentos[], int *total_estacionamentos, Tarifario *tarifario) {
    FILE *file = fopen("Estacionamentos.txt", "r");
    if (file == NULL) {
        printf("Erro: Não foi possível abrir o ficheiro Estacionamentos.txt.\n");
        return 0;
    }

    *total_estacionamentos = 0;
    while (fscanf(file, "%d %10s %d/%d/%d %d:%d %d/%d/%d %d:%d %d %49[^\n]\n",
                  &estacionamentos[*total_estacionamentos].numE,
                  estacionamentos[*total_estacionamentos].matricula,
                  &estacionamentos[*total_estacionamentos].data_entrada.dia,
                  &estacionamentos[*total_estacionamentos].data_entrada.mes,
                  &estacionamentos[*total_estacionamentos].data_entrada.ano,
                  &estacionamentos[*total_estacionamentos].entrada.hora,
                  &estacionamentos[*total_estacionamentos].entrada.min,
                  &estacionamentos[*total_estacionamentos].data_saida.dia,
                  &estacionamentos[*total_estacionamentos].data_saida.mes,
                  &estacionamentos[*total_estacionamentos].data_saida.ano,
                  &estacionamentos[*total_estacionamentos].saida.hora,
                  &estacionamentos[*total_estacionamentos].saida.min,
                  &estacionamentos[*total_estacionamentos].lugar.num_piso,
                  estacionamentos[*total_estacionamentos].observacoes) == 14) {
        
        // Verifica se o registo tem hora de saída definida
        if (estacionamentos[*total_estacionamentos].saida.hora >= 0) {
            // Calcula o valor pago com base nas tarifas
            estacionamentos[*total_estacionamentos].valor_pago = calcular_valor_pago(&estacionamentos[*total_estacionamentos], tarifario);
        } else {
            // Caso não tenha saída, considera o veículo ainda no parque
            estacionamentos[*total_estacionamentos].valor_pago = 0.0f;
        }

        (*total_estacionamentos)++;
    }

    fclose(file);
    printf("Registos carregados com sucesso.\n");
    return 1;
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
