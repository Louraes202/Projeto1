#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

    // Escrever nome, morada e os parâmetros numéricos
    fprintf(file, "%s\n%s\n%d\n%d\n%d\n%d\n", 
            parque->nome, 
            parque->morada, 
            parque->num_pisos, 
            parque->filas, 
            parque->lugares_por_fila, 
            parque->total_lugares);

    if (ferror(file)) { // Verifica se ocorreu um erro durante a escrita
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
        printf("Sistema: Utilize a opção 'Limpar Memória' para reinicializar o ficheiro.\n");
        return 0;
    }

    // Verificar se o ficheiro está vazio
    fseek(file, 0, SEEK_END);
    long tamanho = ftell(file);
    if (tamanho == 0) {
        printf("Sistema: O ficheiro de configuração está vazio.\n");
        fclose(file);
        return 0;
    }
    rewind(file);

    // Ler os dados do ficheiro
    char linha[256];
    if (fgets(parque->nome, sizeof(parque->nome), file) == NULL ||
        fgets(parque->morada, sizeof(parque->morada), file) == NULL ||
        fgets(linha, sizeof(linha), file) == NULL || sscanf(linha, "%d", &parque->num_pisos) != 1 ||
        fgets(linha, sizeof(linha), file) == NULL || sscanf(linha, "%d", &parque->filas) != 1 ||
        fgets(linha, sizeof(linha), file) == NULL || sscanf(linha, "%d", &parque->lugares_por_fila) != 1 ||
        fgets(linha, sizeof(linha), file) == NULL || sscanf(linha, "%d", &parque->total_lugares) != 1) {
        printf("Sistema: O ficheiro de configuração não está no formato esperado.\n");
        fclose(file);
        return 0;
    }

    // Remover caracteres extra das strings
    parque->nome[strcspn(parque->nome, "\n")] = '\0';
    parque->morada[strcspn(parque->morada, "\n")] = '\0';

    // Validar os valores numéricos
    if (parque->num_pisos <= 0 || parque->num_pisos > MAX_PISOS ||
        parque->filas <= 0 || parque->filas > MAX_FILAS ||
        parque->lugares_por_fila <= 0 || parque->lugares_por_fila > MAX_LUGARES) {
        printf("Sistema: O ficheiro de configuração contém valores inválidos.\n");
        fclose(file);
        return 0;
    }

    // Validar consistência do total de lugares
    if (parque->total_lugares != parque->num_pisos * parque->filas * parque->lugares_por_fila) {
        printf("Sistema: O ficheiro de configuração contém inconsistências no total de lugares.\n");
        fclose(file);
        return 0;
    }

    // Inicializar campos derivados
    parque->lugares_ocupados = 0;
    parque->lugares_livres = parque->total_lugares;
    for (int i = 0; i < parque->num_pisos; i++) {
        parque->pisos[i].livres = parque->filas * parque->lugares_por_fila;
        parque->pisos[i].ocupados = 0;
        parque->pisos[i].indisponiveis = 0;
        for (int j = 0; j < parque->filas; j++) {
            for (int k = 0; k < parque->lugares_por_fila; k++) {
                parque->pisos[i].lugares[j][k].estado = 'L';
                memset(parque->pisos[i].lugares[j][k].codigo, 0, sizeof(parque->pisos[i].lugares[j][k].codigo));
            }
        }
    }

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

    // Grava os dados do parque
    fwrite(parque, sizeof(Parque), 1, file);

    // Grava o número total de registos
    fwrite(&total_estacionamentos, sizeof(int), 1, file);

    // Grava os registos de estacionamento
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

    // Lê os dados do parque
    if (fread(parque, sizeof(Parque), 1, file) != 1 || 
        parque->num_pisos == 0 || 
        parque->filas == 0 || 
        parque->lugares_por_fila == 0) {
        printf("Erro ao carregar dados do parque ou parque não configurado. Inicializando com dados padrão.\n");
        fclose(file);
        memset(parque, 0, sizeof(Parque)); // Reinicializa a estrutura
        *total_estacionamentos = 0;
        *ultimo_id = 0;
        return 0;
    }

    // Lê o número total de registos
    if (fread(total_estacionamentos, sizeof(int), 1, file) != 1) {
        printf("Erro ao carregar o total de registos. Inicializando com 0 registos.\n");
        fclose(file);
        *total_estacionamentos = 0;
        return 0;
    }

    // Lê os registos de estacionamento
    if (fread(estacionamentos, sizeof(Estacionamento), *total_estacionamentos, file) != (size_t)*total_estacionamentos) {
        printf("Erro ao carregar registos. Inicializando com registos vazios.\n");
        fclose(file);
        *total_estacionamentos = 0;
        return 0;
    }

    // Recalcula as estatísticas do parque após carregar os dados
    recalcular_estatisticas_parque(parque);

    // Atualiza o último ID com base nos registos carregados
    for (int i = 0; i < *total_estacionamentos; i++) {
        if (estacionamentos[i].numE > *ultimo_id) {
            *ultimo_id = estacionamentos[i].numE;
        }
    }

    fclose(file);
    printf("Sistema: Estado do parque carregado com sucesso.\n");
    return 1;
}

void recalcular_estatisticas_parque(Parque *parque) {
    for (int i = 0; i < parque->num_pisos; i++) {
        parque->pisos[i].livres = 0;
        parque->pisos[i].ocupados = 0;
        parque->pisos[i].indisponiveis = 0;
        for (int j = 0; j < parque->filas; j++) {
            for (int k = 0; k < parque->lugares_por_fila; k++) {
                Lugar *lugar = &parque->pisos[i].lugares[j][k];
                if (lugar->estado == 'L') {
                    parque->pisos[i].livres++;
                } else if (lugar->estado == 'O') {
                    parque->pisos[i].ocupados++;
                } else if (lugar->estado == 'I') {
                    parque->pisos[i].indisponiveis++;
                }
            }
        }
    }

    parque->lugares_ocupados = 0;
    parque->lugares_livres = 0;
    for (int i = 0; i < parque->num_pisos; i++) {
        parque->lugares_livres += parque->pisos[i].livres;
        parque->lugares_ocupados += parque->pisos[i].ocupados;
    }
}



// Função para carregar registos de forma automática (através de ficheiros)
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
        if (piso < 1 || piso > parque->num_pisos ||
            fila_idx < 0 || fila_idx >= parque->filas ||
            lugar < 1 || lugar > parque->lugares_por_fila) {
            printf("Aviso: Registo incompatível (Piso: %d, Fila: %c, Lugar: %d). Ignorado.\n", piso, fila, lugar);
            registos_descartados++;
            continue;
        }

        // Validação do estado do lugar
        Lugar *lugar_atual = &parque->pisos[piso - 1].lugares[fila_idx][lugar - 1];
        if (lugar_atual->estado != 'L') { // Verifica se o lugar está livre
            printf("Aviso: Lugar já ocupado ou indisponível (Piso: %d, Fila: %c, Lugar: %d). Ignorado.\n", piso, fila, lugar);
            registos_descartados++;
            continue;
        }

        // Atualizar o estado do lugar
        lugar_atual->estado = 'O'; // Marca como ocupado
        strcpy(lugar_atual->codigo, estacionamentos[*total_estacionamentos].matricula);

        // Atualizar o estado do parque
        parque->pisos[piso - 1].ocupados++;
        parque->pisos[piso - 1].livres--;

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


void configurar_parque(Parque *parque) {
    // Solicita o nome do parque
    printf("Digite o nome do parque (até 30 caracteres): ");
    scanf("%30s", parque->nome);

    // Solicita a morada do parque
    printf("Digite a morada do parque (até 30 caracteres): ");
    scanf(" %[^\n]%*c", parque->morada);

    // Solicita o número de andares
    do {
        printf("Digite o número de andares (1-5): ");
        scanf("%d", &parque->num_pisos);
        if (parque->num_pisos < 1 || parque->num_pisos > MAX_PISOS) {
            printf("Sistema: O número de andares deve estar entre 1 e %d.\n", MAX_PISOS);
        }
    } while (parque->num_pisos < 1 || parque->num_pisos > MAX_PISOS);

    // Solicita o número de filas por piso
    do {
        printf("Digite o número de filas por piso (1-26): ");
        scanf("%d", &parque->filas);
        if (parque->filas < 1 || parque->filas > MAX_FILAS) {
            printf("Sistema: O número de filas deve estar entre 1 e %d.\n", MAX_FILAS);
        }
    } while (parque->filas < 1 || parque->filas > MAX_FILAS);

    // Solicita o número de lugares por fila
    do {
        printf("Digite o número de lugares por fila (1-50): ");
        scanf("%d", &parque->lugares_por_fila);
        if (parque->lugares_por_fila < 1 || parque->lugares_por_fila > MAX_LUGARES) {
            printf("Sistema: O número de lugares deve estar entre 1 e %d.\n", MAX_LUGARES);
        }
    } while (parque->lugares_por_fila < 1 || parque->lugares_por_fila > MAX_LUGARES);

    // Calcula o total de lugares e inicializa os campos relacionados
    parque->total_lugares = parque->num_pisos * parque->filas * parque->lugares_por_fila;
    parque->lugares_ocupados = 0;
    parque->lugares_livres = parque->total_lugares;

    // Inicializa os dados por piso
    for (int i = 0; i < parque->num_pisos; i++) {
        parque->pisos[i].livres = parque->filas * parque->lugares_por_fila;
        parque->pisos[i].ocupados = 0;
        parque->pisos[i].indisponiveis = 0;
    }

    for (int i = 0; i < parque->num_pisos; i++) {
        for (int j = 0; j < parque->filas; j++) {
            for (int k = 0; k < parque->lugares_por_fila; k++) {
                // Inicializa cada lugar como livre
                parque->pisos[i].lugares[j][k].estado = 'L'; // Livre
                memset(parque->pisos[i].lugares[j][k].codigo, 0, sizeof(parque->pisos[i].lugares[j][k].codigo));
            }
        }
    }
    printf("Todos os lugares foram inicializados como livres.\n");

    // Grava a configuração no ficheiro
    gravar_configuracao_parque(parque);
    printf("Configuração do parque concluída e salva com sucesso.\n");
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
                    printf("Sistema: O número de andares deve estar entre 1 e %d.\n", MAX_PISOS);
                }
            } while (parque->num_pisos < 1 || parque->num_pisos > MAX_PISOS);
            break;
        case 3:
            do {
                printf("Digite o novo número de filas por piso (1-26): ");
                scanf("%d", &parque->filas);
                if (parque->filas < 1 || parque->filas > MAX_FILAS) {
                    printf("Sistema: O número de filas deve estar entre 1 e %d.\n", MAX_FILAS);
                }
            } while (parque->filas < 1 || parque->filas > MAX_FILAS);
            break;
        case 4:
            do {
                printf("Digite o novo número de lugares por fila (1-50): ");
                scanf("%d", &parque->lugares_por_fila);
                if (parque->lugares_por_fila < 1 || parque->lugares_por_fila > MAX_LUGARES) {
                    printf("Sistema: O número de lugares deve estar entre 1 e %d.\n", MAX_LUGARES);
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
        printf("\nSistema: O parque ainda não está configurado ou os dados não foram carregados.\n");
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
        for (int fila = 0; fila < parque->filas && !lugar_atribuido; fila++) {
            for (int lugar = 0; lugar < parque->lugares_por_fila && !lugar_atribuido; lugar++) {
                Lugar *lugar_atual = &parque->pisos[piso].lugares[fila][lugar];
                if (lugar_atual->estado == 'L') {
                    // Atualiza o lugar para ocupado
                    lugar_atual->estado = 'O';
                    strcpy(lugar_atual->codigo, matricula);

                    // Atualiza o parque
                    parque->pisos[piso].ocupados++;
                    parque->pisos[piso].livres--;

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
