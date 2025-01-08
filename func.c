#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estacionamento.h"

// ------------------------ Configuração do Parque ------------------------

// Função para configurar o parque
void configurar_parque(Parque *parque) {
    printf("Digite o nome do parque (até 30 caracteres): ");
    scanf("%30s", parque->nome);

    do {
        printf("Digite o número de andares (1-5): ");
        scanf("%d", &parque->num_pisos);
        if (parque->num_pisos < 1 || parque->num_pisos > MAX_PISOS) {
            printf("Erro: O número de andares deve estar entre 1 e %d.\n", MAX_PISOS);
        }
    } while (parque->num_pisos < 1 || parque->num_pisos > MAX_PISOS);

    do {
        printf("Digite o número de filas por piso (1-26): ");
        scanf("%d", &parque->filas);
        if (parque->filas < 1 || parque->filas > MAX_FILAS) {
            printf("Erro: O número de filas deve estar entre 1 e %d.\n", MAX_FILAS);
        }
    } while (parque->filas < 1 || parque->filas > MAX_FILAS);

    do {
        printf("Digite o número de lugares por fila (1-50): ");
        scanf("%d", &parque->lugares_por_fila);
        if (parque->lugares_por_fila < 1 || parque->lugares_por_fila > MAX_LUGARES) {
            printf("Erro: O número de lugares deve estar entre 1 e %d.\n", MAX_LUGARES);
        }
    } while (parque->lugares_por_fila < 1 || parque->lugares_por_fila > MAX_LUGARES);

    // Calcular total de lugares
    parque->total_lugares = parque->num_pisos * parque->filas * parque->lugares_por_fila;
    parque->lugares_ocupados = 0;
    parque->lugares_livres = parque->total_lugares;

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
        printf("Erro: Não foi possível abrir o ficheiro de configuração do parque.\n");
        return 0;
    }

    fscanf(file, "%30s\n%d\n%d\n%d\n%d\n", 
           parque->nome, &parque->num_pisos, &parque->filas, 
           &parque->lugares_por_fila, &parque->total_lugares);

    parque->lugares_ocupados = 0; // Inicialmente, nenhum lugar ocupado
    parque->lugares_livres = parque->total_lugares;
    fclose(file);

    printf("Configuração do parque carregada com sucesso.\n");
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