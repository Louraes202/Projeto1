#include <locale.h>
#include <windows.h>
#include "estacionamento.h"

void exibir_menu() {
    printf("\n--- Menu Principal ---\n");
    printf("1. Configurar Parque\n");
    printf("2. Configurar Tarifário\n");
    printf("3. Registar Entrada\n");
    printf("4. Registar Saída\n");
    printf("5. Carregar Configuração do Parque\n");
    printf("6. Carregar Tarifário\n");
    printf("7. Mostrar Mapa de Ocupação\n");
    printf("8. Carregar Estacionamentos\n");
    printf("9. Gravar Estacionamentos\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}

int main() {
    setlocale(LC_ALL, "pt_PT.UTF-8");

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    Parque parque;
    Tarifario tarifario;
    Estacionamento estacionamentos[MAX_ESTACIONAMENTOS];
    int total_estacionamentos = 0;

    Parque parques[MAX_PARQUES];
    int total_parques = 0;


    int opcao;

    do {
        exibir_menu();
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                configurar_parque(&parque);
                break;

            case 2:
                configurar_tarifario(&tarifario);
                break;

            case 3: {
                char matricula[10];
                int piso, lugar;
                char fila;
                Horario entrada;

                printf("Digite a matrícula do veículo: ");
                scanf("%9s", matricula);

                printf("Digite o piso (1-%d): ", MAX_PISOS);
                scanf("%d", &piso);

                printf("Digite a fila (A-%c): ", 'A' + MAX_FILAS - 1);
                scanf(" %c", &fila);

                printf("Digite o lugar (1-%d): ", MAX_LUGARES);
                scanf("%d", &lugar);

                printf("Digite o horário de entrada (hh:mm:ss): ");
                scanf("%d:%d:%d", &entrada.hora, &entrada.min, &entrada.seg);

                registar_entrada(&parque, matricula, piso, fila, lugar, entrada);
                break;
            }

            case 4: {
                char matricula[10];
                Horario saida;

                printf("Digite a matrícula do veículo: ");
                scanf("%9s", matricula);

                printf("Digite o horário de saída (hh:mm:ss): ");
                scanf("%d:%d:%d", &saida.hora, &saida.min, &saida.seg);

                registar_saida(&parque, matricula, saida, &tarifario);
                break;
            }

            case 5:
                if (!carregar_configuracao_parque(&parque)) {
                    printf("Erro ao carregar a configuração do parque.\n");
                }
                break;

            case 6:
                if (!carregar_tarifario(&tarifario)) {
                    printf("Erro ao carregar o tarifário.\n");
                }
                break;

            case 7:
                mostrar_ocupacao_parque(parques, total_parques);
                break;


            case 8: {
                char ficheiroEstacionamentos[100]; // Buffer para o nome do ficheiro

                printf("Digite o nome do ficheiro de estacionamentos a carregar: ");
                scanf("%s", ficheiroEstacionamentos);

                if (!carregar_estacionamentos(ficheiroEstacionamentos, estacionamentos, &total_estacionamentos, &parque)) {
                    printf("Erro ao carregar estacionamentos do ficheiro %s.\n", ficheiroEstacionamentos);
                } else {
                    printf("Estacionamentos carregados com sucesso do ficheiro %s.\n", ficheiroEstacionamentos);
                }
                break;
            }


            case 9:
                gravar_estacionamentos("estacionamento.txt", estacionamentos, total_estacionamentos);
                break;

            case 0:
                printf("Encerrando o programa.\n");
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
