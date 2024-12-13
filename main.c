#include <locale.h>  // Para usar setlocale()
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
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}

int main() {

    // Definir a localidade para UTF-8
    setlocale(LC_ALL, "pt_PT.UTF-8");

    // Configurar UTF-8 no Windows
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif

    Parque parque;
    Tarifario tarifario;
    int opcao;

    do {
        exibir_menu();
        scanf("%d", &opcao);
        getchar(); // Limpar o buffer

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
            case 0:
                printf("Encerrando o programa.\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
