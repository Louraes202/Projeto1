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
    printf("Total de lugares no parque: %d\n", parque->total_lugares);
    printf("Lugares ocupados: %d\n", parque->lugares_ocupados);
    printf("Lugares livres: %d\n", parque->lugares_livres);

    // Perguntar ao utilizador se deseja ver detalhes por fila
    char opcao;
    printf("\nDeseja visualizar os detalhes das filas para cada piso? (S/N): ");
    scanf(" %c", &opcao);

    if (opcao == 'S' || opcao == 's') {
        // Mostrar dados detalhados de cada piso
        for (int i = 0; i < parque->num_pisos; i++) {
            const Piso *piso = &parque->pisos[i];
            printf("\nPiso %d:\n", i + 1);
            printf("  Número de filas: %d\n", piso->num_filas);

            // Mostrar número de lugares por fila
            for (int j = 0; j < piso->num_filas; j++) {
                printf("    Fila %c: %d lugares\n", 'A' + j, piso->lugares_por_fila[j]);
            }

            printf("  Lugares livres: %d\n", piso->livres);
            printf("  Lugares ocupados: %d\n", piso->ocupados);
            printf("  Lugares indisponíveis: %d\n", piso->indisponiveis);
        }
    } else {
        printf("\nDetalhes das filas não exibidos.\n");
    }

    printf("------------------------------\n");
}
